//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	pause.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "pause.h"
#include "main.h"
#include "game.h"
#include "player.h"
#include "fade.h"

//=============================================================================
//	�֐���	:CPause()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CPause::CPause(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CPause()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CPause::Init(void)
{

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[0], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[1], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[2], NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME000, &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME001, &m_pTexture[1]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME002, &m_pTexture[2]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME003, &m_pTexture[3]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME004, &m_pTexture[4]);

	m_Pos		= D3DXVECTOR3(PAUSE_POSX, PAUSE_POSY, 0.0f);
	m_fLength	= hypotf(PAUSE_WIDTH, PAUSE_HEIGHT) * 0.5f;
	m_fAngle	= atan2f(PAUSE_WIDTH, PAUSE_HEIGHT);
	SetVtxBuff(&m_pVtxBuff[0]);

	m_Pos = D3DXVECTOR3(PAUSE_MENU_POSX, PAUSE_MENU_POSY, 0.0f);
	m_fLength = hypotf(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT) * 0.5f;
	m_fAngle = atan2f(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	SetVtxBuff(&m_pVtxBuff[1]);

	CRendererDX::SetFullScreenVtx(&m_pVtxBuff[2]);
	VERTEX_2D	*pVtx;	// 3D���_���
	m_pVtxBuff[2]->Lock(0, 0, (void**)&pVtx, 0);
	{
		SIMPLE_FOR(4)
		{
			pVtx[i].col = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.7f);
		}
	}
	m_pVtxBuff[2]->Unlock();

	m_flgPause	= false;
	m_PauseMenu	= 0;
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CPause::SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff)
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

	// �e�N�X�`���\�t���W�ݒ�
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
void CPause::Uninit(void)
{
	// �C���X�^���X�폜
	SIMPLE_FOR(PAUSE_TEXTURE_NUM)
	{
		SafetyRelease(m_pTexture[i]);
	}
	SafetyRelease(m_pVtxBuff[0]);
	SafetyRelease(m_pVtxBuff[1]);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CPause::Update(void)
{
	VERTEX_2D	*pVtx;	// 3D���_���

	if(KT_P)
	{
		ChangePause();
	}

	if(m_flgPause)
	{
		// �|�[�Y���j���[����
		if(KT_W || KRP_W)
		{// �J�[�\�������

			m_PauseMenu--;
			if(m_PauseMenu < 0)
			{
				m_PauseMenu += (uint)PAUSE_MENU_MAX;
			}
		}
		else if(KT_S || KRP_S)
		{// �J�[�\��������

			m_PauseMenu++;
			m_PauseMenu = m_PauseMenu % (uint)PAUSE_MENU_MAX;
		}

		// �J�[�\���ʒu����
		m_pVtxBuff[1]->Lock(0, 0, (void**)&pVtx, 0);
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, (m_PauseMenu / (float)PAUSE_MENU_MAX));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (m_PauseMenu / (float)PAUSE_MENU_MAX));
			pVtx[2].tex = D3DXVECTOR2(0.0f, ((m_PauseMenu / (float)PAUSE_MENU_MAX) + (1.0f / (float)PAUSE_MENU_MAX)));
			pVtx[3].tex = D3DXVECTOR2(1.0f, ((m_PauseMenu / (float)PAUSE_MENU_MAX) + (1.0f / (float)PAUSE_MENU_MAX)));
		}
		m_pVtxBuff[1]->Unlock();

		// �|�[�Y���j���[����
		if(KT_ENTER)
		{
			switch(m_PauseMenu)
			{
			case PAUSE_MENU_CONTINUE:	// ������
				ChangePause();
				break;
			case PAUSE_MENU_RETRY:		// �ŏ�����
				ChangePause();
				CFade::Start(new CGame, FS_OUT);
				break;
			case PAUSE_MENU_QUIT:		// �I��
				ChangePause();
				CFade::Start(new CTitle, FS_OUT);
				break;
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
void CPause::Draw(void)
{
	if(m_flgPause)
	{
		// �A���t�@�e�X�g�J�n
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

		// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetFVF(FVF_VERTEX_2D);

		// ���_�o�b�t�@�̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[2], 0, sizeof(VERTEX_2D));
		// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetTexture(0, NULL);
		// �v���~�e�B�u�`��
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

		// ���_�o�b�t�@�̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[0], 0, sizeof(VERTEX_2D));
		// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		// �v���~�e�B�u�`��
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

		// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture[m_PauseMenu + 2]);
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
}

//=============================================================================
//	�֐���	:Create
//	����	:����
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CPause;

	// ����������
	instance->Init();

	// �C���X�^���X�����^�[��
	return instance;
}