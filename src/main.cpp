
#include <iostream>

#if defined (__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glext.h>
#endif

#include "co_defines.hpp"
#include "gl_ext.hpp"
#include "nn_app.hpp"
#include "os_osx.hpp"
#include "os_win.hpp"


using namespace std;
using namespace ngs;


Os os;
// FIXME:ほかに依存していないのでこれでいい気もするが…
App *app;


void timerCallback(const int msec)
{
	glutTimerFunc(msec, timerCallback, msec);
	glutPostRedisplay();
}

void displayCallback()
{
	app->update();
	app->draw();
	glutSwapBuffers();

#ifdef _DEBUG
	const GLenum gl_err = glGetError();
	if (gl_err != GL_NO_ERROR)
	{
		const GLubyte *str = gluErrorString(gl_err);
		DOUT << "OpenGL Error:" << str << std::endl;
	}
#endif
}

void resizeCallback(const int w, const int h)
{
	DOUT << "resizeCallback:" << w << "x" << h << std::endl;
	app->resize(w, h);
}


void mainFin()
{
	delete app;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	atexit(mainFin);
	// TIPS:アプリ終了時に実行する関数を登録

	glutInitWindowSize(960, 640);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("game05");

	bool glsl = initGlExt();
	assert(glsl && "Can't Execute GLSL code");

	app = new App(os.loadPath(), os.savePath());

	glutReshapeFunc(resizeCallback);
	glutDisplayFunc(displayCallback);
	glutTimerFunc(0, timerCallback, 15);

	glHint(GL_FOG_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	// ↑たいてい無視されている模様:P
	
	glutMainLoop();
}
