
#pragma once

//
// アプリケーション本体
//

#include <iostream>
#include <cmath>
#include "co_defines.hpp"
#include "co_matrix.hpp"
#include "co_texture.hpp"
#include "co_misc.hpp"
#include "co_graph.hpp"
#include "gl_easyShader.hpp"
#include "nn_sphere.hpp"


namespace ngs {

GLfloat vertex_tbl[] = {
	 0.0, -100.0, 0.0,
	-50.0,  0.0, 0.0,
	 50.0,  0.0, 0.0,
};

// シェーダーの簡単なサンプル
const GLchar *testVsh = SHADER_PROG(

	attribute vec4 position;

	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;

	void main()
	{
    gl_Position = projectionMatrix * modelViewMatrix * position;
	}
	
);

const GLchar *testFsh = SHADER_PROG(

	void main()
	{
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	
);


const GLchar *test2Vsh = SHADER_PROG(

	attribute vec4 position;
	attribute vec3 normal;

	uniform mat4 modelViewProjectionMatrix;
	uniform mat3 normalMatrix;

	uniform vec3 lightPosition;
	uniform vec3 ambient;
	uniform vec3 diffuse;
	uniform vec3 specular;
	uniform float shininess;

	varying vec4 dstColor;

	void main()
	{
		vec3 N = normalize(normalMatrix * normal);
		vec3 L = normalize(lightPosition);
		vec3 E = vec3(0, 0, 1);
		vec3 H = normalize(L + E);

		float df = max(0.0, dot(N, L));
		float sf = max(0.0, dot(N, H));
		sf = pow(sf, shininess);

		vec3 color = ambient + df * diffuse + sf * specular;

		dstColor = vec4(color, 1);
		gl_Position = modelViewProjectionMatrix * position;
	}
	
);

const GLchar *test2Fsh = SHADER_PROG(

	varying lowp vec4 dstColor;

	void main()
	{
		gl_FragColor = dstColor;
	}
	
);


const GLchar *test3Vsh = SHADER_PROG(

	attribute vec4 position;
	attribute vec3 normal;
	attribute vec2 uv;

	uniform mat4 modelViewProjectionMatrix;
	uniform mat3 normalMatrix;

	uniform vec3 lightPosition;
	uniform vec3 ambient;
	uniform vec3 diffuse;
	uniform vec3 specular;
	uniform float shininess;

	varying vec4 dstColor;
	varying vec4 dstShine;
	varying vec2 uv_out;

	void main()
	{
		vec3 N = normalize(normalMatrix * normal);
		vec3 L = normalize(lightPosition);
		vec3 E = vec3(0, 0, 1);
		vec3 H = normalize(L + E);

		float df = max(0.0, dot(N, L));
		float sf = max(0.0, dot(N, H));
		sf = pow(sf, shininess);

		vec3 color = ambient + df * diffuse;
		dstColor = vec4(color, 1);
		dstShine = vec4(sf * specular, 1);

    gl_Position = modelViewProjectionMatrix * position;
		uv_out = uv;
	}
	
);

const GLchar *test3Fsh = SHADER_PROG(
	uniform sampler2D sampler;

	varying lowp vec4 dstColor;
	varying lowp vec4 dstShine;
	varying mediump vec2 uv_out;

	void main()
	{
    gl_FragColor = texture2D(sampler, uv_out) * dstColor + dstShine;
	}
	
);

//
// 2D描画向け
// 
const GLchar *test4Vsh = SHADER_PROG(

	attribute vec4 position;
	attribute vec2 uv;

	uniform mat4 projectionMatrix;
	uniform mat4 modelViewMatrix;

	varying vec2 uv_out;

	void main()
	{
    gl_Position = projectionMatrix * modelViewMatrix * position;
		uv_out = uv;
	}
	
);

const GLchar *test4Fsh = SHADER_PROG(

	uniform sampler2D sampler;
	uniform lowp vec4 color;

	varying mediump vec2 uv_out;

	void main()
	{
    gl_FragColor = texture2D(sampler, uv_out) * color;
	}
	
);






class App {
	const std::string *loadPath_;
	const std::string *savePath_;

	
	GLfloat rotate_;
	GLfloat pos_x_;
	GLfloat pos_y_;
	GLfloat scale_x_;
	GLfloat scale_y_;

	GLfloat width_;
	GLfloat height_;

	EasyShader prog_;
	EasyShader prog2_;
	EasyShader prog3_;
	EasyShader prog4_;

	Sphere solid_body_;
	Sphere texture_body_;
	Texture texture_;

	texPtr title_texture_;
	
public:
	App(const std::string& loadPath, const std::string& savePath) :
		loadPath_(&loadPath),
		savePath_(&savePath),
		rotate_(),
		pos_x_(),
		pos_y_(),
		scale_x_(1.0),
		scale_y_(1.0),
		width_(960),
		height_(640),
		prog_(testVsh, testFsh),
		prog2_(test2Vsh, test2Fsh),
		prog3_(test3Vsh, test3Fsh),
		prog4_(test4Vsh, test4Fsh),
		solid_body_(32, 16, 100),
		texture_body_(32, 16, 100, true),
		texture_(loadPath + "earth_4.png", true)
	{
		DOUT << "App()" << std::endl;

		title_texture_ = texPtr(new Texture(loadPath + "title.png"));
	}
	
	~App()
	{
		DOUT << "~App()" << std::endl;
	}

	void resize(const int width, const int height)
	{
		width_ = width;
		height_ = height;

		glViewport(0, 0, width, height);
	}

	void update()
	{
		rotate_ += 0.05;

		pos_x_ = std::sin(rotate_) * 100.0;
		pos_y_ = std::cos(rotate_) * 100.0;

		scale_x_ = 1.0 + std::sin(rotate_ * 0.5);
		scale_y_ = 1.0 + std::cos(rotate_ * 0.5);
	}

	void draw()
	{
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		
		setMatrixMode(Matrix::mode::PROJECTION);
		loadIdentity();
		// frustumMatrix(-width_ / 4.0, width_ / 4.0, -height_ / 4.0, height_ / 4.0, 100, 1000);
		perspectiveMatrix(ang2rad(30.0f), width_ / height_, 240, 1000);

		setMatrixMode(Matrix::mode::MODELVIEW);
		loadIdentity();
		translateMatrix(0, 0, -800.0);

		{
			pushMatrix();

			prog2_();

			rotateMatrix(rotate_ * 0.1, 1, 0, 0);
			translateMatrix(150, 0.0, 0);
			// scaleMatrix(scale_x_, scale_y_, 1.0);
		
			const Mat4f& projection = getProjectionMatrix();
			const Mat4f& model = getModelMatrix();
			Mat4f pm;
			pm = projection * model;
			glUniformMatrix4fv(prog2_.uniform("modelViewProjectionMatrix"), 1, GL_FALSE, pm.data());
			glUniformMatrix3fv(prog2_.uniform("normalMatrix"), 1, GL_FALSE, getNormalMatrix().data());
			glUniform3f(prog2_.uniform("lightPosition"), 0, 10.0, 20.0);
			glUniform3f(prog2_.uniform("ambient"), 0.1, 0.1, 0.4);
			glUniform3f(prog2_.uniform("diffuse"), 0.0, 0.6, 0.5);
			glUniform3f(prog2_.uniform("specular"), 0.8, 0.8, 0.8);
			glUniform1f(prog2_.uniform("shininess"), 20.0);
			// シェーダーに値を渡す
		
			solid_body_.draw(prog2_);
			popMatrix();
			// ソリッドな球体
		}

		{
			pushMatrix();

			prog3_();

			rotateMatrix(rotate_ * 0.1, 1, 0, 0);
			translateMatrix(-150, 0.0, 0);

			const Mat4f& projection = getProjectionMatrix();
			const Mat4f& model = getModelMatrix();
			Mat4f pm;
			pm = projection * model;
			glUniformMatrix4fv(prog3_.uniform("modelViewProjectionMatrix"), 1, GL_FALSE, pm.data());
			glUniformMatrix3fv(prog3_.uniform("normalMatrix"), 1, GL_FALSE, getNormalMatrix().data());
			glUniform3f(prog3_.uniform("lightPosition"), 0, 10.0, 20.0);
			glUniform3f(prog3_.uniform("ambient"), 0.5, 0.5, 0.5);
			glUniform3f(prog3_.uniform("diffuse"), 0.5, 0.5, 0.5);
			glUniform3f(prog3_.uniform("specular"), 0.5, 0.5, 0.5);
			glUniform1f(prog3_.uniform("shininess"), 15.0);
			glUniform1i(prog3_.uniform("sampler"), 0);
			// FIXME:テクスチャユニットの選択がマジックナンバーなのが嫌な感じ

			texture_.bind();
			texture_body_.draw(prog3_);
		
			glBindTexture(GL_TEXTURE_2D, 0);
			popMatrix();
			// テクスチャな球体
		}

		
		setMatrixMode(Matrix::mode::PROJECTION);
		loadIdentity();
		orthoMatrix(-width_ / 2.0, width_ / 2.0, height_ / 2.0, -height_ / 2.0, 1.0, -1.0);

		setMatrixMode(Matrix::mode::MODELVIEW);
		loadIdentity();

		{
			pushMatrix();

			translateMatrix(pos_x_, pos_y_, 0);
			rotateMatrix(rotate_, 0, 0, 1);
			scaleMatrix(scale_x_, scale_y_, 1.0);

			prog_();

			glUniformMatrix4fv(prog_.uniform("projectionMatrix"), 1, GL_FALSE, getProjectionMatrix().data());
			glUniformMatrix4fv(prog_.uniform("modelViewMatrix"), 1, GL_FALSE, getModelMatrix().data());

			GLint position = prog_.attrib("position");
			glEnableVertexAttribArray(position);
			glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, vertex_tbl);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDisableVertexAttribArray(position);

			popMatrix();
		}

		{
			pushMatrix();
			glEnable(GL_BLEND);
			grpSetBlend(GRP_BLEND_NORMAL);

			prog4_();

			for (u_int y = 0; y < 2; ++y)
			{
				for (u_int x = 0; x < 2; ++x)
				{
					grpRect obj;
					grpCol tbl[] = {
						grpCol(1, 0, 0, 1),
						grpCol(0, 1, 0, 1),
						grpCol(0, 0, 1, 1),
						grpCol(1, 0, 1, 1),
					};
					
					obj.pos(-100.0f + x * 200.0f, -100.0f + y * 200.0f);
					obj.size(200.0f, 200.0f);
					obj.uv(0.0f + x * 200.0f, 0.0f + y * 200.0f);
					obj.rotate(rotate_ * 0.4f);
					// obj.scale(1.5f, 1.0f);
					obj.flip_v(true);
					obj.center();
					obj.color(tbl[y * 2 + x]);
					obj.texture(title_texture_);
					obj.draw(prog4_);
				}
			}
			
			glDisable(GL_BLEND);
			popMatrix();
		}
	}
	
};


}
