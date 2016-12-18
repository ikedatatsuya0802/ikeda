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

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	LPD3DXMESH		pMesh;		// ���b�V�����
	LPD3DXBUFFER	pBuffMat;	// �}�e���A�����
	DWORD			NumMat;		// �}�e���A����
} MODELSTATUS;					// 3D���f�����

typedef struct {
	string				FileName;	// ���b�V�����
	LPDIRECT3DTEXTURE9	pTexture;	// �e�N�X�`���ւ̃|�C���^
} TEXTURE;	// �e�N�X�`�����

//=============================================================================
//	�N���X��`
//=============================================================================
class CSceneXDX : public CScene3DDX
{
public:
	void	Init(char* fileName, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSceneXDX	*Create(char* fileName, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR3 rot = VEC3_ZERO);

protected:
	CSceneXDX(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSceneXDX();

	void	LoadModel(char *filename);
	void	AutomaticSetTexture(void);
	void	AddTexture(vector<TEXTURE> &texture, char* fileName);

	MODELSTATUS	m_ModelStatus;	// 3D���f�����
	vector<TEXTURE>	m_pTexture;	// �e�N�X�`���ւ̃|�C���^
};

#endif