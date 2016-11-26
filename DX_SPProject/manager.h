#ifndef _MANAGER_H_
#define _MANAGER_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	manager.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/05/10
//
//=============================================================================
#include "d3dx9.h"
#include "debugProc.h"

//=============================================================================
//	�O���錾
//=============================================================================
class CMode;
class CInput;
class CDebugProc;
class CRendererDX;
class CSceneDX;
class CCameraDX;
class CLightDX;

//=============================================================================
//	�N���X��`
//=============================================================================
class CManager
{
public:
	//=========================================================================
	//	���\�b�h
	//=========================================================================
	static void	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);
	
	static CCameraDX	*GetCamera(void){ return m_Camera; }
	
	static CMode		*GetMode(void){ return m_Mode; }
	static void			SetMode(CMode *mode);

private:
	//=========================================================================
	//	�����o�ϐ�
	//=========================================================================
	static CMode		*m_Mode;		// ���[�h�̃C���X�^���X
	static CCameraDX	*m_Camera;		// �J�����̃C���X�^���X
	static CLightDX		*m_Light;		// ���C�g�̃C���X�^���X

};

#endif