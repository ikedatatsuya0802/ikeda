#ifndef _SPEEDMETER_H_
#define _SPEEDMETER_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(speedmeter.cpp)
//	�t�@�C����	speedmeter.h
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
#define	SPEEDMETER_WIDTH	(SCREEN_HEIGHT * 0.3f)
#define	SPEEDMETER_HEIGHT	(SCREEN_HEIGHT * 0.3f)

#define	SPEEDMETER_ROT_LOW	(-2.0f)						// �j�̊p�x�̉���
#define	SPEEDMETER_ROT_HIGH	(2.0f)						// �j�̊p�x�̏��

#define	SPEEDMETER_TEXFILENAME000	"speedmeter000.png"
#define	SPEEDMETER_TEXFILENAME001	"speedmeter001.png"

//=============================================================================
//	�N���X��`
//=============================================================================
class CSpeedmeter : public CScene2DDX
{
public:
	CSpeedmeter(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSpeedmeter();

	void	Init(float maxSpeed, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO,
			D3DXVECTOR2 size = D3DXVECTOR2(SPEEDMETER_WIDTH, SPEEDMETER_HEIGHT),
			char *str = SPEEDMETER_TEXFILENAME000);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSpeedmeter	*Create(float maxSpeed, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO,
		D3DXVECTOR2 size = D3DXVECTOR2(SPEEDMETER_WIDTH, SPEEDMETER_HEIGHT),
		char *str = SPEEDMETER_TEXFILENAME000);

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff);

	LPDIRECT3DTEXTURE9	m_pTexture[2];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[2];

	float m_MaxSpeed;	// �ő呬�x
};

#endif