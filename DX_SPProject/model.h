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
#include "sceneXDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define SPLINE_BUFF	(0.05f)

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CPlayer;
class CModel : public CSceneXDX
{
public:
	CModel(bool ifListAdd = false, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CModel();

	void	Init(char *filename, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	void			SetParent(CModel *model) { m_Parent = model; }
	void			SetPosDef(D3DXVECTOR3 pos = VEC3_ZERO) { m_PosDef = pos; }
	void			SetRotDef(D3DXVECTOR3 rot = VEC3_ZERO) { m_RotDef = rot; }
	void			SetPos(D3DXVECTOR3 pos = VEC3_ZERO) { m_Pos = pos; }
	void			SetRot(D3DXVECTOR3 rot = VEC3_ZERO) { m_Rot = rot; }

	static CModel	*Create(char *filename, D3DXVECTOR3 pos = VEC3_ZERO);

private:
	void	LoadModel(char *filename);
	void	AutomaticSetTexture(void);
	void	AddTexture(vector<TEXTURE> &texture, char* fileName);

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