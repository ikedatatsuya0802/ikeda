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
#include "sound.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	WINDOW_ASPECT_X		CManager::GetCWAspect().x
#define	WINDOW_ASPECT_Y		CManager::GetCWAspect().y
#define	PUSHBUTTON_BRIGHT	(60)

//=============================================================================
//	�O���錾
//=============================================================================
class CMode;
class CSceneDX;
class CCameraDX;

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

	static bool		GetEdhitMode(void) { return m_ifEdhitMode; }
	static void		ChangeEdhitMode(void) { m_ifEdhitMode = m_ifEdhitMode ? false : true; }
	
	static CCameraDX	*GetCamera(void){ return m_Camera; }	
	static CMode		*GetMode(void){ return m_Mode; }
	static void			SetMode(CMode *mode);

	template<class M>
	static bool	MatchMode(M mode) {
		const type_info& nowMode = typeid(*m_Mode);
		return (nowMode == typeid(mode)) ? true : false;
	}

	static D3DXVECTOR2 GetClientWindowSize(void);

	static D3DXVECTOR2 GetCWAspect(void);

private:
	static CMode		*m_Mode;		// ���[�h�̃C���X�^���X
	static CCameraDX	*m_Camera;		// �J�����̃C���X�^���X

	static bool m_ifEdhitMode;	// �G�f�B�b�g���[�h���ǂ����Afalse->�ʏ�Atrue->�G�f�B�b�g���[�h
};

#endif