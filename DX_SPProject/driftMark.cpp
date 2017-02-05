//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	driftMark.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "driftMark.h"
#include "railLine.h"
#include "player.h"
#include "game.h"

bool		CDriftMark::m_VisibleType	= false;
bool		CDriftMark::m_flgCounter	= true;
bool		CDriftMark::m_Curve			= true;
int			CDriftMark::m_Count			= 0;
int			CDriftMark::m_SetCounter	= 0;

//=============================================================================
//	�֐���	:CScene2DDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CDriftMark::CDriftMark(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

//=============================================================================
//	�֐���	:~CDriftMark()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CDriftMark::~CDriftMark()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CDriftMark::Init(void)
{
	// �e�평��������
	m_DriftMark.Pos = D3DXVECTOR3((SCREEN_WIDTH * 0.3f), (SCREEN_HEIGHT * 0.3f), 0.0f);
	m_DriftMark.Size = D3DXVECTOR2(DRIFTMARK_WIDTH, DRIFTMARK_HEIGHT);
	m_DriftMark.Col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	
	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(DRIFTMARK_TEXNAME000, TEX_FILEPASS), &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(DRIFTMARK_TEXNAME001, TEX_FILEPASS), &m_pTexture[1]);

	SetVtxBuff();
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CDriftMark::SetVtxBuff(void)
{
	VERTEX_2D	*pVtx;	// 3D���_���


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[1].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));
	pVtx[2].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[3].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));
	pVtx[4].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[5].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));
	pVtx[6].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[7].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));

	pVtx[0].Pos.y = (m_DriftMark.Pos.y - (m_DriftMark.Size.y * 0.5f));
	pVtx[1].Pos.y = (m_DriftMark.Pos.y - (m_DriftMark.Size.y * 0.5f));
	pVtx[2].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));
	pVtx[3].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));

	pVtx[5].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));
	pVtx[4].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));
	pVtx[6].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.5f));
	pVtx[7].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.5f));

	for(int i = 0 ; i < VERTEX_SQUARE * 2 ; i++)
	{
		pVtx[i].Pos.z = 0.0f;

		// ���Z�W���ݒ�
		pVtx[i].rhw = 1.0f;

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 0.0f);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.6f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.6f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.6f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.6f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CDriftMark::Uninit(void)
{
	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture[0]);
	SafetyRelease(m_pTexture[1]);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CDriftMark::Update(void)
{
	static float tex = 0.0f;
	// �e�N�X�`�����W�ړ�
	VERTEX_2D	*pVtx;	// 3D���_���
	float oldt	= CGame::GetPlayer1()->GetOldPerSpline();
	float t		= CGame::GetPlayer1()->GetPerSpline();
	float futureoldt = CGame::GetPlayer1()->GetOldPerSpline() + DRIFTMARK_FUTURE;
	float futuret = CGame::GetPlayer1()->GetPerSpline() + DRIFTMARK_FUTURE;

	// �h���t�g�}�[�N�o�����f
	if(CGame::GetRailLine()->GetDriftStatus(futureoldt, futuret).ifDrift)
	{
		if(CGame::GetRailLine()->GetDriftStatus(futureoldt, futuret).Status == -1)
		{// �h���t�g�̎n�_�̏ꍇ

			if(CGame::GetRailLine()->GetDriftStatus(futureoldt, futuret).Curve)
			{// �E�J�[�u�̌x��

				m_DriftMark.Pos = D3DXVECTOR3((SCREEN_WIDTH * 0.2f), (SCREEN_HEIGHT * 0.3f), 0.0f);
				SetVtxBuff();
				CDriftMark::VisibleDriftMark(true, true, 60);
			}
			else
			{// ���J�[�u�̌x��
				m_DriftMark.Pos = D3DXVECTOR3((SCREEN_WIDTH * 0.8f), (SCREEN_HEIGHT * 0.3f), 0.0f);
				SetVtxBuff();
				CDriftMark::VisibleDriftMark(true, false, 60);
			}
		}
	}

	// �h���t�g�}�[�N�������f
	if(CGame::GetRailLine()->GetDriftStatus(oldt, t).ifDrift)
	{
		if(CGame::GetRailLine()->GetDriftStatus(oldt, t).Status == 1)
		{// �h���t�g�̏I�_�̏ꍇ

			CDriftMark::InvisibleDriftMark(30);
		}
	}

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(tex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(tex + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(tex, 0.6f);
	pVtx[3].tex = D3DXVECTOR2(tex + 1.0f, 0.6f);
	m_pVtxBuff->Unlock();
	tex += m_Curve ? -DRIFTMARK_SRIDESPEED : DRIFTMARK_SRIDESPEED;

	if(m_VisibleType)
	{// �_��

		float rad = D3DX_PI / m_SetCounter * (m_SetCounter - m_Count);
		float alpha = sinf(rad);

		if(m_flgCounter)
		{// �J�E���^�����炷

			// �J�E���^�ɉ����ăA���t�@�l��ϓ�
			SetColor(alpha, WHITE);

			// �J�E���^��0�ɋ߂Â���
			m_Count--;

			if(m_Count == 0)
			{
				m_flgCounter = false;
			}
		}
		else
		{// �J�E���^�𑝂₷

			// �J�E���^�ɉ����ăA���t�@�l��ϓ�
			SetColor(alpha, WHITE);

			// �J�E���^��0�ɋ߂Â���
			m_Count++;

			if(m_Count == m_SetCounter)
			{
				m_flgCounter = true;
			}
		}
	}
	else
	{// �_��

		if(m_Count != 0)
		{
			if(m_Count > 0)
			{// �J�E���^������

				// �J�E���^�ɉ����ăA���t�@�l��ϓ�
				SetColor((1.0f / (float)m_SetCounter * (float)(m_SetCounter - m_Count)), WHITE);

				// �J�E���^��0�ɋ߂Â���
				m_Count--;
			}
			else
			{// �J�E���^������

				// �J�E���^�ɉ����ăA���t�@�l��ϓ�
				SetColor((1.0f / fabsf((float)m_SetCounter) * fabsf((float)m_Count)), WHITE);

				// �J�E���^��0�ɋ߂Â���
				m_Count++;

				// �J�E���^��0�ɂȂ����ꍇ�A�A���t�@�l��0��
				if(m_Count == 0)
				{
					SetColor(0.0f, WHITE);
				}
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
void CDriftMark::Draw(void)
{
	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, (m_Curve ? m_pTexture[1] : m_pTexture[0]));
	// ���[�^�[�`��
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);
	// ���[�^�[�`��
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, PRIMITIVE_SQUARE);

	// �A���t�@�e�X�g�I��
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	/*
	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3	pos		-> �����ʒu
//			:D3DXVECTOR2	size	-> �|���S���̃T�C�Y
//			:char			*str	-> �e�N�X�`���̃t�@�C���p�X
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CDriftMark* CDriftMark::Create(void)
{
	CDriftMark *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CDriftMark;

	// ����������
	instance->Init();

	// �C���X�^���X�����^�[��
	return instance;
}

//=============================================================================
//	�֐���	:SetColor
//	����	:float	a		-> �A���t�@�l
//			:float	r		-> R�l
//			:float	g		-> G�l
//			:float	b		-> B�l
//	�߂�l	:����
//	����	:�|���S���F��ݒ肷��B
//=============================================================================
void CDriftMark::SetColor(float a, float r, float g, float b)
{
	VERTEX_2D	*pVtx;	// 2D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < 8 ; i++)
	{
		pVtx[i].col = D3DCOLOR_COLORVALUE(r, g, b, a);
	}

	m_pVtxBuff->Unlock();

}

//=============================================================================
//	�֐���	:VisibleDriftMark
//	����	:float	a		-> �A���t�@�l
//	�߂�l	:����
//	����	:�h���t�g�}�[�N��\������B
//=============================================================================
void CDriftMark::VisibleDriftMark(bool type, bool curve, int time)
{
	m_VisibleType = type;
	m_Curve = curve;
	m_Count = time;
	m_SetCounter = type ? (int)fabsf((float)time + 0.1f) : time;
}

//=============================================================================
//	�֐���	:InvisibleDriftMark
//	����	:float	a		-> �A���t�@�l
//	�߂�l	:����
//	����	:�h���t�g�}�[�N��\������B
//=============================================================================
void CDriftMark::InvisibleDriftMark(int time)
{
	// ��Βl���擾
	time = (int)fabsf((float)time + 0.01f);

	m_Count = -time;
	m_VisibleType = false;
}