/**
* @brief MD5 Parsing class
* @file md5.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 29/10/2013
*
*/

#include "s9/md5.hpp"

using namespace std;
using namespace s9;


MD5Model::MD5Model(const File &file) : Node() {
  _init(); // Init this node - we want a nice matrix
  parse(file);

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


/// Temporary structure for parsing MD5Mesh style vertices
typedef struct {
  float     s, t;
  IndicesType index;
  size_t    count;
} md5_vertex;

/// Temporary structure for MD5 Weights
typedef struct {
  glm::vec3 position;
  float bias;
  size_t bone;
} md5_weight;

/// Temporary structure for joints
typedef struct {
  glm::vec3 position;
  glm::quat rotation;
  size_t parent;
  std::string name;
} md5_joint;



void MD5Model::parse(const File &file) {
  if (!file.exists()){
    cerr << "SEBURO MD5Model - File " << file.path() << " does not exist or cannot be read." << endl;
    return;
  }

  filename_ = file.filename();

  std::ifstream ifs;

  ifs.open (file.path(), std::ifstream::in);

  skeleton_ = Skeleton(CUSTOM_SKELETON);

  add(skeleton_);


  md5_joint *joints;

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
      joints = new md5_joint[num_joints_];
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

        name = remove_char(name, '"');

        glm::quat q = glm::quat();
        q.x = r0; q.y = r1; q.z = r2; computeW(q);
        q = glm::normalize(q);

        glm::vec3 p = glm::vec3(p0,p1,p2);

        // Double up on joints as well so we can compute positions later on
        joints[i].position = p;
        joints[i].rotation = q;
        joints[i].parent = parent;
        joints[i].name = name;

        std::getline(ifs, tline);
      }


      // Create the actual bones
      // NOTE - we are assuming all joints are in order which means that 
      // that parents should exist before their children

      for (size_t i = 0; i < num_joints_; ++i){
        
          Bone * parent = nullptr;
          if (joints[i].parent != -1){
            parent = skeleton_.bone(joints[i].parent);
          }

          skeleton_.addBone(new Bone(joints[i].name, i, parent, joints[i].rotation, joints[i].position));

      }

    }

    /** We use a full trimesh here because we will precompute tangent and normal before
    sending it off to the shader */

    else if (string_contains(line, "mesh {" )){
      std::string tline;

      Node mesh_node;
      
      md5_vertex* verts;
      md5_weight* weights;

      size_t num_verts;

      add(mesh_node);

      ///\todo we are using trimeshskinned here and using per vertex attributes but eventually use textures
      TriMeshSkinned trimesh;
      Skin    skin;
      
      const GeometryT<Vertex3Skin, Face3, AllocationPolicyNew> *geometry; // Really not happy about this :S

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
         

          if (verts[vidx].count > geometry_max_bones) {
            cerr << "SEBURO MD5 Error - Bone attached to vertex exceed max bones count - " << verts[vidx].count  << endl;
          }

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
          trimesh = TriMeshSkinned(num_verts, num_tris * 3);
          geometry = trimesh.geometry();
          mesh_node.add(trimesh);

        }

        else if (string_contains(tline, "tri")) {
          std::istringstream tiss(tline);
          size_t idx, a, b, c;
          string s;
          if (!(tiss >> s >> idx >> a >> b >> c)) { cerr << "SEBURO MD5 Error - Failed to read triangle." << endl; break; }

          geometry->indices()[idx * 3] = c;
          geometry->indices()[idx * 3 + 1] = b;
          geometry->indices()[idx * 3 + 2] = a;

        }

        // Weights read

        else if (string_contains(tline, "numweights")) {
          std::istringstream tiss(tline);
          string s;
          size_t num_weights;
          if (!(tiss >> s >> num_weights)) { break; }
          skin = Skin(num_weights);
          mesh_node.add(skin);

          // MD5Mesh 
          weights = new md5_weight[num_weights];

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
          w.bias = bias;

          // Assuming the skeleton_ / joints appear first
          w.bone = skeleton_.bone(bone_id);
          skin.addWeight(w);

          // Annoyingly we need to double up so we can provide a GPU friendly skinning
          weights[idx].position = glm::vec3(p0,p1,p2);
          weights[idx].bias = bias;
          weights[idx].bone = bone_id;

        }

      }

      // Post process our vertices to get the basic initial pose and vertex position
      // This is our friendly bind pose      
      vector<Skin::SkinIndex> indices = skin.indices();

      for (size_t i =0; i < num_verts; ++i){
        
        // Setup the skin indices at this point
        Skin::SkinIndex si;
        si.index = verts[i].index;
        si.count = verts[i].count;
        indices.push_back(si);

        glm::vec3 pos (0.0f,0.0f,0.0f);

        for (size_t j = 0; j < geometry_max_bones; ++j){
         
          if ( si.count > geometry_max_bones)
            cerr << "SEBURO MD5 Error - Number of bones attached exceeds geometry_max_bones." << endl;

          if (j < si.count ) {
            md5_weight w = weights[si.index + j];
            geometry->vertices()[i].b[j] = w.bone;
            geometry->vertices()[i].w[j] = w.bias; 
          
            glm::vec3 bp = joints[w.bone].rotation * w.position;
            pos += ( (joints[w.bone].position + bp) * w.bias); // assuming all biases add up to 1
                     
          } else {
            geometry->vertices()[i].b[j] = 0;
            geometry->vertices()[i].w[j] = 0; 
          }
        }

        geometry->vertices()[i].p = pos;
        geometry->vertices()[i].u = glm::vec2(verts[i].s, verts[i].t);

        ///\todo precompute normals and tangents

      }
      skeleton_.resetGlobals();
      delete[] verts;
      delete[] weights;
    }
  }

  delete[] joints;
  ifs.close();

  
}


