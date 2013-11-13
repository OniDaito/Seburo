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
#include "gl/shader.hpp"
#include "gl/utils.hpp"

#include <forward_list>

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
	/// Order here is important as objects are drawn in order - lowest first
	typedef enum {
		SHADER,
		POINT_LIGHTS,
		SKIN_WEIGHTS,
		SKELETON,
		CAMERA,
		MATRIX,
		GEOMETRY
	}NodeResponsibility;

	/**
	 * NodeBase and its subclasses form the optional building blocks of a node.
	 * They can be added
	 */

	class NodeBase {
	public:
		NodeBase(NodeResponsibility r) { responsible_ = r; }
	
		/// Called by the Node's draw method and sets up the shader / geometry
		virtual void 					draw() { }

		/// Operation called after this node has been drawn and we move up the tree
		virtual void 					postDraw() { }

		/// A std::string representation of this nodebase
		virtual std::string 	tag() { return ""; }
		
		/// What is this node responsible for?
		NodeResponsibility 		responsible() {return responsible_; }
	

		virtual void sign (gl::ShaderVisitor &v ) {};
	

		bool operator < (const NodeBase& rhs) const {
			if (responsible_ < rhs.responsible_)
				return true;
			return false;
		}

		friend class Node;

	protected:

		NodeResponsibility		responsible_;

	};


	/**
	 * NodeMinimal. All nodes have at least this level of decoration. This does not call
	 * any more nodes and essentially is the floor of the decoration.
	 */

	class NodeMinimal : public NodeBase {
		    	
	public:
		///\todo I have no idea how this constructor can even work! Find out!
		NodeMinimal() : NodeBase(MATRIX), clause_matrix_("uModelMatrix", matrix_global_) { }
		
		glm::mat4 		matrix() { return matrix_; } ;
		void 					set_matrix( const glm::mat4 &matrix) {matrix_ = matrix;  } ;
		std::string 	tag() { return "Matrix"; }
		// Careful not to call this without calling the pop as well
		void					sign(gl::ShaderVisitor &v ) {
			if (matrix_stack_.size() > 0){
				matrix_global_ =  matrix_stack_.back() * matrix_;
			}
			else
				matrix_global_ = matrix_;
			
			matrix_stack_.push_back(matrix_global_);
			v.sign(clause_matrix_); 
		}

		void					postDraw() {
			matrix_stack_.pop_back();
		}

		glm::mat4 matrix_;
		glm::mat4 matrix_global_;
		static std::vector<glm::mat4> matrix_stack_;
		gl::ShaderClause<glm::mat4> clause_matrix_; 
						
	};

	/**
	 * A Camera Decorator
	 * \todo camera is not a shared object so this *should* be a ref or camera should be changed
	 */

	class NodeCamera : public NodeBase {
		    	
	public:
		NodeCamera(Camera c) : NodeBase(CAMERA), camera_(c), 
			clause_camera_view_("uViewMatrix", c.view_matrix()), 
			clause_camera_projection_("uProjectionMatrix", c.projection_matrix()) {} 
	
		std::string 	tag() { return "Camera"; }
		void					sign(gl::ShaderVisitor &v ) { v.sign(clause_camera_projection_); v.sign(clause_camera_view_); }

		Camera			camera_;	

		gl::ShaderClause<glm::mat4> clause_camera_projection_; 
		gl::ShaderClause<glm::mat4> clause_camera_view_; 
	
	};

	/**
	 * A Shape Decorator for node. Shapes are shared objects so can be implicitly copied
	 */

	class NodeShape : public NodeBase {
	public:
		NodeShape (Shape s) : NodeBase(GEOMETRY), shape_(s) { };
		void draw();
		std::string tag() { return "Shape"; }
		Shape shape_;
	};

	/**
	 * Add a skin to this node - essentially a shared object of weights
	 */

	class NodeSkin : public NodeBase {
	public:
		NodeSkin(Skin s) : NodeBase(SKIN_WEIGHTS), skin_(s) { };
		std::string 	tag() { return "Skin Weights"; }
		Skin skin_;
	};

	/**
	 * Add a Shader to this node. This decorator bubbles to the top
	 */

	class NodeShader : public NodeBase {
	public:
		NodeShader(gl::Shader s) : NodeBase(SHADER), shader_(s) {  };
		std::string tag() { return "Shader"; }
		void draw() {	shader_.bind(); }
		void postDraw() {shader_.unbind(); }
		gl::Shader shader_;
	};

	
  /**
   * The Actual Node we instantiate and use in our code
   */

  class SEBUROAPI Node {
	public:
		Node(); 

		// Overridden add methods for attaching things to this node.
		///\todo template these? We could do! :)
		Node& add(Shape &s);
		Node& add(Node &n);
		Node& add(Skin &s);
		Node& add(gl::Shader &s);
		Node& add(Camera &c);

		glm::mat4 matrix();
		void set_matrix(const glm::mat4 &m);

		Node& removeChild(Node p);
		Node& draw();
		Node& reset();

		friend std::ostream& operator<<(std::ostream& out, const Node& o);
		virtual ~Node() {}; 

	protected:
		void _init();
		NodeBasePtr getBase(NodeResponsibility r);

		struct SharedObject{
			std::vector<Node> 							children;
  		std::shared_ptr<NodeMinimal>  	matrix_node; 			// We keep this so we can always get to the matrix
  		std::shared_ptr<NodeShader>			shader_node; 			// Like the above, this is handy for adding data values to the shader
  		std::forward_list<NodeBasePtr> 	bases;

		};

		std::shared_ptr<SharedObject> obj_ = nullptr;

  };

  inline std::ostream& operator<<(std::ostream& os, const Node& obj) {
    os << "SEBURO Node with";
  	for (NodeBasePtr p : obj.obj_->bases)
  		os << " - " << p->tag();
  	return os << std::endl;
  }
}

#endif
