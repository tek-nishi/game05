
#pragma once

// 
// 行列関連
//

#include <vector>
#include <cassert>
#include "Eigen/Core"
#include "Eigen/Geometry"
#include "co_defines.hpp"


namespace ngs {

typedef Eigen::Matrix4f							 Mat4f;
typedef Eigen::Matrix3f							 Mat3f;
typedef Eigen::Matrix<GLfloat, 2, 1> Vec2f;
typedef Eigen::Matrix<GLfloat, 3, 1> Vec3f;
typedef Eigen::Matrix<GLfloat, 4, 1> Vec4f;
typedef Eigen::Matrix<GLint, 2, 1>	 Vec2i;


class Matrix {
public:
	enum mode {
		PROJECTION,
		MODELVIEW,
	};

private:
	std::vector<Mat4f> projection_stack_;
	std::vector<Mat4f> model_stack_;
	std::vector<Mat4f> *current_;

	enum mode mode_;
	
public:
	Matrix() :
		current_(&projection_stack_),
		mode_(PROJECTION)
	{
		projection_stack_.push_back(Mat4f::Identity());
		model_stack_.push_back(Mat4f::Identity());
	}

	void mode(const enum mode mode) {
		mode_ = mode;
		switch (mode)
		{
		case PROJECTION:
			current_ = &projection_stack_;
			break;

		case MODELVIEW:
			current_ = &model_stack_;
			break;
		
		default:
			assert(0);
		}
	}

	Mat4f& current() { return current_->back(); }
	Mat4f& projection() { return projection_stack_.back(); }
	Mat4f& model() { return model_stack_.back(); }

	void push() { current_->push_back(current_->back()); }
	void pop() { current_->pop_back(); }
};

Matrix matrix_stack;
Mat3f normal_matrix;

Mat4f& getCurrentMatrix()
{
	return matrix_stack.current();
}

Mat4f& getProjectionMatrix()
{
	return matrix_stack.projection();
}

Mat4f& getModelMatrix()
{
	return matrix_stack.model();
}

// モデルマトリックスの逆行列→3x3
Mat3f& getNormalMatrix()
{
	Mat4f tmp;
	tmp = getModelMatrix();
	tmp.inverse();
	normal_matrix = tmp.block(0,0,3,3);
	// 4x4 to 3x3

	return normal_matrix;
}

void setMatrixMode(const enum Matrix::mode mode)
{
	matrix_stack.mode(mode);
}

void loadIdentity()
{
	matrix_stack.current() = Mat4f::Identity();
}

void pushMatrix()
{
	matrix_stack.push();
}

void popMatrix()
{
	matrix_stack.pop();
}


void rotateMatrix(const GLfloat angle, const GLfloat x, const GLfloat y, const GLfloat z)
{
	Eigen::Affine3f m;
	m = Eigen::Quaternion<GLfloat>(Eigen::AngleAxis<GLfloat>(angle, Vec3f(x, y, z)));
	
	Mat4f& cur = getCurrentMatrix();
	cur *= m.matrix();
}

void scaleMatrix(const GLfloat x, const GLfloat y, const GLfloat z)
{
	Eigen::Affine3f m;
	m = Eigen::Scaling(x, y, z);

	Mat4f& cur = getCurrentMatrix();
	cur *= m.matrix();
}

void translateMatrix(const GLfloat x, const GLfloat y, const GLfloat z)
{
	Eigen::Affine3f m;
	m = Eigen::Translation<GLfloat, 3>(x, y, z);
	
	Mat4f& cur = getCurrentMatrix();
	cur *= m.matrix();
}

void loadMatrix(const Mat4f& mt)
{
	Mat4f& cur = getCurrentMatrix();
	cur = mt;
}

void multMatrix(const Mat4f& mt)
{
	Mat4f& cur = getCurrentMatrix();
	cur *= mt;
}
	

// 正投影行列を生成して、カレントの行列に掛け合わせる
void orthoMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearval, GLfloat farval)
{
	Mat4f m;

	m(0,0) = 2.0 / (right-left);
	m(0,1) = 0.0;
	m(0,2) = 0.0;
	m(0,3) = -(right+left) / (right-left);

	m(1,0) = 0.0;
	m(1,1) = 2.0 / (top-bottom);
	m(1,2) = 0.0;
	m(1,3) = -(top+bottom) / (top-bottom);

	m(2,0) = 0.0;
	m(2,1) = 0.0;
	m(2,2) = -2.0 / (farval-nearval);
	m(2,3) = -(farval+nearval) / (farval-nearval);

	m(3,0) = 0.0;
	m(3,1) = 0.0;
	m(3,2) = 0.0;
	m(3,3) = 1.0;

	Mat4f& cur = getCurrentMatrix();
	cur *= m;
}

// 透視投影行列を生成して、カレントの行列に掛け合わせる
void frustumMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearval, GLfloat farval)
{
	GLfloat x, y, a, b, c, d;
	Mat4f m;

	x = (2.0*nearval) / (right-left);
	y = (2.0*nearval) / (top-bottom);
	a = (right+left) / (right-left);
	b = (top+bottom) / (top-bottom);
	c = -(farval+nearval) / ( farval-nearval);
	d = -(2.0*farval*nearval) / (farval-nearval);  /* error? */

	m(0,0) = x;
	m(0,1) = 0.0;
	m(0,2) = a;
	m(0,3) = 0.0;
	 
	m(1,0) = 0.0;
	m(1,1) = y;
	m(1,2) = b;
	m(1,3) = 0.0;
	 
	m(2,0) = 0.0;
	m(2,1) = 0.0;
	m(2,2) = c;
	m(2,3) = d;
	 
	m(3,0) = 0.0;
	m(3,1) = 0.0;
	m(3,2) = -1.0;
	m(3,3) = 0.0;

	Mat4f& cur = getCurrentMatrix();
	cur *= m;
}

// 透視投影行列を生成して、カレントの行列に掛け合わせる
void perspectiveMatrix(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	Mat4f m;
	GLfloat sine, cotangent, deltaZ;
	GLfloat radians = fovy / 2.0;

	deltaZ = zFar - zNear;
	sine = std::sin(radians);
	assert((deltaZ != 0.0) && (sine != 0.0) && (aspect != 0.0));
	cotangent = std::cos(radians) / sine;

	m = Mat4f::Identity();
	m(0,0) = cotangent / aspect;
	m(1,1) = cotangent;
	m(2,2) = -(zFar + zNear) / deltaZ;
	m(2,3) = -2.0 * zNear * zFar / deltaZ;
	m(3,2) = -1.0;
	m(3,3) = 0.0;

	Mat4f& cur = getCurrentMatrix();
	cur *= m;
}


}
