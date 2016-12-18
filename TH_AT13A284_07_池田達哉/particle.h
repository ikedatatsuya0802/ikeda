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
#define	PARTICLE_PATTERN	(4)			// �p�[�e�B�N���̃p�^�[����
#define	PARTICLE_LIFE		(120)		// �p�[�e�B�N���̕\������

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CParticle : public CScene3DDX
{
public:
	void	Init(int pattern);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CParticle	*Create(int pattern);

	D3DXMATRIX	*GetWMatrix(void) { return &m_mtxWorld; }	// ���[���h�}�g���b�N�X���擾
	
	// ���\�[�X�̃��[�h
	static void	Load(void) {
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\spring.png", &m_pTexture[0]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\summer.png", &m_pTexture[1]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\autumn.png", &m_pTexture[2]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\winter.png", &m_pTexture[3]);
	}
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) {
		for(int i = 0 ; i < PARTICLE_PATTERN ; i++)
			SafetyRelease(m_pTexture[i]);
	}

protected:
	CParticle(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CParticle();

	virtual void SetVtxBuff(void);

	static LPDIRECT3DTEXTURE9 m_pTexture[PARTICLE_PATTERN];
	int	m_Pattern;

	D3DXVECTOR2 m_Size;		// �T�C�Y
	D3DXVECTOR3	m_Move;		// �ړ���
	D3DXVECTOR3	m_RotMove;	// ��]��
	D3DXVECTOR3	m_Wind;		// ����
	int			m_Life;
};

#endif