#ifndef _SCENE3D_H_
#define _SCENE3D_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
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
#define	POLYGON3DDX_POSX		(0.0f)	// �|���S���̉���
#define	POLYGON3DDX_POSY		(0.0f)	// �|���S���̏c��
#define	POLYGON3DDX_POSZ		(0.0f)	// �|���S���̏c��
#define	POLYGON3DDX_WIDTH		(30.0f)	// �|���S���̉���
#define	POLYGON3DDX_HEIGHT		(300.0f)	// �|���S���̏c��
#define	POLYGON3DDX_TEXFILENAME000	"field000.jpg"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CScene3DDX : public CSceneDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"POLYGON3DDX_TEXFILENAME000, &m_Texture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { if(m_Texture != NULL){m_Texture->Release(); m_Texture = NULL;} }

	static CScene3DDX	*Create(void);
	static CScene3DDX	*Create(D3DXVECTOR3 pos);

protected:
	CScene3DDX(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CScene3DDX();
	
	static LPDIRECT3DTEXTURE9	m_Texture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	
	D3DXMATRIX					m_mtxWorld;	// ���[���h�}�g���b�N�X
};

#endif