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
typedef struct {
	string				FileName;	// ���b�V�����
	LPDIRECT3DTEXTURE9	pTexture;	// �e�N�X�`���ւ̃|�C���^
} TEXTURE;	// �e�N�X�`�����

typedef struct {
	LPD3DXMESH		pMesh;		// ���b�V�����
	LPD3DXBUFFER	pBuffMat;	// �}�e���A�����
	DWORD			NumMat;		// �}�e���A����
	vector<TEXTURE>	Tex;		// �e�N�X�`�����
} MODELSTATUS;					// 3D���f�����


//=============================================================================
//	�N���X��`
//=============================================================================
class CSceneXDX : public CSceneDX
{
public:
	CSceneXDX(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneXDX();

	void	Init(cchar* fileName, MODELSTATUS* mesh = NULL, const D3DXVECTOR3 pos = VEC3_ZERO, const D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSceneXDX* Create(cchar* fileName, MODELSTATUS* mesh = NULL, const D3DXVECTOR3 pos = VEC3_ZERO, const D3DXVECTOR3 rot = VEC3_ZERO);
	
	static void	LoadModel(cchar* filename, MODELSTATUS* modelStatus);
	static void	AutomaticSetTexture(MODELSTATUS* modelStatus);
	static void	AddTexture(vector<TEXTURE> &texture, char* fileName);

	D3DXMATRIX* GetWMatrix(void) { return &m_mtxWorld; }	// ���[���h�}�g���b�N�X���擾


protected:
	D3DXMATRIX		m_mtxWorld;		// ���[���h�}�g���b�N�X

	MODELSTATUS*	m_ModelStatus;	// 3D���f�����
};

#endif