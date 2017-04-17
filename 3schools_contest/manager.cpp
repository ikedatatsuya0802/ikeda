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
#include "DebugProc.h"
#include "rendererDX.h"
#include "cameraDX.h"
#include "lightDX.h"
#include "skybox.h"
#include "model.h"
#include "input.h"
#include "particle.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
CCameraDX	*CManager::m_Camera;
CModel		*CManager::m_Model;
int			CManager::m_Frame;

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

	// ���\�[�X�̃��[�h
	CParticle::Load();

	// �e��C���X�^���X����
	m_Camera		= new CCameraDX();

	// ����������
	CInput::Init(hInstance, hWnd);
	CLightDX::Init();
	m_Camera->Init();
	CDebugProc::Init();

	CSkybox::Create();

	m_Model = CModel::Create("", "human");
	//CModel::Create("", "car");

	m_Frame = 0;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CManager::Uninit(void)
{
	CRendererDX::Uninit();
	CDebugProc::Uninit();
	SafetyUninit(m_Camera);

	CSceneDX::DeleteAll();
	CLightDX::Uninit();

	CInput::Uninit();


	// ���\�[�X�̃A�����[�h
	CParticle::Unload();
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

	CRendererDX::Update();
	m_Camera->Update();


	int frame = CManager::GetFrame() % CManager::GetModel()->GetMorphCountAll();

	// �p�[�e�B�N������
	if((frame >= 300) && (frame < 540))
	{
		if(frame % 3 == 0)
		CParticle::Create(0);
	}
	else if((frame >= 600) && (frame < 900))
	{
		if(frame % 3 == 0)
			CParticle::Create(1);
	}
	else if((frame >= 930) && (frame < 1200))
	{
		if(frame % 3 == 0)
			CParticle::Create(2);
	}
	else if((frame >= 1200) && (frame < 1500))
	{
		if(frame % 2 == 0)
			CParticle::Create(3);
	}

	// �V�[���X�V
	CSceneDX::UpdateAll();

	CDebugProc::Update();
	CLightDX::Update();

	m_Frame++;
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
	CLightDX::Draw();

	// �J�����̐ݒ�
	m_Camera->SetCamera();

	// �V�[���`��
	CSceneDX::DrawAll();

	// �f�o�b�O�v���V�[�W��
	CDebugProc::Draw();
#ifdef _DEBUG
	CDebugProc::DebugProc("FPS:%d\n", GetFPS());
#endif

	// �`��V�[�P���X�I��
	CRendererDX::End();
}