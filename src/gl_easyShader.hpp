
#pragma once

//
// 簡単シェーダー
//

#include <cassert>
#include <iostream>
#include <string>
#include <map>
#include "co_defines.hpp"
#include "co_misc.hpp"

namespace ngs {


#define SHADER_PROG(A) #A
// TIPS:複数行の文字列を単一の文字列にラップする


// 識別子をリストアップ
void listupTokens(std::map<std::string, GLint>& tokens, const std::string& text, const std::string& keyword)
{
	std::string::size_type pos = 0;
	while((pos = text.find(keyword, pos)) != std::string::npos)
	{
		std::string::size_type epos = text.find(";", pos);
		std::string::size_type npos = text.rfind(" ", epos);
		std::string s = text.substr(npos + 1, epos - (npos + 1));
		// キーワードを見つけたら行末の識別子を取得

		if (tokens.find(s) == tokens.end())
		{
			tokens.insert(std::map<std::string, GLint>::value_type(s, 0));
		}

		pos = epos + 1;
	}
}


class EasyShader {
	GLuint program_;
	std::map<std::string, GLint> attribs_;
	std::map<std::string, GLint> uniforms_;

	GLuint compile(GLuint type, char const *source)
	{
		std::string text(source);

		listupTokens(attribs_, text, "attribute");
		listupTokens(uniforms_, text, "uniform");
		// シェーダーの中のattributeとuniform変数をリストアップ
		
#if (TARGET_OS_IPHONE)
		replaceString(text, "iOS;", "precision lowp float;");
		// ES 2.0:floatの精度を指定
#else
		replaceString(text, "iOS;", "");
		replaceString(text, "highp", "");
		replaceString(text, "mediump", "");
		replaceString(text, "lowp", "");
		// ES 2.0以外:要らない文字列を削除
#endif

		GLuint shader = glCreateShader(type);
		char const *text_ptr = text.c_str();
		glShaderSource(shader, 1, &text_ptr, 0);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
#ifdef _DEBUG
		if (compiled == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			std::string log(length + 1, ' ');
			glGetShaderInfoLog(shader, length, &length, &log[0]);
			DOUT << "compile error:" << log << std::endl;

			return GL_FALSE;
		}
#endif
		return shader;
	}

public:
	EasyShader(GLchar const *v_source, GLchar const *f_source) :
		program_(glCreateProgram())
	{
		DOUT << "EasyShader()" << std::endl;

		GLuint vertex_shader = compile(GL_VERTEX_SHADER, v_source);
		GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);

		glAttachShader(program_, vertex_shader);
		glAttachShader(program_, fragment_shader);
		glLinkProgram(program_);
#ifdef _DEBUG
		GLint status;
    glGetProgramiv(program_, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
			std::string log(length + 1, ' ');
			glGetProgramInfoLog(program_, length, &length, &log[0]);
			DOUT << "link error:" << log << std::endl;
		}
#endif

		for (std::map<std::string, GLint>::iterator it = attribs_.begin(); it != attribs_.end(); ++it)
		{
			it->second = glGetAttribLocation(program_, (it->first).c_str());
		}
		
		for (std::map<std::string, GLint>::iterator it = uniforms_.begin(); it != uniforms_.end(); ++it)
		{
			it->second = glGetUniformLocation(program_, (it->first).c_str());
		}
		// リストアップしたattributeとuniform変数をまとめて関連づけ
		// FIXME:mapをiteratorでまわすのはいくない

		if (vertex_shader)
		{
			glDetachShader(program_, vertex_shader);
			glDeleteShader(vertex_shader);
		}
		if (fragment_shader)
		{
			glDetachShader(program_, fragment_shader);
			glDeleteShader(fragment_shader);
		}
		// ↑リンクしたらもう要らない
	}

	~EasyShader()
	{
		glDeleteProgram(program_);
	}
	
	operator GLuint() const { return program_; }
	void operator()() const { glUseProgram(program_); }

	GLint attrib(const std::string& name) const
	{
		std::map<std::string, GLint>::const_iterator it = attribs_.find(name);
		assert(it != attribs_.end());
		return it->second;
	}
	
	GLint uniform(const std::string& name) const
	{
		std::map<std::string, GLint>::const_iterator it = uniforms_.find(name);
		assert(it != uniforms_.end());
		return it->second;
	}

	bool validate()
	{
		glValidateProgram(program_);

		GLint length;
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
		std::string log(length + 1, ' ');
		glGetProgramInfoLog(program_, length, &length, &log[0]);
		DOUT << "validate log:" << log << std::endl;

		GLint status;
		glGetProgramiv(program_, GL_VALIDATE_STATUS, &status);
		return status == GL_TRUE;
	}
};

}
