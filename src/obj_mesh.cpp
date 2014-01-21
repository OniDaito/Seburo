/**
* @brief OBJ Parsing Class
* @file obj_mesh.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 17/01/2014
*
*/

#include "s9/obj_mesh.hpp"

using namespace std;
using namespace s9;


ObjMesh::ObjMesh(const s9::File &file) : Node() {
  Parse(file);
}

void ObjMesh::AddObjMesh( vector<glm::vec3> &vertices,
  vector<glm::vec3> &normals,
  vector<glm::vec2> &texcoords,
  vector<IndicesType> &indices, // assume triangles for now
  vector<IndicesType> &texindices,
  vector<IndicesType> &normalindices,
  vector<Material> &materials,
  vector<gl::Texture> &textures,
  int mesh_material,
  int mesh_texture,
  size_t &vstart,
  size_t &tstart ) {
  
  TriMesh trimesh = TriMesh(vertices.size(), indices.size());
  const GeometryT<Vertex3, Face3, AllocationPolicyNew> *geometry = trimesh.geometry();

  for (size_t i = 0; i < vertices.size(); ++i){
    geometry->vertices()[i].p = vertices[i];
    
    if (texindices.size() > 0){
      if (texindices[i] < texcoords.size() )
        geometry->vertices()[i].u = texcoords[ texindices[i] ];
    }

    if (normalindices.size() > 0) {
      if (normalindices[i] < normals.size() )
        geometry->vertices()[i].n = normals[ normalindices[i] ];
    }
  }

  geometry->SetIndices(indices.data());

  Node mesh_node;
  mesh_node.Add(trimesh);

  if (mesh_material != -1)
    mesh_node.Add(materials[mesh_material]);

  if (mesh_texture != -1)
    mesh_node.Add(textures[mesh_material]);
  
  Add(mesh_node);

  vstart = vertices.size();
  tstart = texcoords.size();

  vertices.clear();
  texcoords.clear();
  normals.clear();
  indices.clear();
  texindices.clear();
  normalindices.clear();
}


/// Parse the Obj File. At the moment, only accept triangulated mesh files
void ObjMesh::Parse(const s9::File &file) {

  std::ifstream ifs;
  ifs.open (file.final_path(), std::ifstream::in);

  Directory directory (file.DirectoryName());
  std::vector<File> dir_files = directory.ListFiles();

  cout << "SEBURO ObjMesh Directory Listing..." << endl;
  for (File f : dir_files){
     cout << "  " << f.final_path() << endl;
  }

 
  // read the lines
  std::string line;

  int meshid = 0;
  size_t vstart = 0;
  size_t tstart = 0;

  vector<glm::vec3> vertices;
  vector<glm::vec3> normals;
  vector<glm::vec2> texcoords;
  vector<IndicesType> indices; // assume triangles for now
  vector<IndicesType> texindices;
  vector<IndicesType> normalindices;

  vector<Material> materials;
  vector<gl::Texture> textures;

  int mesh_material = -1;
  int mesh_texture = -1;

  while (std::getline(ifs, line)){
    
    if (line.compare(0, 6, "mtllib") == 0){
      string mtl;
      std::ifstream ifs_mtl;
      std::istringstream v(line.substr(6));
      v >> mtl;
      cout << "SEBURO ObjMesh Material Found: " << mtl << endl;
      
      // Look through the files for this MTLLib file
      for (File f : dir_files){
        if (StringContains(f.final_path(), mtl )){
          ifs_mtl.open (f.final_path(), std::ifstream::in);
          // Parse MTL File, creating a material
          
          size_t mat_idx = 0;

          while (std::getline(ifs_mtl, line)){
            if ( line.compare(0, 7, "newmtl ") == 0){
              materials.push_back(Material());
              textures.push_back(gl::Texture()); // Push back a blank texture in case we need one
              mat_idx = materials.size() -1;
              std::istringstream v(line.substr(7));
              v >> materials[mat_idx].name;
            }
            else if (line.compare(0, 2, "Ka") == 0) {
              std::istringstream v(line.substr(2));
              float_t x,y,z;
              v >> x >> y >> z;
              materials[mat_idx].ambient = glm::vec4(x,y,z,1.0f);
            }

            else if (line.compare(0, 2, "Kd") == 0) {
              std::istringstream v(line.substr(2));
              float_t x,y,z;
              v >> x >> y >> z;
              materials[mat_idx].diffuse = glm::vec4(x,y,z,1.0f);
            }

            else if (line.compare(0, 2, "Ks") == 0) {
              std::istringstream v(line.substr(2));
              float_t x,y,z;
              v >> x >> y >> z;
              materials[mat_idx].specular = glm::vec4(x,y,z,1.0f);
              
            }

            else if (line.compare(0, 2, "Ns") == 0) {
              std::istringstream v(line.substr(2));
              v >> materials[mat_idx].shine;
            }

            else if (line.compare(0,7, "map_Kd ") == 0){

              // Create the diffuse textures
              std::istringstream v(line.substr(7));
              string filename;
              v >> filename;
              // Hunt in the directory for the textures we need
              for (File f : dir_files){
                if (StringContains(f.final_path(), filename )){ ///\todo this may fail with multiple texture files that contain the filename :S
                  cout << "SEBURO ObjMesh Diffuse Texture - Found: " << f.final_path() << endl;
                  Image img(f);
                  textures[mat_idx] = gl::Texture(f);
                  break;
                }
              }
             
            }

            ///\todo specular, and other textures we should probably load

          }          
          ifs_mtl.close();
        } 
      }
    }

    if ( line.compare(0, 2, "o ") == 0) {
      // We have our first mesh object
      // At present, all objects must have a name to be considered for rendering

      string mesh_name;
      std::istringstream v(line.substr(2));
      v >> mesh_name;

      cout << "SEBURO ObjMesh Mesh Found: " << mesh_name << endl;
      
      if (meshid > 0) {
        AddObjMesh(vertices,normals,texcoords,indices,texindices,
          normalindices,materials,textures,mesh_material,mesh_texture,
          vstart,tstart);
      }

      meshid++;
      mesh_material = -1;
      mesh_texture = -1;
    } 

    if ( line.compare(0, 2, "v ") == 0) {

      std::istringstream v(line.substr(2));
      float_t x,y,z;
      v >> x >> y >> z;
      vertices.push_back(glm::vec3(x,y,z));
    
    } 
    
    else if(line.compare(0, 2, "vt") == 0){

      std::istringstream v(line.substr(3));
      glm::vec2 tex;
      float_t U,V;
      v>>U; v>>V;
      tex= glm::vec2(U,V);
      texcoords.push_back(tex);

    } 

    else if(line.compare(0, 2, "vn") == 0){

      std::istringstream v(line.substr(3));
      float_t x,y,z;
      v >> x >> y >> z;
      normals.push_back(glm::vec3(x,y,z));
    } 

    else if (line.compare(0,7, "usemtl ") == 0) {
      // We have a material on this mesh. We assume that the materials are loaded already 
      ///\todo it appears that blender exports spaces as _ but can we be sure about this with OBJs?
      string name;
      std::istringstream v(line.substr(7));
      v >> name;

      int idx = 0;
      for (Material m : materials) {
        if (m.name.compare(name) == 0) {
          mesh_material = idx;

          // check to see if this material has a texture
          if (textures[mesh_material]){
            mesh_texture = idx;
          }

          break;
        }
        idx++;
      }

    }

    else if(line.compare(0, 2, "f ") == 0){

      string face_raw[3];
      std::istringstream v(line.substr(2));

      // Assuming triangles
      v >> face_raw[0] >> face_raw[1] >> face_raw[2];

      // Split on / if there is any 
      for (size_t i = 0; i < 3; ++i){
        vector<string> tokens = SplitStringChars(face_raw[i],"/");
        indices.push_back( FromStringS9<IndicesType>(tokens[0]) - 1 - vstart );
        //idx[i] = FromStringS9<size_t>(tokens[0]);

        if (tokens.size() > 1){
          // texture co-ords
          if (tokens[1].length() > 0){
            texindices.push_back( FromStringS9<IndicesType>(tokens[1]) - 1 - tstart);
    
          }
          if (tokens.size() > 2){
            //normals
            normalindices.push_back(FromStringS9<IndicesType>(tokens[2]) - 1 - tstart);
          }
        }  
      }
    }
  }

  if (meshid > 0){
    AddObjMesh(vertices,normals,texcoords,indices,texindices,
        normalindices,materials,textures,mesh_material,mesh_texture,
        vstart,tstart);
  }
  
  ifs.close();
}