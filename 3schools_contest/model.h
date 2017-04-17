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

//----- ���_�t�H�[�}�b�g��`
typedef struct {
	float		x, y, z;			// ���_���W�i���W�ϊ�����j
	float		nx, ny, nz;			// �@���x�N�g��
	float		tu, tv;				// �e�N�X�`�����W
} VERTEX;
#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

typedef struct {
	LPD3DXMESH		pMesh;		// ���b�V�����
	LPD3DXBUFFER	pBuffMat;	// �}�e���A�����
	DWORD			NumMat;		// �}�e���A����
	VERTEX			*pt_vertex;	// ���_�f�[�^�ւ̃|�C���^
	WORD			*pt_index;	// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int				num_vertex;	// ���_��
	int				num_face;	// �ʐ�
	int				MorphTime;
} MODELSTATUS_MORPH;			// 3D���f�����

//=============================================================================
//	�N���X��`
//=============================================================================
class CModel : public CSceneXDX
{
public:
	CModel(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CModel();

	void	Init(string fileDir, string filename);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CModel	*Create(string fileDir, string filename);

	int		GetMorphCountAll(void) { return m_MorphCountAll; }

protected:
	void	LoadModel(string fileDir, string filename);
	string	to_string(int val);

	LPDIRECT3DTEXTURE9 m_pTexture;
	
	string		m_FileDir;

	VERTEX		*m_MorphVertex;			// ���[�t�B���O�p���_���[�N�ւ̃|�C���^
	WORD		*m_MorphIndex;			// ���[�t�B���O�p�C���f�b�N�X���[�N�ւ̃|�C���^

	vector<MODELSTATUS_MORPH>	m_ModelStatus;	// 3D���f�����
	int	m_MorphCountAll;	// ���[�t�B���O�J�E���g�̍��v
	int	m_MorphCount;		// ���[�t�B���O�J�E���g
	int	m_MorphStatus;		// ���[�t�B���O���
};

#endif