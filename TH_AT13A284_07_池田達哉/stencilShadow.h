#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"
#include "manager.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	STENCILSHADOW_TEXFILENAME000	"stencilShadow000.jpg"	// �e�N�X�`���̃t�@�C����

#define	STENCILSHADOW_RADIUS		(13.0f)		// ���a
#define	STENCILSHADOW_HORIZONTAL	(32)		// ���u���b�N��
#define	STENCILSHADOW_HEIGHT		(1000.0f)	// �c��

#define	STENCILSHADOW_POLYGON_NUM	(STENCILSHADOW_HORIZONTAL * 2)			// �|���S����
#define	STENCILSHADOW_INDEX_NUM		((STENCILSHADOW_HORIZONTAL + 1) * 2)	// ���_�C���f�b�N�X��

#define	STENCILSHADOW_WALL_VERTEX_NUM	((STENCILSHADOW_HORIZONTAL + 1) * 2)	// ���_��
#define STENCILSHADOW_COVER_VERTEX_NUM	(STENCILSHADOW_HORIZONTAL + 2)			// �~���̂ӂ�
#define STENCILSHADOW_INDEX_STANDARD	((STENCILSHADOW_HORIZONTAL + 1) * 2 + 2)	// �C���f�b�N�X�̊�l

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CStencilShadow : public CScene3DDX
{
public:
	CStencilShadow(bool isListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CStencilShadow();

	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CStencilShadow	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	static LPDIRECT3DTEXTURE9	m_Texture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffWall;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffCoverTop;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffCoverBottom;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffWindow;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	void	SetWorldMatrix(void);
	void	SetIndex(void);
	void	SetVertexData(float alpha = 1.0f);
	void	DrawPillar(void);

	D3DXMATRIX		m_mtxWorld;					// ���[���h�}�g���b�N�X
};

#endif