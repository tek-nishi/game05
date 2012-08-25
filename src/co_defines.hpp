
#pragma once

//
// いろいろ共通定義
//

#ifdef _DEBUG
#define DOUT std::cout
#else
#define DOUT 0 && std::cout
#endif
// std::cout一網打尽マクロ

#define elemsof(a)  ((u_int)(sizeof(a) / sizeof((a)[0])))

typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int   u_int;


#if defined (__APPLE__)

#if (TARGET_OS_IPHONE)

#define glGenVertexArrays(num, array)    glGenVertexArraysOES(num, array)
#define glBindVertexArray(index)         glBindVertexArrayOES(index)
#define glDeleteVertexArrays(num, array) glDeleteVertexArraysOES(num, array)

#else

#define glGenVertexArrays(num, array)    glGenVertexArraysAPPLE(num, array)
#define glBindVertexArray(index)         glBindVertexArrayAPPLE(index)
#define glDeleteVertexArrays(num, array) glDeleteVertexArraysAPPLE(num, array)

#endif

#endif


#if (TARGET_OS_IPHONE)

#ifdef _DEBUG
#define NSLOG(...) NSLog(__VA_ARGS__)
#else
#define NSLOG(...) 
#endif


typedef GLfloat GLdouble;
// FIXME:ちょっと微妙

#endif
