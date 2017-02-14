#ifndef _BUILDING_H_
#define _BUILDING_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	building.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "sceneXDX.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define		BUILDING_SCALE			(5.0f)		// �����̃X�P�[��
const int	BUILDING_TYPE_NUM		= 17;		// �����̎��
const float	BUILDING_MARGIN			= 1000.0f;	// �������m�̊Ԋu
const int	BUILDING_INSIDE			= 15;		// �����̏c�u���b�N��
const int	BUILDING_HORIZONTAL		= 15;		// �����̉��u���b�N��
const int	BUILDING_ALL			= BUILDING_INSIDE * BUILDING_HORIZONTAL;	// �����̑��u���b�N��
const float	BUILDING_CLEAR_LENGTH	= BUILDING_MARGIN;	// �������\���ɂ��鋗��
const float	BUILDING_INVISIBLE_FAR	= 2000.0f;	// �������\���ɂ��鋗��

const float	BUILDING_STRIDE_X		= BUILDING_MARGIN * BUILDING_HORIZONTAL;	// �����̏c�u���b�N��
const float	BUILDING_STRIDE_Z		= BUILDING_MARGIN * BUILDING_INSIDE;		// �����̏c�u���b�N��
const float	BUILDING_MOVE_STRIDE	= BUILDING_MARGIN * 5.0f;						// �����̏c�u���b�N��

typedef struct {
	uint		Type;
	bool		Visible;
	D3DXVECTOR3	Pos;
} BUILDING;

//=============================================================================
//	�N���X��`
//=============================================================================
class CBuilding
{
public:
	static void	Init(void);
	static void	Uninit(void);
	static void	Update(void);
	static void	Draw(void);

	static void	RecalcTexCoord(LPD3DXMESH* mesh);

protected:
	static bool			m_ifInitialize;	// �����������t���O
	static SPLINE*		m_Spline;		// �X�v���C�����
	static D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X

	static MODELSTATUS		m_BuildingMesh[BUILDING_TYPE_NUM];	// ���b�V���ւ̃|�C���^
	static D3DXMATERIAL*	m_BuildingMat[BUILDING_TYPE_NUM];	// ���b�V���ւ̃|�C���^
	static BUILDING			m_Building[BUILDING_ALL];

	static uint __stdcall	UpdateS(void*);
	static uint				m_thID;	// �X���b�hID1
	static HANDLE			m_hTh;	// �X���b�h�n���h��1
};

#endif