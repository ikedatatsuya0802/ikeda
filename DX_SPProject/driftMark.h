#ifndef _DRIFTMARK_H_
#define _DRIFTMARK_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(driftMark.cpp)
//	�t�@�C����	driftMark.h
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
#define	DRIFTMARK_WIDTH		(500.0f * 0.5f)	// �|���S���̉���
#define	DRIFTMARK_HEIGHT	(300.0f * 0.5f)	// �|���S���̏c��

#define	DRIFTMARK_TEXNAME000	"drift001.jpg"
#define	DRIFTMARK_TEXNAME001	"drift002.jpg"
#define	DRIFTMARK_TYPE			(2)
#define	DRIFTMARK_SRIDESPEED	(0.015f)
#define	DRIFTMARK_FUTURE		(0.01f)		// �h���t�g�}�[�N�\���n�_

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	D3DXVECTOR3	Pos;
	D3DXVECTOR2	Size;
	D3DCOLOR	Col;
} DRIFTMARK;

//=============================================================================
//	�N���X��`
//=============================================================================
class CDriftMark : CScene2DDX
{
public:
	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CDriftMark* Create(void);

	static void VisibleDriftMark(bool type, bool curve, int time);
	static void InvisibleDriftMark(int time);

	void	SetColor(float a = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);

protected:
	CDriftMark(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CDriftMark();

	void SetVtxBuff(void);
	
	LPDIRECT3DTEXTURE9		m_pTexture[DRIFTMARK_TYPE];		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	DRIFTMARK	m_DriftMark;

	static bool		m_VisibleType;	// �\���^�C�v�Afalse->�ʏ�, true->�_��
	static bool		m_flgCounter;	// �_�ŗp�t���O�Atrue���ƌ��炵�Afalse���Ƒ��₷
	static bool		m_Curve;
	static int		m_Count;
	static int		m_SetCounter;
};

#endif