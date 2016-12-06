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
#include "input.h"
#include "readfile.h"
#include <vector>
#include <list>
using namespace std;

//=============================================================================
//	�����N
//=============================================================================
#pragma comment(lib, "winmm.lib")

//=============================================================================
//	�}�N����`
//=============================================================================
#define	SCREEN_WIDTH		(1280.0f)	// �E�B���h�E�̉���
#define	SCREEN_HEIGHT		(760.0f)	// �E�B���h�E�̏c��
#define	SCREEN_WIDTH_HALF	(SCREEN_WIDTH * 0.5f)	// �E�B���h�E�̉����̔���
#define	SCREEN_HEIGHT_HALF	(SCREEN_HEIGHT * 0.5f)	// �E�B���h�E�̏c���̔���

//=============================================================================
//	�v���g�^�C�v
//=============================================================================
int		GetFPS(void);
HWND	GethWnd(void);
//void ChangeFlag(bool &Flag) { Flag = Flag ? false : true; } // �t���O�ύX�֐�

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