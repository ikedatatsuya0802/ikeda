#ifndef _CYLINDER_H_
#define _CYLINDER_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"
#include "rendererDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	CYLINDER_TEXFILENAME000	"cylinder000.jpg"	// �e�N�X�`���̃t�@�C����

#define	CYLINDER_RADIUS			(29000.0f)						// ���a
#define	CYLINDER_VERTICAL		(1)								// �c�u���b�N��
#define	CYLINDER_HORIZONTAL		(32)							// ���u���b�N��
#define	CYLINDER_MOVEROT		(0.0f)						// ��]��
#define	CYLINDER_HEIGHT			(CYLINDER_RADIUS * 0.1f)		// �c��

#define	CYLINDER_TOTALWIDTH		(CYLINDER_HORIZONTAL * CYLINDER_WIDTH)	// ���v�̉���
#define	CYLINDER_TOTALHEIGHT	(CYLINDER_VERTICAL * CYLINDER_HEIGHT)	// ���v�̏c��

#define CYLINDER_DEGENERATE_POLYGON_NUM	((CYLINDER_VERTICAL - 1) * 4)															// �k�ރ|���S����
#define	CYLINDER_POLYGON_NUM			((CYLINDER_HORIZONTAL * 2) * CYLINDER_VERTICAL + CYLINDER_DEGENERATE_POLYGON_NUM)		// �|���S����
#define	CYLINDER_INDEX_NUM				(((CYLINDER_HORIZONTAL + 1) * 2) * CYLINDER_VERTICAL + ((CYLINDER_VERTICAL - 1) * 2))	// ���_�C���f�b�N�X��

#define	CYLINDER_VERTEX_NUM				((CYLINDER_VERTICAL + 1) * (CYLINDER_HORIZONTAL + 1))	// ���_��
#define CYLINDER_INDEX_STANDARD			((CYLINDER_HORIZONTAL + 1) * 2 + 2)						// �C���f�b�N�X�̊�l

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CCylinder : public CScene3DDX
{
public:
	CCylinder(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CCylinder();

	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"CYLINDER_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { if(m_pTexture != NULL){m_pTexture->Release(); m_pTexture = NULL;} }
	
	void				SetCylinderData(void);
	static CCylinder	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX		m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3		m_Nor[CYLINDER_VERTEX_NUM];	// �@�����
};

#endif