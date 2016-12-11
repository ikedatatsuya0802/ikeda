//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	farGoal.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "farGoal.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	�֐���	:CFarGoal()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CFarGoal::CFarGoal(bool ifListAdd, int priority, OBJTYPE objtype)
{

}

//=============================================================================
//	�֐���	:~CFarGoal()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CFarGoal::~CFarGoal()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CFarGoal::Init(uint defValue, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	uint valueCal = defValue;	// �����l�̑ޔ�
	m_FarLength = (float)defValue;

	if(valueCal > 0)
	{
		// �����Ԃ񂾂��z���ǉ�����	
		while(valueCal != 0)
		{
			// �z��ǉ�
			m_Number.push_back(0);
			valueCal /= 10;
		}

		valueCal = defValue;
		for(int i = 0 ; i < (int)m_Number.size() ; i++)
		{
			D3DXVECTOR3 numberPos = pos;

			// ����X���W�v�Z
			numberPos.x = pos.x - ((i * 2 - (int)m_Number.size()) * (size.x / (float)m_Number.size() / 2.0f));

			// �l�Z�b�g
			m_Number[i] = CNumber::Create((valueCal % 10), numberPos, D3DXVECTOR2((size.x / m_Number.size()), size.y));
			valueCal /= 10;
		}
	}
	else
	{
		m_Number.push_back(0);
		m_Number[0] = CNumber::Create((valueCal % 10), pos, D3DXVECTOR2((size.x / m_Number.size()), size.y));
	}

	CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.62f), (SCREEN_HEIGHT * 0.05f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((250.0f * WINDOW_ASPECT_X), (60.0f * WINDOW_ASPECT_Y)), "farGoal000.png");
	CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.95f), (SCREEN_HEIGHT * 0.05f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((70.0f * WINDOW_ASPECT_X), (60.0f * WINDOW_ASPECT_Y)), "farGoal001.png");
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CFarGoal::Uninit(void)
{
	// ���X�g�J��
	m_Number.clear();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CFarGoal::Update(void)
{
	int value = 0;
	float per = CGame::GetPlayer1()->GetPerSpline();

	float length = ((per / RAILLINE_GOAL) * m_FarLength);
	value = (int)(m_FarLength - length);

	SetValue(value);
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CFarGoal::Draw(void)
{

}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CFarGoal *CFarGoal::Create(uint defValue, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CFarGoal *farGoal;	// �C���X�^���X

	// �C���X�^���X����
	farGoal = new CFarGoal;

	// ����������
	farGoal->Init(defValue, pos, size);

	// �C���X�^���X�����^�[��
	return farGoal;
}

//=============================================================================
//	�֐���	:SetValue
//	����	:����
//	�߂�l	:����
//	����	:���l���Z�b�g����B
//=============================================================================
void CFarGoal::SetValue(int value)
{
	int valueCal = value;

	SIMPLE_FOR((int)m_Number.size())
	{
		// �l���Z�b�g
		m_Number[i]->SetNumber(valueCal % 10);

		valueCal /= 10;
	}
}