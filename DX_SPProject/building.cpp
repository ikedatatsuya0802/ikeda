//=============================================================================
//
//	�^�C�g��	����
//	�t�@�C����	building.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "building.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "railLine.h"
#include "game.h"
#include "player.h"
#include "cameraDX.h"
#include "goal.h"
#include <process.h>

bool				CBuilding::m_ifInitialize = false;
SPLINE*				CBuilding::m_Spline;
D3DXMATRIX			CBuilding::m_mtxWorld;
MODELSTATUS			CBuilding::m_BuildingMesh[BUILDING_TYPE_NUM];
D3DXMATERIAL*		CBuilding::m_BuildingMat[BUILDING_TYPE_NUM];
BUILDING			CBuilding::m_Building[BUILDING_ALL];
uint				CBuilding::m_thID;
HANDLE				CBuilding::m_hTh;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CBuilding::Init(void)
{
	m_ifInitialize = false;

	// �X�v���C�����̎擾
	m_Spline = CGame::GetRailLine()->GetSpline();

	// �����̃��b�V���������[�h
	for(int i = 0 ; i < BUILDING_TYPE_NUM ; i++)
	{
		string modelFileName = ".\\data\\MODEL\\building\\building";
		stringstream ss;
		MODELSTATUS	mesh;

		// �t�@�C�����ҏW
		ss << (i / 3);
		modelFileName.append(ss.str());
		ss.str("");
		ss.clear(stringstream::goodbit);
		ss << (i % 3);
		modelFileName.append("0");
		modelFileName.append(ss.str());
		modelFileName.append(".x");

		CSceneXDX::LoadModel(modelFileName.c_str(), &m_BuildingMesh[i]);
		m_BuildingMat[i] = (D3DXMATERIAL *)m_BuildingMesh[i].pBuffMat->GetBufferPointer();
	}

	// �������쐬
	for(int z = 0 ; z < BUILDING_INSIDE ; z++)
	{
		for(int x = 0 ; x < BUILDING_HORIZONTAL ; x++)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_int_distribution<int> type(0, (BUILDING_TYPE_NUM - 1));

			int typeNum = type(mt);

			// ��������r���̎�ނ������_���ɐݒ�
			m_Building[z * BUILDING_HORIZONTAL + x].Type	= typeNum;
			m_Building[z * BUILDING_HORIZONTAL + x].Visible	= true;
			m_Building[z * BUILDING_HORIZONTAL + x].Pos.x	= ((x - (BUILDING_HORIZONTAL / 2)) * BUILDING_MARGIN);
			m_Building[z * BUILDING_HORIZONTAL + x].Pos.y	= 0.0f;
			m_Building[z * BUILDING_HORIZONTAL + x].Pos.z	= ((z - (BUILDING_INSIDE / 2)) * BUILDING_MARGIN);
		}
	}

	// ���������H�ɋߐڂ��Ă���ꍇ��\���ɂ���
	for(uint i = 0 ; i < BUILDING_ALL ; i++)
	{
		for(uint j = 0 ; j < m_Spline->PosHermite.size() ; j++)
		{
			if(D3DXVec3Length(&(m_Building[i].Pos - m_Spline->PosHermite[j])) < BUILDING_CLEAR_LENGTH)
			{
				if(m_Building[i].Visible)
				{
					m_Building[i].Visible = false;
				}
			}
		}
	}

	// ������������
	m_ifInitialize = true;

	// ���X���b�h�N����
	//m_hTh = (HANDLE)_beginthreadex(NULL, 0, UpdateS, NULL, 0, &m_thID);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CBuilding::RecalcTexCoord(LPD3DXMESH* mesh)
{
	
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CBuilding::Uninit(void)
{
	// �X���b�h�I��
	CloseHandle(m_hTh);

	for(int i = 0 ; i < BUILDING_TYPE_NUM ; i++)
	{
		SafetyRelease(m_BuildingMesh[i].pBuffMat);
		SafetyRelease(m_BuildingMesh[i].pMesh);
	}

	// ���������K�v
	m_ifInitialize = false;
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CBuilding::Update(void)
{
	GOALPOS goalPos = CGame::GetGoal()->GetGoalPos();

	if(m_ifInitialize)
	{
		static SPLINE oldSpline = *CGame::GetRailLine()->GetSpline();
		D3DXVECTOR3 playerPos = CGame::GetPlayer1()->GetPos();
		D3DXVECTOR3 moveDir = CGame::GetPlayer1()->GetMoveDir();


		// �ʒu�X�V
		bool flug = false;
#pragma omp parallel for
		for(int x = 0 ; x < BUILDING_HORIZONTAL ; x++)
		{
			if(moveDir.x > 0.0f)
			{
				if(playerPos.x > (m_Building[x].Pos.x + BUILDING_MOVE_STRIDE))
				{
					for(int z = 0 ; z < BUILDING_INSIDE ; z++)
					{
						m_Building[z * BUILDING_INSIDE + x].Pos.x += BUILDING_STRIDE_X;
					}
					flug = true;
				}
			}
			else if(moveDir.x < 0.0f)
			{
				if(playerPos.x < (m_Building[x].Pos.x - BUILDING_MOVE_STRIDE))
				{
					for(int z = 0 ; z < BUILDING_INSIDE ; z++)
					{
						m_Building[z * BUILDING_INSIDE + x].Pos.x -= BUILDING_STRIDE_X;
					}
					flug = true;
				}
			}
		}

#pragma omp parallel for
		for(int z = 0 ; z < BUILDING_INSIDE ; z++)
		{
			if(moveDir.z > 0.0f)
			{
				if(playerPos.z > (m_Building[z * BUILDING_INSIDE].Pos.z + BUILDING_MOVE_STRIDE))
				{
					for(int x = 0 ; x < BUILDING_HORIZONTAL ; x++)
					{
						m_Building[z * BUILDING_INSIDE + x].Pos.z += BUILDING_STRIDE_Z;
					}
					flug = true;
				}
			}
			else if(moveDir.z < 0.0f)
			{
				if(playerPos.z < (m_Building[z * BUILDING_INSIDE].Pos.z - BUILDING_MOVE_STRIDE))
				{
					for(int x = 0 ; x < BUILDING_HORIZONTAL ; x++)
					{
						m_Building[z * BUILDING_INSIDE + x].Pos.z -= BUILDING_STRIDE_Z;
					}
					flug = true;
				}
			}
		}

		// �X�v���C����񂪍X�V���ꂽ�ꍇ
		if(flug || (oldSpline.Length - m_Spline->Length > 1.0f))
		{
			// ���������H�ɋߐڂ��Ă���ꍇ��\���ɂ���
#pragma omp parallel for
			for(int i = 0 ; i < BUILDING_ALL ; i++)
			{
				bool ifNearBuilding = false;

				// �����̐��H�ߐڔ���
				for each(D3DXVECTOR3 splPos in m_Spline->PosHermite)
				{
					if(D3DXVec3Length(&(m_Building[i].Pos - splPos)) < BUILDING_CLEAR_LENGTH)
					{
						if(m_Building[i].Visible)
						{
							m_Building[i].Visible = false;
						}
						ifNearBuilding = true;
					}
				}

				// �S�[���̐��H�ߐڔ���
				for(int j = 0 ; j < 100 ; j++)
				{
					D3DXVECTOR3 nowPos = goalPos.nearPos + (goalPos.farPos - goalPos.nearPos) * (j / 100.0f);

					if(D3DXVec3Length(&(m_Building[i].Pos - nowPos)) < BUILDING_CLEAR_LENGTH)
					{
						if(m_Building[i].Visible)
						{
							m_Building[i].Visible = false;
						}
						ifNearBuilding = true;
					}
				}

				if(!ifNearBuilding)
				{
					if(!m_Building[i].Visible)
					{
						m_Building[i].Visible = true;
					}
				}
			}
		}

		// �X�v���C�����ۑ�
		oldSpline = (*m_Spline);
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CBuilding::Draw(void)
{
	D3DXMATERIAL	*pMat = NULL;		// �}�e���A��
	D3DMATERIAL9	matDef;				// �f�t�H���g�̃}�e���A��


	if(!CManager::GetEdhitMode() && m_ifInitialize)
	{
		// Z�e�X�g�J�n
		CRendererDX::EnableZTest();

		// �A���t�@�e�X�g�J�n
		CRendererDX::EnableAlphaTest();

		// ���C�e�B���O�ݒ���I�t��
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
		
		// ���݂̃}�e���A�����擾
		D3D_DEVICE->GetMaterial(&matDef);

		for(int i = 0 ; i < BUILDING_ALL ; i++)
		{
			if(m_Building[i].Visible)
			{
				// �}�g���b�N�X�ݒ�
				CRendererDX::SetMatrix(&m_mtxWorld, m_Building[i].Pos, VEC3_ZERO);

				pMat = m_BuildingMat[m_Building[i].Type];

				// 3D���f���`��
				for(int j = 0 ; j < (int)m_BuildingMesh[m_Building[i].Type].NumMat ; j++)
				{
					// �}�e���A���Z�b�g
					D3D_DEVICE->SetMaterial(&pMat[j].MatD3D);

					// �e�N�X�`���ǂݍ���
					if(pMat[j].pTextureFilename)
					{// �e�N�X�`���L��

						// ���X�g���瓯���̃e�N�X�`����T�����A�Z�b�g
						for each(TEXTURE list in m_BuildingMesh[m_Building[i].Type].Tex)
						{
							if(list.FileName == CharPToString(pMat[j].pTextureFilename))
							{
								D3D_DEVICE->SetTexture(0, list.pTexture);
							}
						}
					}
					else
					{// �e�N�X�`������

						// �e�N�X�`�����Z�b�g���Ȃ�
						D3D_DEVICE->SetTexture(0, NULL);
					}

					// ���f���`��
					m_BuildingMesh[m_Building[i].Type].pMesh->DrawSubset(j);
				}
			}
		}

		// �}�e���A�������ɖ߂�
		D3D_DEVICE->SetMaterial(&matDef);

		// �A���t�@�e�X�g�I��
		CRendererDX::DisableAlphaTest();

		// Z�e�X�g�I��
		CRendererDX::DisableZTest();

		// ���C�e�B���O�ݒ���I����
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
uint __stdcall CBuilding::UpdateS(void*)
{
	DWORD	dwFrameCount	= 0;
	DWORD	dwCurrentTime	= 0;
	DWORD	dwExecLastTime	= 0;
	DWORD	dwFPSLastTime	= 0;

	dwFrameCount = dwCurrentTime = 0;
	dwExecLastTime = dwFPSLastTime = timeGetTime();

	while(1)
	{
		dwCurrentTime = timeGetTime();
		if((dwCurrentTime - dwFPSLastTime) >= 500)
		{
			dwFPSLastTime = dwCurrentTime;
			dwFrameCount = 0;
		}
		if((dwCurrentTime - dwExecLastTime) >= (1000 / GAME_FPS))
		{
			dwExecLastTime = dwCurrentTime;

			if(m_ifInitialize)
			{
				//Update();
			}

			dwFrameCount++;
		}
	}

	return 0;
}