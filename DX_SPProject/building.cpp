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

SPLINE*				CBuilding::m_Spline;
vector<CSceneXDX*>	CBuilding::m_Instance;
MODELSTATUS			CBuilding::m_BuildingMesh[BUILDING_TYPE_NUM];

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CBuilding::Init(void)
{
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
	}

	// �������쐬
	for(int z = -(BUILDING_INSIDE / 2) ; z < (BUILDING_INSIDE / 2) ; z++)
	{
		for(int x = -(BUILDING_HORIZONTAL / 2) ; x < (BUILDING_HORIZONTAL / 2) ; x++)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_int_distribution<int> type(0, (BUILDING_TYPE_NUM - 1));

			int typeNum = type(mt);

			// ��������r���̎�ނ������_���ɐݒ�
			m_Instance.push_back(CSceneXDX::Create("", &m_BuildingMesh[typeNum],
				D3DXVECTOR3((x * BUILDING_MARGIN), 0.0f, (z * BUILDING_MARGIN) + 1000.0f)));
			m_Instance[m_Instance.size() - 1]->UnlinkList();
		}
	}

	// ���������H�ɋߐڂ��Ă���ꍇ��\���ɂ���
	for each(CSceneXDX* list in m_Instance)
	{
		for each(D3DXVECTOR3 splPos in m_Spline->PosHermite)
		{
			if(D3DXVec3Length(&(list->GetPos() - splPos)) < BUILDING_CLEAR_LENGTH)
			{
				if(list->GetDrawFrag())
					list->ChangeDrawFrag();
			}
		}
	}
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CBuilding::Uninit(void)
{
	for each(CSceneXDX* list in m_Instance)
	{
		list->Uninit();
		delete list;
		list = NULL;
	}

	for(int i = 0 ; i < BUILDING_TYPE_NUM ; i++)
	{
		SafetyRelease(m_BuildingMesh[i].pBuffMat);
		SafetyRelease(m_BuildingMesh[i].pMesh);
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CBuilding::Update(void)
{
	static SPLINE oldSpline = *CGame::GetRailLine()->GetSpline();
	D3DXVECTOR3 playerPos = CGame::GetPlayer1()->GetPos();

	
	// �ʒu�X�V
	for each(CSceneXDX* list in m_Instance)
	{
		D3DXVECTOR3 listPos = list->GetPos();

		if(playerPos.z > listPos.z + FOG_END / 2)
		{
			list->SetPos(D3DXVECTOR3(listPos.x, listPos.y, listPos.z + FOG_END));
		}
	}

	// �X�v���C����񂪍X�V���ꂽ�ꍇ
	if(fabsf(oldSpline.Length - m_Spline->Length) < 0.01f)
	{
		// ���������H�ɋߐڂ��Ă���ꍇ��\���ɂ���
		for each(CSceneXDX* list in m_Instance)
		{
			for each(D3DXVECTOR3 splPos in m_Spline->PosHermite)
			{
				if(D3DXVec3Length(&(list->GetPos() - splPos)) < BUILDING_CLEAR_LENGTH)
				{
					if(list->GetDrawFrag())
						list->ChangeDrawFrag();
				}
				else
				{
					//if(!list->GetDrawFrag())
						//list->SetDrawFrag(true);
				}
			}
		}
	}

	// �X�v���C�����ۑ�
	oldSpline = (*m_Spline);
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CBuilding::Draw(void)
{
	for each(CSceneXDX* list in m_Instance)
	{
		if(list->GetDrawFrag())
		{
			D3DXVECTOR3 playerPos = CGame::GetPlayer1()->GetPos();

			if(D3DXVec3Length(&(list->GetPos() - playerPos)) < BUILDING_INVISIBLE_FAR)
			{
				list->Draw();
			}
		}
	}
}