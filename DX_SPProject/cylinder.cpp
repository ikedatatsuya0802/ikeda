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
#include "cameraDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CCylinder::m_pTexture;

//=============================================================================
//	�֐���	:CCylinder()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CCylinder::CCylinder(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
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
void CCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * CYLINDER_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// ���_���Z�b�g
	SetVtxBuff();

	// �C���f�b�N�X�o�b�t�@�̊m��
	D3D_DEVICE->CreateIndexBuffer((sizeof(WORD) * CYLINDER_INDEX_NUM), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

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

	Load();
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CCylinder::SetVtxBuff(void)
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
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CCylinder::Uninit(void)
{
	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
	SafetyRelease(m_pIdxBuff);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CCylinder::Update(void)
{
	// �J�������W�ɒǏ]
	m_Pos = CManager::GetCamera()->GetCameraPosV();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CCylinder::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
		
	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetIndices(m_pIdxBuff);								// �C���f�b�N�X�o�b�t�@�̃o�C���h
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
	D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CYLINDER_VERTEX_NUM, 0, CYLINDER_POLYGON_NUM);	// ���b�V���t�B�[���h�`��
		
	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���B
//=============================================================================
CCylinder *CCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CCylinder *cylinder;
	
	cylinder = new CCylinder;

	cylinder->Init(pos, rot);

	return cylinder;
}