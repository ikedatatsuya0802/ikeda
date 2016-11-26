#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	debugProc.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/26
//
//=============================================================================
#include "d3dx9.h"
//=============================================================================
//	�}�N����`
//=============================================================================
#define	DEBUGSTR_MAX	(1024)	// �z����E

//=============================================================================
//	�v���g�^�C�v
//=============================================================================
class CDebugProc
{
public:
	static HRESULT	Init(void);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);

	static void	DebugProc(char *fmt, ...);

private:
	static LPD3DXFONT	m_pFont;					// �t�H���g�ւ̃|�C���^
	static char			m_aStrDebug[DEBUGSTR_MAX];	// �f�o�b�O�p������

};

#endif