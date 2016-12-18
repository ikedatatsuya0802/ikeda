#ifndef _PARTICLE_H_
#define _PARTICLE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	POLYGON3DDX_POSX		(0.0f)	// �|���S���̉���
#define	POLYGON3DDX_POSY		(0.0f)	// �|���S���̏c��
#define	POLYGON3DDX_POSZ		(0.0f)	// �|���S���̏c��
#define	POLYGON3DDX_WIDTH		(300.0f)	// �|���S���̉���
#define	POLYGON3DDX_HEIGHT		(300.0f)	// �|���S���̏c��
#define	POLYGON3DDX_TEXFILENAME000	"field000.jpg"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CParticle : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CParticle	*Create(bool ifListAdd = true, int priority = 2, OBJTYPE objtype = OBJTYPE_NONE,
		D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);

	D3DXMATRIX	*GetWMatrix(void) { return &m_mtxWorld; }	// ���[���h�}�g���b�N�X���擾
	
	// ���\�[�X�̃��[�h
	void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGON3DDX_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	void	Unload(void) { SafetyRelease(m_pTexture); }
	void	SetColor(float a = 1.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f);

protected:
	CParticle(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CParticle();

	virtual void SetVtxBuff(void);
	
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
};

#endif