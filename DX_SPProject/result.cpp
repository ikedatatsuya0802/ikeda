//=============================================================================
//
//	�^�C�g��	�^�C�g������
//	�t�@�C����	result.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene2DDX.h"
#include "ranking.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
int CResult::m_Frame;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CResult::Init(void)
{
	m_ResultBG = CScene2DDX::Create(".\\data\\TEXTURE\\result00.jpg",
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.0f);
	m_PushEnter = CScene2DDX::Create(".\\data\\TEXTURE\\pushbutton000.png",
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.9f), 0.0f),
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), 0.0f);

	CRanking::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.65f, SCREEN_HEIGHT* 0.4f, 0.0f),
		D3DXVECTOR2(SCREEN_WIDTH * 0.6f, SCREEN_HEIGHT * 0.7f), 10.0f);

	m_Alpha = 0.0f;
	m_Frame = -1;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CResult::Uninit(void)
{
	CSceneDX::DeleteAll();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CResult::Update(void)
{
	// �V�[���X�V
	CSceneDX::UpdateAll();
	if(CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetKeyTrigger(DIK_SPACE))
	{
		CFade::Start(new CTitle, FS_OUT);
	}

	m_Frame++;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CResult::Draw(void)
{
	// �{�^�������\����_��
	if(m_Frame % PUSHBUTTON_BRIGHT > (PUSHBUTTON_BRIGHT / 2))
	{
		m_Alpha += (1.0f / (PUSHBUTTON_BRIGHT / 2));
		if(m_Alpha > 1.0f) m_Alpha = 1.0f;
	}
	else
	{
		m_Alpha -= (1.0f / (PUSHBUTTON_BRIGHT / 2));
		if(m_Alpha < 0.0f) m_Alpha = 0.0f;
	}
	m_PushEnter->SetColor(m_Alpha, 1.0f, 1.0f, 1.0f);

	// �V�[���`��
	CSceneDX::DrawAll();
}