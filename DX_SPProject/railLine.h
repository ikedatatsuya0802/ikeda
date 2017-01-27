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
#define	RAILLINE_TEXFILENAME000	"effect000.jpg"			// �e�N�X�`���̃t�@�C����
#define	RAILLINE_TEXFILENAME001	"drift000.png"			// �e�N�X�`���̃t�@�C����
#define	RAILLINE_TEXFILENAME002	"drift001.png"			// �e�N�X�`���̃t�@�C����
#define	RAILLINE_TEXFILENAME003	"drift002.png"			// �e�N�X�`���̃t�@�C����

#define	RAILLINE_GOAL			(0.98f)					// �S�[���n�_�̃X�v���C�����W
#define	RAILLINE_LENGTH			(10000.0f)				// ����
#define	RAILLINE_WIDTH			(15.0f)					// ����
#define	RAILLINE_MARGIN			(50.0f)					// ���[���̊Ԋu
#define	RAILLINE_SET			(100)					// ���[���̕�����
#define	RAILLINE_VECTOR			(4)						// ���[���̃x�N�g����
#define	RAILLINE_VERTEX			(RAILLINE_SET * 2 + 2)	// ���[���̒��_��
#define	RAILLINE_SPOINT_SIZE	(20.0f)					// ���[�����_�̊m�F�p�G�t�F�N�g�T�C�Y
#define	RAILLINE_LPOINT_SIZE	(50.0f)					// �X�v���C�����_�̊m�F�p�G�t�F�N�g�T�C�Y
#define	RAILLINE_DRAG_SIZE		(100.0f)				// �h���b�O�̑I�𔼌a
#define	RAILLINE_DRIFT_SEARCH	(1000)					// �}�E�X����̍ŋߓ_�̒T����
#define	RAILLINE_DRIFT_DRAG		(0.02f)					// �h���t�g�}�[�N�̉��͈�

#define	RAILLINE_SLOPE			(1.0f)					// ��̌��z

//=============================================================================
//	�\����
//=============================================================================
typedef enum {
	ET_POSVEC = 0,
	ET_DRIFT,
	ET_MAX
} EDIT_TYPE;		// �h���t�g�_���

typedef struct {
	float	Begin;		// �h���t�g�̊J�n�_
	float	End;		// �h���t�g�̏I���_
	bool	Curve;		// �ǂ���ɃJ�[�u���邩(false:��, true:�E)
	bool	ifHoldBegin;// �z�[���h���
	bool	ifHoldEnd;	// �z�[���h���
} DRIFT_POINT;			// �h���t�g�_���

typedef struct {
	vector<D3DXVECTOR3>	Pos;		// �X�v���C���̗U���_
	vector<D3DXVECTOR3>	Vec;		// �X�v���C���̗U���x�N�g��
	vector<D3DXVECTOR3>	PosHermite;	// �X�v���C�������_
	D3DXVECTOR3			Rot;		// �X�v���C����ł�(���Y����)��]�l
	vector<int> 		ifHold;		// �}�E�X�Ńz�[���h�����ǂ���
	vector<DRIFT_POINT>	Drift;		// �h���t�g�_���
	vector<float>		LengthMin;	// �X�v���C����Ԓ�(��������)
	float				Length;		// �X�v���C����(��������)
} SPLINE;

typedef struct {
	bool	ifDrift;	// �h���t�g�͈͂�
	bool	Curve;		// �ǂ���̃J�[�u��
	int		Status;		// �h���t�g�̂ǂ̏�Ԃ��@-1->�n�_, 0->�h���t�g��, 1->�I�_
} DRIFT_STATUS;			// �h���t�g�X�e�[�^�X

//=============================================================================
//	�N���X��`
//=============================================================================
class CRailLine : public CScene3DDX
{
private:
	static LPDIRECT3DTEXTURE9	m_pTextures[2];	// �e�N�X�`���ւ̃|�C���^

public:
	void	Init(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CRailLine	*Create(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) {
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAILLINE_TEXFILENAME000, &m_pTextures[0]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAILLINE_TEXFILENAME001, &m_pTextures[1]);
	}

	static void	Unload(void) {
		SafetyRelease(m_pTextures[0]);
		SafetyRelease(m_pTextures[1]);
	}

	SPLINE* GetSpline(void) { return &m_Spline; }
	D3DXVECTOR3	GetSplinePos(float t);
	DRIFT_STATUS GetDriftStatus(float oldt, float t);

	D3DXVECTOR3 GetMoveVec(float t);
	static float AngleOf2Vector(D3DXVECTOR3 a, D3DXVECTOR3 b);

private:
	CRailLine(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
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
	void	SetSplineVtxDrift(int line = 0);
	
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffVec;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffSPoints;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffLPoints;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffPointer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuffDrift;	// ���_�o�b�t�@�ւ̃|�C���^


	EDIT_TYPE	m_EditType;		// �G�f�B�b�g���[�h�A0->Pos, 1->Vec, 2->Drift
	float		m_YScale;		// �J�����̍����ɉ������X�P�[��
	int			m_RailLine;		// ���[���̔z�u
	SPLINE		m_Spline;		// �X�v���C�����
};

#endif