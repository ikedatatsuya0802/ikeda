//=============================================================================
//
//	�^�C�g��	�v���C���[
//	�t�@�C����	player.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/14
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include <stdio.h>
#include "player.h"
#include "model.h"
#include "main.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "sceneXDX.h"
#include "meshfield.h"

//=============================================================================
//	�֐���	:CPlayer()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CPlayer::CPlayer(bool isListAdd, int priority, OBJTYPE objType) : CSceneXDX(isListAdd, priority, objType)
{

}

//=============================================================================
//	�֐���	:~CPlayer()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CPlayer::Init(D3DXVECTOR3 pos)
{
	m_NumParts	= 0;
	m_NumMotion	= 0;
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_Move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove	= 0;
	m_bJump		= false;
	m_Pause		= false;

	m_MotionType= MT_NONE;
	m_NumKey	= 0;
	m_Key		= 0;
	m_Frame		= 0;

	m_MotionBlend	= MT_NONE;
	m_NumKeyBlend	= 0;
	m_KeyBlend		= 0;
	m_FrameBlend	= 0;
	
	m_FrameBlendMotion	= 0;

	// ���f���E���[�V�����̓ǂݍ���
	LoadMotion("data/motion.txt");

	SetMotion(MT_NEUTRAL);
	SetMotion(MT_NEUTRAL);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CPlayer::Uninit(void)
{

}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CPlayer::Update(void)
{
	// �v���C���[�ړ�
	UpdateMove();
	
	// ���[�V�����ݒ�
	//UpdateMotion();
}

//=============================================================================
//	�֐���	:UpdateMove
//	����	:����
//	�߂�l	:����
//	����	:�I�u�W�F�N�g���ړ��E��]������B
//=============================================================================
void CPlayer::UpdateMove(void)
{
	CCameraDX	*camera		= CManager::GetCamera();	// �J����
	CMeshfield	*mesh		= CManager::GetMeshfield();	// ���b�V���t�B�[���h


	// �v���C���[�̍�����ݒ�
	if(m_Pos.y < (mesh->GetHeight(m_Pos) + 0.0f))
	{
		m_Pos.y = mesh->GetHeight(m_Pos) + 0.0f;
		m_bJump = false;
	}
	else
	{
		// �W�����v�ʂ̌���
		m_Move.y -= PLAYER_GRAVITY;
	}

	// ��]�ʕ␳
	if(m_RotMove.y > D3DX_PI)				// ��]�ʂ��v���X�����ɋt�����̏ꍇ
	{
		// ��]�ʂ��t������
		m_RotMove.y -= (D3DX_PI * 2.0f);
	}
	else if(m_RotMove.y < -D3DX_PI)			// ��]�ʂ��}�C�i�X�����ɋt�����̏ꍇ
	{
		// ��]�ʂ��t������
		m_RotMove.y += (D3DX_PI * 2.0f);
	}

	// �J�E���^���L���̏ꍇ��]����
	if(m_nCntMove > 0)
	{
		m_Rot.y += (m_RotMove.y / PLAYER_ROT_STEP);

		m_nCntMove--;
	}

	// ��]�p�̋��E�␳
	if(m_Rot.y > D3DX_PI)
	{
		m_Rot.y = (-D3DX_PI + (m_Rot.y - D3DX_PI));
	}
	else if(m_Rot.y < -D3DX_PI)
	{
		m_Rot.y = (D3DX_PI + (m_Rot.y + D3DX_PI));
	}
}

//=============================================================================
//	�֐���	:UpdateMotion
//	����	:����
//	�߂�l	:����
//	����	:���[�V�������X�V����B
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	// �p�[�c���������[�V�������X�V����
	if(m_MotionType != MT_NONE)
	{// ���[�V���������s��

		for(int i = 0 ; i < m_NumParts ; i++)
		{
			KEY *key, *keyNext;		// �L�[���
			D3DXVECTOR3 pos, rot;	// ���W�E��]�o�b�t�@
			float rate;				// ���[�V�����u�����h����

			KEY *keyBlend, *keyNextBlend;		// �L�[���
			D3DXVECTOR3 posBlend, rotBlend;		// ���W�E��]�o�b�t�@
			float rateBlend;					// ���[�V�����u�����h����

			D3DXVECTOR3 posBlendMotion, rotBlendMotion;		// ���W�E��]�o�b�t�@
			float rateBlendMotion;							// ���[�V�����u�����h����

			
			// �L�[���̐ݒ�
			key		= &m_Motion[m_MotionType].KeyInfo[m_Key].Key[i];
			keyNext	= &m_Motion[m_MotionType].KeyInfo[(m_Key + 1) % m_NumKey].Key[i];

			// �u�����h�䗦�̐ݒ�
			rate = (float)m_Frame / m_Motion[m_MotionType].KeyInfo[m_Key].Frame;

			// ���W�̃u�����h
			pos.x = (key->PosX * (1.0f - rate)) + (keyNext->PosX * rate);
			pos.y = (key->PosY * (1.0f - rate)) + (keyNext->PosY * rate);
			pos.z = (key->PosZ * (1.0f - rate)) + (keyNext->PosZ * rate);

			// ��]�̃u�����h
			rot.x = (key->RotX * (1.0f - rate)) + (keyNext->RotX * rate);
			rot.y = (key->RotY * (1.0f - rate)) + (keyNext->RotY * rate);
			rot.z = (key->RotZ * (1.0f - rate)) + (keyNext->RotZ * rate);

			// ���[�V�������������s�����ǂ���
			if(m_MotionBlend != MT_NONE)
			{// ���[�V�������������s��

				// �L�[���̐ݒ�
				keyBlend		= &m_Motion[m_MotionBlend].KeyInfo[m_KeyBlend].Key[i];
				keyNextBlend	= &m_Motion[m_MotionBlend].KeyInfo[(m_KeyBlend + 1) % m_NumKeyBlend].Key[i];
				
				// �u�����h�䗦�̐ݒ�
				rateBlend = (float)m_FrameBlend / m_Motion[m_MotionBlend].KeyInfo[m_KeyBlend].Frame;

				// ���W�̃u�����h
				posBlend.x = (keyBlend->PosX * (1.0f - rateBlend)) + (keyNextBlend->PosX * rateBlend);
				posBlend.y = (keyBlend->PosY * (1.0f - rateBlend)) + (keyNextBlend->PosY * rateBlend);
				posBlend.z = (keyBlend->PosZ * (1.0f - rateBlend)) + (keyNextBlend->PosZ * rateBlend);

				// ��]�̃u�����h
				rotBlend.x = (keyBlend->RotX * (1.0f - rateBlend)) + (keyNextBlend->RotX * rateBlend);
				rotBlend.y = (keyBlend->RotY * (1.0f - rateBlend)) + (keyNextBlend->RotY * rateBlend);
				rotBlend.z = (keyBlend->RotZ * (1.0f - rateBlend)) + (keyNextBlend->RotZ * rateBlend);
				
				// �u�����h�䗦�̐ݒ�
				rateBlendMotion = (float)m_FrameBlendMotion / MOTION_CHANGE_FRAME;

				// ���W�̃u�����h
				posBlendMotion.x = (pos.x * (1.0f - rateBlendMotion)) + (posBlend.x * rateBlendMotion);
				posBlendMotion.y = (pos.y * (1.0f - rateBlendMotion)) + (posBlend.y * rateBlendMotion);
				posBlendMotion.z = (pos.z * (1.0f - rateBlendMotion)) + (posBlend.z * rateBlendMotion);

				// ��]�̃u�����h
				rotBlendMotion.x = (rot.x * (1.0f - rateBlendMotion)) + (rotBlend.x * rateBlendMotion);
				rotBlendMotion.y = (rot.y * (1.0f - rateBlendMotion)) + (rotBlend.y * rateBlendMotion);
				rotBlendMotion.z = (rot.z * (1.0f - rateBlendMotion)) + (rotBlend.z * rateBlendMotion);

				// �u�����h���[�V�����̍��W�E��]��ݒ�
				m_Model[i]->SetPos(posBlendMotion);
				m_Model[i]->SetRot(rotBlendMotion);
			}
			else
			{
				// ���[�V�����̍��W�E��]��ݒ�
				m_Model[i]->SetPos(pos);
				m_Model[i]->SetRot(rot);
			}
		}

		// �t���[���J�E���^�̑���
		if(m_Frame++ >= m_Motion[m_MotionType].KeyInfo[m_Key].Frame)
		{// �t���[���J�E���^���L�[�t���[���̃t���[�����𒴂����ꍇ

			// �L�[���̍Đݒ�
			m_Key = (m_Key + 1) % m_NumKey;
			// �t���[���J�E���^�̏�����
			m_Frame = 0;
			
			// ���[�V�����̓��[�v���邩�ǂ���
			if(!m_Motion[m_MotionType].Loop && (m_Key == (m_Motion[m_MotionType].NumKey - 1)))
			{
				SetMotion(MT_NEUTRAL);
			}
		}
		
		
		// ���[�V�������������s�����ǂ���
		if(m_MotionBlend != MT_NONE)
		{// ���[�V�������������s��

			if(m_FrameBlend++ >= m_Motion[m_MotionBlend].KeyInfo[m_KeyBlend].Frame)
			{// �t���[���J�E���^���L�[�t���[���̃t���[�����𒴂����ꍇ

				// �L�[���̍Đݒ�
				m_KeyBlend = (m_KeyBlend + 1) % m_NumKeyBlend;
				// �t���[���J�E���^�̏�����
				m_FrameBlend = 0;
				
				// ���[�V�����̓��[�v���邩�ǂ���
				if(!m_Motion[m_MotionBlend].Loop && (m_KeyBlend == (m_Motion[m_MotionBlend].NumKey - 1)))
				{
					SetMotion(MT_NEUTRAL);
				}
			}

			// �u�����h�J�E���^�̉��Z
			if(m_FrameBlendMotion++ >= MOTION_CHANGE_FRAME)
			{// �u�����h���I������
				m_FrameBlendMotion	= 0;
				m_MotionType		= m_MotionBlend;
				m_NumKey			= m_NumKeyBlend;
				m_Key				= m_KeyBlend;
				m_Frame				= m_FrameBlend;
				
				m_MotionBlend	= MT_NONE;
				m_NumKeyBlend	= 0;
				m_KeyBlend		= 0;
				m_FrameBlend	= 0;
			}
		}
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CPlayer::Draw(void)
{
	// ���[���h�}�g���b�N�X�̐ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// �`�揈��
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		m_Model[i]->Draw();
	}

	// �A���t�@�e�X�g�I��
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	
	// �f�o�b�O���\��
#ifdef _DEBUG
	CDebugProc::DebugProc("���f�����W:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	CDebugProc::DebugProc("Key:%d, Frame:%d\n", m_Key, m_Frame);
#endif
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *player;	// �C���X�^���X

	// �C���X�^���X����
	player = new CPlayer;

	// ����������
	player->Init(pos);

	// �C���X�^���X��Ԃ�
	return player;
}

//=============================================================================
//	�֐���	:LoadMotion
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:���[�V������ǂݍ��݁E�Z�b�g����B
//=============================================================================
void CPlayer::LoadMotion(char *fileName)
{
	int		nCntData	= 0;	// �z�񐶐��p�J�E���^
	char	*str		= NULL;	// �t�@�C�����e�i�[������
	int		offset		= 0;	// �I�t�Z�b�g
	

	// �o�C�i���t�@�C���ǂݍ���
	str = CReadFile::ReadBinaryFile(fileName);

	// �p�[�c���̓ǂݎ��
	offset = CReadFile::SearchWordByString(str, offset, "NUM_PARTS ");
	sscanf_s(&str[offset], "NUM_PARTS = %d", &m_NumParts);
	
	// 3D���f���t�@�C���̓ǂݎ��
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		char modelFileName[256] = {NULL};

		// ���������Z�b�g
		memset(modelFileName, NULL, sizeof(modelFileName));

		offset = CReadFile::SearchWordByString(str, offset + 3, "MODEL_FILENAME ");
		sscanf(&str[offset], "MODEL_FILENAME = %s", modelFileName);
		
		// ���f���̃C���X�^���X�̐���
		m_Model[i] = CModel::Create(modelFileName);
	}

	// ���f���������̓ǂݎ��E�Z�b�g
	offset = CReadFile::SearchWordByString(str, offset, "MODEL_INIT");
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		D3DXVECTOR3 posdef, rotdef;

		// �e�I�u�W�F�N�g�ԍ��̓ǂݎ��
		offset = CReadFile::SearchWordByString(str, offset + 3, "PARENT");
		nCntData = 0;
		sscanf_s(&str[offset], "PARENT = %d", &nCntData);

		// �e�I�u�W�F�N�g�ԍ��̃Z�b�g
		if(nCntData >= 0)
		{
			m_Model[i]->SetParent(m_Model[nCntData]);
		}
		else
		{
			m_Model[i]->SetParent(NULL);
		}

		// �������΍��W�̓ǂݎ��
		offset = CReadFile::SearchWordByString(str, offset, "POS");
		sscanf_s(&str[offset], "POS = %f %f %f", &posdef.x, &posdef.y, &posdef.z);

		// �������΍��W�̃Z�b�g
		m_Model[i]->SetPosDef(posdef);
		
		// �������Ή�]�̓ǂݎ��
		offset = CReadFile::SearchWordByString(str, offset, "ROT");
		sscanf_s(&str[offset], "ROT = %f %f %f", &rotdef.x, &rotdef.y, &rotdef.z);
		
		// �������Ή�]�̃Z�b�g
		m_Model[i]->SetRotDef(rotdef);
	}

	

	/* ���[�V�������ǂݍ��� */
	// ���[�V�������ǂݍ���
	offset = CReadFile::SearchWordByString(str, offset, "MOTION_NUM");
	nCntData = 0;
	sscanf_s(&str[offset], "MOTION_NUM = %d", &m_NumMotion);
	
	// ���[�V�������̐���
	m_Motion = new MOTION[m_NumMotion];
	
	for(int i = 0 ; i < m_NumMotion ; i++)
	{
		// ���[�v�ǂݍ���
		offset = CReadFile::SearchWordByString(str, offset, "LOOP");
		nCntData = 0;
		sscanf_s(&str[offset], "LOOP = %d", &nCntData);
		(nCntData == 0) ? m_Motion[i].Loop = false : m_Motion[i].Loop = true;

		// �L�[���ǂݍ���
		offset = CReadFile::SearchWordByString(str, offset, "NUM_KEY");
		nCntData = 0;
		sscanf_s(&str[offset], "NUM_KEY = %d", &nCntData);
		m_Motion[i].NumKey = nCntData;

		// �|�[�Y��񐶐�
		m_Motion[i].KeyInfo = new KEY_INFO[nCntData];
		
		for(int j = 0 ; j < m_Motion[i].NumKey ; j++)
		{
			// �t���[�����ǂݍ���
			offset = CReadFile::SearchWordByString(str, offset, "FRAME");
			nCntData = 0;
			sscanf_s(&str[offset], "FRAME = %d", &nCntData);
			m_Motion[i].KeyInfo[j].Frame = nCntData;

			// �L�[����
			m_Motion[i].KeyInfo[j].Key = new KEY[m_NumParts];

			// �L�[���ǂݍ���
			for(int k = 0 ; k < m_NumParts ; k++)
			{
				// ���W�ǂݍ���
				offset = CReadFile::SearchWordByString(str, offset + 1, "POS");
				nCntData = 0;
				sscanf_s(&str[offset], "POS = %f %f %f",
					&m_Motion[i].KeyInfo[j].Key[k].PosX,
					&m_Motion[i].KeyInfo[j].Key[k].PosY,
					&m_Motion[i].KeyInfo[j].Key[k].PosZ);

				// ��]�ǂݍ���
				offset = CReadFile::SearchWordByString(str, offset + 1, "ROT");
				nCntData = 0;
				sscanf_s(&str[offset], "ROT = %f %f %f",
					&m_Motion[i].KeyInfo[j].Key[k].RotX,
					&m_Motion[i].KeyInfo[j].Key[k].RotY,
					&m_Motion[i].KeyInfo[j].Key[k].RotZ);
			}
		}
	}
}

//=============================================================================
//	�֐���	:SetMotion
//	����	:MOTIONTYPE motionType(���[�V�����^�C�v)
//	�߂�l	:����
//	����	:���[�V�������Z�b�g����B
//=============================================================================
void CPlayer::SetMotion(MOTIONTYPE motionType)
{
	// ���[�V�����𓯎����s���邩�ǂ���
	if(m_MotionType == MT_NONE)
	{// ���[�V�����𓯎����s���Ȃ�

		m_MotionType	= motionType;
		m_NumKey		= m_Motion[(int)motionType].NumKey;
		m_Key			= 0;
		m_Frame			= 0;
	}
	else
	{// ���[�V�����𓯎����s����

		if(m_FrameBlendMotion == 0)
		{
			m_MotionBlend	= motionType;
			m_NumKeyBlend	= m_Motion[(int)motionType].NumKey;
			m_KeyBlend		= 0;
			m_FrameBlend	= 0;
		}
	}

	// ���[�V�����u�����h�J�E���^�̏�����
	m_FrameBlendMotion	= 0;
}