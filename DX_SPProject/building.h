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
const int	BUILDING_INSIDE			= 10;		// �����̏c�u���b�N��
const int	BUILDING_HORIZONTAL		= 10;		// �����̉��u���b�N��
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
	static SPLINE*		m_Spline;	// �X�v���C�����

	static vector<CSceneXDX*> m_Instance;					// �����̃C���X�^���X
	static MODELSTATUS m_BuildingMesh[BUILDING_TYPE_NUM];	// ���b�V���ւ̃|�C���^
};

#endif