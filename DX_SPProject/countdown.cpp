//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	countdown.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "countdown.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	�֐���	:CCountdown()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CCountdown::CCountdown(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	�֐���	:~CCountdown()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CCountdown::~CCountdown()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CCountdown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	m_Rot = VEC3_ZERO;
	m_Alpha = 0.0f;
	m_Tex = 0.0f;
	m_SizePow = 1.0f;
	m_Size = size;
	m_fLength	= hypotf((m_Size.x * m_SizePow), (m_Size.y * m_SizePow)) * 0.5f;
	m_fAngle	= atan2f((m_Size.x * m_SizePow), (m_Size.y * m_SizePow));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"COUNTDOWN_TEXFILENAME000, &m_pTexture);
	

	SetVtxBuff();
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CCountdown::SetVtxBuff(void)
{
	VERTEX_2D	*pVtx;	// 3D���_���


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_Alpha);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, m_Tex);
	pVtx[1].tex = D3DXVECTOR2(1.0f, m_Tex);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_Tex + 0.25f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, m_Tex + 0.25f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CCountdown::Uninit(void)
{
	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CCountdown::Update(void)
{
	int frame = CGame::GetFrame();


	m_fLength = hypotf((m_Size.x * m_SizePow), (m_Size.y * m_SizePow)) * 0.5f;
	m_fAngle = atan2f((m_Size.x * m_SizePow), (m_Size.y * m_SizePow));
	
	if(frame >= (COUNTDOWN_START + MINUTE * COUNTDOWN_MINUTE))
	{
		if(frame == (COUNTDOWN_START + MINUTE * COUNTDOWN_MINUTE) + 1) m_SizePow = 1.0f;

		m_Tex = 0.75f;

		m_Alpha -= (1.0f / MINUTE);
		if(m_Alpha < 0.0f) m_Alpha = 0.0f;

		m_SizePow += (1.0f / MINUTE * 1.5f);
	}
	else if(frame > COUNTDOWN_START)
	{
		if(frame % MINUTE == 1) m_SizePow = 1.0f;

		if((frame - (MINUTE * COUNTDOWN_MINUTE)) / MINUTE == 0)
		{// �J�E���g3

			m_Tex = 0.0f;

			m_Alpha = ((frame - (MINUTE * COUNTDOWN_MINUTE)) % MINUTE) / (float)MINUTE;

			m_SizePow += (1.0f / MINUTE * COUNTDOWN_SIZE_POW);
		}
		else if((frame - (MINUTE * COUNTDOWN_MINUTE)) / MINUTE == 1)
		{// �J�E���g2

			m_Tex = 0.25f;

			m_Alpha = ((frame - (MINUTE * COUNTDOWN_MINUTE)) % MINUTE) / (float)MINUTE;

			m_SizePow += (1.0f / MINUTE * COUNTDOWN_SIZE_POW);
		}
		else if((frame - (MINUTE * COUNTDOWN_MINUTE)) / MINUTE == 2)
		{// �J�E���g1

			m_Tex = 0.5f;

			m_Alpha = ((frame - (MINUTE * COUNTDOWN_MINUTE)) % MINUTE) / (float)MINUTE;

			m_SizePow += (1.0f / MINUTE * COUNTDOWN_SIZE_POW);
		}
	}

	SetVtxBuff();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CCountdown::Draw(void)
{
	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// ���_�o�b�t�@�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);
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
CCountdown *CCountdown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CCountdown *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CCountdown;

	// ����������
	instance->Init(pos, size);

	// �C���X�^���X�����^�[��
	return instance;
}