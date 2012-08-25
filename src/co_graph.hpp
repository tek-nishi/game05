
#pragma once

//
// 描画基本クラス
//

#ifndef _MSC_VER
#include <tr1/memory>
// OSXとiOSはファイルが別
#endif

#include <cstddef>
#include "co_defines.hpp"
#include "co_matrix.hpp"
#include "co_texture.hpp"
#include "gl_easyShader.hpp"


namespace ngs {

typedef Eigen::Matrix<GLfloat, 4, 1> grpCol;
typedef std::tr1::shared_ptr<Texture> texPtr;

enum GRP_BLEND {
	GRP_BLEND_NORMAL,							// 通常のブレンディング
	GRP_BLEND_ADD,								// 加算半透明
	GRP_BLEND_REV,								// 反転表示
	GRP_BLEND_XOR,								// XOR
	GRP_BLEND_MUL,								// 乗算
	GRP_BLEND_SCREEN,							// スクリーン合成
};

// ブレンドモード指定
void grpSetBlend(const u_int blend)
{
	struct Blend {
		GLenum src, dst;
	};
	const Blend tbl[] = {
		{ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA },
		{ GL_SRC_ALPHA, GL_ONE },
		{ GL_ONE_MINUS_DST_COLOR, GL_ZERO },
		{ GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR },
		{ GL_ZERO, GL_ONE_MINUS_SRC_COLOR },
		{ GL_DST_COLOR, GL_ONE },
	};

	glBlendFunc(tbl[blend].src, tbl[blend].dst);
}

// 行列のセットアップ
void setupMatrix(Mat4f& res, const Vec2f& pos, const Vec2f& center, const Vec2f& scale, const float rotate)
{
	res = Mat4f::Identity();

	Eigen::Affine3f m;
	m = Eigen::Translation<GLfloat, 3>(pos.x(), pos.y(), 0.0f);
	res *= m.matrix();

	m = Eigen::Quaternion<GLfloat>(Eigen::AngleAxis<GLfloat>(rotate, Vec3f(0.0f, 0.0f, 1.0f)));
	res *= m.matrix();

	m = Eigen::Scaling(scale.x(), scale.y(), 1.0f);
	res *= m.matrix();

	m = Eigen::Translation<GLfloat, 3>(-center.x(), -center.y(), 0.0f);
	res *= m.matrix();
}


// テクスチャ付き矩形
// TIPS:uv座標はpixcelで扱う
class grpRect {
	Vec2f pos_;
	Vec2f size_;
	Vec2f center_;
	Vec2f scale_;
	float rotate_;
	Vec2f uv_top_;
	Vec2f uv_bottom_;
	texPtr texture_;
	grpCol color_;
	
	bool flip_h_;
	bool flip_v_;

	struct Vtx {
		GLfloat x, y;
	};
	struct Uv {
		GLfloat u, v;
	};
	struct Body {
		Vtx vtx;
		Uv uv;
	};

public:
	grpRect() :
		center_(),
		scale_(1.0f, 1.0f),
		rotate_(),
		color_(1.0f, 1.0f, 1.0f, 1.0f),
		flip_h_(),
		flip_v_()
	{}
	
	void pos(const float x, const float y) {
		pos_ << x, y;
	}
	void pos(const Vec2f& pos) {
		pos_ = pos;
	}
	const Vec2f& pos() const { return pos_; }

	void size(const float width, const float height) {
		size_ << width, height;
	}
	void size(const Vec2f& size) {
		size_ = size;
	}
	const Vec2f& size() const { return size_; }

	void center() {
		center_ = size_ / 2.0f;
	}
	void center(const Vec2f& center) {
		center_ = center;
	}
	void center(const float x, const float y) {
		center_ << x, y;
	}

	void scale() {
		scale_ = size_ / 2.0f;
	}
	void scale(const Vec2f& scale) {
		scale_ = scale;
	}
	void scale(const float x, const float y) {
		scale_ << x, y;
	}

	void rotate(const float r) {
		rotate_ = r;
	}
	float rotate() { return rotate_; }

	void color(const float r, const float g, const float b, const float a = 1.0f) {
		color_ << r, g, b, a;
	}
	void color(const grpCol& col) {
		color_ = col;
	}
	const grpCol& color() {
		return color_;
	}
	
	void uv(const Vec2f& top, const Vec2f& bottom) {
		uv_top_ = top;
		uv_bottom_ = bottom;
	}
	void uv(const Vec2f& top) {
		uv_top_ = top;
		uv_bottom_ = top + size_;
	}
	void uv(const float top_x, const float top_y, const float bottom_x, const float bottom_y) {
		uv_top_ << top_x, top_y;
		uv_bottom_ << bottom_x, bottom_y;
	}
	void uv(const float top_x, const float top_y) {
		uv_top_ << top_x, top_y;
		uv_bottom_ << top_x + size_.x(), top_y + size_.y();
	}

	void texture(texPtr texture) {
		texture_ = texture;
	}

	void flip_h(const bool flip) {
		flip_h_ = flip;
	}
	void flip_v(const bool flip) {
		flip_v_ = flip;
	}
	
	void draw(const EasyShader& shader)
	{
		const Vec2i& tex_size = texture_->size();
		const Vec2f& tex_uv = texture_->uv();
		float top_u = uv_top_.x() * tex_uv.x() / tex_size.x();
		float top_v = uv_top_.y() * tex_uv.y() / tex_size.y();
		float bottom_u = uv_bottom_.x() * tex_uv.x() / tex_size.x();
		float bottom_v = uv_bottom_.y() * tex_uv.y() / tex_size.y();
		// 正規化座標でのUVを求める
		
		if(flip_h_) std::swap(top_u, bottom_u);
		if(flip_v_) std::swap(top_v, bottom_v);
		// フリップ
		
		Body body[] = {
			{ { 0.0, 0.0 }, { top_u, top_v } },
			{ { 0.0f, size_.y() }, { top_u, bottom_v } },
			{ { size_.x(), 0.0f }, { bottom_u, top_v } },
			{ { size_.x(), size_.y() }, { bottom_u, bottom_v } },
		};
		
		Mat4f mtx;
		setupMatrix(mtx, pos_, center_, scale_, rotate_);
		setMatrixMode(Matrix::mode::MODELVIEW);
		pushMatrix();
		multMatrix(mtx);
		// 表示行列のセットアップ

		glUniformMatrix4fv(shader.uniform("projectionMatrix"), 1, GL_FALSE, getProjectionMatrix().data());
		glUniformMatrix4fv(shader.uniform("modelViewMatrix"), 1, GL_FALSE, getModelMatrix().data());
		glUniform1i(shader.uniform("sampler"), 0);
		glUniform4f(shader.uniform("color"), color_(0), color_(1), color_(2), color_(3));

		GLint position = shader.attrib("position");
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(Body), body);

		GLint uv = shader.attrib("uv");
		glEnableVertexAttribArray(uv);
		glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Body), (GLchar *)body + offsetof(Body, uv));

		texture_->bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, elemsof(body));

		glDisableVertexAttribArray(position);
		glDisableVertexAttribArray(uv);
		
		popMatrix();
	}
};



}
