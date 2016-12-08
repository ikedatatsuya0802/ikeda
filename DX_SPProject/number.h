#ifndef _NUMBER_H_
#define _NUMBER_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(number.cpp)
//	�t�@�C����	number.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/21
//
//=============================================================================
#include "sceneDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	NUMBER_TEXFILENAME000	"number000.png"	// �|���S���̃t�@�C����

//=============================================================================
//	�}�N����`
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CNumber : public CSceneDX
{
public:
	CNumber(bool ifListAdd = false, int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CNumber();
	
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO){}
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO, int value = 0);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"NUMBER_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CNumber	*Create(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO, int value = 0);
	
	void	SetNumber(int value = 0);

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	float	m_fLength;	// �Ίp���̒���
	float	m_fAngle;	// �p�x
};

#endif