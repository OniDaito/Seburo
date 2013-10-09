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

		/// This is overridden by nodes with attached drawables - the DNode
		virtual void drawToScreen() {};  
	
		glm::mat4 _getMatrix(NodePtr p, glm::mat4 m);
		std::vector<NodePtr> children_;	
		glm::mat4 matrix_;

		
	public:
		Node() {
			matrix_ = glm::mat4(1.0f);
		}

	//	virtual operator int() const { return mObj.use_count() > 0; };

		virtual ~Node(); 
		
		Node& translate(glm::vec3 p);
		NodePtr rotate(glm::vec3 r);

		NodePtr addChild(NodePtr p) { children_.push_back(p); return std::shared_ptr<Node>(this); };
		NodePtr removeChild(NodePtr p);
		
		NodePtr yaw(float a);
		NodePtr pitch(float a);
		NodePtr roll(float a);


		virtual void draw();
						
	};

	
}

#endif
