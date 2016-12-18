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
#include "main.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	SCENE_NUM		(4)		// �V�[����

//=============================================================================
//	�O���錾
//=============================================================================
class CMode;
class CInput;
class CRendererDX;
class CSceneDX;
class CCameraDX;
class CLightDX;
class CMeshfield;
class CPlayer;

//=============================================================================
//	�N���X��`
//=============================================================================
class CManager
{
public:
	static void	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);
	
	static CCameraDX	*GetCamera(void){ return m_Camera; }
	static CMeshfield	*GetMeshfield(void){ return m_Meshfield; }
	static CPlayer		*GetPlayer(void){ return m_Player; }

private:
	static CCameraDX	*m_Camera;		// �J�����̃C���X�^���X
	static CMeshfield	*m_Meshfield;	// ���b�V���t�B�[���h�̃C���X�^���X
	static CPlayer		*m_Player;	// �v���C���[�̃C���X�^���X

};

#endif