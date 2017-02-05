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
#include "scene3DDX.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "meshfield.h"

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CScene3DDX::CScene3DDX(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneDX(ifListAdd, priority, objtype)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CScene3DDX::~CScene3DDX()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CScene3DDX::Init(cchar *str, cVec3 pos, cVec3 rot)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_�o�b�t�@�ݒ�
	SetVtxBuff();

	// �e�N�X�`���ǂݍ���
	Load(str);
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CScene3DDX::SetVtxBuff(void)
{
	VERTEX_3D	*pVtx;	// 3D���_���


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = -(POLYGON3DDX_WIDTH * 0.5f);
	pVtx[0].Pos.y = (POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[0].Pos.z = 0.0f;

	pVtx[1].Pos.x = (POLYGON3DDX_WIDTH * 0.5f);
	pVtx[1].Pos.y = (POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[1].Pos.z = 0.0f;

	pVtx[2].Pos.x = -(POLYGON3DDX_WIDTH * 0.5f);
	pVtx[2].Pos.y = -(POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[2].Pos.z = 0.0f;

	pVtx[3].Pos.x = (POLYGON3DDX_WIDTH * 0.5f);
	pVtx[3].Pos.y = -(POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[3].Pos.z = 0.0f;

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
void CScene3DDX::Uninit(void)
{
	Unload();

	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CScene3DDX::Update(void)
{

}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CScene3DDX::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// �A���t�@�EZ�e�X�g�J�n
	CRendererDX::EnableAlphaTest();
	CRendererDX::EnableZTest();
	
	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);

	// �`��
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);

	// �A���t�@�EZ�e�X�g�I��
	CRendererDX::DisableAlphaTest();
	CRendererDX::DisableZTest();
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CScene3DDX *CScene3DDX::Create(cchar *str, cVec3 pos, cVec3 rot)
{
	CScene3DDX *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CScene3DDX();

	// ����������
	instance->Init(str, pos, rot);

	// �C���X�^���X�����^�[��
	return instance;
}

//=============================================================================
//	�֐���	:SetMeshIndex
//	����	:LPDIRECT3DINDEXBUFFER9 *idx	->	�C���f�b�N�X
//			:int x							->	���b�V���̉��u���b�N��
//			:int y							->	���b�V���̏c�u���b�N��
//	�߂�l	:����
//	����	:���b�V���̃C���f�b�N�X���Z�b�g����B
//=============================================================================
void CScene3DDX::SetMeshIndex(LPDIRECT3DINDEXBUFFER9 *idxBuff, cint horizontal, cint vertical)
{
	WORD	*pIdx;
	int		idxNum		= ((horizontal + 1) * 2) * vertical + ((vertical - 1) * 2);
	int		idxStd		= ((horizontal + 1) * 2 + 2);
	int		idxVtxNum	= (horizontal + 1) * (vertical + 1);

	// �C���f�b�N�X�o�b�t�@�̊m��
	D3D_DEVICE->CreateIndexBuffer((sizeof(WORD) * idxNum), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, idxBuff, NULL);


	(*idxBuff)->Lock(0, 0, (void**)&pIdx, 0);
	{
		int buff = (idxVtxNum / (vertical + 1));

		for(int nCntVertical = 0 ; nCntVertical < vertical ; nCntVertical++)
		{
			for(int nCntIdx = 0 ; nCntIdx < idxStd ; nCntIdx++)
			{
				if((nCntVertical == (vertical - 1)) && (nCntIdx == (idxStd - 2)))
				{// �����ł��؂�
					break;
				}

				if(nCntIdx == (idxStd - 1))
				{
					buff += (horizontal + 2);
					pIdx[nCntVertical * idxStd + nCntIdx] = buff;
				}
				else if(nCntIdx == (idxStd - 2))
				{
					pIdx[nCntVertical * idxStd + nCntIdx] = buff;
				}
				else
				{
					pIdx[nCntVertical * idxStd + nCntIdx] = buff;

					if(nCntIdx % 2 == 0)
					{
						buff -= (horizontal + 1);
					}
					else
					{
						if(nCntIdx != (idxStd - 3))
						{
							buff += (horizontal + 2);
						}
					}
				}
			}
		}
	}
	(*idxBuff)->Unlock();
}