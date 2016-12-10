#ifndef _PAUSE_H_
#define _PAUSE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(pause.cpp)
//	�t�@�C����	pause.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene2DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	PAUSE_TEXFILENAME000			"pause000.png"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME001			"pause001.jpg"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME002			"pause002.png"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME003			"pause003.png"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME004			"pause004.png"						// �|�[�Y�|���S���̃t�@�C����

//=============================================================================
//	�N���X��`
//=============================================================================
class CPause : public CScene2DDX
{
public:
	CPause(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CPause();

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CPause	*Create(void);

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff);

	LPDIRECT3DTEXTURE9	m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[2];

	float m_MaxSpeed;	// �ő呬�x
};

#endif