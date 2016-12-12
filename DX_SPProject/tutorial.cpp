//=============================================================================
//
//	�^�C�g��	�^�C�g������
//	�t�@�C����	tutorial.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene2DDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
int	CTutorial::m_Frame;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CTutorial::Init(void)
{
	m_TutorialBG = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "tutorial000.jpg");
	m_PushEnter = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.8f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), "pushbutton000.png");

	m_Alpha = 0.0f;
	m_Frame = -1;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CTutorial::Uninit(void)
{
	CSceneDX::DeleteAll();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CTutorial::Update(void)
{
	// �V�[���X�V
	CSceneDX::UpdateAll();
	if(CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetKeyTrigger(DIK_SPACE))
	{
		CFade::Start(new CGame, FS_OUT);
	}

	m_Frame++;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CTutorial::Draw(void)
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