/**
* @brief Node Classes
* @file primitive.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/


#ifndef S9_NODE_HPP
#define S9_NODE_HPP

#include "common.hpp"
#include "visualapp.hpp"
#include "shapes.hpp"
#include "camera.hpp"


/*
 * Node represents a *thing* that has a position in space and time (though not a size)
 * Node combines geometry with matrices to represent position in space. It can be matched with a shader
 * to create something that is drawable. It can also have children and thus create a scene graph
 *
 * \todo addition of extra things on this node to match with the shader
 * \todo node and its derivatives do not followed shared object model. Check this! 
 *
 */
 
namespace s9 {

	class Node;
	
	typedef std::shared_ptr<Node> NodePtr;

	class SEBUROAPI Node {
		
	protected:

		virtual void drawToScreen() {};  

		// No shared object as a node is either just copyable data OR shared_ptrs
    glm::mat4 							matrix_;
    std::vector<NodePtr> 		children_;	
    Camera									camera_;
    Shape 									geometry_;
		
	public:
		Node()  {};

		// Defaulting to GL for now

		virtual ~Node() {}; 

		// Overridden add methods for attaching things to this node.
		Node& add(Shape &shape);
		Node& add(NodePtr p) {return addChild(p); }
		
		Node& translate(glm::vec3 p);
		Node& rotate(glm::vec3 r);

		Node& addChild(NodePtr p) { children_.push_back(p); return *this; };
		Node& removeChild(NodePtr p);
		
		Node& yaw(float a);
		Node& pitch(float a);
		Node& roll(float a);

		glm::mat4 matrix() { return matrix_; } ;
		void set_matrix(const glm::mat4 &matrix) { matrix_ = matrix; } ;

		void draw();
						
	};

	
}

#endif
