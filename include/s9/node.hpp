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
#include "shapes.hpp"
#include "camera.hpp"
#include "skeleton.hpp"
#include "gl/texture.hpp"
#include "gl/shader.hpp"
#include "gl/utils.hpp"
#include "gl/utils.hpp"

#include <list>

/*
 * Node represents a *thing* that has a position in space and time (though not a size)
 * Node combines geometry with matrices to represent position in space. It can be matched with a shader
 * to create something that is drawable. It can also have children and thus create a scene graph
 *
 * \todo addition of extra things on this node to match with the shader
 * \todo node and its derivatives do not followed shared object model. Check this! 
 * \todo presenting with things like cameras only works for the currently bound shader.
 * if we override that shader with one in the child node, we dont get the one from the parent ><
 *
 */
 
namespace s9 {

	class Node;
	class NodeBase;
	
	typedef std::shared_ptr<NodeBase> NodeBasePtr;

	/// Responsibilities that a NodeBase has.
	/// Order here is important as objects are drawn in order - lowest first
	typedef enum {
		PREDRAW,
		SHADER,
		POINT_LIGHTS,
		SKIN_WEIGHTS,
		SKELETON,
		CAMERA,
		TEXTURE,
		MATRIX,
		CLAUSE,
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
		virtual void 					draw(GeometryPrimitive overide) { }

		/// Called before draw and collect
		virtual void 					preDraw() {}

		/// Operation called after this node has been drawn and we move up the tree
		virtual void 					postDraw() { }

		/// A std::string representation of this nodebase
		virtual std::string 	tag() { return ""; }
		
		/// What is this node responsible for?
		NodeResponsibility 		responsible() {return responsible_; }
	
		/// Allow a shader to come in and collect data
		virtual void collect (gl::ShaderVisitor &v ) {};
	

		bool operator < (const NodeBase& rhs) const {
			if (responsible_ < rhs.responsible_)
				return true;
			return false;
		}


		friend class Node;

	protected:

		NodeResponsibility		responsible_;

	};

	static bool compareNodeBasePtr( NodeBasePtr l, NodeBasePtr r){
 		return (*l) < (*r);
 	} 

	/**
	 * NodeMinimal. All nodes have at least this level of decoration. This does not call
	 * any more nodes and essentially is the floor of the decoration.
	 */

	class NodeMinimal : public NodeBase {
		    	
	public:
		///\todo I have no idea how this constructor can even work! Find out!
		NodeMinimal() : NodeBase(MATRIX), clause_matrix_("uModelMatrix", matrix_global_) { }
		
		glm::mat4 		matrix() { return matrix_; } ;
		void 					set_matrix( const glm::mat4 &matrix) { matrix_ = matrix;  } ;
		std::string 	tag() { return "Matrix"; }

		// Careful not to call this without calling the pop as well
		void preDraw() {
			if (matrix_stack_.size() > 0){
				matrix_global_ =  matrix_stack_.back() * matrix_;
			}
			else
				matrix_global_ = matrix_;

			matrix_stack_.push_back(matrix_global_);
		}

		void collect(gl::ShaderVisitor &v ) {
			v.sign(clause_matrix_); 
		}

		void					postDraw() {
			matrix_stack_.pop_back();
		}

		glm::mat4 matrix_;
		glm::mat4 matrix_global_;
		static std::vector<glm::mat4> matrix_stack_; // works because of the tree traversal
		gl::ShaderClause<glm::mat4,1> clause_matrix_; 
						
	};

	/**
	 * This class is a container for a clause - some uniform data to pass to the bound shader
	 */

	template<typename T, size_t U>
	class NodeClause : public NodeBase {
	public:
		NodeClause( gl::ShaderClause<T,U> c ) : NodeBase(CLAUSE), clause_(c) {}		

		void collect(gl::ShaderVisitor &v ) {
			v.sign(clause_);
				
		}

		std::string 	tag() { return "Clause"; }

		gl::ShaderClause<T,U> clause_; 

	};


	/**
	 * A Camera Decorator
	 * Cameras are odd because unless they are overridden, they are global in scope. 
	 * Therefore every node has a blank Camera that works in very much the same way as the
	 * matrix node above except we override instead of multiply the matrices
	 */

	class NodeCamera : public NodeBase {
		    	
	public:
		NodeCamera(Camera c) : NodeBase(CAMERA), camera_(c), 
			clause_camera_view_("uViewMatrix", view_matrix_global_), 
			clause_camera_projection_("uProjectionMatrix", projection_matrix_global_) {
				projection_matrix_global_ = glm::mat4(1.0f);
				view_matrix_global_ = glm::mat4(1.0f);
			} 

		NodeCamera() : NodeBase(CAMERA), 
			clause_camera_view_("uViewMatrix", view_matrix_global_), 
			clause_camera_projection_("uProjectionMatrix", projection_matrix_global_) {
				projection_matrix_global_ = glm::mat4(1.0f);
				view_matrix_global_ = glm::mat4(1.0f);
			} 

		void preDraw() {

			if (camera_){
				// Would call here but we override these matrices with the oculus 
				// Not sure what to do here - could set the camera completely with existing options?
				if (camera_.update_on_node_draw()) camera_.update();
				glViewport(camera_.left(), camera_.bottom(), camera_.width(), camera_.height());

				projection_matrix_global_ = camera_.projection_matrix();
				view_matrix_global_ = camera_.view_matrix();

				projection_matrix_stack_.push_back(projection_matrix_global_);
				view_matrix_stack_.push_back(view_matrix_global_);

			} else {
				if (projection_matrix_stack_.size() > 0){
					projection_matrix_global_ = projection_matrix_stack_.back();
				}
				if (view_matrix_stack_.size() > 0){
					view_matrix_global_ = view_matrix_stack_.back();
				}
			}	
			
		}

		void	postDraw() {
			// Pop if we've hit our camera node again
			if (camera_){
				projection_matrix_stack_.pop_back();
				view_matrix_stack_.pop_back();
			}
		}

	
		std::string 	tag() { return "Camera"; }
		void					collect(gl::ShaderVisitor &v ) { 
			v.sign(clause_camera_projection_); 
			v.sign(clause_camera_view_); 

		}

		Camera			camera_;	

		gl::ShaderClause<glm::mat4,1> clause_camera_projection_; 
		gl::ShaderClause<glm::mat4,1> clause_camera_view_; 

		glm::mat4 projection_matrix_global_;
		glm::mat4 view_matrix_global_;
		static std::vector<glm::mat4> projection_matrix_stack_;
		static std::vector<glm::mat4> view_matrix_stack_;

	};

	/**
	 * A Shape Decorator for node. Shapes are shared objects so can be implicitly copied
	 */

	class NodeShape : public NodeBase {
	public:
		NodeShape (Shape s) : NodeBase(GEOMETRY), shape_(s) { };
		void draw(GeometryPrimitive overide);
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
	 * Add a skeleton to this node - essentially a set of quaternions and positions
	 * represented as 4x2 matrices
	 */

	class NodeSkeleton : public NodeBase {
	public:
		NodeSkeleton (Skeleton s) : NodeBase(SKELETON), skeleton_(s){}
		std::string 	tag() { return "Skeleton"; }
		void					collect(gl::ShaderVisitor &v );

		Skeleton skeleton_;

	};

	/**
	 * Add a Shader to this node. This decorator bubbles to the top
	 */

	class NodeShader : public NodeBase {
	public:
		NodeShader(gl::Shader s) : NodeBase(SHADER), shader_(s) {  };
		std::string tag() { return "Shader"; }
		void preDraw(){ bind_count_++; }
		void draw(GeometryPrimitive overide) { shader_.bind(); 	}
		void postDraw() {shader_.unbind(); bind_count_--; }
		gl::Shader shader_;
		static size_t bind_count_;
	};

	/**
	 * Add a Texture to this node. We can have one per texture unit
	 * \todo consider the tree here. What if we bind textures that override others? 
	 * We dont at present go back to previous binding in history :S
	 */

	class NodeTexture : public NodeBase {
	public:
		NodeTexture(gl::Texture t) : NodeBase(TEXTURE), texture_(t) {  };
		std::string tag() { return "Texture"; }
		void draw(GeometryPrimitive overide) { texture_.bind(); }
		void postDraw() {texture_.unbind(); }

		gl::Texture texture_;
	};
	
  /**
   * The Actual Node we instantiate and use in our code
   * This is a shared object as we want to have node control 
   * just the bits it needs control over.
   */

  class SEBUROAPI Node {
	public:
		Node(); 
		Node(Shape s);

		// Overridden add methods for attaching things to this node.
		///\todo template these? We could do! :)
		///\todo normally we'd pass by reference. Should we do that?

		Node& add(Shape s);
		Node& add(Node &n);
		Node& add(Skin s);
		Node& add(gl::Shader s);
		Node& add(Camera c);
		Node& add(Skeleton s);
		Node& add(gl::Texture t);

		// Removals
		Node& remove(Shape s);
		Node& remove(Node &n) { removeChild(n); return *this; };
		Node& remove(Skin s);
		Node& remove(gl::Shader s);
		Node& remove(Camera c);
		Node& remove(Skeleton s);
		Node& remove(gl::Texture s);

		bool hasResponsibility(NodeResponsibility r) {
			for (NodeBasePtr b : obj_->bases){
				if (b->responsible() == r)
					return false;
			}
			return true;
		}

		template<typename T, size_t U>
		Node& add(gl::ShaderClause<T,U> c) {
			if (obj_ == nullptr) _init();
			obj_->bases.push_front( std::shared_ptr<NodeClause<T,U> >(new NodeClause<T,U>(c)) );
			obj_->bases.sort(compareNodeBasePtr);
			return *this;
		}

		glm::mat4 matrix();
		void set_matrix(const glm::mat4 &m);


		Node& removeChild(Node p);
		Node& draw(GeometryPrimitive gp = NONE);
		Node& clear();

		std::vector<Node> & children() { if (obj_ == nullptr) assert(false); return obj_->children; }

		void set_geometry_cast(GeometryPrimitive gc) {obj_->geometry_cast = gc; }

		friend std::ostream& operator<<(std::ostream& out, const Node& o);
		virtual ~Node() {}; 

	protected:

		void _init();
		NodeBasePtr getBase(NodeResponsibility r);
		Node& remove(NodeBasePtr p);


		struct SharedObject {
			virtual void update() {} // A Cheeky function that allows overriding in subclasses 
			
			std::vector< Node > 						children;
  		std::shared_ptr<NodeMinimal>  	matrix_node; 			// We keep this so we can always get to the matrix
  		std::shared_ptr<NodeShader>			shader_node; 			// Like the above, this is handy for adding data values to the shader
  		std::shared_ptr<NodeCamera>			camera_node;			// Temporary / Blank Camera Node
  		std::list<NodeBasePtr> 					bases;
  		GeometryPrimitive 							geometry_cast;		///\todo potentially replace this with something else we can pass to draw? User stuff
		};

		std::shared_ptr<SharedObject> obj_ = nullptr;
		
	public:

    bool operator == (const Node &ref) const { return this->obj_ == ref.obj_; }
    typedef std::shared_ptr<SharedObject> Node::*unspecified_bool_type;
    operator unspecified_bool_type() const { return ( obj_.get() == 0 ) ? 0 : &Node::obj_; }
    void reset() { obj_.reset(); }

  };

  inline std::ostream& operator<<(std::ostream& os, const Node& obj) {
   
    if (obj.obj_ != nullptr){
    	os << "SEBURO Node with";
  		for (NodeBasePtr p : obj.obj_->bases)
  			os << " - " << p->tag();
  	} else {
  		os << "SEBURO Node UN-INITIALISED!";
  	}
  	return os << std::endl;
  }
}

#endif
