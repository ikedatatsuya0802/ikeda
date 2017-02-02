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
#include "title.h"
#include "player.h"
#include "mode.h"

//=============================================================================
//	�֐���	:CCameraDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CCameraDX::CCameraDX()
{
	// �A�j���[�V������񏉊���
	LoadCameraAnim();
	m_Anim.ifAnim = true;
	//m_Anim2.ifAnim = true;
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

	if(CManager::MatchMode(CTitle()))
	{
		// �ʏ펞�J�����ݒ�
		m_CS.posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
		m_CS.posR = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	}
	else if(CManager::MatchMode(CGame()))
	{
		// �ʏ펞�J�����ݒ�
		m_CS.posV = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		m_CS.posR = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	}
	m_CS.vecU			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot			= D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.Distance		= CAMERA_POSV_TOPLAYER;
	m_CS.Vib.vPos		= VEC3_ZERO;
	m_CS.Vib.Cnt		= 0;
	m_CS.Vib.Width	= 0.0f;

	// �G�f�B�b�g���J�����ݒ�
	m_CSEdit.posV		= CAMERA_EDIT_V1;
	m_CSEdit.posR		= CAMERA_EDIT_R1;
	m_CSEdit.vecU		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CSEdit.Rot		= D3DXVECTOR3(0.0f, atan2f((m_CSEdit.posR.x - m_CSEdit.posV.x), (m_CSEdit.posR.z - m_CSEdit.posV.z)), 0.0f);
	m_CSEdit.Distance	= hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
	m_CSEdit.Vib.vPos	= VEC3_ZERO;
	m_CSEdit.Vib.Cnt	= 0;
	m_CSEdit.Vib.Width	= 0.0f;

	m_Key		= 0;
	m_Frame		= 0;
	m_DisVec	= 0;
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
	if(CManager::MatchMode(CTitle()))
	{// �^�C�g���̏ꍇ�̂ݏ������s

		CameraAnimation();
	}		
	else if(CManager::MatchMode(CGame()))
	{// �Q�[�����[�h�̏ꍇ�̂ݏ������s

		CPlayer	*player = NULL;	// �v���C���[�C���X�^���X

		if(m_flgCameraMode)
		{
			CameraMove();

			// ���_������������
			if(CInput::GetKeyTrigger(DIK_2))
			{
				m_CSEdit.posV = CAMERA_EDIT_V1;
				m_CSEdit.posR = CAMERA_EDIT_R1;
				m_CSEdit.Rot = VEC3_ZERO;
				m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			}
			else if(CInput::GetKeyTrigger(DIK_3))
			{
				m_CSEdit.posV = CAMERA_EDIT_V2;
				m_CSEdit.posR = CAMERA_EDIT_R2;
				m_CSEdit.Rot = VEC3_ZERO;
				m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			}
			else if(CInput::GetKeyTrigger(DIK_4))
			{
				D3DXVECTOR3 pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);

				m_CSEdit.posV = D3DXVECTOR3(pos.x, pos.y + 3000.0f, pos.z - 10.0f);
				m_CSEdit.posR = D3DXVECTOR3(pos.x, pos.y, pos.z);
				m_CSEdit.Rot = VEC3_ZERO;
				m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			}


			if(CInput::GetMouseNotch() > 0)
			{
				// ���ɉ�]�i�`���g�j����

				if(CInput::GetMouseNotch() > 1)
				{
					m_CSEdit.posV.y -= CAMERA_WHEEL_CHANGE_Y;
				}
				m_CSEdit.posV.y -= CAMERA_WHEEL_CHANGE_Y;

				CInput::SetMouseNotch(0);
			}
			else if(CInput::GetMouseNotch() < 0)
			{
				// ���ɉ�]�i�`���g�j����

				if(CInput::GetMouseNotch() < -1)
				{
					m_CSEdit.posV.y += CAMERA_WHEEL_CHANGE_Y;
				}
				m_CSEdit.posV.y += CAMERA_WHEEL_CHANGE_Y;

				CInput::SetMouseNotch(0);
			}
		}
		else
		{
#ifdef _DEBUG
			if(KH_Q)
			{// �����_�ړ�(��)

			 // �p�x����
				m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * 0.25f;
			}
			else if(KH_E)
			{// �����_�ړ�(�E)

			 // �p�x����
				m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * 0.25f;
			}
#endif

			player = CGame::GetPlayer1();	// �v���C���[���̎擾

			CameraVibrate();

			if(player != NULL)
			{
				// ���_�ݒ�
				/*m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;*/

				// �����_�ݒ�
				m_CS.posR.x = player->GetPos().x;
				m_CS.posR.z = player->GetPos().z;
				m_CS.posR.y = player->GetPos().y + (CAMERA_POSV_TOHIGHPLAYER / 2);

				// �J���������ݒ�
				m_CS.Distance += m_DisVec;

				// �J���������ω��ʌ���
				m_DisVec *= 0.8f;

				// ���_�ݒ�
				m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;
			}
		}



		if(KT_L)
		{
			ChangeCameraMode();
		}
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
//	�֐���	:CameraAnimation
//	����	:����
//	�߂�l	:����
//	����	:�J�������A�j���[�V����������B
//=============================================================================
void CCameraDX::CameraAnimation(void)
{
	if(1)
	{// posV��posR�w��̃A�j���[�V����

		D3DXVECTOR3 posV;
		D3DXVECTOR3 posR;
		D3DXVECTOR3 nextVec;
		int keyNext = 0;
		float rate = 0.0f;

		if(m_Key != ((int)m_Anim.Status.size() - 1))
		{
			keyNext = m_Key + 1;
		}
		else
		{
			keyNext = 0;
		}

		rate = (m_Frame / (float)m_Anim.Status[keyNext].Frame);


		// ���݃t���[���ł̎��_�̌v�Z
		nextVec = m_Anim.Status[keyNext].PosV - m_Anim.Status[m_Key].PosV;
		posV.x = m_Anim.Status[m_Key].PosV.x + (nextVec.x * rate);
		posV.y = m_Anim.Status[m_Key].PosV.y + (nextVec.y * rate);
		posV.z = m_Anim.Status[m_Key].PosV.z + (nextVec.z * rate);

		// ���݃t���[���ł̒����_�̌v�Z
		nextVec = m_Anim.Status[keyNext].PosR - m_Anim.Status[m_Key].PosR;
		posR.x = m_Anim.Status[m_Key].PosR.x + (nextVec.x * rate);
		posR.y = m_Anim.Status[m_Key].PosR.y + (nextVec.y * rate);
		posR.z = m_Anim.Status[m_Key].PosR.z + (nextVec.z * rate);

		// ���W���f
		m_CS.posV = posV;
		m_CS.posR = posR;

		// �t���[���̑���
		m_Frame++;
		if(m_Frame >= m_Anim.Status[keyNext].Frame)
		{
			m_Key = (m_Key + 1) % m_Anim.Status.size();
			m_Frame = 0;

			if(!m_Anim.Loop && (m_Key == (m_Anim.Status.size() - 1)))
			{
				m_Anim.ifAnim = false;
			}
		}
	}
	else
	{// posV�Ɗp�x�w��̃A�j���[�V����

		D3DXVECTOR3 posV;
		D3DXVECTOR3 posR;
		D3DXVECTOR3 nextVec;
		int keyNext = 0;
		float rate = 0.0f;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 vec;

		if(m_Key != ((int)m_Anim2.Status.size() - 1))
		{
			keyNext = m_Key + 1;
		}
		else
		{
			keyNext = 0;
		}

		rate = (m_Frame / (float)m_Anim2.Status[keyNext].Frame);


		// ���݃t���[���ł̎��_�̌v�Z
		nextVec = m_Anim2.Status[keyNext].PosV - m_Anim2.Status[m_Key].PosV;
		posV.x = m_Anim2.Status[m_Key].PosV.x + (nextVec.x * rate);
		posV.y = m_Anim2.Status[m_Key].PosV.y + (nextVec.y * rate);
		posV.z = m_Anim2.Status[m_Key].PosV.z + (nextVec.z * rate);

		// ���݃t���[���ł̊p�x�v�Z
		rot = m_Anim2.Status[keyNext].Rot - m_Anim2.Status[m_Key].Rot;
		rot.x = m_Anim2.Status[m_Key].Rot.x + (rot.x * rate);
		rot.y = m_Anim2.Status[m_Key].Rot.y + (rot.y * rate);
		rot.z = m_Anim2.Status[m_Key].Rot.z + (rot.z * rate);
		CDebugProc::DebugProc("rot:%f %f %f\n", rot.x, rot.y, rot.z);
		CDebugProc::DebugProc("KEY:%d, FRAME:%d\n", m_Key, m_Frame);
		

		// �p�x�����ɒ����x�N�g���v�Z
		vec.x = CAMERA_VIEW_FAR * sinf(rot.y);
		vec.y = CAMERA_VIEW_FAR * sinf(rot.x);
		vec.z = CAMERA_VIEW_FAR * cosf(rot.y) + CAMERA_VIEW_FAR * cosf(rot.x);

		// �����x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);
		vec * CAMERA_VIEW_FAR;

		posR.x = posV.x + vec.x;
		posR.y = posV.y + vec.y;
		posR.z = posV.z + vec.z;


		// ���W���f
		m_CS.posV = posV;
		m_CS.posR = posR;

		// �t���[���̑���
		m_Frame++;
		if(m_Frame >= m_Anim2.Status[keyNext].Frame)
		{
			m_Key = (m_Key + 1) % m_Anim2.Status.size();
			m_Frame = 0;

			if(!m_Anim2.Loop && (m_Key == (m_Anim2.Status.size() - 1)))
			{
				m_Anim2.ifAnim = false;
			}
		}
	}
}

//=============================================================================
//	�֐���	:LoadCameraAnim
//	����	:����
//	�߂�l	:����
//	����	:�J�����̃A�j���[�V�����������[�h����B
//=============================================================================
void CCameraDX::LoadCameraAnim(void)
{
	FILE	*fp = NULL;	// �t�@�C���|�C���^

	fp = fopen("./data/camera_anim.txt", "r");

	m_Anim.ifAnim	= false;
	m_Anim.Loop		= false;

	// END_SCRIPT�܂œǂݍ���
	while(!feof(fp))
	{
		char str[65535] = { NULL };
		memset(str, NULL, sizeof(str));

		// �P����擾
		fscanf(fp, "%s", str);

		if(strcmp(str, "LOOP") == 0)
		{// ���[�v����ǂݍ���

			int a = 0;
			fscanf(fp, " = %d\n", &a);
			m_Anim.Loop = (bool)a;
		}
		else if(strcmp(str, "FRAME") == 0)
		{// �X�v���C������_��ǉ�

			// �A�j���[�V�����f�[�^��ǉ�
			m_Anim.Status.push_back(CAMERA_ANIM_STATUS());

			uint aNum = m_Anim.Status.size() - 1;	// �I�[�̔z��ԍ�

			// �t���[������ǂݍ���
			fscanf(fp, " = %d\n", &m_Anim.Status[aNum].Frame);
			// ���_����ǂݍ���
			fscanf(fp, "POSV = %f %f %f\n", &m_Anim.Status[aNum].PosV.x, &m_Anim.Status[aNum].PosV.y, &m_Anim.Status[aNum].PosV.z);
			// �����_����ǂݍ���
			fscanf(fp, "POSR = %f %f %f\n", &m_Anim.Status[aNum].PosR.x, &m_Anim.Status[aNum].PosR.y, &m_Anim.Status[aNum].PosR.z);
		}
		else if(strcmp(str, "END_SCRIPT") == 0)
		{// �X�v���C���ǂݍ��݂��I��

			break;
		}
	}

	// �t�@�C���N���[�Y
	fclose(fp);
}

//=============================================================================
//	�֐���	:CameraVibrate
//	����	:����
//	�߂�l	:����
//	����	:�J�����̐U�������s�B
//=============================================================================
void CCameraDX::CameraVibrate(void)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<float> width(-(m_CS.Vib.Width / 2), (m_CS.Vib.Width / 2));

	// �G�f�B�b�g���[�h�łȂ��ꍇ�̂ݎ��s
	if(!m_flgCameraMode)
	{
		// �J�E���^���ݒ肳��Ă���ꍇ���s
		if(m_CS.Vib.Cnt != 0)
		{
			// �����_���ɗh�������
			m_CS.Vib.vPos.x = width(mt) * (m_CS.Vib.Cnt / (float)m_CS.Vib.MaxCnt);
			m_CS.Vib.vPos.y = width(mt) * (m_CS.Vib.Cnt / (float)m_CS.Vib.MaxCnt);

			if(m_CS.Vib.Cnt)
			{
				// �J�E���^����
				m_CS.Vib.Cnt--;
			}
		}
		else
		{
			m_CS.Vib.vPos	= VEC3_ZERO;
			m_CS.Vib.MaxCnt = 0;
			m_CS.Vib.Cnt	= 0;
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
	//CDebugProc::DebugProc("�J�������_�@(%5.2f:%5.2f:%5.2f)\n", camera->posV.x, camera->posV.y, camera->posV.z);
	//CDebugProc::DebugProc("�J���������_(%5.2f:%5.2f:%5.2f)\n", camera->posR.x, camera->posR.y, camera->posR.z);
#endif
}

//=============================================================================
//	�֐���	:GetCameraAnimFrame
//	����	:����
//	�߂�l	:����
//	����	:�A�j���[�V�����̑��t���[������Ԃ��B
//=============================================================================
int CCameraDX::GetCameraAnimFrame(void)
{
	int frame = 0;

	for(int i = 0 ; i < (int)m_Anim.Status.size() ; i++)
	{
		frame += m_Anim.Status[i].Frame;
	}

	return frame;
}