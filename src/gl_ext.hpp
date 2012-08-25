
#pragma once

// 
// OpenGL拡張機能管理
//

#if defined (_MSC_VER)

namespace ngs {


PFNGLCREATESHADERPROC								glCreateShader;
PFNGLSHADERSOURCEPROC								glShaderSource;
PFNGLCOMPILESHADERPROC							glCompileShader;
PFNGLGETSHADERIVPROC								glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC						glGetShaderInfoLog;
PFNGLDELETEPROGRAMPROC							glDeleteProgram;
PFNGLUSEPROGRAMPROC									glUseProgram;
PFNGLVALIDATEPROGRAMPROC						glValidateProgram;
PFNGLGETPROGRAMIVPROC								glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC					glGetProgramInfoLog;
PFNGLGENVERTEXARRAYSPROC						glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC						glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC				glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYPROC		glDisableVertexAttribArray;
PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays;
PFNGLGENBUFFERSPROC									glGenBuffers;
PFNGLBINDBUFFERPROC									glBindBuffer;
PFNGLBUFFERDATAPROC									glBufferData;
PFNGLDELETEBUFFERSPROC							glDeleteBuffers;
PFNGLBLENDEQUATIONPROC							glBlendEquation;
PFNGLUNIFORM1IPROC									glUniform1i;
PFNGLUNIFORM1FPROC									glUniform1f;
PFNGLUNIFORM3FPROC									glUniform3f;
PFNGLUNIFORM4FPROC									glUniform4f;
PFNGLUNIFORMMATRIX3FVPROC						glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC						glUniformMatrix4fv;
PFNGLATTACHSHADERPROC								glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC					glBindAttribLocation;
PFNGLGETATTRIBLOCATIONPROC					glGetAttribLocation;
PFNGLCREATEPROGRAMPROC							glCreateProgram;
PFNGLDELETESHADERPROC								glDeleteShader;
PFNGLGETUNIFORMLOCATIONPROC					glGetUniformLocation;
PFNGLLINKPROGRAMPROC								glLinkProgram;
PFNGLDETACHSHADERPROC								glDetachShader;
PFNGLGENERATEMIPMAPPROC							glGenerateMipmap;
//
// FIXME:Windowsではglextのメソッドを自分で取得する必要がある
// 


bool initGlExt()
{
	bool res = true;

	if ((glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader")) == 0) res = false;
	if ((glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource")) == 0) res = false;
	if ((glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader")) == 0) res = false;
	if ((glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv")) == 0) res = false;
	if ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog")) == 0) res = false;
	if ((glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram")) == 0) res = false;
	if ((glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram")) == 0) res = false;
	if ((glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram")) == 0) res = false;
	if ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv")) == 0) res = false;
	if ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog")) == 0) res = false;
	if ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays")) == 0) res = false;
	if ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray")) == 0) res = false;
	if ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArray")) == 0) res = false;
	if ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer")) == 0) res = false;
	if ((glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i")) == 0) res = false;
	if ((glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f")) == 0) res = false;
	if ((glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f")) == 0) res = false;
	if ((glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f")) == 0) res = false;
	if ((glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv")) == 0) res = false;
	if ((glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv")) == 0) res = false;
	if ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray")) == 0) res = false;
	if ((glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays")) == 0) res = false;
	if ((glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers")) == 0) res = false;
	if ((glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer")) == 0) res = false;
	if ((glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData")) == 0) res = false;
	if ((glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers")) == 0) res = false;
	if ((glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader")) == 0) res = false;
	if ((glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation")) == 0) res = false;
	if ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation")) == 0) res = false;
	if ((glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram")) == 0) res = false;
	if ((glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader")) == 0) res = false;
	if ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation")) == 0) res = false;
	if ((glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram")) == 0) res = false;
	if ((glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader")) == 0) res = false;
	if ((glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap")) == 0) res = false;

	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(GLint);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
	{
		DOUT << "wglSwapIntervalEXT() ready for use." << std::endl;
		wglSwapIntervalEXT(1);
		// vsync有効
	}
	
	return res;
}


}

#elif defined (__APPLE__)

#include <OpenGL/OpenGL.h>
#include <CoreFoundation/CoreFoundation.h>

bool initGlExt()
{
	int sync = 1;
	CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
	// vsync有効

	return true;
}

#else

#define initGlExt() true

#endif
