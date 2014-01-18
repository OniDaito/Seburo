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
  vector<glm::vec2> texcoords;
  vector<IndicesType> indices; // assume triangles for now
  vector<IndicesType> texindices;

  while (std::getline(ifs, line)){
    
    if (line.compare(0, 6, "mtllib") == 0){
      string mtl;
      std::istringstream v(line.substr(6));
      v >> mtl;
      cout << "SEBURO ObjMesh Material Found: " << mtl << endl;
      ///\todo materials

    }

    if ( line.compare(0, 2, "o ") == 0) {
      // We have our first mesh object
      // At present, all objects must have a name to be considered for rendering

      string mesh_name;
      std::istringstream v(line.substr(2));
      v >> mesh_name;

      cout << "SEBURO ObjMesh Mesh Found: " << mesh_name << endl;
      
      if (meshid > 0) {

        ///\todo normals

        TriMesh trimesh = TriMesh(vertices.size(), indices.size());
        const GeometryT<Vertex3, Face3, AllocationPolicyNew> *geometry = trimesh.geometry();

        for (size_t i = 0; i < vertices.size(); ++i){
          geometry->vertices()[i].p = vertices[i];
          
          if (texindices.size() > 0){
            if (texindices[i] < texcoords.size() )
              geometry->vertices()[i].u = texcoords[ texindices[i] ];
          }
         
        }

        geometry->SetIndices(indices.data());

        Node mesh_node;
        mesh_node.Add(trimesh);
        Add(mesh_node);

        vstart = vertices.size();
        tstart = texcoords.size();

        vertices.clear();
        texcoords.clear();
        indices.clear();
        texindices.clear();
      }

      meshid++;

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
       
          }
        }  
      }
    }
  }

  if (meshid > 0){
    TriMesh trimesh = TriMesh(vertices.size(), indices.size());
    const GeometryT<Vertex3, Face3, AllocationPolicyNew> *geometry = trimesh.geometry();

    for (size_t i = 0; i < vertices.size(); ++i){
      geometry->vertices()[i].p = vertices[i];
      
      if (texindices.size() > 0){
        if (texindices[i] < texcoords.size() )
          geometry->vertices()[i].u = texcoords[ texindices[i] ];
      }
     
    }

    geometry->SetIndices(indices.data());

    Node mesh_node;
    mesh_node.Add(trimesh);
    Add(mesh_node);
  }
  

}