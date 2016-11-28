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
#include "main.h"
#include "player.h"
#include "model.h"
#include "game.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "scene3DDX.h"
#include "meshfield.h"
#include "railLine.h"

//=============================================================================
//	�֐���	:CPlayer()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CPlayer::CPlayer(int priority) : CSceneDX(priority)
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
	char			*str	= NULL;	// �t�@�C�����e�i�[�z��
	unsigned int	offset	= 0;	// ������w��q

	m_NumParts	= 0;
	m_NumMotion	= 0;
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_Move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove	= 0;
	m_bJump		= false;
	m_NumKey	= 0;
	m_Key		= 0;
	m_Frame		= 0;
	m_Pause		= false;

	// ���f���E���[�V�����̓ǂݍ���
	LoadMotion("./data/motion.txt");
	SetMotion(MT_NONE);
	m_Pause		= false;
	
	m_RailLine			= 0;
	//m_Spline->nNum		= 0;
	m_Per		= 0.0f;
	m_PerMove	= 0.0f;
	
	// �X�v���C���̓ǂݍ���
	//LoadSpline(m_RailLine);
	m_Spline = CGame::GetRailLine()->GetSpline();
	CalcSpline(0);
	//SetPos(D3DXVECTOR3(m_Spline->PosHermite[0].x, m_Spline->PosHermite[0].y, m_Spline->PosHermite[0].z));
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CPlayer::Uninit(void)
{
	for(int i = 0 ; i < m_NumMotion ; i++)
	{
		for(int j = 0 ; j < m_Motion[i].NumKey ; j++)
		{
			if(m_Motion[i].KeyInfo[j].Key != NULL)
			{
				m_Motion[i].KeyInfo[j].Key = NULL;
			}
		}
		if(m_Motion[i].KeyInfo != NULL)
		{
			m_Motion[i].KeyInfo = NULL;
		}
	}
	if(m_Motion != NULL)
	{
		m_Motion = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CPlayer::Update(void)
{
	if(KT_P)
	{
		m_Pause = (m_Pause ? false : true);
	}

	if(m_Pause == false)
	{
		// �v���C���[�ړ�
		UpdateMove();
	
		// ���Ή�]�ݒ�
		UpdateMotion();
	}
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
	CMeshfield	*mesh		= CGame::GetMeshfield();		// ���b�V���t�B�[���h

	float nowt = (m_Per - ((int)m_Per));
	
	if(CInput::GetKeyboardPress(DIK_W))				// ��
	{
		// �ړ��ʂ�ݒ�
		m_PerMove += PLAYER_MOVEMENT;
	}

	// �ړ��ʔ��f
	if((m_Per <= (m_Spline->nNum - 1)) && (m_Per >= 0.0f))
	{
		m_Per += m_PerMove;
		if(m_Per > (m_Spline->nNum - 1))
		{
			m_Per -= (m_Spline->nNum - 1);
		}
	}
	else if(m_Per < 0.0f)
	{
		m_Per = 0.0f;
	}

	// �ʒu���f
	m_Pos.x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)m_Per + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)m_Per + 1].x)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)m_Per + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)m_Per + 1].x);
	m_Pos.z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)m_Per + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)m_Per + 1].z)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)m_Per + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)m_Per + 1].z);	
	//m_Pos.y = 1.0f;

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if((int)(nowt * RAIL_SET) > 0)
	{
		vec = D3DXVECTOR3((m_Spline->PosHermite[(int)(nowt * RAIL_SET) + 1].x - m_Spline->PosHermite[(int)(nowt * RAIL_SET) - 1].x),
			0.0f,
			(m_Spline->PosHermite[(int)(nowt * RAIL_SET) + 1].z - m_Spline->PosHermite[(int)(nowt * RAIL_SET) - 1].z));
	}
	D3DXVec3Normalize(&vec, &vec);
	vec *= RAIL_MARGIN;

	if(m_RailLine > 0)
	{
		m_Pos.x -= cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) - sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
		m_Pos.z -= sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) + cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
	}
	else if(m_RailLine < 0)
	{
		m_Pos.x += cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) - sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
		m_Pos.z += sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) + cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
	}
	
	// ��]���f
	float tDis = nowt + 0.01f;
	D3DXVECTOR3 vecDis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vecDis.x = (pow((tDis - 1), 2) * (2 * tDis + 1) * m_Spline->Pos[(int)m_Per + 0].x) + (powf(tDis, 2) * (3 - 2 * tDis) * m_Spline->Pos[(int)m_Per + 1].x)
		+ (pow((1 - tDis), 2) * tDis * m_Spline->Vec[(int)m_Per + 0].x) + ((tDis - 1) * powf(tDis, 2) * m_Spline->Vec[(int)m_Per + 1].x);
	vecDis.z = (pow((tDis - 1), 2) * (2 * tDis + 1) * m_Spline->Pos[(int)m_Per + 0].z) + (powf(tDis, 2) * (3 - 2 * tDis) * m_Spline->Pos[(int)m_Per + 1].z)
		+ (pow((1 - tDis), 2) * tDis * m_Spline->Vec[(int)m_Per + 0].z) + ((tDis - 1) * powf(tDis, 2) * m_Spline->Vec[(int)m_Per + 1].z);

	m_Rot.y = atan2f((m_Pos.x - vecDis.x), (m_Pos.z - vecDis.z));

	m_PerMove += (-m_PerMove * PLAYER_SPEED_DOWN);

	// �W�����v
	if(KT_SPACE && !m_bJump)
	{
		m_Move.y += PLAYER_JUMP;

		m_bJump = true;
	}

	// �W�����v�ʂ̔��f
	m_Pos.y += m_Move.y;

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
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		KEY *key, *keyNext;
		float rate;
		D3DXVECTOR3 pos, rot;

		key		= &m_Motion[m_MotionType].KeyInfo[m_Key].Key[i];
		keyNext	= &m_Motion[m_MotionType].KeyInfo[(m_Key + 1) % m_NumKey].Key[i];
		rate = (float)m_Frame / m_Motion[m_MotionType].KeyInfo[m_Key].Frame;
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
	if(m_Frame >= m_Motion[m_MotionType].KeyInfo[m_Key].Frame)
	{
		m_Key = (m_Key + 1) % m_NumKey;
		m_Frame = 0;
		
		if(!m_Motion[m_MotionType].Loop && (m_Key == (m_Motion[m_MotionType].NumKey - 1)))
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
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// �}�g���b�N�X

	
	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	//D3DXMatrixRotationQuaternion(&mtxRot, &m_Quaternion);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
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
	CDebugProc::DebugProc("�X�v���C�����W:(%.4f)\n", m_Per);
	CDebugProc::DebugProc("���f�����W:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	//CDebugProc::DebugProc("Key:%d, Frame:%d\n", m_Key, m_Frame);
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
//	�֐���	:LoadMotion
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:���[�V������ǂݍ��݁E�Z�b�g����B
//=============================================================================
void CPlayer::SetMotion(MOTIONTYPE motionType)
{
	switch(motionType)
	{
	case MT_NEUTRAL:
		{
			m_NumKey = m_Motion[(int)motionType].NumKey;
			break;
		}
	case MT_WALK:
		{
			m_NumKey = m_Motion[(int)motionType].NumKey;
			break;
		}
	default:
		m_NumKey = m_Motion[(int)motionType].NumKey;
		break;
	}
	m_MotionType	= motionType;
	m_Key			= 0;
	m_Frame			= 0;
}

//=============================================================================
//	�֐���	:LoadSpline
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�Ȃ�
//=============================================================================
void CPlayer::LoadSpline(int line)
{/*
	FILE	*fp = NULL;	// �t�@�C���|�C���^

	fp = fopen("./data/spline.txt", "r");

	// �t�@�C���I���܂œǂݍ���
	while(feof(fp) != EOF)
	{
		char str[65535] = {NULL};
		memset(str, NULL, sizeof(str));

		fscanf(fp, "%s", str);

		if(strcmp(str, "NUM_POSITION") == 0)
		{
			fscanf(fp, " = %d", &m_Spline->nNum);

			if(m_Spline->nNum > 0)
			{
				m_Spline->PosHermite = new D3DXVECTOR3[(m_Spline->nNum - 1) * RAIL_SET];
				m_Spline->Pos = new D3DXVECTOR3[m_Spline->nNum];
				m_Spline->Vec = new D3DXVECTOR3[m_Spline->nNum];
			}
		}
		else if(strcmp(str, "POS") == 0)
		{
			// �G���~�[�g���W�ǂݎ��
			for(int i = 0 ; i < m_Spline->nNum ; i++)
			{
				if(i > 0)
				{
					if(fscanf(fp, "POS = %f %f %f\n", &m_Spline->Pos[i].x, &m_Spline->Pos[i].y, &m_Spline->Pos[i].z) == EOF)
					{
						m_Spline->Pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					if(fscanf(fp, " = %f %f %f\n", &m_Spline->Pos[i].x, &m_Spline->Pos[i].y, &m_Spline->Pos[i].z) == EOF)
					{
						m_Spline->Pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
		else if(strcmp(str, "VEC") == 0)
		{
			// �G���~�[�g�x�N�g���ǂݎ��
			for(int i = 0 ; i < m_Spline->nNum ; i++)
			{
				if(i > 0)
				{
					if(fscanf(fp, "VEC = %f %f %f\n", &m_Spline->Vec[i].x, &m_Spline->Vec[i].y, &m_Spline->Vec[i].z) == EOF)
					{
						m_Spline->Vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					if(fscanf(fp, " = %f %f %f\n", &m_Spline->Vec[i].x, &m_Spline->Vec[i].y, &m_Spline->Vec[i].z) == EOF)
					{
						m_Spline->Vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
			}

			// �ǂݍ��ݏI��
			break;
		}
	}

	fclose(fp);*/
}

//=============================================================================
//	�֐���	:CalcSpline
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�Ȃ�
//=============================================================================
void CPlayer::CalcSpline(int line)
{
	float t		= 0.0f;
	float rot	= 0.0f;
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if(m_Spline->nNum >= 2)
	{
		for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++, t += (1.0f / RAIL_SET))
		{
			float nowt = (t - ((int)t));

			m_Spline->PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].x)
								+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].x);
			m_Spline->PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].z)
								+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].z);	
			m_Spline->PosHermite[i].y = 1.0f;
		}
	}

	
	for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++, t += (1.0f / RAIL_SET))
	{
		float nowt = (t - ((int)t));

		if((int)t == 0)
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t + 1].x - m_Spline->PosHermite[(int)t].x), 0.0f, (m_Spline->PosHermite[(int)t + 1].z - m_Spline->PosHermite[(int)t].z));
		}
		else if(i == (RAIL_SET - 1))
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t].x - m_Spline->PosHermite[(int)t - 1].x), 0.0f, (m_Spline->PosHermite[(int)t].z - m_Spline->PosHermite[(int)t - 1].z));
		}
		else
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t + 1].x - m_Spline->PosHermite[(int)t - 1].x), 0.0f, (m_Spline->PosHermite[(int)t + 1].z - m_Spline->PosHermite[(int)t - 1].z));
		}
		D3DXVec3Normalize(&vec, &vec);
		vec *= RAIL_MARGIN;

		if(line > 0)
		{
			m_Spline->PosHermite[i].x -= cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline->PosHermite[i].z -= sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}
		else if(line < 0)
		{
			m_Spline->PosHermite[i].x += cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline->PosHermite[i].z += sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}

		t += (1.0f / RAIL_SET);
	}
}