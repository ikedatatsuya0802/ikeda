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
#include "sceneXDX.h"

typedef struct {
	LPD3DXMESH		pMesh;		// ���b�V�����
	LPD3DXBUFFER	pBuffMat;	// �}�e���A�����
	DWORD			NumMat;		// �}�e���A����
	int				MorphTime;
} MODELSTATUS_MORPH;			// 3D���f�����

//=============================================================================
//	�N���X��`
//=============================================================================
class CModel : public CSceneXDX
{
public:
	CModel(bool ifListAdd = false, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CModel();

	void	Init(string fileDir, string filename);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CModel	*Create(string fileDir, string filename);
	
	void			SetParent(CModel *model) { m_Parent = model; }
	void			SetPosDef(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_PosDef = pos; }
	void			SetRotDef(D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { m_RotDef = rot; }

protected:
	void	LoadModel(string fileDir, string filename);
	void	AutomaticSetTexture(void);
	void	AddTexture(vector<TEXTURE> &texture, string fileName);
	string	to_string(int val);

	string			m_FileDir;
	CModel			*m_Parent;		// �e�p�[�c
	
	D3DXVECTOR3		m_PosDef;		// ����W
	D3DXVECTOR3		m_RotDef;		// ���]

	vector<MODELSTATUS_MORPH>	m_ModelStatus;	// 3D���f�����
	int	m_MorphCount;	// ���[�t�B���O�J�E���g
	int	m_MorphStatus;	// ���[�t�B���O���
};

#endif