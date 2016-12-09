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

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CResult::Init(void)
{
	m_ResultBG = CScene2DDX::Create(true, 2, OBJTYPE_NONE,
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), ".\\data\\TEXTURE\\result000.jpg");
	m_PushEnter = CScene2DDX::Create(true, 2, OBJTYPE_NONE, D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.8f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), ".\\data\\TEXTURE\\pushbutton000.png");

	m_Alpha = 0.0f;
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
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CResult::Draw(void)
{
	m_Alpha += 0.1f;
	m_PushEnter->SetColor(sinf(m_Alpha), 1.0f, 1.0f, 1.0f);

	// �V�[���`��
	CSceneDX::DrawAll();
}