
#pragma once

/* Windows専用プリコンパイル用ヘッダ */

#define _USE_MATH_DEFINES
// Windowsは、これでM_PIとかが有効になる
#define _CRT_SECURE_NO_WARNINGS

#define EIGEN_DONT_ALIGN
// Eigenの16バイトアライメントは無効

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif
#include <windows.h>
#include <mmsystem.h>

#include <GL/glut.h>
#include <GL/glext.h>

#include <random>
#include <memory>
#include <functional>
#include <locale>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <zlib.h>
#include <png.h>
