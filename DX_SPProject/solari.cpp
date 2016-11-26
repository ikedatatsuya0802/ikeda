//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(3D)
//	�t�@�C����	scene3D.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "solari.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSolari::CSolari(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSolari::~CSolari()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CSolari::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;										// 3D���_���
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3D�f�o�C�X

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\"SOLARI_TEXFILENAME000, &m_Texture);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), (SOLARI_HEIGHT * 0.5f), 0.0f);
	pVtx[1].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), (SOLARI_HEIGHT * 0.5f), 0.0f);
	pVtx[2].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	pVtx[3].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	
	pVtx[4].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	pVtx[5].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	pVtx[6].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), -(SOLARI_HEIGHT * 0.5f), 0.0f);
	pVtx[7].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), -(SOLARI_HEIGHT * 0.5f), 0.0f);
	
	for(int nCntInit = 0 ; nCntInit < (VERTEX_NUM * 2) ; nCntInit++)
	{	
		// �@���ݒ�
		pVtx[nCntInit].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx[nCntInit].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// �e�N�X�`���\�t���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
	
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
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
void CSolari::Uninit(void)
{
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if(m_Texture != NULL)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSolari::Update(void)
{
	if(CInput::GKP(DIK_SPACE))
	{
		m_Rot.x += 0.02f;
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CSolari::Draw(void)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;	// �}�g���b�N�X
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();	// 3D�f�o�C�X
	CCameraDX	*camera = CManager::GetCamera();	// 3D�f�o�C�X

	
	// �ˉe�ϊ��s��
	/*camera->m_CS.mtxProjection	= D3DXMATRIX(
     2/SCREEN_WIDTH , 0.0f, 0.0f, 0.0f,
     0.0f, 2/SCREEN_HEIGHT , 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f
	);*/
	D3DXMATRIX proj;
	D3DXMATRIX view;

	// �v���W�F�N�V�����}�g���N�X�̏�����
	D3DXMatrixIdentity(&proj);

	// Ortho�ł̎ˉe�s��̍쐬
	D3DXMatrixOrthoLH(&proj, SCREEN_WIDTH, SCREEN_HEIGHT, CAMERA_NEARZ, CAMERA_FARZ);

	// �v���W�F�N�V�����}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	// �r���[�}�g���N�X�̏�����
	D3DXMatrixIdentity(&view);

	// �r���[�}�g���N�X�̍쐬
	D3DXMatrixLookAtLH(&view, &camera->m_CS.posV, &camera->m_CS.posR, &camera->m_CS.vecU);

	// �r���[�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &view);


	
	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
		
	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
		
	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// ���C�e�B���O�ݒ���I�t��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O�ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`�揈��
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_Texture);								// �e�N�X�`���̐ݒ�
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, PRIMITIVE_NUM);	// �`��

	// �J�����O�ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O�ݒ���I����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CSolari *CSolari::Create(D3DXVECTOR3 pos)
{
	CSolari *scene3D;	// �C���X�^���X

	// �C���X�^���X����
	scene3D = new CSolari;

	// ����������
	scene3D->Init(pos);

	// �C���X�^���X��Ԃ�
	return scene3D;
}