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
#include "scene2DDX.h"
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
class CNumber : public CScene2DDX
{
public:
	CNumber(int value = 0);
	~CNumber();
	
	void	Init(int value, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CNumber	*Create(int value = 0, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"NUMBER_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }
	
	void	SetNumber(int value = 0);

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
};

#endif