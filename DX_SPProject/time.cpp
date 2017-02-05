//=============================================================================
//
//	�^�C�g��	�o�ߎ���
//	�t�@�C����	time.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2017/01/21
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "time.h"

//=============================================================================
//	�֐���	:CTime()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CTime::CTime(bool ifListAdd, int priority, OBJTYPE objType) : CScene2DDX(ifListAdd, priority, objType)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	�֐���	:~CTime()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CTime::Init(cVec3 pos, cVec2 size)
{
	m_ifCountStart	= false;
	m_StartTime		= 0;
	m_TimeValue		= 0;
	m_Time.SetTime(0, 0, 0);


	m_Instance[0] = CNumber::Create(0, D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 3.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[1] = CNumber::Create(0, D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 2.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[2] = CScene2DDX::Create(".\\data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 1.5f), pos.y, 0.0f),
		VEC3_ZERO, D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[3] = CNumber::Create(0, D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 0.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[4] = CNumber::Create(0, D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 0.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[5] = CScene2DDX::Create(".\\data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 1.5f), pos.y, 0.0f),
		VEC3_ZERO, D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[6] = CNumber::Create(0, D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 2.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[7] = CNumber::Create(0, D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 3.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));

	CountStart();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CTime::Uninit(void)
{

}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CTime::Update(void)
{
	// �J�E���g���̏ꍇ
	if(m_ifCountStart)
	{
		// ���ݎ��Ԏ擾
		m_TimeValue = timeGetTime() - m_StartTime;

		// ���ԕϊ�
		TimeConvert(&m_Time, m_TimeValue);

		// ���Ԃ��Z�b�g
		((CNumber*)m_Instance[0])->SetNumber((m_Time.minute <= 0) ? 0 : (m_Time.minute / 10));
		((CNumber*)m_Instance[1])->SetNumber((m_Time.minute <= 0) ? 0 : (m_Time.minute % 10));
		((CNumber*)m_Instance[3])->SetNumber((m_Time.second <= 0) ? 0 : (m_Time.second / 10));
		((CNumber*)m_Instance[4])->SetNumber((m_Time.second <= 0) ? 0 : (m_Time.second % 10));
		((CNumber*)m_Instance[6])->SetNumber((m_Time.millisec <= 0) ? 0 : (m_Time.millisec / 10));
		((CNumber*)m_Instance[7])->SetNumber((m_Time.millisec <= 0) ? 0 : (m_Time.millisec % 10));
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CTime::Draw(void)
{

}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CTime *CTime::Create(cVec3 pos, cVec2 size)
{
	CTime *instance;	// �C���X�^���X
	
	// �C���X�^���X����
	instance = new CTime();
	
	// ����������
	instance->Init(pos, size);
	
	// �C���X�^���X�����^�[��
	return instance;
}

//=============================================================================
//	�֐���	:SaveTime
//	����	:����
//	�߂�l	:����
//	����	:�����L���O�Ɉ����n�����Ԃ�ۑ��B
//=============================================================================
void CTime::SaveTime(void)
{
	FILE* fp;

	fopen_s(&fp, TIME_FILENAME, "w");

	fprintf_s(fp, "%d", m_TimeValue);

	fclose(fp);
}

//=============================================================================
//	�֐���	:TimeConvert
//	����	:����
//	�߂�l	:����
//	����	:�~���b�ŕ\���ꂽ���Ԃ𕪕b�~���b�ŕ\����鎞�Ԃɕϊ��B
//=============================================================================
void CTime::TimeConvert(TIME* timeStr, DWORD time)
{
	timeStr->minute = time / 1000 / 60;
	timeStr->second = time / 1000;
	timeStr->millisec = time % 1000 / 10;

	if(timeStr->minute > 99) timeStr->minute = 99;
	if(timeStr->minute > 59) timeStr->minute = 59;
	if(timeStr->minute > 99) timeStr->minute = 99;
}