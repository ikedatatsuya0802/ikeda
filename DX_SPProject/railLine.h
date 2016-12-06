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
#include <vector>
using namespace std;

//=============================================================================
//	�}�N����`
//=============================================================================
#define	RAILLINE_WIDTH			(15.0f)					// ����
#define	RAILLINE_MARGIN			(50.0f)					// ���[���̊Ԋu
#define	RAILLINE_SET			(80)					// ���[���̕�����
#define	RAILLINE_VECTOR			(4)						// ���[���̃x�N�g����
#define	RAILLINE_VERTEX			(RAILLINE_SET * 2 + 2)	// ���[���̒��_��
#define	RAILLINE_TEXFILENAME000	"effect000.jpg"			// �e�N�X�`���̃t�@�C����
#define	RAILLINE_SPOINT_SIZE	(20.0f)					// ���[�����_�̊m�F�p�G�t�F�N�g�T�C�Y
#define	RAILLINE_LPOINT_SIZE	(50.0f)					// �X�v���C�����_�̊m�F�p�G�t�F�N�g�T�C�Y
#define	RAILLINE_DRAG_SIZE		(100.0f)					// �h���b�O�̑I�𔼌a

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	float	Begin;	// �h���t�g�̊J�n�_
	float	End;	// �h���t�g�̏I���_
	bool	Curve;	// �ǂ���ɃJ�[�u���邩(true:��, false:�E)
} DRIFT_POINT;		// �h���t�g�_���

typedef struct {
	vector<D3DXVECTOR3>	Pos;			// �X�v���C���̗U���_
	vector<D3DXVECTOR3>	Vec;			// �X�v���C���̗U���x�N�g��
	vector<D3DXVECTOR3>	PosHermite;		// �X�v���C�������_
	D3DXVECTOR3			Rot;			// �X�v���C����ł�(���Y����)��]�l
	vector<bool>		ifHold;			// �}�E�X�Ńz�[���h�����ǂ���
	vector<DRIFT_POINT>	Drift;			// �h���t�g�_���
} SPLINE;

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

	static CRailLine	*Create(int line = 0, D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SPLINE* GetSpline(void) { return &m_Spline; }

private:
	CRailLine(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CRailLine();

	void	MouseEdit();
	void	AddPoints();
	void	DeletePoints();
	void	SaveSpline();
	void	LoadSpline(int line = 0);
	void	CalcSpline(int line = 0);
	void	SetSplineVtx(int line = 0);
	void	SetSplineVtxVec(int line = 0);
	void	SetSplineVtxSPoints(int line = 0);
	void	SetSplineVtxLPoints(int line = 0);
	void	SetSplineVtxPointer(int line = 0);
	
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffVec;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffSPoints;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLPoints;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffPointer;	// ���_�o�b�t�@�ւ̃|�C���^

	float		m_YScale;		// �J�����̍����ɉ������X�P�[��
	int			m_RailLineLine;	// ���[���̔z�u
	SPLINE		m_Spline;		// �X�v���C�����
};

#endif