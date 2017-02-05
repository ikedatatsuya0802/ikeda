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
#include "player_title.h"
#include "cameraDX.h"
#include "meshfield.h"
#include "sceneXDX.h"
#include "model.h"
#include "manager.h"

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CPlayer_Title::Init(D3DXVECTOR3 pos)
{
	m_Pos = pos;
	m_Move = VEC3_ZERO;
	m_Move.z = PLAYER_TITLE_SPEED;

	// ���f���E���[�V�����̓ǂݍ���
	LoadMotion("./data/motion.txt");
	m_Pause = false;

	m_Frame = 0;
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CPlayer_Title::Update(void)
{
	// �ړ����Ă��銴��
	m_Pos.z += m_Move.z;

	m_Frame++;
	if(CManager::GetCamera()->ifCameraAnimInitialized())
	{
		if(m_Frame > CManager::GetCamera()->GetCameraAnimFrame())
		{
			m_Frame = 0;
			m_Pos.z = -(MESHFIELD_TOTALHEIGHT * 0.5f);
		}
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CPlayer_Title::Draw(void)
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

#ifdef _DEBUG
	CDebugProc::DebugProc("���f�����W:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
#endif
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CPlayer_Title *CPlayer_Title::Create(bool ifListAdd, int priority, OBJTYPE objtype, D3DXVECTOR3 pos)
{
	CPlayer_Title *player;	// �C���X�^���X

	// �C���X�^���X����
	player = new CPlayer_Title;

	// ����������
	player->Init(pos);

	// �C���X�^���X�����^�[��
	return player;
}