
#pragma once

//
// 球を生成する
//

#include <vector>
#include <cmath>
#include "Eigen/Core"
#include "co_defines.hpp"
#include "gl_easyShader.hpp"


namespace ngs {

class Sphere {
	struct Vtx {
		GLfloat x, y, z;
	};
	struct Uv {
		GLfloat u, v;
	};
	struct Body {
		Vtx vtx;
		Vtx nor;
		Uv uv;
	};

	struct Face {
		GLushort v1, v2, v3;
	};

	std::vector<Body> body_;
	std::vector<Face> face_;

	const GLuint vertices_;
	const GLuint faces_;
	const GLuint points_;

	bool texture_;

	GLuint vbo_[2];
	
public:
	Sphere(const int slices, const int stacks, const float radius, const bool texture = false) :
		vertices_((slices + 1) * (stacks + 1)),
		faces_(slices * stacks * 2),
		points_(faces_ * 3),
		texture_(texture)
	{
		body_.reserve(256);
		face_.reserve(256);
		// あらかじめ予約しておくとpush_back()が軽くなる

		for(int j = 0; j <= stacks; ++j)
		{
			float ph = M_PI * (float)j / (float)stacks;
			float y = radius * std::cos(ph);
			float r = radius * std::sin(ph);
			for(int i = 0; i <= slices; ++i)
			{
				Body b;
				
				float th = 2.0f * M_PI * (float)i / (float)slices;
				float x = r * std::cos(th);
				float z = r * std::sin(th);
				b.vtx.x = x;
				b.vtx.y = y;
				b.vtx.z = z;

				b.nor.x = x;
				b.nor.y = y;
				b.nor.z = z;
				// TIPS:ノーマライズはシェーダー側で処理

				b.uv.u = 1.0 - (float)i / (float)slices;
				b.uv.v = (float)j / (float)stacks;

				body_.push_back(b);
			}
		}

		for(int j = 0; j < stacks; ++j)
		{
			for(int i = 0; i < slices; ++i)
			{
				int count = (slices + 1) * j + i;

				Face face;

				face.v1 = count;
				face.v2 = count + 1;
				face.v3 = count + slices + 2;
				face_.push_back(face);
				
				face.v1 = count;
				face.v2 = count + slices + 2;
				face.v3 = count + slices + 1;
				face_.push_back(face);
				// GL_TRIANGLESで描画するのでfaceを２つづつ生成
			}
		}

		glGenBuffers(2, vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Body) * body_.size(), &body_[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * face_.size(), &face_[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// 割り当てを解除しておく

		DOUT << "Sphere face:" << face_.size() << std::endl;
	}
	
	~Sphere()
	{
		glDeleteBuffers(2, vbo_);
	}

	void texture(const bool texture) { texture_ = texture; }

	void draw(const EasyShader& shader)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);

		GLint position = shader.attrib("position");
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Body), 0);

		GLint normal = shader.attrib("normal");
		glEnableVertexAttribArray(normal);
		glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Body), (GLvoid *)(sizeof(Vtx)));
		if (texture_)
		{
			GLint uv = shader.attrib("uv");
			glEnableVertexAttribArray(uv);
			glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Body), (GLvoid *)(sizeof(Vtx) + sizeof(Vtx)));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);
		glDrawElements(GL_TRIANGLES, points_, GL_UNSIGNED_SHORT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// 割り当てを解除しておく
	}
	
};

}
