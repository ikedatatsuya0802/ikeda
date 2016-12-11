//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	speedmeter.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "speedmeter.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	�֐���	:CSpeedmeter()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSpeedmeter::CSpeedmeter(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	�֐���	:~CSpeedmeter()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSpeedmeter::~CSpeedmeter()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CSpeedmeter::Init(float maxSpeed, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, char *str)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);
	m_MaxSpeed	= maxSpeed;

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[0], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[1], NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"SPEEDMETER_TEXFILENAME000, &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"SPEEDMETER_TEXFILENAME001, &m_pTexture[1]);
	
	SetVtxBuff(&m_pVtxBuff[0]);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, SPEEDMETER_ROT_LOW);
	SetVtxBuff(&m_pVtxBuff[1]);
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CSpeedmeter::SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff)
{
	VERTEX_2D	*pVtx;	// 3D���_���


	(*vtxBuff)->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.z = 0.0f;

	pVtx[1].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.z = 0.0f;

	pVtx[2].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.z = 0.0f;

	pVtx[3].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.z = 0.0f;

	for(int i = 0 ; i < VERTEX_NUM ; i++)
	{
		// ���Z�W���ݒ�
		pVtx[i].rhw = 1.0f;

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	(*vtxBuff)->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSpeedmeter::Uninit(void)
{
	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff[0]);
	SafetyRelease(m_pVtxBuff[1]);
	SafetyRelease(m_pTexture[0]);
	SafetyRelease(m_pTexture[1]);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSpeedmeter::Update(void)
{
	VERTEX_2D	*pVtx;	// 3D���_���
	CPlayer* player = CGame::GetPlayer1();


	// ���[�^�[�p�x�̕␳
	//m_Rot.z = player->GetSpeed() * (4.0f);
	m_Rot.z = ((player->GetSpeed() / PLAYER_SPEED_MAX) * 4.0f);
	m_Rot.z -= 2.0f;

	// �e��X�R�A���̍X�V
	m_pVtxBuff[1]->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���W�̍X�V
		pVtx[0].Pos.x = m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength);
		pVtx[0].Pos.y = m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength);

		pVtx[1].Pos.x = m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength);
		pVtx[1].Pos.y = m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength);

		pVtx[2].Pos.x = m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength);
		pVtx[2].Pos.y = m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength);

		pVtx[3].Pos.x = m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength);
		pVtx[3].Pos.y = m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength);
	}
	m_pVtxBuff[1]->Unlock();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CSpeedmeter::Draw(void)
{
	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// ���_�o�b�t�@�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[0], 0, sizeof(VERTEX_2D));
	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture[0]);
	// �v���~�e�B�u�`��
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

	// ���_�o�b�t�@�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[1], 0, sizeof(VERTEX_2D));
	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture[1]);
	// �v���~�e�B�u�`��
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

	// �A���t�@�e�X�g�I��
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3	pos		-> �����ʒu
//			:D3DXVECTOR2	size	-> �|���S���̃T�C�Y
//			:char			*str	-> �e�N�X�`���̃t�@�C���p�X
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CSpeedmeter *CSpeedmeter::Create(float maxSpeed, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, char *str)
{
	CSpeedmeter *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CSpeedmeter;

	// ����������
	instance->Init(maxSpeed, pos, rot, size, str);

	// �C���X�^���X�����^�[��
	return instance;
}