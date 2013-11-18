## Todo

* Issue with adding shared objects to nodes is that they can be un-initialised (shapes often are) and then we loose the binding to the name. Consider adding a camera before the Camera(pos) constructor is called - COMPILER should check the Node.add method for un-initialised objects and warn the user

* Problem with nodes is they hold a shape and we often want the type (TriMesh, Quad etc) back out ><
* Consider CPack with CMake for packaging examples and applications
* FILE Sorting and packaging linux vs osx vs windows
* Sponza Atrium Model
* Shadow maps and shadow volumes
* Lighting!
* A better route for Deferred Shading
* Procedural, 2 triangle stuff
* Error checking for 4.3 callback - http://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
* GLFW Built with GCC does not like being included with CLANG Built Seburo lib. Name mangling here? Rebuilt GLFW with clang.
* Shader version control with OpenGL
  * Shaders to be hard-coded and ready made as strings inside the app
* Resource loading cross platform
* Add QHull Library
* Add Freeimage Library for textures
* Add Lighting solutions
* Add Links to Blender
* More Shaders including Geometry
* Support for external application bundling
* Current Directory cross platform function
