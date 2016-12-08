#ifndef _ORBIT_H_
#define _ORBIT_H_
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

//=============================================================================
//	�}�N����`
//=============================================================================
#define	ORBIT_TEXFILENAME000	"field000.jpg"	// �e�N�X�`����

#define	ORBIT_VERTICAL		(1)				// ���b�V���̏c�u���b�N��
#define	ORBIT_HORIZONTAL	(5)				// ���b�V���̉��u���b�N��
#define	ORBIT_WIDTH			(20.0f)				// �|���S���̉���
#define	ORBIT_HEIGHT		(10.0f)			// �|���S���̏c��

#define	ORBIT_TOTALWIDTH	(ORBIT_HORIZONTAL * ORBIT_WIDTH)	// ���b�V���̍��v�̉���
#define	ORBIT_TOTALHEIGHT	(ORBIT_VERTICAL * ORBIT_HEIGHT)		// ���b�V���̍��v�̏c��

#define ORBIT_DEGENERATE_POLYGON_NUM	((ORBIT_VERTICAL - 1) * 4)																// �k�ރ|���S����
#define	ORBIT_POLYGON_NUM				((ORBIT_HORIZONTAL * 2) * ORBIT_VERTICAL + ORBIT_DEGENERATE_POLYGON_NUM)		// �|���S����
#define	ORBIT_INDEX_NUM					(((ORBIT_HORIZONTAL + 1) * 2) * ORBIT_VERTICAL + ((ORBIT_VERTICAL - 1) * 2))	// ���_�C���f�b�N�X��

#define	ORBIT_VERTEX_NUM				((ORBIT_VERTICAL + 1) * (ORBIT_HORIZONTAL + 1))	// ���_��
#define ORBIT_INDEX_STANDARD			((ORBIT_HORIZONTAL + 1) * 2 + 2)					// �C���f�b�N�X�̊���_

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class COrbit : public CSceneDX
{
public:
	COrbit(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~COrbit();

	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ORBIT_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static COrbit		*Create(D3DXVECTOR3 pos = VEC3_ZERO);
	
	void	SetOrbitData(void);
	void	SetLocPos(D3DXVECTOR3 locPos1, D3DXVECTOR3 locPos2) { m_LocPos[0] = locPos1; m_LocPos[1] = locPos2; }


protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX		m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_Nor[ORBIT_VERTEX_NUM];	// �@�����

	D3DXVECTOR3		m_OldPos[ORBIT_VERTEX_NUM];
	D3DXVECTOR3		m_LocPos[2];

};

#endif