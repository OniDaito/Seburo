/**
* @brief MD5 Parsing class
* @file md5.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#include "s9/md5.hpp"
#include "s9/utils.hpp"

using namespace std;
using namespace s9;



MD5Model::MD5Model(const File &file) : Node(), obj_( shared_ptr<SharedObject>( new SharedObject())) {
  parse(file);
}


/// small function to discern W for our Quaternions
float computeW (float x, float y, float z) {
  float t = 1.0f - (x * x) - (y * y) - (z * z);
  if (t < 0.0f)
    return 0.0f;
  else
    return -sqrt (t);
}


typedef struct {
  float s, t;
} md5_vertex;



void MD5Model::parse(const File &file) {
  if (!file.exists()){
    cerr << "SEBURO MD5Model - File " << file.path() << " does not exist or cannot be read." << endl;
    return;
  }

  obj_->filename = file.filename();

  std::ifstream ifs;

  ifs.open (file.path(), std::ifstream::in);

  obj_->skeleton = Skeleton(CUSTOM_SKELETON);

  // vector to hold indices for parent bones

  vector<int> bone_indices; // Could be a -1 here you see, for the top tree

  // Parse the ASCII format, extracting all the bits we need :)
  std::string line;
  while (std::getline(ifs, line)){
    std::istringstream iss(line);

    if (string_contains(line, "MD5Version") ){
      int a;
      string s;
      if (!(iss >> s >> a)) { break; }
      obj_->version = a;
    } 

    else if (string_contains(line, "numJoints") ){
      string s;
      if (!(iss >> s >> obj_->num_joints)) { break; }
    } 

    else if (string_contains(line, "numMeshes") ){
      string s;
      if (!(iss >> s >> obj_->num_meshes)) { break; }
    } 

    else if (string_contains(line, "joints {") ){

      std::string tline;
      std::getline(ifs, tline);

      for (size_t i = 0; i < obj_->num_joints; ++i){
        float p0,p1,p2,r0,r1,r2;
        int parent;
        string name;
        
        tline = remove_char(tline, '(');
        tline = remove_char(tline, ')');
        std::istringstream tiss(tline);

        if (!(tiss >> name >> parent >> p0 >> p1 >> p2 >> r0 >> r1 >> r2)) { break; }

        bone_indices.push_back(parent);
        name = remove_char(name, '"');

        glm::quat q = glm::quat(r0,r1,r2, computeW(r0,r1,r2));
        glm::vec3 p = glm::vec3(p0,p1,p2);

        obj_->skeleton.addBone ( new Bone(name, nullptr, q, p) );

  
        std::getline(ifs, tline);
      }

      // Match up parents with actual pointers to bones

    }

    /** We use a full trimesh here because we will precompute tangent and normal before
    sending it off to the shader */

    else if (string_contains(line, "mesh {" )){
      std::string tline;

      Node mesh_node;
      add(mesh_node);

      md5_vertex* verts;

      TriMesh trimesh;
      
      const GeometryT<Vertex3, Face3, AllocationPolicyNew> *geometry; // Really not happy about this :S

      size_t vidx = 0;

      while(std::getline(ifs, tline) && !(string_contains(tline, "}"))) {
        
        // Vertices read

        if (string_contains(tline, "numverts")){
          std::istringstream tiss(tline);
          string s;
          size_t num_verts;
          if (!(tiss >> s >> num_verts)) { break; }
          obj_->num_verts.push_back(num_verts);

          ///\todo this is not ideal for the memory usage I have to say :S

          verts = new md5_vertex[num_verts];
        
        } 

        else if (string_contains(tline, "vert")) { // careful here are numverts contains vert! :O

          // We make the assumption all MD5 vertices arrive in order in the mesh

          tline = remove_char(tline, '(');
          tline = remove_char(tline, ')');

          std::istringstream tiss(tline);

          size_t idx;
          string s;
          SkinWeightIndex w;

          if (!(tiss >> s >> idx >> verts[vidx].s >> verts[vidx].t >> w.index >> w.count)) { break; }
          ++vidx;

          // Again, assuming weights and indices are in order
          obj_->skeleton.addWeightIndex(w);
        }

        // Triangles read

        else if (string_contains(tline, "numtris")) {
          std::istringstream tiss(tline);
          string s;
          size_t num_tris;
          if (!(tiss >> s >> num_tris)) { break; }
          obj_->num_tris.push_back(num_tris);

          // Making the assumption (perhaps incorrectly) that numverts always comes before numtris so we have both here
          // We make our geometry and add it as a node
          trimesh = TriMesh(obj_->num_verts.back(), num_tris * 3);
          geometry = trimesh.geometry();
          mesh_node.add(trimesh);

        }

        else if (string_contains(tline, "tri")) {
          std::istringstream tiss(tline);
          size_t idx, a, b, c;
          string s;
          if (!(tiss >> s >> idx >> a >> b >> c)) { cerr << "SEBURO MD5 Error - Failed to read triangle." << endl; break; }

          geometry->indices()[idx * 3] = a;
          geometry->indices()[idx * 3 + 1] = b;
          geometry->indices()[idx * 3 + 2] = c;

        }

        // Weights read

        else if (string_contains(tline, "numweights")) {
          std::istringstream tiss(tline);
          string s;
          size_t num_weights;
          if (!(tiss >> s >> num_weights)) { break; }
          obj_->num_weights.push_back(num_weights);
        }

        else if (string_contains(tline, "weight")){
          tline = remove_char(tline, '(');
          tline = remove_char(tline, ')');
          std::istringstream tiss(tline);

          size_t idx,bone_id;
          float bias, p0, p1, p2;
          string s;

          SkinWeight w;

          if (!(tiss >> s >> idx >> bone_id >> bias >> p0 >> p1 >> p2 )) { break; }

          w.position = glm::vec3(p0,p1,p2);
          w.bias = bias;
          // Assuming the skeleton / joints appear first

          w.bone = obj_->skeleton.bones()[bone_id];

          // Again, assuming weights appear in order
          obj_->skeleton.addWeight(w);

        }

      }



      // Post process our vertices to get the basic initial pose and vertex position
      vector<SkinWeightIndex> indices = obj_->skeleton.weight_indices();
      vector<SkinWeight> weights = obj_->skeleton.weights();

      for (size_t i =0; i < obj_->num_verts.back(); ++i){
        
        SkinWeightIndex wi = indices[i];

        glm::vec3 pos (0.0f,0.0f,0.0f);

        for (size_t j = 0; j < wi.count; ++j){
          SkinWeight w = weights[wi.index + wi.count];

  
          glm::vec3 bp = w.bone->rotation * w.position;

          pos += ( (w.bone->position + bp) * w.bias); // assuming all biases add up to 1
      
        }

        geometry->vertices()[i].p = pos;

      }


      delete[] verts;
    }

  }


  ifs.close();

  
}


