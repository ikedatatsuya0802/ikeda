#ifndef _MAP_H_
#define _MAP_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	map.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	MAP_WIDTH			(SCREEN_WIDTH * 0.3f * WINDOW_ASPECT_X)	// �}�b�v�̉���
#define	MAP_HEIGHT			(MAP_WIDTH)								// �}�b�v�̏c��
#define	MAP_POINT_WIDTH		(MAP_WIDTH * 0.05f)	// �}�b�v�̉���
#define	MAP_POINT_HEIGHT	(MAP_WIDTH * 0.05f)	// �}�b�v��
#define	MAP_POINT_ASPECT	(0.01f)		// �}�b�v�̏c��

#define	MAP_TEXFILENAME000	"map000.png"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CMap : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MAP_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CMap	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CMap(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CMap();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff[5];	// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9	m_pMapTexture;	// �}�b�v�e�N�X�`��
	LPDIRECT3DSURFACE9	m_pMapSurf;		// �}�b�v�e�N�X�`���̃T�[�t�F�X
	LPDIRECT3DTEXTURE9	m_pMapTexture2;	// �}�b�v�e�N�X�`��
	LPDIRECT3DSURFACE9	m_pMapSurf2;		// �}�b�v�e�N�X�`���̃T�[�t�F�X
	LPDIRECT3DSURFACE9	m_BackBufferSurf;

	int			m_MapLine;				// ���[���̔z�u
	SPLINE*		m_Spline;
	D3DXVECTOR3 m_SplinePos;
};

#endif