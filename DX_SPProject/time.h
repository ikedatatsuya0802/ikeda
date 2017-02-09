#ifndef _TIME_H_
#define _TIME_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(time.cpp)
//	�t�@�C����	time.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2017/01/21
//
//=============================================================================
#include "scene2DDX.h"
#include "number.h"
#include <windows.h>

//=============================================================================
//	�}�N����`
//=============================================================================
#define	TIME_FILENAME	".\\data\\time.txt"	// ���Ԃ̕ۑ��t�@�C��

const int TIME_FIGURE = 8;	// ���Ԃ̌���(mm:ss:mm)

//=============================================================================
//	�}�N����`
//=============================================================================
typedef struct {
	int minute;		// ��
	int second;		// �b
	int millisec;	// �~���b
public:
	void SetTime(int min, int sec, int mil) { minute = min; second = sec; millisec = mil; }
} TIME;

//=============================================================================
//	�N���X��`
//=============================================================================
class CTime : public CScene2DDX
{
public:
	CTime(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objType = OBJTYPE_NUMBER);
	~CTime();
	
	void	Init(DWORD time = 0, cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CTime* Create(DWORD time = 0, cVec3 pos = VEC3_ZERO, cVec2 size = VEC2_ZERO);

	void SetColor(cfloat a = 1.0f, cfloat r = 1.0f, cfloat g = 1.0f, cfloat b = 1.0f);
	void CountStart(void) { m_ifCountStart = true; m_StartTime = timeGetTime(); }
	void CountStop(void) { m_ifCountStart = false; }
	void SaveTime(void);

	static void TimeConvert(TIME* timeStr, DWORD time);

protected:
	CScene2DDX*		m_Instance[TIME_FIGURE];	// �i���o�[�̃C���X�^���X
	bool			m_ifCountStart;				// �J�E���g�����ǂ���
	DWORD			m_StartTime;				// �v���J�n����
	DWORD			m_TimeValue;				// �o�ߎ���
	TIME			m_Time;						// ���b�~���b�̎���
};

#endif