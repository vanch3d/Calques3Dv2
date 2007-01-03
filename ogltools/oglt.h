#ifndef OGLT_H
#define OGLT_H

#ifndef _OGLTOOLS_LIB 
	#pragma message("Adding library: OpenGL32.lib, glu32.lib" ) 
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
#endif
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
