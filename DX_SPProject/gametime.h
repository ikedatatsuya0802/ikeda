#ifndef _GAMETIME_H_
#define _GAMETIME_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(gametime.cpp)
//	�t�@�C����	gametime.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/21
//
//=============================================================================
#include "number.h"

//=============================================================================
//	�}�N����`
//=============================================================================

//=============================================================================
//	�\����
//=============================================================================
typedef struct FIGURE{
	FIGURE(int upper = 0, bool dot = 0, int lower = 0) { Upper = upper; Dot = dot; Lower = lower; }	// �L���X�^

	int		Upper;	// ������
	bool	Dot;	// �����_
	int		Lower;	// ������
} FIGURE;			// ���������_���^

//=============================================================================
//	�N���X��`
//=============================================================================
class CGametime : public CNumber
{
public:
	CGametime(int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CGametime();
	
	void	Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CGametime	*Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure);

	void	SetValue(float value);

protected:
	static FIGURE	m_Figure;
	static int		m_NumFigure;

	CNumber	**m_Number;
	float	m_Time;
};

#endif