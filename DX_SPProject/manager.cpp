//=============================================================================
//
//	�^�C�g��	�J�����t�@�C��
//	�t�@�C����	light.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/26
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "manager.h"
#include "main.h"
#include "input.h"
#include "DebugProc.h"
#include "rendererDX.h"
#include "mode.h"
#include "fade.h"
#include "cameraDX.h"
#include "lightDX.h"
#include "sceneDX.h"
#include "sound.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
CMode		*CManager::m_Mode;			// ���[�h�N���X
CCameraDX	*CManager::m_Camera;		// �J�����̃C���X�^���X
CLightDX	*CManager::m_Light;			// ���C�g�̃C���X�^���X

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �����_���\�ǂݍ���
	CRendererDX::Init(hInstance, hWnd, TRUE);

	// �e��C���X�^���X����
	m_Mode			= new CGame();
	m_Camera		= new CCameraDX();
	m_Light			= new CLightDX();

	// ����������
	m_Mode->Init();
	CFade::Init();
	CInput::InitKeyboard(hInstance, hWnd);
	CInput::InitMouse(hInstance, hWnd);
	CDebugProc::Init();
	m_Camera->Init();
	m_Light->Init();
	CSound::Init();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CManager::Uninit(void)
{
	CInput::UninitKeyboard();
	CInput::UninitMouse();
	CRendererDX::Uninit();
	CDebugProc::Uninit();
	if(m_Camera != NULL)
	{
		m_Camera->Uninit();
		delete m_Camera;
		m_Camera = NULL;
	}
	
	CSceneDX::DeleteAll();
	CSound::Uninit();
	CFade::Uninit();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CManager::Update(void)
{
	m_Mode->Update();
	CFade::Update();
	
	CInput::UpdateKeyboard();
	CRendererDX::Update();
	m_Camera->Update();
	CDebugProc::Update();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CManager::Draw(void)
{
	// �`��V�[�P���X�J�n
	CRendererDX::Begin();

	// ���C�g�x�N�g�����
	m_Light->Draw();

	// �J�����̐ݒ�
	m_Camera->SetCamera();

	// �V�[���`��
	m_Mode->Draw();

	// �t�F�[�h�`��
	CFade::Draw();

	// �f�o�b�O�v���V�[�W��
	CDebugProc::Draw();
#ifdef _DEBUG
	CDebugProc::DebugProc("FPS:%d\n", GetFPS());
#endif

	// �`��V�[�P���X�I��
	CRendererDX::End();
}

//=============================================================================
//	�֐���	:SetMode
//	����	:CMode *mode(�N���X�̃C���X�^���X)
//	�߂�l	:����
//	����	:���[�h��؂�ւ���B
//=============================================================================
void CManager::SetMode(CMode *mode)
{
	if((m_Mode != NULL))
	{
		// ���[�h�̏I������
		m_Mode->Uninit();
		// ���[�h�̃C���X�^���X�̍폜
		delete m_Mode;
	}
	if(mode != NULL)
	{
		// ���[�h�̐؂�ւ�
		m_Mode = mode;
		// ���[�h�̏�������
		m_Mode->Init();
	}
}