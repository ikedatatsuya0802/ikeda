#ifndef _SKYBOX_H_
#define _SKYBOX_H_
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
#define	SKYBOX_TEXFILENAME000	"skybox003.jpg"						// �e�N�X�`���̃t�@�C����
#define	SKYBOX_SIZE				(60000.0f)							// �h�[���̏c�u���b�N��
#define	SKYBOX_PRIMITIVE_SQUARE	(6)									// �v���~�e�B�u��
#define	SKYBOX_VERTEX_NUM		(VERTEX_SQUARE * SKYBOX_PRIMITIVE_SQUARE)	// ���_��

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CSkybox : public CScene3DDX
{
public:
	CSkybox(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkybox();

	void	Init(bool ifLight = false, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSkybox	*Create(bool ifLight = false, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(SKYBOX_TEXFILENAME000), &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

protected:
	void	SetVtxBuff(bool ifLight);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;	// �e�N�X�`���ւ̃|�C���^
};

#endif