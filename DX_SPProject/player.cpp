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
#include "player.h"
#include "game.h"
#include "meshfield.h"
#include "railLine.h"
#include "cameraDX.h"
#include "driftMark.h"
#include "countdown.h"
#include "lightDX.h"

//=============================================================================
//	�֐���	:CPlayer()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CPlayer::CPlayer(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneXDX(ifListAdd, priority, objtype)
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
	m_Pos		= VEC3_ZERO;
	m_Rot		= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_Move		= VEC3_ZERO;
	m_RotMove	= VEC3_ZERO;
	m_MoveVec	= VEC3_ZERO;
	m_nCntMove	= 0;
	m_bJump		= false;
	m_Pause		= false;

	// ���f���E���[�V�����̓ǂݍ���
	LoadMotion("./data/motion.txt");
	m_Pause		= false;
	
	m_RailLine	= 0;
	//m_Spline->nNum		= 0;
	m_Per		= PLAYER_START_PER;
	m_PerOld	= PLAYER_START_PER;
	m_PerMove	= 0.0f;
	
	// �X�v���C���̓ǂݍ���
	//LoadSpline(m_RailLine);
	m_Spline = CGame::GetRailLine()->GetSpline();

	m_SplineTime = 0.0f;
	m_DriftCurve = 0;
	m_CntDrift = 0;

	// �X�|�b�g���C�g�̋N��
	//CLightDX::ChangeLight(CLightDX::GetLightNum() - 1, true);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CPlayer::Uninit(void)
{
	// �X�|�b�g���C�g�̏I��
	//CLightDX::ChangeLight(CLightDX::GetLightNum() - 1, false);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CPlayer::Update(void)
{
	// �X�v���C���ʒu��ۑ�
	m_PerOld = m_Per;

	if(m_Per <= RAILLINE_GOAL)
	{// �S�[���ɓ�������܂�

		// �v���C���[�ړ�
		UpdateMove();


		// �h���t�g�}�[�N�J�n���f
		if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).ifDrift)
		{
			if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).Status == -1)
			{// �h���t�g�̊J�n

				if(!CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).Curve)
				{// ���J�[�u

					if((m_DriftCurve >= 0) && (m_PerMove > PLAYER_CURVESPEED_MAX))
					{// �h���t�g���Ă��Ȃ������ꍇ�A80km/h�𒴂��Ă����猸��

						CManager::GetCamera()->SetCameraVibrate(180, 10.0f);
						m_PerMove -= PLAYER_NOT_DRIFT;
						if(m_PerMove < 0.0f) m_PerMove = 0;
					}
				}
				else
				{// �E�J�[�u

					if((m_DriftCurve <= 0) && (m_PerMove > PLAYER_CURVESPEED_MAX))
					{// �h���t�g���Ă��Ȃ������ꍇ�A80km/h�𒴂��Ă����猸��

						CManager::GetCamera()->SetCameraVibrate(180, 10.0f);
						m_PerMove -= PLAYER_NOT_DRIFT;
						if(m_PerMove < 0.0f) m_PerMove = 0;
					}
				}
			}
		}

		UpdateDrift();
	}
	else
	{// �S�[�����B��

		D3DXVECTOR3 pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);
		D3DXVECTOR3 vecDis = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL + 0.01f);

		// ��]�ʌv�Z
		m_Spline->Rot.y = atan2f((pos.x - vecDis.x), (pos.z - vecDis.z));

		// �ʒu���f
		m_Pos.x -= m_RealSpeed * sinf(m_Spline->Rot.y);
		m_Pos.z -= m_RealSpeed * cosf(m_Spline->Rot.y);
	}

	D3DXVECTOR3 lightVec = VEC3_ZERO;
	D3DXVec3Normalize(&lightVec, &m_MoveVec);

	// �X�|�b�g���C�g�̐ݒ�
	//CLightDX::SetSpotLight(0, m_Pos, m_MoveVec);
}

//=============================================================================
//	�֐���	:UpdateMove
//	����	:����
//	�߂�l	:����
//	����	:�I�u�W�F�N�g���ړ��E��]������B
//=============================================================================
void CPlayer::UpdateMove(void)
{
	CCameraDX	*camera		= DX_CAMERA;	// �J����
	int			posNum = (int)(m_Per * ((int)m_Spline->Pos.size() - 1));
	

	// �X�v���C���̃��[�h
	m_Spline = CGame::GetRailLine()->GetSpline();

	
	if((CGame::GetFrame() > COUNTDOWN_END_TIME) && !CManager::GetEdhitMode() && KH_W)
	{// ����

		m_PerMove += PLAYER_MOVEMENT;
	}
	else if(!CManager::GetEdhitMode() && KH_S)
	{// ����

		m_PerMove -= PLAYER_MOVEMENT * 2;
		if(m_PerMove < 0.0f)
		{
			m_PerMove = 0.0f;
		}
	}
	
	// ���x����
	if(m_PerMove > PLAYER_SPEED_MAX)
	{
		m_PerMove = PLAYER_SPEED_MAX;
	}

	// �ړ��ʔ��f
	float	realPer = ((1.0f / 60.0f) * (m_PerMove / (m_Spline->LengthMin[posNum] / 10.0f)));
	float	farT = m_Per + realPer;
	int		farPosNum = (int)(farT * ((int)m_Spline->Pos.size() - 1));

	if(posNum != farPosNum)
	{
		//float farRealPer = ((1.0f / 60.0f) * (m_PerMove / (m_Spline->LengthMin[farPosNum] / 10.0f)));
		//realPer = (realPer + farRealPer) / 2.0f;
	}

	m_Per += realPer;
	CDebugProc::DebugProc("Per:%f\n", m_Per);
	CDebugProc::DebugProc("���݋��:%d->%d\n", posNum, posNum + 1);
	CDebugProc::DebugProc("�����x:%f\n", realPer);

	// ��Έړ��ʂ̌v��
	m_RealSpeed = D3DXVec3Length(&(CGame::GetRailLine()->GetSplinePos(m_Per) - m_Pos));

	// �ʒu���f
	m_Pos.x = CGame::GetRailLine()->GetSplinePos(m_Per).x;
	m_Pos.z = CGame::GetRailLine()->GetSplinePos(m_Per).z;
	m_Pos.y = CGame::GetRailLine()->GetSplinePos(m_Per).y;

	
	// ��]���f
	float tDis = m_Per + 0.01f;
	m_MoveVec.x = CGame::GetRailLine()->GetSplinePos(tDis).x;
	m_MoveVec.z = CGame::GetRailLine()->GetSplinePos(tDis).z;

	// ��]�ʌv�Z
	m_Spline->Rot.y = atan2f((m_Pos.x - m_MoveVec.x), (m_Pos.z - m_MoveVec.z));

	
	// �X�΂̉�]���v�Z
	D3DXVECTOR3 vec = CGame::GetRailLine()->GetMoveVec(m_Per);
	D3DXVec3Normalize(&vec, &vec);

	m_VecQuat.x = -vec.z;
	m_VecQuat.y = 0.0f;
	m_VecQuat.z = vec.x;

	// �X�΂̉�]���v�Z
	m_RotQuat = CGame::GetRailLine()->AngleOf2Vector(vec, D3DXVECTOR3(vec.x, 0.0f, vec.z));
}

//=============================================================================
//	�֐���	:UpdateDrift
//	����	:����
//	�߂�l	:����
//	����	:�h���t�g�̎��s�Ǘ��B
//=============================================================================
void CPlayer::UpdateDrift(void)
{
	static int m_DriftTime = 0;	// �h���t�g���Ă��鎞��

	// �h���t�g
	if(KT_LEFT && (CGame::GetFrame() > COUNTDOWN_END_TIME) && !m_CntDrift
		&& !m_bJump && ((m_DriftCurve == 0) || (m_DriftCurve == 2)))
	{
		m_Move.y += PLAYER_JUMP;

		m_bJump = true;

		if(!m_DriftCurve) m_DriftCurve--;
		m_DriftCurve--;
		m_CntDrift = PLAYER_ROT_STEP;

		// �G�t�F�N�g�\��
		m_DriftTime = 120;
		CGame::GetHakushin()->SetColor(1.0f);

		// �J���������ݒ�
		CManager::GetCamera()->SetDisVec(-PLAYER_CAMERA_DISVEC);
	}
	else if(KT_LEFT && (CGame::GetFrame() > COUNTDOWN_END_TIME) && !m_CntDrift
		&& !m_bJump && ((m_DriftCurve == 0) || (m_DriftCurve == -2)))
	{
		m_Move.y += PLAYER_JUMP;

		m_bJump = true;

		if(!m_DriftCurve) m_DriftCurve++;
		m_DriftCurve++;
		m_CntDrift = PLAYER_ROT_STEP;

		// �G�t�F�N�g�\��
		m_DriftTime = 120;
		CGame::GetHakushin()->SetColor(1.0f);

		// �J���������ݒ�
		CManager::GetCamera()->SetDisVec(-PLAYER_CAMERA_DISVEC);
	}

	// �h���t�g�����ǂ���
	if((m_DriftCurve == 2) || (m_DriftCurve == -2))
	{
		if(m_DriftTime > 0)
		{
			CGame::GetHakushin()->SetSize(true, 0.997f, 0.997f);
			m_DriftTime--;
		}
		else
		{
			m_DriftTime = 0;
		}
	}
	else
	{
		m_DriftTime = 0;
		CGame::GetHakushin()->SetSize(false, SCREEN_WIDTH * 1.5f, SCREEN_HEIGHT * 1.5f);
	}

	// �h���t�g�}�[�N�I�����f
	if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).ifDrift)
	{
		if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).Status == 1)
		{// �h���t�g�̏I�_�̏ꍇ

			if(m_DriftCurve == 2)
			{
				m_DriftCurve--;
				m_CntDrift = PLAYER_ROT_STEP;
				m_Move.y += PLAYER_JUMP;

				m_bJump = true;

				// �J���������ݒ�
				CManager::GetCamera()->SetDisVec(PLAYER_CAMERA_DISVEC);
			}
			else if(m_DriftCurve == -2)
			{
				m_DriftCurve++;
				m_CntDrift = PLAYER_ROT_STEP;
				m_Move.y += PLAYER_JUMP;

				m_bJump = true;

				// �J���������ݒ�
				CManager::GetCamera()->SetDisVec(PLAYER_CAMERA_DISVEC);
			}
		}
	}

	if(m_CntDrift > 0)
	{
		switch(m_DriftCurve)
		{
		case -2:
			m_Rot.y -= PLAYER_ROT_SPEED;
			break;
		case -1:
			m_Rot.y += PLAYER_ROT_SPEED;
			break;
		case 0:
			break;
		case 1:
			m_Rot.y -= PLAYER_ROT_SPEED;
			break;
		case 2:
			m_Rot.y += PLAYER_ROT_SPEED;
			break;
		default:
			break;
		}

		m_CntDrift--;
		if(((m_DriftCurve == -1) || (m_DriftCurve == 1)) && !m_CntDrift)
		{
			m_DriftCurve = 0;
		}
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
	D3DXVECTOR3 rot = D3DXVECTOR3(m_Rot.x, m_Rot.y + m_Spline->Rot.y, m_Rot.z);
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxQuat, mtxTrans;			// �}�g���b�N�X


	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �X�ΐݒ�
	D3DXQUATERNION quat = D3DXQUATERNION(m_Pos.x, m_Pos.x, m_Pos.x, 0.0f);
	D3DXQuaternionRotationAxis(&quat, &m_VecQuat, m_RotQuat);
	D3DXMatrixRotationQuaternion(&mtxQuat, &quat);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxQuat);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
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
	
	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// �f�o�b�O���\��
#ifdef _DEBUG
	//CDebugProc::DebugProc("�X�v���C�����W:(%.4f)\n", m_Per);
	CDebugProc::DebugProc("���f�����W:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	CDebugProc::DebugProc("QUAT:(%5.2f:%5.2f:%5.2f:%5.2f)\n", quat.x, quat.y, quat.z, quat.w);
	CDebugProc::DebugProc("ROTQUAT:%5.2f\n", m_RotQuat);
#endif
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CPlayer *CPlayer::Create(bool ifListAdd, D3DXVECTOR3 pos)
{
	CPlayer *player;	// �C���X�^���X

	// �C���X�^���X����
	player = new CPlayer(ifListAdd);

	// ����������
	player->Init(pos);

	// �C���X�^���X�����^�[��
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