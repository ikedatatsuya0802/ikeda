//=============================================================================
//
//	�^�C�g��	�o�ߎ���
//	�t�@�C����	ranking.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2017/01/21
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "ranking.h"

//=============================================================================
//	�֐���	:CRanking()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CRanking::CRanking(bool ifListAdd, int priority, OBJTYPE objType) : CScene2DDX(ifListAdd, priority, objType)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	�֐���	:~CRanking()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:const D3DXVECTOR3	->	pos(�����ʒu)
//			:const D3DXVECTOR2	->	size(�T�C�Y)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CRanking::Init(cVec3 pos, cVec2 size, cfloat margin)
{
	DWORD value;
	m_RankIn = -1;

	// �����L���O�ǂݍ���
	LoadRanking();
	value = m_NowValue;

	for(int i = 0, j = 0 ; i < RANKING_NUM ; i++)
	{
		if(m_Ranking[j] < value)
		{
			m_NewRanking.push_back(m_Ranking[j]);
			j++;
		}
		else
		{
			m_NewRanking.push_back(value);
			m_RankIn = i;
			value = 1000000;
		}
	}

	// �����L���O��ۑ�
	SaveRanking();


	// �����L���O��\��
	for(int i = 0 ; i < RANKING_NUM ; i++)
	{
		float height = (size.y - (RANKING_NUM - 1) * margin) / RANKING_NUM;
		float y = pos.y + (i - (RANKING_NUM / 2)) * (height + margin);

		CScene2DDX::Create(PRIORITY_2D, ".\\data\\TEXTURE\\ranking000.png", D3DXVECTOR3((pos.x - SCREEN_WIDTH * 0.3f), y, 0.0f), D3DXVECTOR2(height * 2, height))
			->SetUV(D3DXVECTOR2(0.0f, (i / 10.0f)), D3DXVECTOR2(1.0f, (i / 10.0f + 0.1f)));
		m_Instance[i] = CTime::Create(m_NewRanking[i], D3DXVECTOR3(pos.x, y, 0.0f), D3DXVECTOR2(size.x, height));
	}
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CRanking::Uninit(void)
{

}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CRanking::Update(void)
{
	static float alpha = 0.0f;

	if(m_RankIn >= 0)
	{
		m_Instance[m_RankIn]->SetColor((cosf(alpha) / 2 + 0.5f));
	}

	alpha += 0.1f;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CRanking *CRanking::Create(cVec3 pos, cVec2 size, cfloat margin)
{
	CRanking *instance;	// �C���X�^���X
	
	// �C���X�^���X����
	instance = new CRanking();
	
	// ����������
	instance->Init(pos, size, margin);
	
	// �C���X�^���X�����^�[��
	return instance;
}

//=============================================================================
//	�֐���	:SaveRanking
//	����	:����
//	�߂�l	:����
//	����	:�����L���O��ۑ��B
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE* fp;

	fopen_s(&fp, RANKING_FILENAME, "w");

	// ���ʐ��Ԃ񃉃��L���O����ۑ�
	for(uint i = 0 ; i < RANKING_NUM ; i++)
	{
		fprintf_s(fp, "%d\n", m_NewRanking[i]);
	}

	fclose(fp);
}

//=============================================================================
//	�֐���	:LoadRanking
//	����	:����
//	�߂�l	:����
//	����	:�����L���O��ǂݍ��݁B
//=============================================================================
void CRanking::LoadRanking(void)
{
	FILE* fp;


	// ����̃X�R�A��ǂݍ���
	fopen_s(&fp, TIME_FILENAME, "r");
	fscanf_s(fp, "%d", &m_NowValue);
	fclose(fp);


	fopen_s(&fp, RANKING_FILENAME, "r");

	// ���ʐ��Ԃ񃉃��L���O����ǂݍ���
	for(uint i = 0 ; i < RANKING_NUM ; i++)
	{
		DWORD value;
		fscanf_s(fp, "%d\n", &value);
		m_Ranking.push_back(value);
	}

	fclose(fp);
}