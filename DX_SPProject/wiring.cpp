//=============================================================================
//
//	�^�C�g��	����
//	�t�@�C����	wiring.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "wiring.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "game.h"
#include "railLine.h"
#include "player.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CWiring::CWiring(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneXDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CWiring::~CWiring()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CWiring::Init(D3DXVECTOR3 pos)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	// �X�v���C�����̎擾
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 3D���f���ǂݍ���
	m_ModelStatus	= new MODELSTATUS;
	//m_pTexture		= new TEXTURE;
	LoadModel(".\\data\\MODEL\\wiring.x", m_ModelStatus);
}

//=============================================================================
//	�֐���	:SetWiringData
//	����	:����
//	�߂�l	:����
//	����	:���[���̏����Z�b�g����B
//=============================================================================
void CWiring::SetVtxBuff(void)
{

}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CWiring::Uninit(void)
{
	CSceneXDX::Uninit();
	
	if(m_ModelStatus)
	{
		delete m_ModelStatus;
		m_ModelStatus = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CWiring::Update(void)
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
void CWiring::Draw(void)
{
	D3DXMATERIAL		*pMat = NULL;		// �}�e���A��
	D3DMATERIAL9		matDef;				// �f�t�H���g�̃}�e���A��

	if(!CManager::GetEdhitMode())
	{
		// �A���t�@�EZ�e�X�g�J�n
		CRendererDX::EnableAlphaTest();
		CRendererDX::EnableZTest();

		// ���݂̃}�e���A�����擾
		D3D_DEVICE->GetMaterial(&matDef);

		for(int i = 2 ; i < (int)m_Spline->PosHermite.size() ; i += 3)
		{
			float rot = 0.0f;

			// �ː����̉�]�l�v�Z
			if((i > 0) && (i != (int)m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[i + 1].x - m_Spline->PosHermite[i - 1].x),
					(m_Spline->PosHermite[i + 1].z - m_Spline->PosHermite[i - 1].z));
			}
			else
			{
				rot = 0.0f;
			}

			// �}�g���b�N�X�ݒ�
			CRendererDX::SetMatrix(&m_mtxWorld,
				D3DXVECTOR3(m_Spline->PosHermite[i].x, m_Spline->PosHermite[i].y, m_Spline->PosHermite[i].z),
				D3DXVECTOR3(0.0f, rot, 0.0f));

			// �}�e���A���ϊ�
			pMat = (D3DXMATERIAL *)m_ModelStatus->pBuffMat->GetBufferPointer();

			// 3D���f���`��
			for(int mat = 0 ; mat < (int)m_ModelStatus->NumMat ; mat++)
			{
				float			playerPos = CGame::GetPlayer1()->GetPerSpline();	// �v���C���̃X�v���C�����W
				D3DXMATERIAL	pMatAlpha = *pMat;									// �����}�e���A��


				// �v���C���[�ɋ߂Â����瓧�߂�����
				float length = (1.0f / m_Spline->PosHermite.size() * i) - playerPos;

				if(length < WIRING_CLEAR_BORDER)
				{
					(&pMatAlpha)[mat].MatD3D.Diffuse.a = (1.0f + (length - WIRING_CLEAR_BORDER) / 0.005f);

					if((&pMatAlpha)[mat].MatD3D.Diffuse.a < 0.0f)
					{
						(&pMatAlpha)[mat].MatD3D.Diffuse.a = 0.0f;
					}

					// �}�e���A���Z�b�g
					D3D_DEVICE->SetMaterial(&(&pMatAlpha)[mat].MatD3D);
				}
				else
				{
					// �}�e���A���Z�b�g
					D3D_DEVICE->SetMaterial(&pMat[mat].MatD3D);
				}

				// �e�N�X�`�����Z�b�g���Ȃ�
				D3D_DEVICE->SetTexture(0, NULL);

				// ���f���`��
				m_ModelStatus->pMesh->DrawSubset(mat);
			}

			// �}�e���A�������ɖ߂�
			D3D_DEVICE->SetMaterial(&matDef);
		}

		// �A���t�@�EZ�e�X�g�I��
		CRendererDX::DisableAlphaTest();
		CRendererDX::DisableZTest();
	}
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CWiring *CWiring::Create(D3DXVECTOR3 pos)
{
	CWiring *instance;

	instance = new CWiring;
	
	instance->Init(pos);

	return instance;
}