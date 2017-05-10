#ifndef _FARGOAL_H_
#define _FARGOAL_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(farGoal.cpp)
//	�t�@�C����	farGoal.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/12/10
//
//=============================================================================
#include "number.h"
#include "sceneDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CFarGoal : CSceneDX
{
public:
	CFarGoal(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CFarGoal();
	
	void	Init(uint defValue, cVec3 pos, cVec2 size);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CFarGoal* Create(uint defValue, cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO);

	void	SetValue(int value);

protected:
	vector<CNumber*>	m_Number;
	float	m_FarLength;		// �S�[���܂ł̋���
};

#endif