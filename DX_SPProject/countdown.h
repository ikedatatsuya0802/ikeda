#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(countdown.cpp)
//	�t�@�C����	countdown.h
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
#define	COUNTDOWN_WIDTH		(SCREEN_HEIGHT * 0.3f)
#define	COUNTDOWN_HEIGHT	(SCREEN_HEIGHT * 0.3f)

#define	COUNTDOWN_START		(180.0f)
#define	MINUTE				(60)
#define	COUNTDOWN_MINUTE	(3)
#define	COUNTDOWN_SIZE_POW	(2.0f)
#define	COUNTDOWN_END_TIME	(COUNTDOWN_START + MINUTE * COUNTDOWN_MINUTE)

#define	COUNTDOWN_TEXFILENAME000	"countdown000.png"
#define	COUNTDOWN_TEXFILENAME001	"countdown001.png"

//=============================================================================
//	�N���X��`
//=============================================================================
class CCountdown : public CScene2DDX
{
public:
	CCountdown(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CCountdown();

	void	Init(D3DXVECTOR3 pos = VEC3_ZERO,
			D3DXVECTOR2 size = D3DXVECTOR2(COUNTDOWN_WIDTH, COUNTDOWN_HEIGHT));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CCountdown	*Create(D3DXVECTOR3 pos = VEC3_ZERO,
		D3DXVECTOR2 size = D3DXVECTOR2(COUNTDOWN_WIDTH, COUNTDOWN_HEIGHT));

protected:
	void	SetVtxBuff();

	D3DXVECTOR2	m_Size;
	float		m_SizePow;
	float		m_Tex;
	float		m_Alpha;
};

#endif