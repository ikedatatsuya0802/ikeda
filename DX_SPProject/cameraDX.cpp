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
	m_CS.posV			= VEC3_ZERO;
	m_CS.posR			= D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	m_CS.vecU			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot			= D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.Distance		= hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));
	m_CS.Vib.vPos			= VEC3_ZERO;
	m_CS.Vib.Cnt		= 0;
	m_CS.Vib.Width	= 0.0f;

	// �G�f�B�b�g���J�����ݒ�
	m_CSEdit.posV		= CAMERA_EDIT_V1;
	m_CSEdit.posR		= CAMERA_EDIT_R1;
	m_CSEdit.vecU		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CSEdit.Rot		= D3DXVECTOR3(0.0f, atan2f((m_CSEdit.posR.x - m_CSEdit.posV.x), (m_CSEdit.posR.z - m_CSEdit.posV.z)), 0.0f);
	m_CSEdit.Distance	= hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
	m_CSEdit.Vib.vPos			= VEC3_ZERO;
	m_CSEdit.Vib.Cnt		= 0;
	m_CSEdit.Vib.Width	= 0.0f;
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

		// ���_������������
		if(CInput::GetKeyTrigger(DIK_2))
		{
			m_CSEdit.posV		= CAMERA_EDIT_V1;
			m_CSEdit.posR		= CAMERA_EDIT_R1;
			m_CSEdit.Rot		= VEC3_ZERO;
			m_CSEdit.Distance	= hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		}
		if(CInput::GetKeyTrigger(DIK_3))
		{
			m_CSEdit.posV		= CAMERA_EDIT_V2;
			m_CSEdit.posR		= CAMERA_EDIT_R2;
			m_CSEdit.Rot		= VEC3_ZERO;
			m_CSEdit.Distance	= hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		}
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

		CameraVibrate();
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
		m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->SetSplineRot().y) * CAMERA_POSV_TOPLAYER);
		m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->SetSplineRot().y) * CAMERA_POSV_TOPLAYER);
		m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;

		// �����_�ݒ�
		//m_CS.posR.x = player->GetPos().x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		//m_CS.posR.z = player->GetPos().z + (cosf(m_CS.Rot.y) * m_CS.Distance);
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
	if(KH_W && !KH_A && !KH_D)
	{// �J�����ړ�(��)

		// ���_�ړ�
		m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
	else if(KH_S && !KH_A && !KH_D)
	{// �J�����ړ�(��O)

		// ���_�ړ�
		m_CSEdit.posV.x -= sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z -= cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
	else if(KH_A)
	{// �J�����ړ�(��)

		if(CInput::GetKeyPress(DIK_UP))
		{// ����

			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else if(KH_S)
		{// ����O

			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else
		{// ��

			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
	}
	else if(KH_D)
	{// �J�����ړ�(�E)

		if(KH_W)
		{// �E��

			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else if(KH_S)
		{// �E��O

			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else
		{// �E

			// ���_�ړ�
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
	}

	
	// ���_�ړ�
	if(KH_Y && !KH_Z && !KH_C)
	{// ���_�ړ�(��)

		// ���_�ݒ�
		m_CSEdit.posV.y += CAMERA_POSV_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)
	{// ���_�ړ�(��)

		// ���_�ݒ�
		m_CSEdit.posV.y -= CAMERA_POSV_MOVEMENT_Y;
	}

	// �����_�ړ�
	if(KH_T && !KH_Q && !KH_E)
	{// �����_�ړ�(��)

		// �����_�ݒ�
		m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)
	{// �����_�ړ�(��)

		// �����_�ݒ�
		m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)
	{// �����_�ړ�(��)

		if(KH_T)
		{// ����

			// ���_�ύX
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// ����

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
		m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
	else if(KH_E)
	{// �����_�ړ�(�E)

		if(KH_T)
		{// �E��

			// ���_�ύX
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// �p�x����
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// �E��

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
		m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			
		// �����_�ݒ�
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
}

//=============================================================================
//	�֐���	:CameraVibrate
//	����	:����
//	�߂�l	:����
//	����	:�J�����̐U�������s�B
//=============================================================================
void CCameraDX::CameraVibrate(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<float> width(-(m_CS.Vib.Width / 2), (m_CS.Vib.Width / 2));

	// �G�f�B�b�g���[�h�łȂ��ꍇ�̂ݎ��s
	if(!m_flgCameraMode)
	{
		// �J�E���^���ݒ肳��Ă���ꍇ���s
		if(m_CS.Vib.Cnt != 0)
		{
			// �����_���ɗh�������
			m_CS.Vib.vPos.x = width(mt);
			m_CS.Vib.vPos.y = width(mt);

			if(m_CS.Vib.Cnt)
			{
				// �J�E���^����
				m_CS.Vib.Cnt--;
			}
		}
		else
		{
			m_CS.Vib.vPos = VEC3_ZERO;
			m_CS.Vib.Cnt		= 0;
			m_CS.Vib.Width	= 0.0f;
		}
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
	if(m_flgCameraMode)
	{
		D3DXMatrixLookAtLH(&camera->mtxView, &camera->posV, &camera->posR, &camera->vecU);
	}
	else
	{
		D3DXMatrixLookAtLH(&camera->mtxView, &camera->posV, &(camera->posR + camera->Vib.vPos), &camera->vecU);
	}
	// �r���[�}�g���N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &camera->mtxView);


	// �f�o�b�O���\��
#ifdef _DEBUG
	CDebugProc::DebugProc("�J�������_�@(%5.2f:%5.2f:%5.2f)\n", camera->posV.x, camera->posV.y, camera->posV.z);
	CDebugProc::DebugProc("�J����V���_ (%5.2f:%5.2f:%5.2f)\n", camera->Vib.vPos.x, camera->Vib.vPos.y, camera->Vib.vPos.z);
	CDebugProc::DebugProc("�J���������_(%5.2f:%5.2f:%5.2f)\n", camera->posR.x, camera->posR.y, camera->posR.z);
#endif
}