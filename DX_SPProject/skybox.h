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
#include "sceneDX.h"
#include "rendererDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	SKYBOX_TEXFILENAME000	"skybox000.jpg"						// �e�N�X�`���̃t�@�C����
#define	SKYBOX_SIZE				(60000.0f)							// �h�[���̏c�u���b�N��
#define	SKYBOX_PRIMITIVE_NUM	(6)									// �v���~�e�B�u��
#define	SKYBOX_VERTEX_NUM		(VERTEX_NUM * SKYBOX_PRIMITIVE_NUM)	// ���_��

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CSkybox : public CSceneDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(CRendererDX::GetDevice(), ".\\data\\TEXTURE\\"SKYBOX_TEXFILENAME000, &m_Texture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { if(m_Texture != NULL){m_Texture->Release(); m_Texture = NULL;} }

	static CSkybox	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSkybox(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkybox();

	void	SetSkyboxData(VERTEX_3D *pVtx);
	void	DrawSkyboxData(void);
	
	static LPDIRECT3DTEXTURE9	m_Texture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
};

#endif