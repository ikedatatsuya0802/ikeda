#ifndef _SKYDOME_H_
#define _SKYDOME_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	SKYDOME_TEXFILENAME000	"sky001.jpg"						// �e�N�X�`���̃t�@�C����
#define	SKYDOME_VERTICAL		(8)						// �h�[���̏c�u���b�N��
#define	SKYDOME_HORIZONTAL		(32)					// �h�[���̉��u���b�N��
#define	SKYDOME_RADIUS			(5000.0f)				// �h�[���̔��a
const float SKYDOME_HEIGHT		= SKYDOME_RADIUS / 20.0f;	// �h�[���̏c��
const float SKYDOME_HEIGHTMAX	= SKYDOME_HEIGHT * (SKYDOME_VERTICAL + 1);	// �h�[���̓V��̍���
#define	SKYDOME_MOVEROT			(0.00001f)				// �h�[���̉�]��

const int	SKYDOME_DEGENERATE_POLYGON_NUM	= ((SKYDOME_VERTICAL - 1) * 4);	// �k�ރ|���S����
const int	SKYDOME_POLYGON_NUM
	= ((SKYDOME_HORIZONTAL * 2) * SKYDOME_VERTICAL + SKYDOME_DEGENERATE_POLYGON_NUM);		// �|���S����
const int	SKYDOME_INDEX_NUM
	= (((SKYDOME_HORIZONTAL + 1) * 2) * SKYDOME_VERTICAL + ((SKYDOME_VERTICAL - 1) * 2));	// ���_�C���f�b�N�X��

const int	SKYDOME_VERTEX_NUM			= ((SKYDOME_VERTICAL + 1) * (SKYDOME_HORIZONTAL + 1));	// ���_��
const int	SKYDOME_INDEX_STANDARD		= ((SKYDOME_HORIZONTAL + 1) * 2 + 2);					// �C���f�b�N�X�̊�l

//=============================================================================
//	�O���錾
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CSkydome : public CScene3DDX
{
public:
	CSkydome(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkydome();

	void	Init(bool ifLight = false, cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSkydome* Create(bool ifLight = false, cVec3 pos = VEC3_ZERO, cVec3 rot = VEC3_ZERO);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(SKYDOME_TEXFILENAME000), &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

protected:
	void	SetVtxBuff(bool ifLight);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
};

#endif