//=============================================================================
//
//	�^�C�g��	���[���t�@�C��
//	�t�@�C����	rail.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "rail_title.h"
#include "rendererDX.h"
#include "meshfield.h"

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CRail_Title::Init(int line, D3DXVECTOR3 pos)
{	
	char		* str	= NULL;	// �t�@�C�����e�i�[�z��
	unsigned int	offset	= 0;	// ������w��q

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(RAIL_TEXFILENAME000), &m_pTexture);
	
	// ���[�����Z�b�g
	SetVtxBuff();

	Load();

	m_flgDraw = true;
}

//=============================================================================
//	�֐���	:SetRail_TitleData
//	����	:����
//	�߂�l	:����
//	����	:���[���̏����Z�b�g����B
//=============================================================================
void CRail_Title::SetVtxBuff(void)
{
	VERTEX_3D* pVtx;	// 3D���_���


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = -(RAIL_WIDTH * 0.5f);
	pVtx[0].Pos.y = 0.1f;
	pVtx[0].Pos.z = (MESHFIELD_TOTALHEIGHT * 0.5f);

	pVtx[1].Pos.x = (RAIL_WIDTH * 0.5f);
	pVtx[1].Pos.y = 0.1f;
	pVtx[1].Pos.z = (MESHFIELD_TOTALHEIGHT * 0.5f);

	pVtx[2].Pos.x = -(RAIL_WIDTH * 0.5f);
	pVtx[2].Pos.y = 0.1f;
	pVtx[2].Pos.z = -(MESHFIELD_TOTALHEIGHT * 0.5f);

	pVtx[3].Pos.x = (RAIL_WIDTH * 0.5f);
	pVtx[3].Pos.y = 0.1f;
	pVtx[3].Pos.z = -(MESHFIELD_TOTALHEIGHT * 0.5f);

	for(int nCntSet = 0 ; nCntSet < VERTEX_SQUARE ; nCntSet++)
	{
		// �@���ݒ�
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, MESHFIELD_TOTALHEIGHT * 0.02f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, MESHFIELD_TOTALHEIGHT * 0.02f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CRail_Title::Update(void)
{

}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CRail_Title::Draw(void)
{
	if(m_flgDraw)
	{
		// �}�g���b�N�X�ݒ�
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// ���C�e�B���O�ݒ���I�t��
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �`�揈��
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);	// �`��

		// ���C�e�B���O�ݒ���I����
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CRail_Title *CRail_Title::Create(int line, D3DXVECTOR3 pos)
{
	CRail_Title *instance;

	instance = new CRail_Title;

	instance->Init(line, pos);

	return instance;
}