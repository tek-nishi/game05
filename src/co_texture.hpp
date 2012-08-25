
#pragma once

//
// テクスチャ管理
//

#include <string>
#include <fstream>
#include "co_defines.hpp"
#include "co_matrix.hpp"
#include "co_png.hpp"
#include "co_pvrtc.hpp"
#include "co_filepath.hpp"
#include "co_misc.hpp"


namespace ngs {

bool checkPvrtc(std::string& res, const std::string& path)
{
#if (TARGET_OS_IPHONE)
	std::string::size_type index = path.rfind(".png");
	if(index != std::string::npos)
	{
		res = path;
		const std::string ext = ".pvr";
		res.replace(index, ext.length(), ext);
		if (isFileExists(res)) return true;
	}
#endif
	return false;
}

void setupTextureParam(const bool mipmap)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST);
}


class Texture
{
	Vec2i size_gl_;
	// OpenGLで管理するサイズ
	GLuint id_;
	Vec2i size_;
	Vec2f uv_;
	const std::string name_;
	bool mipmap_;

	void setupPng(std::ifstream& fstr, const bool mipmap)
	{
		std::size_t size = fstr.seekg(0, std::ios::end).tellg();
		std::vector<png_byte> buf(size);
		void *p = &buf[0];
		fstr.seekg(0, std::ios::beg).read(static_cast<char *>(p), size);
		fstr.close();

		Png png_obj(&buf[0], size);
		size_ = png_obj.size;
		size_gl_ << int2pow(size_.x()), int2pow(size_.y());
		uv_ << (float)size_.x() / (float)size_gl_.x(), (float)size_.y() / (float)size_gl_.y();

		glBindTexture(GL_TEXTURE_2D, id_);
		setupTextureParam(mipmap);

		GLint type = (png_obj.type == PNG_COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
#if defined (_MSC_VER)
		if (mipmap)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, type, size_gl_.x(), size_gl_.y(), type, GL_UNSIGNED_BYTE, png_obj.image());
			// FIXME:WindowsだとglGenerateMipmap()が使えない？
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, type, size_gl_.x(), size_gl_.y(), 0, type, GL_UNSIGNED_BYTE, png_obj.image());
		}
#else
		glTexImage2D(GL_TEXTURE_2D, 0, type, size_gl_.x(), size_gl_.y(), 0, type, GL_UNSIGNED_BYTE, png_obj.image());
		if (mipmap) glGenerateMipmap(GL_TEXTURE_2D);
#endif
		
		DOUT << "Texture:" << name_ << ((type == PNG_COLOR_TYPE_RGB) ? " RGB" : " RGBA") << std::endl;
	}

	bool setupPvrtc(std::ifstream& fstr)
	{
#if (TARGET_OS_IPHONE)
		std::size_t size = fstr.seekg(0, std::ios::end).tellg();
		std::vector<char> buf(size);
		fstr.seekg(0, std::ios::beg).read(&buf[0], size);
		fstr.close();

		Pvrtc obj(&buf[0]);
		size_gl_ = obj.size();
		size_ = size_gl_;
		uv_ << 1, 1;
		bool mipmap = obj.mipmap();

		glBindTexture(GL_TEXTURE_2D, id_);
		setupTextureParam(mipmap);
		obj.submit();

		return mipmap;
#else
		return false;
#endif
	}
	
public:
	Texture(const std::string& path, const bool mipmap = false) :
		name_(getFileName(path)),
		mipmap_(mipmap)
	{
		DOUT << "Texture()" << std::endl;
		glGenTextures(1, &id_);

		std::string res;
		bool pvrtc = checkPvrtc(res, path);
		const std::string& p = pvrtc ? res : path;
		std::ifstream fstr(p.c_str(), std::ios::binary);
		if(fstr.is_open())
		{
			if (pvrtc)
			{
				mipmap_ = setupPvrtc(fstr);
				// TIPS:iOSでのみ有効なコード
			}
			else
			{
				setupPng(fstr, mipmap);
			}
		}
	}
	
	~Texture() {
		DOUT << "~Texture()" << std::endl;
		glDeleteTextures(1, &id_);
	}

	const Vec2i& size() const { return size_; }
	const Vec2f& uv() const { return uv_; }
	const std::string& name() const { return name_; }

	void bind() const
	{
		glBindTexture(GL_TEXTURE_2D, id_);
	}
};

}
