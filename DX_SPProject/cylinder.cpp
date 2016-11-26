//=============================================================================
//
//	�^�C�g��	���b�V���t�B�[���h
//	�t�@�C����	cylinder.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/05/10
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "cylinder.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "sceneXDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CCylinder::m_Texture;

//=============================================================================
//	�֐���	:CCylinder()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CCylinder::CCylinder(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CCylinder()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CCylinder::~CCylinder()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CCylinder::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3D�f�o�C�X

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer((sizeof(VERTEX_3D) * CYLINDER_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// ���_���Z�b�g
	SetCylinderData();

	// �C���f�b�N�X�o�b�t�@�̊m��
	pDevice->CreateIndexBuffer((sizeof(WORD) * CYLINDER_INDEX_NUM), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	WORD *pIdx;

	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	{
		int buff = (CYLINDER_VERTEX_NUM / (CYLINDER_VERTICAL + 1));

		for(int nCntVertical = 0 ; nCntVertical < CYLINDER_VERTICAL ; nCntVertical++)
		{
			for(int nCntIdx = 0 ; nCntIdx < CYLINDER_INDEX_STANDARD ; nCntIdx++)
			{
				if((nCntVertical == (CYLINDER_VERTICAL - 1)) && (nCntIdx == (CYLINDER_INDEX_STANDARD - 2)))
				{// �����ł��؂�
					break;
				}

				if(nCntIdx == (CYLINDER_INDEX_STANDARD - 1))
				{
					buff += (CYLINDER_HORIZONTAL + 2);
					pIdx[nCntVertical * CYLINDER_INDEX_STANDARD + nCntIdx] = buff;
				}
				else if(nCntIdx == (CYLINDER_INDEX_STANDARD - 2))
				{
					pIdx[nCntVertical * CYLINDER_INDEX_STANDARD + nCntIdx] = buff;
				}
				else
				{
					pIdx[nCntVertical * CYLINDER_INDEX_STANDARD + nCntIdx] = buff;
				
					if(nCntIdx % 2 == 0)
					{
						buff -= (CYLINDER_HORIZONTAL + 1);
					}
					else
					{
						if(nCntIdx != (CYLINDER_INDEX_STANDARD - 3))
						{
							buff += (CYLINDER_HORIZONTAL + 2);
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
void CCylinder::Uninit(void)
{
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
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
void CCylinder::Update(void)
{
	VERTEX_3D *pVtx;	// ���_�o�b�t�@
	WORD *pIdx;			// �C���f�b�N�X�o�b�t�@


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �@���ݒ�
	for(int y = 0 ; y <= CYLINDER_VERTICAL ; y++)
	{
		for(int x = 0 ; x <= CYLINDER_HORIZONTAL ; x++)
		{
			if((y != 0) && (y != (CYLINDER_VERTICAL))
				&& (x != 0) && (x != (CYLINDER_HORIZONTAL)))
			{
				// �@���ݒ�
				D3DXVECTOR3 n0;
				if(1)
				{// �����Ƃ����@���ݒ�
					D3DXVECTOR3 n1, n2, n3, n4, n5, n6, v01, v02, v03, v04, v05, v06;
					v01 = pVtx[(y * (CYLINDER_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v02 = pVtx[((y - 1) * (CYLINDER_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v03 = pVtx[((y - 1) * (CYLINDER_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v04 = pVtx[((y) * (CYLINDER_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v05 = pVtx[((y + 1) * (CYLINDER_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v06 = pVtx[((y + 1) * (CYLINDER_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					D3DXVec3Cross(&n1, &v01, &v02);
					D3DXVec3Normalize(&n1, &n1);
					D3DXVec3Cross(&n2, &v02, &v03);
					D3DXVec3Normalize(&n2, &n2);
					D3DXVec3Cross(&n3, &v03, &v04);
					D3DXVec3Normalize(&n3, &n3);
					D3DXVec3Cross(&n4, &v04, &v05);
					D3DXVec3Normalize(&n4, &n4);
					D3DXVec3Cross(&n5, &v05, &v06);
					D3DXVec3Normalize(&n5, &n5);
					D3DXVec3Cross(&n6, &v06, &v01);
					D3DXVec3Normalize(&n6, &n6);
					n0 = (n1 + n2 + n3 + n4 + n5 + n6) / 6.0f;
					D3DXVec3Normalize(&n0, &n0);
				}
				else
				{// �y�`�Ȗ@���ݒ�
					D3DXVECTOR3 n1, n2;
					D3DXVECTOR3 v1, v2;
					v1 = pVtx[(y * (CYLINDER_HORIZONTAL + 1)) + (x + 1)].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1)) + (x - 1)].Pos;
					v2 = pVtx[((y - 1) * (CYLINDER_HORIZONTAL + 1)) + x].Pos - pVtx[((y + 1) * (CYLINDER_HORIZONTAL + 1)) + x].Pos;
					n1 = D3DXVECTOR3(-v1.y, v1.x, 0.0f);
					D3DXVec3Normalize(&n1, &n1);
					n2 = D3DXVECTOR3(0.0f, v2.z, -v2.y);
					D3DXVec3Normalize(&n2, &n2);
					n0 = n1 + n2;
					D3DXVec3Normalize(&n0, &n0);
				}
				pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Nor = n0;
			}
			else
			{
				pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
		}
	}
	
	m_pIdxBuff->Unlock();
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3D�f�o�C�X
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// �}�g���b�N�X

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

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);		// ���[���h�}�g���b�N�X�̐ݒ�
		
	// ���C�e�B���O�ݒ���I�t��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// �`�揈��
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetIndices(m_pIdxBuff);								// �C���f�b�N�X�o�b�t�@�̃o�C���h
	pDevice->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, m_Texture);								// �e�N�X�`���̐ݒ�
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CYLINDER_VERTEX_NUM, 0, CYLINDER_POLYGON_NUM);	// ���b�V���t�B�[���h�`��
		
	// ���C�e�B���O�ݒ���I����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	�֐���:SetCylinderData
//	����:VERTEX_3D *pVtx(���_�f�[�^)
//	�߂�l:����
//	����:���_�f�[�^���Z�b�g����B
//=============================================================================
void CCylinder::SetCylinderData(void)
{
	VERTEX_3D *pVtx;
	static float rot = 1.0f;
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= CYLINDER_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= CYLINDER_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[nCntVtxVertical * (CYLINDER_HORIZONTAL + 1) + nCntVtxHorizontal].Pos.x
				= sinf(D3DX_PI * 2.0f / CYLINDER_HORIZONTAL * nCntVtxHorizontal) * CYLINDER_RADIUS;
			
			pVtx[nCntVtxVertical * (CYLINDER_HORIZONTAL + 1) + nCntVtxHorizontal].Pos.y
				= ((CYLINDER_HEIGHT * 1.0f * CYLINDER_VERTICAL)) - (CYLINDER_HEIGHT * nCntVtxVertical);
			
			pVtx[nCntVtxVertical * (CYLINDER_HORIZONTAL + 1) + nCntVtxHorizontal].Pos.z
				= cosf(D3DX_PI * 2.0f / CYLINDER_HORIZONTAL * nCntVtxHorizontal) * CYLINDER_RADIUS;
		}
	}
	
	for(int nCntVertex = 0 ; nCntVertex < CYLINDER_VERTEX_NUM ; nCntVertex++)
	{
		// �@���ݒ�
		pVtx[nCntVertex].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// �F�ݒ�
		pVtx[nCntVertex].col = D3DCOLOR_COLORVALUE(0.8f, 0.8f, 0.8f, 0.8f);
	}
	
	// �e�N�X�`���\�t���W�ݒ�
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= CYLINDER_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= CYLINDER_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[nCntVtxVertical * (CYLINDER_HORIZONTAL + 1) + nCntVtxHorizontal].tex.x = (1.0f / 8.0f * nCntVtxHorizontal);
			pVtx[nCntVtxVertical * (CYLINDER_HORIZONTAL + 1) + nCntVtxHorizontal].tex.y = (1.0f / CYLINDER_VERTICAL * nCntVtxVertical);
		}
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���B
//=============================================================================
CCylinder *CCylinder::Create(D3DXVECTOR3 pos)
{
	CCylinder *cylinder;
	
	cylinder = new CCylinder;

	cylinder->Init();

	return cylinder;
}