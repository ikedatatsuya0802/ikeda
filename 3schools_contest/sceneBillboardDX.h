#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(sceneBillboard.cpp)
//	�t�@�C����	sceneBillboard.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	POLYGONBILLBOARDDX_POSX		(0.0f)	// �|���S���̉���
#define	POLYGONBILLBOARDDX_POSY		(100.0f)	// �|���S���̏c��
#define	POLYGONBILLBOARDDX_POSZ		(0.0f)	// �|���S���̏c��
#define	POLYGONBILLBOARDDX_WIDTH		(20.0f)	// �|���S���̉���
#define	POLYGONBILLBOARDDX_HEIGHT		(20.0f)	// �|���S���̏c��
#define	POLYGONBILLBOARDDX_TEXFILENAME000	"effect000.jpg"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CSceneBillboardDX : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGONBILLBOARDDX_TEXFILENAME000, &m_pTexture); }
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CSceneBillboardDX	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSceneBillboardDX(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneBillboardDX();
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	
	D3DXMATRIX				m_mtxWorld;	// ���[���h�}�g���b�N�X
};

#endif