#ifndef _RANKING_H_
#define _RANKING_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(ranking.cpp)
//	�t�@�C����	ranking.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2017/01/21
//
//=============================================================================
#include "main.h"
#include "scene2DDX.h"
#include "number.h"
#include <windows.h>

//=============================================================================
//	�}�N����`
//=============================================================================
#define	RANKING_FILENAME	".\\data\\ranking.txt"	// ���Ԃ̕ۑ��t�@�C��
cint	RANKING_NUM			= 10;


//=============================================================================
//	�N���X��`
//=============================================================================
class CRanking : public CScene2DDX
{
public:
	CRanking(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objType = OBJTYPE_NUMBER);
	~CRanking();
	
	void	Init(cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO, cfloat margin = 0.0f);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CRanking	*Create(cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO, cfloat margin = 0.0f);

	void SaveRanking(void);
	void LoadRanking(void);
	
private:
	DWORD			m_NowValue;
	vector<DWORD>	m_Ranking;
	vector<DWORD>	m_NewRanking;
};

#endif