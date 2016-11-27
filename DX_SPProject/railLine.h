#ifndef _RAILLINE_H_
#define _RAILLINE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	railLine.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "rail.h"
#include <vector>
using namespace std;

//=============================================================================
//	�}�N����`
//=============================================================================
#define	RAILLINE_WIDTH			(15.0f)				// ����
#define	RAILLINE_MARGIN			(50.0f)				// ���[���̊Ԋu
#define	RAILLINE_SET			(20)				// ���[���̕�����
#define	RAILLINE_VECTOR			(4)					// ���[���̃x�N�g����
#define	RAILLINE_VERTEX			(RAILLINE_SET * 2 + 2)	// ���[���̒��_��
#define	RAILLINE_TEXFILENAME000	"rail000.png"	// �|���S���̃t�@�C����
#define	RAILLINE_SPOINT_SIZE	(10.0f)			// 
#define	RAILLINE_LPOINT_SIZE	(30.0f)			// 

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	int			nNum;					// �X�v���C���̗U���_��
	vector<D3DXVECTOR3>	Pos;			// �X�v���C���̗U���_
	vector<D3DXVECTOR3>	Vec;			// �X�v���C���̗U���x�N�g��
	vector<D3DXVECTOR3>	PosHermite;		// �X�v���C�������_
	D3DXVECTOR3	Rot;					// �X�v���C����ł�(���Y����)��]�l
} SPLINE2;

//=============================================================================
//	�N���X��`
//=============================================================================
class CRailLine : public CScene3DDX
{
public:
	void	Init(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAILLINE_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { if(m_pTexture != NULL){m_pTexture->Release(); m_pTexture = NULL;} }

	static CRailLine	*Create(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CRailLine(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CRailLine();

	void	LoadSpline(int line = 0);
	void	CalcSpline(int line = 0);
	void	SetSplineVtx(int line = 0);
	void	SetSplineVtxSPoints(int line = 0);
	void	SetSplineVtxLPoints(int line = 0);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DTEXTURE9	m_pTexturePoints;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffSPoints;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLPoints;	// ���_�o�b�t�@�ւ̃|�C���^
	
	D3DXMATRIX	m_mtxWorld;				// ���[���h�}�g���b�N�X
	int			m_RailLineLine;				// ���[���̔z�u
	SPLINE2		m_Spline;
};

#endif