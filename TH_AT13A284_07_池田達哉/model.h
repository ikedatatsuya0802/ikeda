#ifndef _MODEL_H_
#define _MODEL_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(model.cpp)
//	�t�@�C����	model.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/14
//
//=============================================================================
#include "sceneDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================

//=============================================================================
//	�\����
//=============================================================================
typedef struct{
	LPD3DXMESH		pMesh;		// ���b�V�����
	LPD3DXBUFFER	pBuffMat;	// �}�e���A�����
	DWORD			NumMat;		// �}�e���A����
} MODELSTATUS;					// 3D���f�����

typedef struct {
	char*				FileName;	// ���b�V�����
	LPDIRECT3DTEXTURE9	pTexture;	// �e�N�X�`���ւ̃|�C���^
} TEXTURE;	// �e�N�X�`�����

//=============================================================================
//	�N���X��`
//=============================================================================
class CPlayer;
class CModel
{
public:
	CModel(int priority = 1);
	~CModel();

	void	Init(char *filename, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	void			SetParent(CModel *model) { m_Parent = model; }
	void			SetPosDef(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_PosDef = pos; }
	void			SetRotDef(D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_RotDef = rot; }
	void			SetPos(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_Pos = pos; }
	void			SetRot(D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_Rot = rot; }

	static CModel	*Create(char *filename, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void			LoadModel(char *filename);

private:
	vector<TEXTURE>	m_Texture;	// �e�N�X�`���ւ̃|�C���^
	void			AutomaticSetTexture(void);
	void			AddTexture(vector<TEXTURE> &texture, char* fileName);

	CModel				*m_Parent;		// �e�p�[�c
	
	D3DXVECTOR3			m_PosDef;		// ����W
	D3DXVECTOR3			m_RotDef;		// ���]
	D3DXVECTOR3			m_Pos;			// ���W
	D3DXVECTOR3			m_Rot;			// ��]
	D3DXMATRIX			m_mtxWorld;		// ���[���h�}�g���b�N�X
	MODELSTATUS			m_ModelStatus;	// 3D���f�����
	
	D3DXMATRIX			GetWorldMatrix(void) { return m_mtxWorld; }	// ���[���h�}�g���b�N�X�󂯓n��
};

#endif