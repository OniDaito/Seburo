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
#include "gl/drawable.hpp"


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
	
		struct SharedObj {
			std::vector<NodePtr> 	children;	
			glm::mat4 				matrix;
      	};

      	std::shared_ptr<SharedObj> obj_;
		
	public:
		Node() : obj_ ( std::shared_ptr<SharedObj>(new SharedObj())) {};

		// Defaulting to GL for now


		virtual ~Node() {}; 

		
		Node& translate(glm::vec3 p);
		Node& rotate(glm::vec3 r);

		Node& addChild(NodePtr p) { obj_->children.push_back(p); return *this; };
		Node& removeChild(NodePtr p);
		
		Node& yaw(float a);
		Node& pitch(float a);
		Node& roll(float a);

		glm::mat4 matrix() { return obj_->matrix; } ;
		void set_matrix(const glm::mat4 &matrix) { obj_->matrix = matrix; } ;

		virtual void draw();

		//@{
		//! Emulates shared_ptr-like behavior
		typedef std::shared_ptr<SharedObj> Node::*unspecified_bool_type;
		operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Node::obj_; }
		void reset() { obj_.reset(); }
		//@} 
						
	};

	
}

#endif
