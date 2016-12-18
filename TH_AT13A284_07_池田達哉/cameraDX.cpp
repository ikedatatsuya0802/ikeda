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
#include "input.h"

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
	//m_Anim.ifAnim = true;
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
	// �ʏ펞�J�����ݒ�
	m_CS.posV = D3DXVECTOR3(150.0f, 200.0f, -1000.0f);
	m_CS.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CS.vecU			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot			= D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.Distance		= hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));
	m_CS.Vib.vPos		= VEC3_ZERO;
	m_CS.Vib.Cnt		= 0;
	m_CS.Vib.Width		= 0.0f;

	m_Key	= 0;
	m_Frame = 0;
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
	CameraMove();
	//CameraAnimation();
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
		m_CS.posV.x += sinf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CS.posV.z += cosf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;

		// �����_�ݒ�
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
	}
	else if(KH_S && !KH_A && !KH_D)
	{// �J�����ړ�(��O)

	 // ���_�ړ�
		m_CS.posV.x -= sinf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CS.posV.z -= cosf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;

		// �����_�ݒ�
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
	}
	else if(KH_A)
	{// �J�����ړ�(��)

		if(CInput::GetKeyPress(DIK_UP))
		{// ����

		 // ���_�ړ�
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else if(KH_S)
		{// ����O

		 // ���_�ړ�
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else
		{// ��

		 // ���_�ړ�
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
	}
	else if(KH_D)
	{// �J�����ړ�(�E)

		if(KH_W)
		{// �E��

		 // ���_�ړ�
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else if(KH_S)
		{// �E��O

		 // ���_�ړ�
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else
		{// �E

		 // ���_�ړ�
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// �����_�ݒ�
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
	}


	// ���_�ړ�
	if(KH_Y && !KH_Z && !KH_C)
	{// ���_�ړ�(��)

	 // ���_�ݒ�
		m_CS.posV.y += CAMERA_POSV_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)
	{// ���_�ړ�(��)

	 // ���_�ݒ�
		m_CS.posV.y -= CAMERA_POSV_MOVEMENT_Y;
	}

	// �����_�ړ�
	if(KH_T && !KH_Q && !KH_E)
	{// �����_�ړ�(��)

	 // �����_�ݒ�
		m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)
	{// �����_�ړ�(��)

	 // �����_�ݒ�
		m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)
	{// �����_�ړ�(��)

		if(KH_T)
		{// ����

		 // ���_�ύX
			m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// �p�x����
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// ����

		 // ���_�ύX
			m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// �p�x����
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// �p�x����
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}

		// ���_�����_�ԋ����X�V
		m_CS.Distance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// �����_�ݒ�
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
	}
	else if(KH_E)
	{// �����_�ړ�(�E)

		if(KH_T)
		{// �E��

		 // ���_�ύX
			m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// �p�x����
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// �E��

		 // ���_�ύX
			m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// �p�x����
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// �p�x����
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}

		// ���_�����_�ԋ����X�V
		m_CS.Distance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// �����_�ݒ�
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
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
	if(m_Anim.ifAnim)
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

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CCameraDX::SetCamera(void)
{
	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_CS.mtxProjection);
	
	// �v���W�F�N�V�����}�g���N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_CS.mtxProjection,						// �v���W�F�N�V�����}�g���N�X
								(D3DX_PI * 0.25),								// ����p
								((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// �A�X�y�N�g��
								CAMERA_NEARZ,									// NearZ�l
								CAMERA_FARZ);									// FarZ�l

	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_PROJECTION, &m_CS.mtxProjection);
	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_CS.mtxView);
	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&m_CS.mtxView, &m_CS.posV, &(m_CS.posR + m_CS.Vib.vPos), &m_CS.vecU);
	// �r���[�}�g���N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &m_CS.mtxView);


	// �f�o�b�O���\��
#ifdef _DEBUG
	//CDebugProc::DebugProc("�J�������_�@(%5.2f:%5.2f:%5.2f)\n", m_CS.posV.x, m_CS.posV.y, m_CS.posV.z);
	//CDebugProc::DebugProc("�J���������_(%5.2f:%5.2f:%5.2f)\n", m_CS.posR.x, m_CS.posR.y, m_CS.posR.z);
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