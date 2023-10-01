#pragma once

#include <string>
#include <Windows.h>
#include <assert.h>

#include <vector> 
#include <list> 
#include <map> 
#include <bitset> 
#include <set> 
#include <functional> 

#include <cmath>
#include <algorithm> 
#include <limits> 
#include <memory> 
#include <filesystem> 
#include <wrl.h>
#include <stack>

#include "CommonInclude.h"
#include "zzEnum.h"
#include "zzMath.h"

#include "function.h"

#define NOMINMAX

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\Box2d\\lib\\Debug\\box2d.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\OpenCV\\lib\\Debug\\opencv_world480d.lib")
#endif


// #include "../External/Box2d/include/box2d.h"
