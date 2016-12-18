//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(�r���{�[�h)
//	�t�@�C����	sceneBillboard.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "sceneBillboardDX.h"
#include "main.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CSceneBillboardDX::m_pTexture;

//=============================================================================
//	�֐���	:CSceneBillboard()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSceneBillboardDX::CSceneBillboardDX(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{
	
}

//=============================================================================
//	�֐���	:~CSceneBillboard()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSceneBillboardDX::~CSceneBillboardDX()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CSceneBillboardDX::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;									// 3D���_���
	

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGONBILLBOARDDX_TEXFILENAME000, &m_pTexture);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = -(POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[0].Pos.y = (POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[0].Pos.z = 0.0f;
	
	pVtx[1].Pos.x = (POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[1].Pos.y = (POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[1].Pos.z = 0.0f;
	
	pVtx[2].Pos.x = -(POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[2].Pos.y = -(POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[2].Pos.z = 0.0f;
	
	pVtx[3].Pos.x = (POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[3].Pos.y = -(POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[3].Pos.z = 0.0f;
	
	for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
	{
		// �@���ݒ�
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		// �F�ݒ�
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
	// �e�N�X�`���\�t���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSceneBillboardDX::Uninit(void)
{
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSceneBillboardDX::Update(void)
{

#ifdef _DEBUG
	//CDebugProc::DebugProc("�Q�ƒ��_:%d(%5.2f:%5.2f:%5.2f)\n", num, vec.x, vec.y, vec.z);
#endif
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CSceneBillboardDX::Draw(void)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;				// �}�g���b�N�X
	
	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// �r���{�[�h�ݒ�
	D3D_DEVICE->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

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
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���Z�����̐ݒ�
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// �r���{�[�h�`��

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	// �����_�[�X�e�[�g�ݒ��߂�
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CSceneBillboardDX *CSceneBillboardDX::Create(D3DXVECTOR3 pos)
{
	CSceneBillboardDX *sceneBillboard;

	sceneBillboard = new CSceneBillboardDX;

	sceneBillboard->Init(pos);

	return sceneBillboard;
}