#ifndef _SCENEX_H_
#define _SCENEX_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(sceneX.cpp)
//	�t�@�C����	sceneX.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "sceneDX.h"
#include "scene3DDX.h"
#include "model.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	MODEL_TEXTURENUM		(4)
#define	MODEL_TEXFILENAME000	"player000.png"	// �e�N�X�`���̃t�@�C����
#define	MODEL_TEXFILENAME001	"player001.png"	// �e�N�X�`���̃t�@�C����
#define	MODEL_TEXFILENAME002	"player002.jpg"	// �e�N�X�`���̃t�@�C����
#define	MODEL_TEXFILENAME003	"player003.jpg"	// �e�N�X�`���̃t�@�C����

#define	MODEL_MOVEMENT			(0.3f)			// �e�N�X�`���̃t�@�C����
#define	MODEL_ROT_STEP			(8)				// �v���C���[�̉�]�X�e�b�v

#define	MODEL_SPEED_DOWN		(0.05f)			// �v���C���[�̉�]�X�e�b�v
#define	MODEL_JUMP				(20.0f)			// �v���C���[�̉�]�X�e�b�v
#define	MODEL_GRAVITY			(0.5f)			// �v���C���[�̉�]�X�e�b�v

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CSceneXDX : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(int texNum) {	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MODEL_TEXFILENAME000, &m_pTexture[0]);
									D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MODEL_TEXFILENAME001, &m_pTexture[1]);
									D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MODEL_TEXFILENAME002, &m_pTexture[2]);
									D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MODEL_TEXFILENAME003, &m_pTexture[3]);
									D3DXLoadMeshFromX("./data/MODEL/box.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL, &m_pBuffMat, NULL, &m_NumMat, &m_pMesh);}
	// ���\�[�X�̃A�����[�h
	static void	Unload(int texNum) { for(int i = 0 ; i < MODEL_TEXTURENUM ; i++){if(m_pTexture != NULL){m_pTexture[texNum]->Release(); m_pTexture[texNum] = NULL;}}
									if(m_pMesh != NULL){ m_pMesh->Release(); m_pMesh = NULL; }if(m_pBuffMat != NULL){ m_pBuffMat->Release(); m_pBuffMat = NULL;} }

	static CSceneXDX	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

	D3DXMATRIX	*GetWMatrix(void) { return &m_mtxWorld; }	// ���[���h�}�g���b�N�X���擾

protected:
	CSceneXDX(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneXDX();

	static LPDIRECT3DTEXTURE9	m_pTexture[MODEL_TEXTURENUM];	// �e�N�X�`���ւ̃|�C���^

	D3DXMATRIX			m_mtxWorld;		// ���[���h�}�g���b�N�X
	static LPD3DXMESH	m_pMesh;		// ���b�V�����
	static LPD3DXBUFFER	m_pBuffMat;		// �}�e���A�����
	static DWORD		m_NumMat;		// �}�e���A����

	int					m_nCntMove;		// �ړ��J�E���^
	D3DXVECTOR3			m_Move;			// �v���C���[�̑��Έړ���
	D3DXVECTOR3			m_RotMove;		// �v���C���[�̑��Ή�]��
	bool				m_bJump;		// �W�����v���
};

#endif