#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
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
#define	MESHFIELD_TEXFILENAME000	"field000.jpg"	// �n�ʃ|���S���̃t�@�C����

#define	MESHFIELD_VERTICAL		(30)				// ���b�V���t�B�[���h�̏c�u���b�N��
#define	MESHFIELD_HORIZONTAL	(30)				// ���b�V���t�B�[���h�̉��u���b�N��
#define	MESHFIELD_WIDTH			(2000.0f)			// �n�ʂ̉���
#define	MESHFIELD_HEIGHT		(2000.0f)			// �n�ʂ̏c��

#define	MESHFIELD_TOTALWIDTH	(MESHFIELD_HORIZONTAL * MESHFIELD_WIDTH)	// ���b�V���t�B�[���h�̍��v�̉���
#define	MESHFIELD_TOTALHEIGHT	(MESHFIELD_VERTICAL * MESHFIELD_HEIGHT)		// ���b�V���t�B�[���h�̍��v�̏c��

#define MESHFIELD_DEGENERATE_POLYGON_NUM	((MESHFIELD_VERTICAL - 1) * 4)																// �k�ރ|���S����
#define	MESHFIELD_POLYGON_NUM				((MESHFIELD_HORIZONTAL * 2) * MESHFIELD_VERTICAL + MESHFIELD_DEGENERATE_POLYGON_NUM)		// �|���S����
#define	MESHFIELD_INDEX_NUM					(((MESHFIELD_HORIZONTAL + 1) * 2) * MESHFIELD_VERTICAL + ((MESHFIELD_VERTICAL - 1) * 2))	// ���_�C���f�b�N�X��

#define	MESHFIELD_VERTEX_NUM				((MESHFIELD_VERTICAL + 1) * (MESHFIELD_HORIZONTAL + 1))	// ���_��
#define MESHFIELD_INDEX_STANDARD			((MESHFIELD_HORIZONTAL + 1) * 2 + 2)					// �C���f�b�N�X��

#define MESHFIELD_WAVEROT	(0.007f)	// �E�F�[�u�̎���
#define MESHFIELD_WAVE		(100.0f)	// �E�F�[�u�̏c��
#define MESHFIELD_WAVESPEED	(10.0f)		// �E�F�[�u�̃X�s�[�h

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CMeshfield : public CSceneDX
{
public:
	CMeshfield(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CMeshfield();

	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MESHFIELD_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	inline int	GetFrontMesh(D3DXVECTOR3 pos);
	float		GetHeight(D3DXVECTOR3 pos);
	
	void				SetMeshfieldData(void);
	static CMeshfield	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	D3DXVECTOR3			GetMeshfieldPos(void){ return m_Pos; }
	D3DXVECTOR3			*GetMeshfieldNor(void){ return m_Nor; }
	static D3DXVECTOR3	GetVtxPos(int nNumVtx);
	static D3DXVECTOR3	GetVtxNor(int nNumVtx);

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX		m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_Nor[MESHFIELD_VERTEX_NUM];	// �@�����
};

#endif