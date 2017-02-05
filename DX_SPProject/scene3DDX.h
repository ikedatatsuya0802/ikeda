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
class CScene3DDX : public CSceneDX
{
public:
	CScene3DDX(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_3D);
	~CScene3DDX();

	void	Init(cchar *str = ".\\data\\TEXTURE\\field000.jpg", D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CScene3DDX	*Create(cchar* str = ".\\data\\TEXTURE\\field000.jpg",
		D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);

	// ���[���h�}�g���b�N�X���擾
	D3DXMATRIX	*GetWMatrix(void) { return &m_mtxWorld; }
	
	// ���\�[�X�̃��[�h
	void	Load(cchar* str) { D3DXCreateTextureFromFile(D3D_DEVICE, str, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	void	Unload(void) { SafetyRelease(m_pTexture); }

protected:
	LPDIRECT3DTEXTURE9		m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;	// ���[���h�}�g���b�N�X

	virtual void SetVtxBuff(void);
	static void SetMeshIndex(LPDIRECT3DINDEXBUFFER9 *idxBuff, const int horizontal, const int vertical);
	
};

#endif