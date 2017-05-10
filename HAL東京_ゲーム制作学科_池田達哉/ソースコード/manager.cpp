//=============================================================================
//
//	�^�C�g��	�}�l�[�W��
//	�t�@�C����	manager.cpp
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
#include "number.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
CMode*		CManager::m_Mode;
CCameraDX*	CManager::m_Camera;
bool		CManager::m_ifEdhitMode;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_ifEdhitMode = false;

	// �����_���\�ǂݍ���
	CRendererDX::Init(hInstance, hWnd, TRUE);

	// �e��C���X�^���X����
#ifdef _DEBUG
	m_Mode = new CTitle();
#else
	m_Mode = new CGame();
#endif // DEBUG
	m_Camera = new CCameraDX();

	// ����������
	CInput::Init(hInstance, hWnd);
	CSound::Init();
	CLightDX::Init();
	m_Mode->Init();
	m_Camera->Init();
	CFade::Init();
	CDebugProc::Init();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CManager::Uninit(void)
{
	CInput::Uninit();
	CRendererDX::Uninit();
	CDebugProc::Uninit();
	SafetyUninit(m_Camera);
	
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
	CInput::Update();

	m_Mode->Update();
	CFade::Update();
	
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

	// �J�����̐ݒ�
	m_Camera->SetCamera();

	// �V�[���`��
	m_Mode->Draw();

	// �t�F�[�h�`��
	CFade::Draw();

#ifdef _DEBUG
	// �f�o�b�O�v���V�[�W��
	if(CManager::GetEdhitMode())
		CDebugProc::Draw();
#endif
	CDebugProc::Clear();
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

//=============================================================================
//	�֐���	:GetClientWindowSize
//	����	:����
//	�߂�l	:����
//	����	:�E�B���h�E�̃N���C�A���g�T�C�Y���擾����B
//=============================================================================
D3DXVECTOR2 CManager::GetClientWindowSize(void)
{
	D3DXVECTOR2 size;
	RECT rect;
	GetClientRect(GethWnd(), &rect);
	size.x = (float)(rect.right - rect.left);
	size.y = (float)(rect.bottom - rect.top);
	return size;
}

//=============================================================================
//	�֐���	:GetCWAspect
//	����	:����
//	�߂�l	:����
//	����	:�z�肵�Ă��鏉���E�B���h�E�T�C�Y�ɑ΂��Ă̔䗦��Ԃ��B
//=============================================================================
D3DXVECTOR2 CManager::GetCWAspect(void)
{
	D3DXVECTOR2 size;
	size.x = GetClientWindowSize().x / DEFAULT_SCREEN_WIDTH;
	size.y = GetClientWindowSize().y / DEFAULT_SCREEN_HEIGHT;
	return size;
}