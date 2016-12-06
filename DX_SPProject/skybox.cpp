//=============================================================================
//
//	�^�C�g��	���b�V���t�B�[���h
//	�t�@�C����	skybox.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/05/10
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "skybox.h"
#include "rendererDX.h"
#include "cameraDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CSkybox::m_pTexture;

//=============================================================================
//	�֐���	:CSkybox()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSkybox::CSkybox(int priority, OBJTYPE objtype) : CSceneDX(priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CSkybox()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSkybox::~CSkybox()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CSkybox::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;										// 3D���_���
	

	// �e�평��������
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer(((sizeof(VERTEX_3D) * SKYBOX_VERTEX_NUM)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
			SetSkyboxData(pVtx);
	}
	m_pVtxBuff->Unlock();

	Load();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSkybox::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSkybox::Update(void)
{
	// �J�������W�ɒǏ]
	m_Pos = CManager::GetCamera()->GetCameraPosV();

	m_Rot.y -= 0.0001f;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CSkybox::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
		
	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));		// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);										// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);									// �e�N�X�`���̐ݒ�
	for(int i = 0 ; i < SKYBOX_PRIMITIVE_NUM ; i++)
	{
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * VERTEX_NUM), PRIMITIVE_NUM);	// �`��
	}

	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�I��
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	�֐���:SetSkyboxData
//	����:VERTEX_3D *pVtx(���_�f�[�^)
//	�߂�l:����
//	����:���_�f�[�^���Z�b�g����B
//=============================================================================
void CSkybox::SetSkyboxData(VERTEX_3D *pVtx)
{
	// ���W�ݒ�
	// �V��
	pVtx[0].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[1].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[2].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[3].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	
	// ���
	pVtx[4].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[5].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[6].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[7].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	
	// �O��
	pVtx[8].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[9].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[10].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[11].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	
	// �w��
	pVtx[12].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[13].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[14].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[15].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	
	// ������
	pVtx[16].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[17].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[18].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[19].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	
	// �E����
	pVtx[20].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[21].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[22].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[23].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	
	for(int i = 0 ; i < SKYBOX_VERTEX_NUM ; i++)
	{
		// �@���ݒ�
		pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(0.2f, 0.2f, 0.4f, 1.0f);
	}
	
	// �e�N�X�`���\�t���W�ݒ�
	// �V��
	pVtx[0].tex		= D3DXVECTOR2((1 * 0.25f), (0 * (1.0f / 3.0f)));
	pVtx[1].tex		= D3DXVECTOR2((2 * 0.25f), (0 * (1.0f / 3.0f)));
	pVtx[2].tex		= D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[3].tex		= D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	// ���
	pVtx[4].tex		= D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[5].tex		= D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[6].tex		= D3DXVECTOR2((1 * 0.25f), (3 * (1.0f / 3.0f)));
	pVtx[7].tex		= D3DXVECTOR2((2 * 0.25f), (3 * (1.0f / 3.0f)));
	// �O��
	pVtx[8].tex		= D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[9].tex		= D3DXVECTOR2((4 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[10].tex	= D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[11].tex	= D3DXVECTOR2((4 * 0.25f), (2 * (1.0f / 3.0f)));
	// �w��
	pVtx[12].tex	= D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[13].tex	= D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[14].tex	= D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[15].tex	= D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	// ������
	pVtx[16].tex	= D3DXVECTOR2((0 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[17].tex	= D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[18].tex	= D3DXVECTOR2((0 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[19].tex	= D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	// �E����
	pVtx[20].tex	= D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[21].tex	= D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[22].tex	= D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[23].tex	= D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f)));
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���B
//=============================================================================
CSkybox *CSkybox::Create(D3DXVECTOR3 pos)
{
	CSkybox *skybox;

	skybox = new CSkybox;

	skybox->Init();

	return skybox;
}