//=============================================================================
//
//	�^�C�g��	���b�V���t�B�[���h
//	�t�@�C����	stencilShadow.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/11/15
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "stencilShadow.h"
#include "manager.h"
#include "player.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CStencilShadow::m_Texture;

//=============================================================================
//	�֐���	:CStencilShadow()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CStencilShadow::CStencilShadow(bool isListAdd, int priority, OBJTYPE objtype) : CScene3DDX(isListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CStencilShadow()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CStencilShadow::~CStencilShadow()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CStencilShadow::Init(D3DXVECTOR3 pos)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * STENCILSHADOW_WALL_VERTEX_NUM),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffWall, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * STENCILSHADOW_COVER_VERTEX_NUM),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffCoverTop, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * STENCILSHADOW_COVER_VERTEX_NUM),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffCoverBottom, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffWindow, NULL);

	// ���_���Z�b�g
	SetVertexData(0.8f);


	// �C���f�b�N�X�o�b�t�@�̊m��
	D3D_DEVICE->CreateIndexBuffer((sizeof(WORD) * STENCILSHADOW_INDEX_NUM),
		D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	SetIndex();
}

//=============================================================================
//	�֐���	:SetVertexData
//	����	:����
//	�߂�l	:����
//	����	:���_�f�[�^���Z�b�g����B
//=============================================================================
void CStencilShadow::SetVertexData(float alpha)
{
	VERTEX_2D *pVtx2d;
	VERTEX_3D *pVtx3d;

	m_pVtxBuffWall->Lock(0, 0, (void**)&pVtx3d, 0);

	// ���W�ݒ�
	for(int y = 0 ; y < 2 ; y++)
	{
		for(int x = 0 ; x <= STENCILSHADOW_HORIZONTAL ; x++)
		{
			pVtx3d[y * (STENCILSHADOW_HORIZONTAL + 1) + x].Pos.x
				= sinf(D3DX_PI * 2.0f / STENCILSHADOW_HORIZONTAL * x) * STENCILSHADOW_RADIUS;
			pVtx3d[y * (STENCILSHADOW_HORIZONTAL + 1) + x].Pos.y
				= (STENCILSHADOW_HEIGHT * 0.5f) - (STENCILSHADOW_HEIGHT * y);
			pVtx3d[y * (STENCILSHADOW_HORIZONTAL + 1) + x].Pos.z
				= -cosf(D3DX_PI * 2.0f / STENCILSHADOW_HORIZONTAL * x) * STENCILSHADOW_RADIUS;
		}
	}

	for(int i = 0 ; i < STENCILSHADOW_WALL_VERTEX_NUM ; i++)
	{
		// �@���ݒ�
		pVtx3d[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx3d[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx3d[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	m_pVtxBuffWall->Unlock();

	pVtx3d = NULL;

	m_pVtxBuffCoverTop->Lock(0, 0, (void**)&pVtx3d, 0);

	// ���W�ݒ�
	pVtx3d[0].Pos
		= D3DXVECTOR3(0.0f, (STENCILSHADOW_HEIGHT * 0.5f), 0.0f);
	for(int x = 0 ; x <= STENCILSHADOW_HORIZONTAL ; x++)
	{
		pVtx3d[1 + x].Pos.x
			= sinf(D3DX_PI * 2.0f / STENCILSHADOW_HORIZONTAL * x) * STENCILSHADOW_RADIUS;
		pVtx3d[1 + x].Pos.y
			= (STENCILSHADOW_HEIGHT * 0.5f);
		pVtx3d[1 + x].Pos.z
			= cosf(D3DX_PI * 2.0f / STENCILSHADOW_HORIZONTAL * x) * STENCILSHADOW_RADIUS;
	}

	for(int i = 0 ; i < STENCILSHADOW_COVER_VERTEX_NUM ; i++)
	{
		// �@���ݒ�
		pVtx3d[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx3d[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx3d[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	m_pVtxBuffCoverTop->Unlock();

	pVtx3d = NULL;

	m_pVtxBuffCoverBottom->Lock(0, 0, (void**)&pVtx3d, 0);

	// ���W�ݒ�
	pVtx3d[0].Pos
		= D3DXVECTOR3(0.0f, -(STENCILSHADOW_HEIGHT * 0.5f), 0.0f);
	for(int x = 0 ; x <= STENCILSHADOW_HORIZONTAL ; x++)
	{
		pVtx3d[1 + x].Pos.x
			= sinf(D3DX_PI * 2.0f / STENCILSHADOW_HORIZONTAL * x) * STENCILSHADOW_RADIUS;
		pVtx3d[1 + x].Pos.y
			= -(STENCILSHADOW_HEIGHT * 0.5f);
		pVtx3d[1 + x].Pos.z
			= -cosf(D3DX_PI * 2.0f / STENCILSHADOW_HORIZONTAL * x) * STENCILSHADOW_RADIUS;
	}

	for(int i = 0 ; i < STENCILSHADOW_COVER_VERTEX_NUM ; i++)
	{
		// �@���ݒ�
		pVtx3d[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx3d[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx3d[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	m_pVtxBuffCoverBottom->Unlock();

	m_pVtxBuffWindow->Lock(0, 0, (void**)&pVtx2d, 0);

	// ���W�ݒ�
	pVtx2d[0].Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx2d[1].Pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx2d[2].Pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx2d[3].Pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	for(int i = 0 ; i < VERTEX_NUM ; i++)
	{
		// ���Z�W���ݒ�
		pVtx2d[i].rhw = 1.0f;

		// �F�ݒ�
		pVtx2d[i].col = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, alpha);

		// �e�N�X�`�����W
		pVtx2d[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	m_pVtxBuffWindow->Unlock();
}

//=============================================================================
//	�֐���	:SetIndex
//	����	:����
//	�߂�l	:����
//	����	:�C���f�b�N�X���Z�b�g����B
//=============================================================================
void CStencilShadow::SetIndex(void)
{
	WORD *pIdx;

	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	{
		int buff = (STENCILSHADOW_WALL_VERTEX_NUM / 2);

		for(int nCntVertical = 0; nCntVertical < 1; nCntVertical++)
		{
			for(int nCntIdx = 0; nCntIdx < STENCILSHADOW_INDEX_STANDARD; nCntIdx++)
			{
				if((nCntVertical == 0) && (nCntIdx == (STENCILSHADOW_INDEX_STANDARD - 2)))
				{// �����ł��؂�
					break;
				}

				if(nCntIdx == (STENCILSHADOW_INDEX_STANDARD - 1))
				{
					buff += (STENCILSHADOW_HORIZONTAL + 2);
					pIdx[nCntVertical * STENCILSHADOW_INDEX_STANDARD + nCntIdx] = buff;
				}
				else if(nCntIdx == (STENCILSHADOW_INDEX_STANDARD - 2))
				{
					pIdx[nCntVertical * STENCILSHADOW_INDEX_STANDARD + nCntIdx] = buff;
				}
				else
				{
					pIdx[nCntVertical * STENCILSHADOW_INDEX_STANDARD + nCntIdx] = buff;

					if(nCntIdx % 2 == 0)
					{
						buff -= (STENCILSHADOW_HORIZONTAL + 1);
					}
					else
					{
						if(nCntIdx != (STENCILSHADOW_INDEX_STANDARD - 3))
						{
							buff += (STENCILSHADOW_HORIZONTAL + 2);
						}
					}
				}
			}
		}
	}
	m_pIdxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CStencilShadow::Uninit(void)
{
	if(m_pVtxBuffWall != NULL)
	{
		m_pVtxBuffWall->Release();
		m_pVtxBuffWall = NULL;
	}
	if(m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CStencilShadow::Update(void)
{
	CPlayer *player = CManager::GetPlayer();

	m_Pos.x = player->GetPos().x;
	m_Pos.z = player->GetPos().z;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CStencilShadow::Draw(void)
{
	// ���[���h�}�g���b�N�X�̐ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, NULL);
	
	// �X�e���V���ݒ�
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
	D3D_DEVICE->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_STENCILREF, 1);

	D3D_DEVICE->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	// �~���`��
	DrawPillar();
	
	D3D_DEVICE->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	
	// �~���`��
	DrawPillar();

	// �S��ʃ|���S���`��ݒ�
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	D3D_DEVICE->SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// �S��ʃ|���S���`��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuffWindow, 0, sizeof(VERTEX_2D));
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);

	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	�֐���	:DrawPillar
//	����	:����
//	�߂�l	:����
//	����	:�~����`�悷��B
//=============================================================================
void CStencilShadow::DrawPillar(void)
{
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuffWall, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetIndices(m_pIdxBuff);	// �C���f�b�N�X�o�b�t�@�̃o�C���h
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);		// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, STENCILSHADOW_WALL_VERTEX_NUM, 0, STENCILSHADOW_POLYGON_NUM);

	D3D_DEVICE->SetStreamSource(0, m_pVtxBuffCoverTop, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);											// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, STENCILSHADOW_HORIZONTAL);

	D3D_DEVICE->SetStreamSource(0, m_pVtxBuffCoverBottom, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);												// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, STENCILSHADOW_HORIZONTAL);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���B
//=============================================================================
CStencilShadow *CStencilShadow::Create(D3DXVECTOR3 pos)
{
	CStencilShadow *stencilShadow;

	stencilShadow = new CStencilShadow;

	stencilShadow->Init();

	return stencilShadow;
}