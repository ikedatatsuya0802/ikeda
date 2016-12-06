//=============================================================================
//
//	�^�C�g��	�J�����t�@�C��
//	�t�@�C����	camera.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/26
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "cameraDX.h"
#include <typeinfo>
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	�֐���	:CCameraDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CCameraDX::CCameraDX()
{

}

//=============================================================================
//	�֐���	:~CCameraDX()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CCameraDX::~CCameraDX()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CCameraDX::Init(void)
{
	// �J�������[�h�ݒ�
	m_flgCameraMode = false;

	// �ʏ펞�J�����ݒ�
	m_CS.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CS.posR = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	m_CS.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot = D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

	// �G�f�B�b�g���J�����ݒ�
	m_CSEdit.posV = D3DXVECTOR3(0.0f, 3000.0f, -10.0f);
	m_CSEdit.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CSEdit.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CSEdit.Rot = D3DXVECTOR3(0.0f, atan2f((m_CSEdit.posR.x - m_CSEdit.posV.x), (m_CSEdit.posR.z - m_CSEdit.posV.z)), 0.0f);
	m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CCameraDX::Uninit(void)
{

}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CCameraDX::Update(void)
{
#ifdef _DEBUG
	if(m_flgCameraMode)
	{
		CameraMove();
	}

	if(KT_L)
	{
		ChangeCameraMode();
	}
#endif

	CPlayer	*player	= NULL;			// �v���C���[�C���X�^���X
		
	const type_info& this_id = typeid(*CManager::GetMode());
	//const type_info& client_id = typeid(CGame);
	if(this_id == typeid(CGame))
	{// �Q�[�����[�h�̏ꍇ�̂ݏ������s

		player	= CGame::GetPlayer1();	// �v���C���[���̎擾
	}
	else
	{
		player	= NULL;
	}
	
	if(player != NULL)
	{
		// ���_�ݒ�
		m_CS.posR.x = player->GetPos().x;
		m_CS.posR.z = player->GetPos().z;
		m_CS.posR.y = player->GetPos().y + (CAMERA_POSV_TOHIGHPLAYER / 2);

		// ���_�ݒ�
		m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->GetRot().y) * CAMERA_POSV_TOPLAYER);
		m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->GetRot().y) * CAMERA_POSV_TOPLAYER);
		m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;

		// �����_�ݒ�
		//m_CS.posR.x = player->GetPos().x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
		//m_CS.posR.z = player->GetPos().z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}

	if(CInput::GetMouseNotch() > 0)
	{
		// ���ɉ�]�i�`���g�j����

		if(CInput::GetMouseNotch() > 1)
		{
			m_CSEdit.posV.y -= CAMERA_WHEEL_CHANGE_Y;
		}
		m_CSEdit.posV.y		-= CAMERA_WHEEL_CHANGE_Y;

		CInput::SetMouseNotch(0);
	}
	else if(CInput::GetMouseNotch() < 0)
	{
		// ���ɉ�]�i�`���g�j����

		if(CInput::GetMouseNotch() < -1)
		{
			m_CSEdit.posV.y += CAMERA_WHEEL_CHANGE_Y;
		}
		m_CSEdit.posV.y		+= CAMERA_WHEEL_CHANGE_Y;

		CInput::SetMouseNotch(0);
	}
}

//=============================================================================
//	�֐���	:CameraMove
//	����	:����
//	�߂�l	:����
//	����	:�J�����̑����Ɨ��B
//=============================================================================
void CCameraDX::CameraMove(void)
{
	if(CInput::GetKeyPress(DIK_UP) && !CInput::GetKeyPress(DIK_LEFT) && !CInput::GetKeyPress(DIK_RIGHT))		// �J�����ړ�(��)
	{
		// ���_�ړ�
		m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(CInput::GetKeyPress(DIK_DOWN) && !CInput::GetKeyPress(DIK_LEFT) && !CInput::GetKeyPress(DIK_RIGHT))		// �J�����ړ�(��O)
	{
		// ���_�ړ�
		m_CSEdit.posV.x -= sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z -= cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(CInput::GetKeyPress(DIK_LEFT))		// �J�����ړ�(��)
	{
		if(CInput::GetKeyPress(DIK_UP))			// ����
		{
			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else if(CInput::GetKeyPress(DIK_DOWN))	// ����O
		{
			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else								// ��
		{
			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
	}
	else if(CInput::GetKeyPress(DIK_RIGHT))		// �J�����ړ�(�E)
	{
		if(CInput::GetKeyPress(DIK_UP))			// �E��
		{
			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else if(CInput::GetKeyPress(DIK_DOWN))	// �E��O
		{
			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else								// �E
		{
			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
	}

	
	// ���_�ړ�
	if(KH_Y && !KH_Z && !KH_C)			// ���_�ړ�(��)
	{
		// ���_�ݒ�
		m_CSEdit.posV.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)	// ���_�ړ�(��)
	{
		// ���_�ݒ�
		m_CSEdit.posV.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Z)		// ���_�ړ�(��)
	{
		if(KH_Y)			// ����
		{
			// ���_�ύX
			m_CSEdit.posV.y += CAMERA_POSR_MOVEMENT_Y;
			
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		else if(KH_N)	// ����
		{
			// ���_�ύX
			m_CSEdit.posV.y -= CAMERA_POSR_MOVEMENT_Y;
			
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		else
		{
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		
		// ���_�����_�ԋ����X�V
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// ���_�ݒ�
		m_CSEdit.posV.x = m_CSEdit.posR.x - (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posV.z = m_CSEdit.posR.z - (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(KH_C)		// ���_�ړ�(�E)
	{
		if(KH_Y)			// �E��
		{
			// ���_�ύX
			m_CSEdit.posV.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_N)	// �E��
		{
			// ���_�ύX
			m_CSEdit.posV.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}
			
		// ���_�����_�ԋ����X�V
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// ���_�ݒ�
		m_CSEdit.posV.x = m_CSEdit.posR.x - (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posV.z = m_CSEdit.posR.z - (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}

	// �����_�ړ�
	if(KH_T && !KH_Q && !KH_E)			// �����_�ړ�(��)
	{
		// �����_�ݒ�
		m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)	// �����_�ړ�(��)
	{
		// �����_�ݒ�
		m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)		// �����_�ړ�(��)
	{
		if(KH_T)			// ����
		{
			// ���_�ύX
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)	// ����
		{
			// ���_�ύX
			m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}
			
		// ���_�����_�ԋ����X�V
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(KH_E)		// �����_�ړ�(�E)
	{
		if(KH_T)			// �E��
		{
			// ���_�ύX
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)	// �E��
		{
			// ���_�ύX
			m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
			
		// ���_�����_�ԋ����X�V
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			
		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CCameraDX::SetCamera(void)
{
	CAMERA* camera = m_flgCameraMode ? &m_CSEdit : &m_CS;

	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxProjection);
	
	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,						// �v���W�F�N�V�����}�g���N�X
								(D3DX_PI * 0.25),								// ����p
								((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// �A�X�y�N�g��
								CAMERA_NEARZ,									// NearZ�l
								CAMERA_FARZ);									// FarZ�l

	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&camera->mtxView);
	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&camera->mtxView, &camera->posV, &camera->posR, &camera->vecU);
	// �r���[�}�g���N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &camera->mtxView);


	// �f�o�b�O���\��
#ifdef _DEBUG
	CDebugProc::DebugProc("�J�������_�@(%5.2f:%5.2f:%5.2f)\n", camera->posV.x, camera->posV.y, camera->posV.z);
	CDebugProc::DebugProc("�J���������_(%5.2f:%5.2f:%5.2f)\n", camera->posR.x, camera->posR.y, camera->posR.z);
#endif
}