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
#include "skeleton.hpp"


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
	class NodeBase;
	
	typedef std::shared_ptr<NodeBase> NodeBasePtr;

	/// Responsibilities that a node has - allows some casting
	typedef enum {
		GEOMETRY,
		CAMERA,
		POINT_LIGHTS,
		MATRIX,
		SKIN_WEIGHTS,
		SKELETON
	}NodeResponsibility;

	/**
	 * Decorator pattern - NodeDecorator allows nodes to be decorated with
	 * functionality and variables such as skin weights etc. We wrap each 
	 * kind of data we want to add to nodes and present to our shader contract
	 * present an enum and some data
	 */

	class NodeBase {
	public:
		NodeBase() {component_ = nullptr;}
		NodeBase(NodeBasePtr c) : component_(c)  {}
		virtual void 				draw() {};
		virtual std::string print() { return ""; };

	protected:

		NodeBasePtr						component_;	
		NodeResponsibility		responsible_;
	};


	/**
	 * NodeMinimal. All nodes have at least this level of decoration. This does not call
	 * any more nodes and essentially is the floor of the decoration.
	 */

	class NodeMinimal : public NodeBase {
		    	
	public:
		NodeMinimal() { responsible_ = MATRIX; };
		
		glm::mat4 	matrix() { return matrix_; } ;
		void 				set_matrix(const glm::mat4 &matrix) { matrix_ = matrix; } ;
		void 				draw() { };
		std::string print() { return " - Matrix"; }
		
		glm::mat4 matrix_;
						
	};

	/**
	 * A Camera Decorator
	 * \todo camera is not a shared object so this *should* be a ref or camera should be changed
	 */

	class NodeCamera : public NodeBase {
		    	
	public:
		NodeCamera(Camera c, NodeBasePtr p) : NodeBase(p) { camera_ = c; responsible_ = CAMERA; };

		///\todo override, presenting the camera data to the shader
		void 				draw() { component_->draw(); };
		std::string print() { return component_->print() + " - Camera"; }

		Camera			camera_;	
	
	};

	/**
	 * A Shape Decorator for node. Shapes are shared objects so can be implicitly copied
	 */

	class NodeShape : public NodeBase {
	public:
		NodeShape (Shape s,  NodeBasePtr p) : NodeBase(p), shape_(s) { responsible_ = GEOMETRY; brewed_ = false; };
		void draw();
		std::string print() { return component_->print() + " - Shape"; }
		
		Shape shape_;
		bool brewed_;
	};

	/**
	 * Add a skin to this node - essentially a shared object of weights
	 */

	class NodeSkin : public NodeBase {
	public:
		NodeSkin(Skin s, NodeBasePtr p) : NodeBase(p), skin_(s) { responsible_ = SKIN_WEIGHTS; };
		std::string print() { return component_->print() + " - Skin Weights"; }
		Skin skin_;
	};

  
  typedef std::shared_ptr<Node> NodePtr;

  /**
   * The Actual Node we instantiate and use in our code
   */

  class SEBUROAPI Node {
	public:
		Node() { base_ = std::shared_ptr<NodeMinimal>(new NodeMinimal()); node_ = base_; }

		// Overridden add methods for attaching things to this node.
		Node& add(Shape s);
		Node& add(NodePtr p) { return addChild(p); }
		Node& add(const Node &n) {NodePtr p = NodePtr(new Node()); *p = n; return *this; }
		Node& add(Skin s) { node_ = NodeBasePtr(new NodeSkin(s, node_)); return *this; }

		glm::mat4 matrix() {return base_->matrix(); }
		void set_matrix(const glm::mat4 &m) { base_->set_matrix(m); }

		Node& removeChild(NodePtr p);
		Node& addChild(NodePtr p) { children_.push_back(p); return *this; }
		Node& draw();

		friend std::ostream& operator<<(std::ostream& out, const Node& o);
		virtual ~Node() {}; 

	protected:
		std::vector<NodePtr> 					children_;
  	std::shared_ptr<NodeMinimal>  base_; // We keep this so we can always get to the matrix
  	NodeBasePtr 									node_;

  };

  inline std::ostream& operator<<(std::ostream& os, const Node& obj) {
    return os << "SEBURO Node with - " << obj.node_->print();
  }
}

#endif
