#ifndef _RESULT_H_
#define _RESULT_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	result.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
#include "mode.h"
class CScene2DDX;
//=============================================================================
//	�N���X��`
//=============================================================================
class CResult : public CMode
{
public:
	CResult(){}
	~CResult(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static int		GetFrame(void) { return m_Frame; }

private:
	static int	m_Frame;	// �ėp�J�E���^
	CScene2DDX *m_ResultBG;
	CScene2DDX *m_PushEnter;
	float		m_Alpha;
};

#endif