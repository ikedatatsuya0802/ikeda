//=============================================================================
//
//	�^�C�g��	�^�C�g������
//	�t�@�C����	title.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "title.h"
#include "manager.h"
#include "cameraDX.h"
#include "lightDX.h"
#include "input.h"
#include "fade.h"
#include "scene2DDX.h"
#include "player_title.h"
#include "rail_title.h"
#include "skybox.h"
#include "meshfield.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CTitle::Init(void)
{
	/*
	m_TitleBG = CScene2DDX::Create(true, 2, OBJTYPE_NONE,
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "title000.jpg");
		*/
	m_PushEnter = CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.8f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((SCREEN_WIDTH * 0.4f), (SCREEN_HEIGHT * 0.05f)), "pushbutton000.png");

	m_Alpha = 0.0f;
	

	m_Frame = 0;

	CManager::GetCamera()->Init();
	CLightDX::ChangeHolLight(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 3D
	CMeshfield::Create();
	CSkybox::Create(true, 2, OBJTYPE_NONE, true);
	CRail_Title::Create();
	CPlayer_Title::Create(true, 2, OBJTYPE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -(MESHFIELD_TOTALHEIGHT * 0.5f)));
	//CPlayer_Title::Create(true, 2, OBJTYPE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 100.0f));
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CTitle::Uninit(void)
{
	CSceneDX::DeleteAll();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CTitle::Update(void)
{
	// �V�[���X�V
	CSceneDX::UpdateAll();
	if(CInput::GetKeyTrigger(DIK_RETURN) || CInput::GetKeyTrigger(DIK_SPACE))
	{
		CFade::Start(new CTutorial, FS_OUT);
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CTitle::Draw(void)
{
	m_Alpha += 0.05f;
	m_PushEnter->SetColor((cosf(m_Alpha) * 0.5f + 0.5f), 1.0f, 1.0f, 1.0f);

	// �V�[���`��
	CSceneDX::DrawAll();
}