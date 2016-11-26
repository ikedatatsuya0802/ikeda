#ifndef _TITLE_H_
#define _TITLE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	title.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
#include "mode.h"
class CScene2DDX;
//=============================================================================
//	�N���X��`
//=============================================================================
class CTitle : public CMode
{
public:
	CTitle(){}
	~CTitle(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
private:
	CScene2DDX	*m_TitleBG;
	CScene2DDX	*m_PushEnter;
	float		m_Alpha;
};

#endif