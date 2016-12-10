#ifndef _SCENE2DDX_H_
#define _SCENE2DDX_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene2DDX.cpp)
//	�t�@�C����	scene2DDX.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "sceneDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	POLYGON2DDX_POSX	(SCREEN_WIDTH * 0.5f)		// �|���S���̉���
#define	POLYGON2DDX_POSY	(SCREEN_HEIGHT * 0.5f)		// �|���S���̏c��
#define	POLYGON2DDX_WIDTH	(100.0f)					// �|���S���̉���
#define	POLYGON2DDX_HEIGHT	(100.0f)					// �|���S���̏c��
#define	POLYGON2DDX_TURN	(-0.05f)
#define	POLYGON2DDX_MOVE	(-0.03f)

#define	POLYGON2DDX_TEXFILENAME000	"polygon000.jpg"	// �|���S���̃t�@�C����

//=============================================================================
//	�N���X��`
//=============================================================================
class CScene2DDX : public CSceneDX
{
public:
	CScene2DDX(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CScene2DDX();

	void	Init(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO,
			D3DXVECTOR2 size = D3DXVECTOR2(POLYGON2DDX_WIDTH, POLYGON2DDX_HEIGHT),
			char *str = strcat(POLYGON2DDX_TEXFILENAME000, TEX_FILEPASS));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CScene2DDX	*Create(bool ifListAdd = true, int priority = 2, OBJTYPE objtype = OBJTYPE_NONE,
		D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO,
		D3DXVECTOR2 size = D3DXVECTOR2(POLYGON2DDX_WIDTH, POLYGON2DDX_HEIGHT),
		char *str = strcat(POLYGON2DDX_TEXFILENAME000, TEX_FILEPASS));
	void	SetColor(float a = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);

protected:
	virtual void SetVtxBuff(void);

	float	m_fLength;	// �Ίp���̒���
	float	m_fAngle;	// �p�x
};

#endif