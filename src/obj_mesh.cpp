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

void ObjMesh::CreateNodes() {

  // We should have a set of TempMesh at this point - one for each material
/*

  TriMesh trimesh = TriMesh(temp_.vertices.size(), temp_.indices.size());
  const GeometryT<Vertex3, Face3, AllocationPolicyNew> *geometry = trimesh.geometry();

  for (size_t i = 0; i < temp_.vertices.size(); ++i){
    geometry->vertices()[i].p = temp_.vertices[i];
    
    if (temp_.texindices.size() > 0){
      if (temp_.texindices[i] < temp_.texcoords.size() )
        geometry->vertices()[i].u = temp_.texcoords[ temp_.texindices[i] ];
    }

    if (temp_.normalindices.size() > 0) {
      if (temp_.normalindices[i] < temp_.normals.size() )
        geometry->vertices()[i].n = temp_.normals[ temp_.normalindices[i] ];
    }
  }

  geometry->SetIndices(temp_.indices.data());

  Node mesh_node;
  mesh_node.Add(trimesh);


  Add(mesh_node);

  vstart = temp_.vertices.size();
  tstart = temp_.texcoords.size();

  // Clear out temp mesh structure
  temp_.vertices.clear();
  temp_.texcoords.clear();
  temp_.normals.clear();
  temp_.indices.clear();
  temp_.texindices.clear();
  temp_.normalindices.clear();*/

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

 
  ///\todo can we make some guess as to how big our global vertices vectors and such need to be?

  // read the lines
  std::string line;

  TempMesh * current_mesh = nullptr;
  
  using_materials_ = false;

  while (std::getline(ifs, line)){
    
    if (line.compare(0, 6, "mtllib") == 0){
      using_materials_ = true;

      // This line MUST come before all the actual vertex and face data
      string mtl;
      std::ifstream ifs_mtl;
      std::istringstream v(line.substr(6));
      v >> mtl;
      cout << "SEBURO ObjMesh Material Found: " << mtl << endl;
      
      // Look through the files for this MTLLib file
      // We divide our mesh by material, so each material is basically a new node

      for (File f : dir_files){
        if (StringContains(f.final_path(), mtl )){
          ifs_mtl.open (f.final_path(), std::ifstream::in);
          // Parse MTL File, creating a material
          
          size_t mat_idx = 0;

          TempMesh new_mesh;
          bool existing = false;

          while (std::getline(ifs_mtl, line)){
            if ( line.compare(0, 7, "newmtl ") == 0){
              
              if (existing) {
                temp_.push_back(new_mesh);
              } else {
                existing = true;
              }
              
              std::istringstream v(line.substr(7));
              v >> new_mesh.tag;
              cout << "SEBURO ObjMesh - New material node created: " << new_mesh.tag << endl;
            }
            else if (line.compare(0, 2, "Ka") == 0) {
              std::istringstream v(line.substr(2));
              float_t x,y,z;
              v >> x >> y >> z;
              new_mesh.material.ambient = glm::vec4(x,y,z,1.0f);
            }

            else if (line.compare(0, 2, "Kd") == 0) {
              std::istringstream v(line.substr(2));
              float_t x,y,z;
              v >> x >> y >> z;
              new_mesh.material.diffuse = glm::vec4(x,y,z,1.0f);
            }

            else if (line.compare(0, 2, "Ks") == 0) {
              std::istringstream v(line.substr(2));
              float_t x,y,z;
              v >> x >> y >> z;
              new_mesh.material.specular = glm::vec4(x,y,z,1.0f);
              
            }

            else if (line.compare(0, 2, "Ns") == 0) {
              std::istringstream v(line.substr(2));
              v >> new_mesh.material.shine;
            }

            else if (line.compare(0,7, "map_Kd ") == 0){

              // Create the diffuse temp_.textures
              std::istringstream v(line.substr(7));
              string filename;
              v >> filename;
              // Hunt in the directory for the temp_.textures we need
              for (File f : dir_files){
                if (StringContains(f.final_path(), filename )){ ///\todo this may fail with multiple texture files that contain the filename :S
                  cout << "SEBURO ObjMesh Diffuse Texture - Found: " << f.final_path() << endl;
                  Image img(f);
                  new_mesh.texture = gl::Texture(f);
                  break;
                }
              }
             
            }

            ///\todo specular, and other temp_.textures we should probably load
          }

          if (existing){
            temp_.push_back(new_mesh); 
            current_mesh = &(temp_[0]);
          }


          ifs_mtl.close();
        } 
      }
    }

    // Now the actual vertices, faces and similar


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

 /*   else if (line.compare(0,7, "usemtl ") == 0) {
      // We have a material change. Check to see if this is the same as the current mesh material
      // If it is, do nothing. If it isnt, set the offset and switch material

      ///\todo it appears that blender exports spaces as _ but can we be sure about this with OBJs?
      string name;
      std::istringstream v(line.substr(7));
      v >> name;
      if (current_mesh->tag.compare(name) != 0) {
        // We need to update offsets and switch meshes
        TempMesh * new_mesh = nullptr;
        for (size_t i=0; i< temp_.size(); ++i){
          if (temp_[i].tag.compare(name) == 0){
            new_mesh = &(temp_[i]);
          }
        }

        assert(new_mesh != nullptr);
        
        new_mesh->offset_vert = current_mesh->vertices.size() + current_mesh->offset_vert;
        new_mesh->offset_tex = current_mesh->texcoords.size() + current_mesh->offset_tex;
        new_mesh->offset_norm = current_mesh->normals.size() + current_mesh->offset_norm;

        current_mesh = new_mesh;
      }

    }*/

    else if(line.compare(0, 2, "f ") == 0){
      // We have a face definition
      // It seems that faces come after all their vertices are defined.

      std::istringstream v(line.substr(2));

      ///\todo figure out this code
      vector<string> verts{istream_iterator<string>{v},
         istream_iterator<string>{}};

      if (verts.size() == 3) {
        // This is a triangle so do the normal thing
        for (size_t i = 0; i < verts.size(); ++i){

          // Split on / if there is any
          vector<string> tokens = SplitStringChars(verts[i],"/");
          
          // We subtract the offsets of the current mesh as OBJ files index into one master array
          // whereas we want to index into seperate arrays per temp mesh 
        /*  IndicesType tx = FromStringS9<IndicesType>(tokens[0]);

          // We have a vertex shared between two faces in different meshes so we must duplicate and add a new one
          if (tx < current_mesh->offset_vert){ ///\todo < or <= ?
            current_mesh->vertices.push_back(vertices[tx]);
            current_mesh->indices.push_back(current_mesh->vertices.size() + current_mesh->vertices.size() - 2 );
          } else 
            current_mesh->indices.push_back( tx - 1 - current_mesh->offset_vert);
            
          if (tokens.size() > 1){
            if (tokens[1].length() > 0)
              current_mesh->texindices.push_back( FromStringS9<IndicesType>(tokens[1]) - 1 - current_mesh->offset_tex);
            if (tokens.size() > 2)
              current_mesh->normalindices.push_back(FromStringS9<IndicesType>(tokens[2]) - 1 - current_mesh->offset_norm);
          } */
        }
      } else if (verts.size() == 4){
        // We need to triangulate as this is a silly quad that we dont like ><

        vector<IndicesType> tempi;
        vector<IndicesType> tempn;
        vector<IndicesType> tempt;

        for (size_t i = 0; i < verts.size(); ++i){

          // Split on / if there is any
          vector<string> tokens = SplitStringChars(verts[i],"/");
          tempi.push_back( FromStringS9<IndicesType>(tokens[0]) - 1);
            
          if (tokens.size() > 1){
            if (tokens[1].length() > 0)
              tempt.push_back( FromStringS9<IndicesType>(tokens[1]) - 1);
            if (tokens.size() > 2)
              tempn.push_back(FromStringS9<IndicesType>(tokens[2]) - 1 );
          }
        }

        // Triangulate the quad 
        current_mesh->indices.push_back( tempi[0] );
        current_mesh->indices.push_back( tempi[1] );
        current_mesh->indices.push_back( tempi[2] );
        current_mesh->indices.push_back( tempi[0] );
        current_mesh->indices.push_back( tempi[2] );
        current_mesh->indices.push_back( tempi[3] );

        if (tempn.size() == 4 ){
          current_mesh->normalindices.push_back( tempn[0] );
          current_mesh->normalindices.push_back( tempn[1] );
          current_mesh->normalindices.push_back( tempn[2] );
          current_mesh->normalindices.push_back( tempn[0] );
          current_mesh->normalindices.push_back( tempn[2] );
          current_mesh->normalindices.push_back( tempn[3] );
        }
        
        if (tempt.size() == 4){
          current_mesh->texindices.push_back( tempt[0] );
          current_mesh->texindices.push_back( tempt[1] );
          current_mesh->texindices.push_back( tempt[2] );
          current_mesh->texindices.push_back( tempt[0] );
          current_mesh->texindices.push_back( tempt[2] );
          current_mesh->texindices.push_back( tempt[3] );
        }
      }
    }
  }

  vertices.clear();
  normals.clear();
  texcoords.clear();
  
  ifs.close();
}