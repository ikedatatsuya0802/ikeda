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
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Pause		= false;

	m_MotionType= MT_NONE;
	m_Key		= 0;
	m_Frame		= 0;

	// ���f���E���[�V�����̓ǂݍ���
	LoadMotion("motion.txt");
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

}

//=============================================================================
//	�֐���	:UpdateMotion
//	����	:����
//	�߂�l	:����
//	����	:���[�V�������X�V����B
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	for(int i = 0 ; i < (int)m_Model.size() ; i++)
	{
		KEY *key, *keyNext;
		float rate;
		D3DXVECTOR3 pos, rot;

		key = &m_Motion.KeyInfo[m_Key].Key[i];
		keyNext = &m_Motion.KeyInfo[(m_Key + 1) % m_Motion.KeyInfo.size()].Key[i];
		rate = (float)m_Frame / m_Motion.KeyInfo[m_Key].Frame;
		pos.x = (key->PosX * (1.0f - rate)) + (keyNext->PosX * rate);
		pos.y = (key->PosY * (1.0f - rate)) + (keyNext->PosY * rate);
		pos.z = (key->PosZ * (1.0f - rate)) + (keyNext->PosZ * rate);
		rot.x = (key->RotX * (1.0f - rate)) + (keyNext->RotX * rate);
		rot.y = (key->RotY * (1.0f - rate)) + (keyNext->RotY * rate);
		rot.z = (key->RotZ * (1.0f - rate)) + (keyNext->RotZ * rate);
		m_Model[i]->SetPos(pos);
		m_Model[i]->SetRot(rot);
	}

	m_Frame++;
	if(m_Frame >= m_Motion.KeyInfo[m_Key].Frame)
	{
		m_Key = (m_Key + 1) % m_Motion.KeyInfo.size();
		m_Frame = 0;

		if(!m_Motion.Loop && (m_Key == (m_Motion.KeyInfo.size() - 1)))
		{
			SetMotion(MT_NEUTRAL);
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
	for(int i = 0 ; i < (int)m_Model.size() ; i++)
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
	FILE	*fp			= NULL;	// �t�@�C���|�C���^
	FILE	*fpBuff		= NULL;	// �t�@�C���|�C���^
	string	optional	= ".\\data\\";
	optional += fileName;

	fp = fopen(optional.c_str(), "r");

	// END_SCRIPT�܂œǂݍ���
	while(!feof(fp))
	{
		string word;
		char wordBuff[1024] = { NULL };

		// �P����擾
		fscanf(fp, "%s", wordBuff);
		word += wordBuff;

		if(word == "MODEL_DIR")
		{// ���f������ǂݍ���

			char buff[127] = { NULL };
			char buff2[127] = { NULL };
			string buf;
			vector<string> filename;

						
			// �t�@�C���f�B���N�g���A�t�@�C�����擾
			fscanf(fp, " = %s\n", buff);
			fscanf(fp, "MODEL_FILENAME = %s\n", buff2);

			// ���f���C���X�^���X����
			m_Model.push_back(CModel::Create(buff, buff2));

			int parent = 0;
			D3DXVECTOR3 vec = VEC3_ZERO;

			// �e�I�u�W�F�N�g�̓ǂݍ���
			fscanf(fp, "PARENT = %d\n", &parent);
			// �e�I�u�W�F�N�g�ԍ��̃Z�b�g
			m_Model[m_Model.size() - 1]->SetParent((parent >= 0) ? m_Model[parent] : NULL);

			// ���W�ǂݍ���
			fscanf(fp, "SCL = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			// ���W�̃Z�b�g
			m_Model[m_Model.size() - 1]->SetScl(vec);

			// ���W�ǂݍ���
			fscanf(fp, "DEFPOS = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			// ���W�̃Z�b�g
			m_Model[m_Model.size() - 1]->SetPosDef(vec);

			// ��]�ǂݍ���
			fscanf(fp, "DEFROT = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			// ��]�̃Z�b�g
			m_Model[m_Model.size() - 1]->SetRotDef(vec);

		}
		else if(word == "LOOP")
		{// ���[�V�����̃��[�v���ǂݍ���

			fscanf(fp, " = %d\n", &m_Motion.Loop);
		}
		else if(word == "FRAME")
		{// �L�[���ǂݍ���

			// �L�[���z�񐶐�
			m_Motion.KeyInfo.push_back(KEY_INFO());

			// �t���[�����i�[
			fscanf(fp, " = %d\n", &m_Motion.KeyInfo[m_Motion.KeyInfo.size() - 1].Frame);
		}
		else if(word == "POS")
		{// �L�[�z�񐶐�

			KEY key;
			// ���W���i�[
			fscanf(fp, " = %f %f %f\n", &key.PosX, &key.PosY, &key.PosZ);
			// ��]���i�[
			fscanf(fp, "ROT = %f %f %f\n", &key.RotX, &key.RotY, &key.RotZ);

			// �L�[�z�񐶐������i�[
			m_Motion.KeyInfo[m_Motion.KeyInfo.size() - 1].Key.push_back(key);
		}
	}

	// �t�@�C���N���[�Y
	fclose(fp);
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
		m_NumKey		= m_Motion.KeyInfo.size();
		m_Key			= 0;
		m_Frame			= 0;
	}
	else
	{// ���[�V�����𓯎����s����

		if(m_FrameBlendMotion == 0)
		{
			m_MotionBlend	= motionType;
			m_NumKeyBlend	= m_Motion.KeyInfo.size();
			m_KeyBlend		= 0;
			m_FrameBlend	= 0;
		}
	}

	// ���[�V�����u�����h�J�E���^�̏�����
	m_FrameBlendMotion	= 0;
}