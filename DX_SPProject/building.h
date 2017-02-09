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
const int	BUILDING_TYPE_NUM		= 17;		// �����̎��
const float	BUILDING_MARGIN			= 100.0f;	// �������m�̊Ԋu
const int	BUILDING_INSIDE			= 5;		// �����̏c�u���b�N��
const int	BUILDING_HORIZONTAL		= 5;		// �����̉��u���b�N��
const float	BUILDING_CLEAR_LENGTH	= 200.0f;	// �������\���ɂ��鋗��
const float	BUILDING_INVISIBLE_FAR	= 2000.0f;	// �������\���ɂ��鋗��

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

protected:
	static bool		m_ifInitialize;	// �����������t���O
	static SPLINE*	m_Spline;		// �X�v���C�����

	static vector<CSceneXDX*> m_Instance;					// �����̃C���X�^���X
	static MODELSTATUS m_BuildingMesh[BUILDING_TYPE_NUM];	// ���b�V���ւ̃|�C���^

	static uint __stdcall	UpdateThread(void*);
	static uint				m_thIDUpdate;			// �X���b�hID1
	static HANDLE			m_hThUpdate;			// �X���b�h�n���h��1
	static uint __stdcall	DrawThread(void*);
	static HANDLE			m_hThDraw;				// �X���b�h�n���h��1
	static uint				m_thIDDraw;				// �X���b�hID1
};

#endif