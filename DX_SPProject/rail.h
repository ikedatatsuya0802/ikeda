#ifndef _RAIL_H_
#define _RAIL_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	rail.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	RAIL_WIDTH		(15.0f)				// ����
#define	RAIL_MARGIN		(50.0f)				// ���[���̊Ԋu
#define	RAIL_SET		(20)				// ���[���̕�����
#define	RAIL_VECTOR		(4)					// ���[���̃x�N�g����
#define	RAIL_VERTEX		(RAIL_SET * 2 + 2)	// ���[���̒��_��
#define	RAIL_TEXFILENAME000	"rail000.png"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CRail : public CScene3DDX
{
public:
	void	Init(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAIL_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CRail	*Create(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	LoadSpline(int line = 0);
	void	CalcSpline(int line = 0);

protected:
	CRail(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CRail();
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	
	D3DXMATRIX	m_mtxWorld;				// ���[���h�}�g���b�N�X
	int			m_RailLine;				// ���[���̔z�u
	SPLINE*		m_Spline;
};

#endif