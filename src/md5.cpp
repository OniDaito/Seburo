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


MD5Model::MD5Model(const File &file) : Node() {
  parse(file);

  // Add the matrix rotation as this is in American co-ords
  set_matrix(glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
}


/// small function to discern W for our Quaternions
glm::quat computeW (glm::quat &q) {
  float t = 1.0f - (q.x * q.x) - (q.y * q.y) - (q.z * q.z);
  if (t < 0.0f)
    q.w = 0.0f;
  else
    q.w = -sqrt (t);
  return q;
}


typedef struct {
  float     s, t;
  IndicesType index;
  size_t    count;
} md5_vertex;



void MD5Model::parse(const File &file) {
  if (!file.exists()){
    cerr << "SEBURO MD5Model - File " << file.path() << " does not exist or cannot be read." << endl;
    return;
  }

  filename_ = file.filename();

  std::ifstream ifs;

  ifs.open (file.path(), std::ifstream::in);

  skeleton_ = Skeleton(CUSTOM_SKELETON);

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
      version_ = a;
    } 

    else if (string_contains(line, "numJoints") ){
      string s;
      if (!(iss >> s >> num_joints_)) { break; }
    } 

    else if (string_contains(line, "numMeshes") ){
      string s;
      if (!(iss >> s >> num_meshes_)) { break; }
    } 

    else if (string_contains(line, "joints {") ){

      std::string tline;
      std::getline(ifs, tline);

      for (size_t i = 0; i < num_joints_; ++i){
        float p0,p1,p2,r0,r1,r2;
        int parent;
        string name;
        
        tline = remove_char(tline, '(');
        tline = remove_char(tline, ')');
        std::istringstream tiss(tline);

        if (!(tiss >> name >> parent >> p0 >> p1 >> p2 >> r0 >> r1 >> r2)) { break; }

        bone_indices.push_back(parent);
        name = remove_char(name, '"');

        glm::quat q = glm::quat();
        q.x = r0; q.y = r1; q.z = r2; computeW(q);
        glm::vec3 p = glm::vec3(p0,p1,p2);

        skeleton_.addBone ( new Bone(name, nullptr, q, p) );

        std::getline(ifs, tline);
      }

      // Match up parents with actual pointers to bones
      for (size_t i = 0; i < num_joints_; ++i){
        if (bone_indices[i] != -1)
          skeleton_.bone(i)->parent = skeleton_.bone(bone_indices[i]);
      }

    }

    /** We use a full trimesh here because we will precompute tangent and normal before
    sending it off to the shader */

    else if (string_contains(line, "mesh {" )){
      std::string tline;

      Node mesh_node;
      add(mesh_node);

      md5_vertex* verts;
      size_t num_verts;

      TriMesh trimesh;
      Skin    skin;
      
      const GeometryT<Vertex3, Face3, AllocationPolicyNew> *geometry; // Really not happy about this :S

      size_t vidx = 0;

      while(std::getline(ifs, tline) && !(string_contains(tline, "}"))) {
        
        // Vertices read

        if (string_contains(tline, "numverts")){
          std::istringstream tiss(tline);
          string s;
          if (!(tiss >> s >> num_verts)) { break; }

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

          if (!(tiss >> s >> idx >> verts[vidx].s >> verts[vidx].t >> verts[vidx].index >> verts[vidx].count)) { break; }
          ++vidx;

        }

        // Triangles read

        else if (string_contains(tline, "numtris")) {
          std::istringstream tiss(tline);
          string s;
          size_t num_tris;
          if (!(tiss >> s >> num_tris)) { break; }
        
          // Making the assumption (perhaps incorrectly) that numverts always comes before numtris so we have both here
          // We make our geometry and add it as a node
          trimesh = TriMesh(num_verts, num_tris * 3);
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
          skin = Skin(num_weights);
          mesh_node.add(skin);

        }

        else if (string_contains(tline, "weight")){
          tline = remove_char(tline, '(');
          tline = remove_char(tline, ')');
          std::istringstream tiss(tline);

          size_t idx,bone_id;
          float bias, p0, p1, p2;
          string s;

          if (!(tiss >> s >> idx >> bone_id >> bias >> p0 >> p1 >> p2 )) { break; }

          Skin::SkinWeight w;
          w.position = glm::vec3(p0,p1,p2);
          w.bias = bias;

          // Assuming the skeleton_ / joints appear first
          w.bone = skeleton_.bones()[bone_id];
          skin.addWeight(w);
        }
      }

      // Post process our vertices to get the basic initial pose and vertex position
      vector<Skin::SkinIndex> indices = skin.indices();
      vector<Skin::SkinWeight> weights = skin.weights();

      for (size_t i =0; i < num_verts; ++i){
        
        // Setup the skin indices at this point
        Skin::SkinIndex si;
        si.index = verts[i].index;
        si.count = verts[i].count;
        indices.push_back(si);

        glm::vec3 pos (0.0f,0.0f,0.0f);

        for (size_t j = 0; j < si.count; ++j){
          Skin::SkinWeight w = weights[si.index + j];

          glm::vec3 bp = w.bone->rotation * w.position;
          pos += ( (w.bone->position + bp) * w.bias); // assuming all biases add up to 1
      
        }

        geometry->vertices()[i].p = pos;
        geometry->vertices()[i].u = glm::vec2(verts[i].s, verts[i].t);

      }


      delete[] verts;
    }

  }


  ifs.close();

  
}


