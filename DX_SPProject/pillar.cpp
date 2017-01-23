//=============================================================================
//
//	�^�C�g��	���[���t�@�C��
//	�t�@�C����	pillar.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "pillar.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "game.h"
#include "railLine.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CPillar::m_pTexture;

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CPillar::CPillar(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CPillar::~CPillar()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CPillar::Init(D3DXVECTOR3 pos)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	// �X�v���C�����̎擾
	m_Spline = CGame::GetRailLine()->GetSpline();

	// ���_�o�b�t�@�ǉ�
	for each (auto itr in m_Spline->Pos)
	{
		AddVtxBuffer(m_pVtxBuff, (sizeof(VERTEX_3D) * PILLAR_VERTEX_NUM));
	}

	// ���_���Z�b�g
	SetVtxBuff();

	// �@�����Z�b�g
	//SetMeshNor();

	// �C���f�b�N�X�Z�b�g
	SetMeshIndex(&m_pIdxBuff, PILLAR_HORIZONTAL, PILLAR_VERTICAL);

	Load();
}

//=============================================================================
//	�֐���	:SetPillarData
//	����	:����
//	�߂�l	:����
//	����	:���[���̏����Z�b�g����B
//=============================================================================
void CPillar::SetVtxBuff(void)
{
	VERTEX_3D *pVtx = NULL;


	for(int i = 0 ; i < (int)m_Spline->Pos.size() ; i++)
	{
		m_pVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

		// ���W�ݒ�
		for(int y = 0 ; y <= PILLAR_VERTICAL ; y++)
		{
			for(int x = 0 ; x <= PILLAR_HORIZONTAL ; x++)
			{
				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos.x
					= cosf(D3DX_PI * 2.0f / PILLAR_HORIZONTAL * x) * PILLAR_RADIUS;

				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos.y
					= ((PILLAR_HEIGHT * 1.0f * PILLAR_VERTICAL)) - ((m_Spline->Pos[i].y - 10.0f) * y);

				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos.z
					= sinf(D3DX_PI * 2.0f / PILLAR_HORIZONTAL * x) * PILLAR_RADIUS;

				m_MeshPos[y * (PILLAR_HORIZONTAL + 1) + x] = pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos;
			}
		}

		for(int j = 0 ; j < PILLAR_VERTEX_NUM ; j++)
		{
			// �@���ݒ�
			pVtx[j].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// �F�ݒ�
			pVtx[j].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�ݒ�
		for(int y = 0 ; y <= PILLAR_VERTICAL ; y++)
		{
			for(int x = 0 ; x <= PILLAR_HORIZONTAL ; x++)
			{
				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].tex.x = (1.0f / 8.0f * x);
				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].tex.y = (1.0f / PILLAR_VERTICAL * y);
			}
		}

		m_pVtxBuff[i]->Unlock();
	}
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CPillar::Uninit(void)
{
	for(int i = 0 ; i < (int)m_pVtxBuff.size() ; i++)
	{
		SafetyRelease(m_pVtxBuff[i]);
	}
	SafetyRelease(m_pTexture);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CPillar::Update(void)
{
	static uint oldSize = m_pVtxBuff.size();

	if(oldSize < m_Spline->Pos.size())
	{
		oldSize = m_Spline->Pos.size();
		AddVtxBuffer(m_pVtxBuff, oldSize);
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CPillar::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, VEC3_ZERO, VEC3_ZERO);

	// Z�e�X�g�J�n
	CRendererDX::EnableZTest();

	// �`�揈��
	D3D_DEVICE->SetIndices(m_pIdxBuff);		// �C���f�b�N�X�o�b�t�@�̃o�C���h
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);		// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);	// �e�N�X�`���̐ݒ�

	for(int i = 0 ; i < (int)m_Spline->Pos.size() ; i++)
	{
		// �Œ჉�C���̍����ɂ͒����o���Ȃ�
		if(m_Spline->Pos[i].y > 2.0f)
		{
			// �ݒu�ʒu�E�T�C�Y�̐ݒ�
			CRendererDX::SetMatrix(&m_mtxWorld,
				D3DXVECTOR3(m_Spline->Pos[i].x, 0.0f, m_Spline->Pos[i].z),
				VEC3_ZERO,
				D3DXVECTOR3(1.0f, (m_Spline->Pos[i].y / PILLAR_HEIGHT - 0.1f), 1.0f));

			// ���_�o�b�t�@�̐ݒ�
			D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[i], 0, sizeof(VERTEX_3D));

			// �`��
			D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, PILLAR_VERTEX_NUM, 0, PILLAR_POLYGON_NUM);
		}
	}

	// Z�e�X�g�I��
	CRendererDX::DisableZTest();
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CPillar *CPillar::Create(D3DXVECTOR3 pos)
{
	CPillar *instance;

	instance = new CPillar;
	
	instance->Init(pos);

	return instance;
}

//=============================================================================
//	�֐���	:SetMeshNor
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�B
//=============================================================================
void CPillar::SetMeshNor(void)
{
	VERTEX_3D* pVtx = NULL;

	for(int i = 0 ; i < (int)m_pVtxBuff.size() ; i++)
	{
		m_pVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);
		memcpy(pVtx, m_pVtxBuff[i], sizeof(m_pVtxBuff[i]));

		for(int j = 0 ; j < PILLAR_VERTEX_NUM ; j++)
		{
			// �@���ݒ�
			D3DXVECTOR3 nor = VEC3_ZERO;

			if(j == 0)
			{// �ŏ��̓_

				D3DXVECTOR3 v1, v2;

				v1 = m_MeshPos[1] - m_MeshPos[0];
				v2 = m_MeshPos[PILLAR_HORIZONTAL] - m_MeshPos[0];

				D3DXVec3Cross(&nor, &v1, &v2);
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j < PILLAR_HORIZONTAL)
			{// ��[�̓_

				D3DXVECTOR3 n1, n2, v1, v2, v3;
				
				// ��_����̃x�N�g���Z�o
				v1 = m_MeshPos[j + 1] - m_MeshPos[j];
				v2 = m_MeshPos[j + PILLAR_HORIZONTAL] - m_MeshPos[j];
				v3 = m_MeshPos[j - 1] - m_MeshPos[j];

				// �O�ςɂ��2�̃x�N�g���ɂ�����@���Z�o
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// �ŏI�I�ȃx�N�g���Z�o
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j > (PILLAR_VERTEX_NUM - PILLAR_HORIZONTAL))
			{// ���[�̓_

				D3DXVECTOR3 n1, n2, v1, v2, v3;

				// ��_����̃x�N�g���Z�o
				v1 = m_MeshPos[j - 1] - m_MeshPos[j];
				v2 = m_MeshPos[j - PILLAR_HORIZONTAL] - m_MeshPos[j];
				v3 = m_MeshPos[j + 1] - m_MeshPos[j];

				// �O�ςɂ��2�̃x�N�g���ɂ�����@���Z�o
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// �ŏI�I�ȃx�N�g���Z�o
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j % PILLAR_HORIZONTAL == 0)
			{// ���[�̓_

				D3DXVECTOR3 n1, n2, v1, v2, v3;

				// ��_����̃x�N�g���Z�o
				v1 = m_MeshPos[j - PILLAR_HORIZONTAL] - m_MeshPos[j];
				v2 = m_MeshPos[j + 1] - m_MeshPos[j];
				v3 = m_MeshPos[j + PILLAR_HORIZONTAL] - m_MeshPos[j];

				// �O�ςɂ��2�̃x�N�g���ɂ�����@���Z�o
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// �ŏI�I�ȃx�N�g���Z�o
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j % PILLAR_HORIZONTAL == (PILLAR_HORIZONTAL - 1))
			{// ���[�̓_

				D3DXVECTOR3 n1, n2, v1, v2, v3;

				// ��_����̃x�N�g���Z�o
				v1 = m_MeshPos[j - PILLAR_HORIZONTAL] - m_MeshPos[j];
				v2 = m_MeshPos[j + 1] - m_MeshPos[j];
				v3 = m_MeshPos[j + PILLAR_HORIZONTAL] - m_MeshPos[j];

				// �O�ςɂ��2�̃x�N�g���ɂ�����@���Z�o
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// �ŏI�I�ȃx�N�g���Z�o
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else
			{// �ړ_��6�̓_

				D3DXVECTOR3 n1, n2, n3, n4, n5, n6, v1, v2, v3, v4, v5, v6;

				v1 = m_MeshPos[0] - m_MeshPos[0];
				v2 = m_MeshPos[0] - m_MeshPos[0];
				v3 = m_MeshPos[0] - m_MeshPos[0];
				v4 = m_MeshPos[0] - m_MeshPos[0];
				v5 = m_MeshPos[0] - m_MeshPos[0];
				v6 = m_MeshPos[0] - m_MeshPos[0];
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);
				D3DXVec3Cross(&n3, &v3, &v4);
				D3DXVec3Normalize(&n3, &n3);
				D3DXVec3Cross(&n4, &v4, &v5);
				D3DXVec3Normalize(&n4, &n4);
				D3DXVec3Cross(&n5, &v5, &v6);
				D3DXVec3Normalize(&n5, &n5);
				D3DXVec3Cross(&n6, &v6, &v1);
				D3DXVec3Normalize(&n6, &n6);
				nor = (n1 + n2 + n3 + n4 + n5 + n6) / 6.0f;
				D3DXVec3Normalize(&nor, &nor);
			}

			// �@�����Z�b�g
			pVtx[j].Nor = nor;


			m_pVtxBuff[i]->Unlock();
		}
	}
}

void CPillar::Vector3Round(D3DXVECTOR3 *vec)
{
	(*vec).x = (float)(int)(*vec).x;
	(*vec).y = (float)(int)(*vec).y;
	(*vec).z = (float)(int)(*vec).z;
}