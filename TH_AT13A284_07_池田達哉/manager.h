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
class CCameraDX;
class CMeshfield;
class CSkyBox;
class CModel;

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
	static CModel		*GetModel(void) { return m_Model; }
	static int			GetFrame(void) { return m_Frame; }

private:
	static CCameraDX	*m_Camera;		// �J�����̃C���X�^���X
	static CModel		*m_Model;		// ���b�V���t�B�[���h�̃C���X�^���X

	static int	m_Frame;	// �ėp�J�E���^
};

#endif