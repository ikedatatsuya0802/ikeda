#ifndef _MAIN_H_
#define _MAIN_H_
//=============================================================================
//
//	�^�C�g�� �w�b�_�t�@�C��(main.cpp)
//	�t�@�C���� main.h
//	�쐬�� AT13A284_07 �r�c�B��
//	�쐬�� 2016/04/13
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include <windows.h>
#include "readfile.h"
#include <vector>
#include <list>
#include <random>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

//=============================================================================
//	�����N
//=============================================================================
#pragma comment(lib, "winmm.lib")

//=============================================================================
//	�}�N����`
//=============================================================================
#define SIMPLE_FOR(max)			for(int i=0;i<max;i++)	// �P�������񏈗�
#define	uchar					unsigned char
#define	uint					unsigned int
#define	GAME_FPS				(60)
#define	DEFAULT_SCREEN_WIDTH	(1024.0f)	// �E�B���h�E�̉���
#define	DEFAULT_SCREEN_HEIGHT	(768.0f)	// �E�B���h�E�̏c��
#define	SCREEN_WIDTH			(1024.0f)	// �E�B���h�E�̉���
#define	SCREEN_HEIGHT			(768.0f)	// �E�B���h�E�̏c��
#define	SCREEN_WIDTH_HALF		(SCREEN_WIDTH * 0.5f)	// �E�B���h�E�̉����̔���
#define	SCREEN_HEIGHT_HALF		(SCREEN_HEIGHT * 0.5f)	// �E�B���h�E�̏c���̔���

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	float r;
	float g;
	float b;
} COLOR3;
#define FCOLOR(x)	(x/255.0f)

//=============================================================================
//	�v���g�^�C�v
//=============================================================================
int		GetFPS(void);
HWND	GethWnd(void);
char*	StringToCharP(const string str);
string	CharPToString(const char* str);
vector<std::string> split(const string &str, char sep);

// �C���X�^���X�폜�֐�
template <class INSTANCE>
void SafetyRelease(INSTANCE instance)
{
	if(instance != NULL)
	{
		instance->Release();
		instance = NULL;
	}
}

// �C���X�^���X�폜�֐�(����N���X��)
template <class INSTANCE>
void SafetyUninit(INSTANCE instance)
{
	if(instance != NULL)
	{
		instance->Uninit();
		delete instance;
		instance = NULL;
	}
}

// �f�[�^�����֐�
template <class SWAPDATA>
void SwapData(SWAPDATA &data1, SWAPDATA &data2)
{
	SWAPDATA dataBuff;
	dataBuff = data1;
	data1 = data2;
	data2 = dataBuff;
}


#endif