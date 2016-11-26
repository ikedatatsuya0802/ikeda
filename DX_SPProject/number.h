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
	CNumber(int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CNumber();
	
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)){}
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f), int value = 0);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"NUMBER_TEXFILENAME000, &m_Texture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { if(m_Texture != NULL){m_Texture->Release(); m_Texture = NULL;} }

	static CNumber	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2 size = D3DXVECTOR2(0.0f, 0.0f), int value = 0);
	
	void	SetNumber(int value = 0);

protected:
	static LPDIRECT3DTEXTURE9	m_Texture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	float	m_fLength;	// �Ίp���̒���
	float	m_fAngle;	// �p�x
};

#endif