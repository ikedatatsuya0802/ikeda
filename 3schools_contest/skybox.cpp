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
#include "manager.h"
#include "model.h"

//=============================================================================
//	�֐���	:CSkybox()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSkybox::CSkybox(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
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
	// �e�평��������
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_BoxColor.r = 0.1f;
	m_BoxColor.g = 0.1f;
	m_BoxColor.b = 0.1f;
	m_FadeColor.r = 0.0f;
	m_FadeColor.g = 0.0f;
	m_FadeColor.b = 0.0f;
	m_ChangeColorTime = 0;

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer(((sizeof(VERTEX_3D) * SKYBOX_VERTEX_NUM)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	SetVtxData();
}

//=============================================================================
//	�֐���:SetSkyboxData
//	����:VERTEX_3D *pVtx(���_�f�[�^)
//	�߂�l:����
//	����:���_�f�[�^���Z�b�g����B
//=============================================================================
void CSkybox::SetVtxData(void)
{
	VERTEX_3D	*pVtx;	// 3D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	// �V��
	pVtx[0].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[1].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[2].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[3].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));

	// ���
	pVtx[4].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[5].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[6].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[7].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));

	// �O��
	pVtx[8].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[9].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[10].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[11].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));

	// �w��
	pVtx[12].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[13].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[14].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[15].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));

	// ������
	pVtx[16].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[17].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[18].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[19].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));

	// �E����
	pVtx[20].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[21].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[22].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[23].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));

	for(int i = 0 ; i < SKYBOX_VERTEX_NUM ; i++)
	{
		// �@���ݒ�
		pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(m_BoxColor.r, m_BoxColor.g, m_BoxColor.b, 1.0f);
	}

	// �e�N�X�`���\�t���W�ݒ�
	// �V��
	pVtx[0].tex = D3DXVECTOR2((1 * 0.25f), (0 * (1.0f / 3.0f)));
	pVtx[1].tex = D3DXVECTOR2((2 * 0.25f), (0 * (1.0f / 3.0f)));
	pVtx[2].tex = D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[3].tex = D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	// ���
	pVtx[4].tex = D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[5].tex = D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[6].tex = D3DXVECTOR2((1 * 0.25f), (3 * (1.0f / 3.0f)));
	pVtx[7].tex = D3DXVECTOR2((2 * 0.25f), (3 * (1.0f / 3.0f)));
	// �O��
	pVtx[8].tex = D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[9].tex = D3DXVECTOR2((4 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[10].tex = D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[11].tex = D3DXVECTOR2((4 * 0.25f), (2 * (1.0f / 3.0f)));
	// �w��
	pVtx[12].tex = D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[13].tex = D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[14].tex = D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[15].tex = D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	// ������
	pVtx[16].tex = D3DXVECTOR2((0 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[17].tex = D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[18].tex = D3DXVECTOR2((0 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[19].tex = D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	// �E����
	pVtx[20].tex = D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[21].tex = D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[22].tex = D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[23].tex = D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f)));

	m_pVtxBuff->Unlock();
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
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSkybox::Update(void)
{
	int frame = CManager::GetFrame() % CManager::GetModel()->GetMorphCountAll();

	// �J�E���^���Z�b�g
	if(frame == 285)
	{
		ChangeColor(60, FCOLOR(255), FCOLOR(228), FCOLOR(225));
	}
	else if(frame == 615)
	{
		ChangeColor(60, FCOLOR(136), FCOLOR(206), FCOLOR(250));
	}
	else if(frame == 915)
	{
		ChangeColor(60, FCOLOR(255), FCOLOR(165), FCOLOR(0));
	}
	else if(frame == 1230)
	{
		ChangeColor(60, FCOLOR(120), FCOLOR(120), FCOLOR(120));
	}
	else if(frame == 1600)
	{
		ChangeColor(60, 0.1f, 0.1f, 0.1f);
	}

	// �J�E���^���ݒ肳��Ă���ꍇ
	if(m_ChangeColorTime > 0)
	{
		// ���݂̐F���Z�b�g
		SetColor(m_BoxColor.r, m_BoxColor.g, m_BoxColor.b);

		// �F��ς���
		m_BoxColor.r += m_FadeColor.r;
		m_BoxColor.g += m_FadeColor.g;
		m_BoxColor.b += m_FadeColor.b;

		// �J�E���^����
		m_ChangeColorTime--;
	}
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

	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));		// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);										// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, NULL);									// �e�N�X�`���̐ݒ�
	for(int i = 0 ; i < SKYBOX_PRIMITIVE_NUM ; i++)
	{
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * VERTEX_NUM), PRIMITIVE_NUM);	// �`��
	}

	// Z�e�X�g���@�X�V
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
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

//=============================================================================
//	�֐���	:SetColor
//	����	:float	a		-> �A���t�@�l
//			:float	r		-> R�l
//			:float	g		-> G�l
//			:float	b		-> B�l
//	�߂�l	:����
//	����	:�|���S���F��ݒ肷��B
//=============================================================================
void CSkybox::SetColor(float r, float g, float b)
{
	VERTEX_3D	*pVtx;	// 2D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < SKYBOX_VERTEX_NUM ; i++)
	{
		pVtx[i].col = D3DCOLOR_COLORVALUE(r, g, b, 1.0f);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:SetColor
//	����	:int	time	-> �t�F�[�h����
//			:float	r		-> R�l
//			:float	g		-> G�l
//			:float	b		-> B�l
//	�߂�l	:����
//	����	:���Ԃɂ��|���S���F��ς���B
//=============================================================================
void CSkybox::ChangeColor(int time, float r, float g, float b)
{
	m_ChangeColorTime = time;

	// 1�t���[�����ɕς���F��ݒ�
	m_FadeColor.r = (r - m_BoxColor.r) / time;
	m_FadeColor.g = (g - m_BoxColor.g) / time;
	m_FadeColor.b = (b - m_BoxColor.b) / time;
}