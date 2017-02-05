//=============================================================================
//
//	タイトル	高架
//	ファイル名	building.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
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
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CBuilding::Init(void)
{
	// スプライン情報の取得
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 建物のメッシュ情報をロード
	for(int i = 0 ; i < BUILDING_TYPE_NUM ; i++)
	{
		string modelFileName = ".\\data\\MODEL\\building\\building";
		stringstream ss;
		MODELSTATUS	mesh;

		// ファイル名編集
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

	// 建物を作成
	for(int z = -(BUILDING_INSIDE / 2) ; z < (BUILDING_INSIDE / 2) ; z++)
	{
		for(int x = -(BUILDING_HORIZONTAL / 2) ; x < (BUILDING_HORIZONTAL / 2) ; x++)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_int_distribution<int> type(0, (BUILDING_TYPE_NUM - 1));

			int typeNum = type(mt);

			// 生成するビルの種類をランダムに設定
			m_Instance.push_back(CSceneXDX::Create("", &m_BuildingMesh[typeNum],
				D3DXVECTOR3((x * BUILDING_MARGIN), 0.0f, (z * BUILDING_MARGIN) + 1000.0f)));
			m_Instance[m_Instance.size() - 1]->UnlinkList();
		}
	}

	// 建物が線路に近接している場合非表示にする
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
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
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
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CBuilding::Update(void)
{
	static SPLINE oldSpline = *CGame::GetRailLine()->GetSpline();
	D3DXVECTOR3 playerPos = CGame::GetPlayer1()->GetPos();

	
	// 位置更新
	for each(CSceneXDX* list in m_Instance)
	{
		D3DXVECTOR3 listPos = list->GetPos();

		if(playerPos.z > listPos.z + FOG_END / 2)
		{
			list->SetPos(D3DXVECTOR3(listPos.x, listPos.y, listPos.z + FOG_END));
		}
	}

	// スプライン情報が更新された場合
	if(fabsf(oldSpline.Length - m_Spline->Length) < 0.01f)
	{
		// 建物が線路に近接している場合非表示にする
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

	// スプライン情報保存
	oldSpline = (*m_Spline);
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
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