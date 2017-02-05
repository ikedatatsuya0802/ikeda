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
	CTime(int value = 0);
	~CTime();
	
	void	Init(int value, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CTime	*Create(int value = 0, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);

	void CountStart(void) { m_ifCountStart ? 0 : true; m_StartTime = timeGetTime(); }
	void CountStop(void) { m_ifCountStart = false; }
	void SaveTime(void);

protected:
	vector<CNumber>	m_Instance;		// �i���o�[�̃C���X�^���X
	bool			m_ifCountStart;	// �J�E���g�����ǂ���
	DWORD			m_StartTime;	// �v���J�n����
	TIME			m_Time;			// ���b�~���b�̎���
};

#endif