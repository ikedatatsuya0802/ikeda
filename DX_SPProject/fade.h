#ifndef _FADE_H_
#define _FADE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	fade.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
#include "rendererDX.h"
class CMode;
class CScene2DDX;

//=============================================================================
//	�}�N����`
//=============================================================================
#define	FADE_SPEED		(1.0f / 60)	// �t�F�[�h�X�s�[�h
#define	FADE_CLOSESPEED	(30.0f)		// �t�F�[�h������X�s�[�h
#define	FADE_CLOSETIME	(60)		// �t�F�[�h�̕��Ă��鎞��
#define	PHASETIME_NONE	(60)		// �����t�F�[�Y�̎���

//=============================================================================
//	�\����
//=============================================================================
typedef enum{
	FS_NONE = 0,// �������Ă��Ȃ�
	FS_FIRST,	// �����t�F�[�h
	FS_IN,		// �t�F�[�h�C��
	FS_OUT,		// �t�F�[�h�A�E�g
	FS_MAX		// �t�F�[�h��
}FADE_STATE;

typedef enum{
	FADE_TYPE_DOOR = 0,	// �h�A�t�F�[�h
	FADE_TYPE_WHITE,	// ���]�t�F�[�h
	FADE_TYPE_MAX		// �t�F�[�h��
}FADE_TYPE;

typedef struct{
	FADE_STATE	State;			// �t�F�[�h���
	int			nCntFade;		// �t�F�[�h�J�E���g
	float		fLength;		// �t�F�[�h���m�̋���
	int			nCntPhaseNone;	// �����t�F�[�Y�̃J�E���g
}FADESTATUS;

typedef struct{
	D3DXVECTOR3	Pos;		// �ʒu
	D3DXVECTOR3	Rot;		// ��]�p
	float		fLength;	// �Ίp���̒���
	float		fAngle;		// �Ίp���̊p�x
}FADE;

//=============================================================================
//	�N���X��`
//=============================================================================
class CFade
{
public:
	static void	Init(void);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);

	static void			Start(CMode *nextMode, FADE_STATE fadeState = FS_OUT);
	static FADE_STATE	GetFade(void){ return m_FadeState.State; }
private:
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DTEXTURE9		m_pTexture[2];	// �e�N�X�`���ւ̃|�C���^
	static FADE						m_Fade[2];		// �t�F�[�h

	static CMode		*m_NextMode;
	static FADESTATUS	m_FadeState;	// �t�F�[�h�̏��
};


#endif