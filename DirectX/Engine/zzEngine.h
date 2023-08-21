#pragma once

#include <string>
#include <Windows.h>
#include <assert.h>

#include <vector> // �迭
#include <list> //��ũ�帮��Ʈ 
#include <map> // 2��Ʈ��
#include <bitset> // ��Ʈ �迭 ���ϰ� ������ִ� ���̺귯��
#include <set> // �ؽ� ���̺�
#include <functional> // �Լ� ������

#include <cmath> //����
#include <algorithm> //���� �˰���
#include <limits> //�ε� �Ҽ��� ǥ�� �� �ݿø��� ���õ� Ŭ���� ���ø� numeric_limits �� �� ���� �������� �����մϴ�.
#include <memory> //�޸� ���õ� ���̺귯��
#include <filesystem> // ��������� ���ϰ� �����ִ� ���̺귯��
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
