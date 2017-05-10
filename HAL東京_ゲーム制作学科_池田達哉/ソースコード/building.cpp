//=============================================================================
//
//	タイトル	建物
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
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CBuilding::Init(void)
{
	m_ifInitialize = false;

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
		m_BuildingMat[i] = (D3DXMATERIAL *)m_BuildingMesh[i].pBuffMat->GetBufferPointer();
	}

	// 建物を作成
	for(int z = 0 ; z < BUILDING_INSIDE ; z++)
	{
		for(int x = 0 ; x < BUILDING_HORIZONTAL ; x++)
		{
			random_device rd;
			mt19937 mt(rd());
			uniform_int_distribution<int> type(0, (BUILDING_TYPE_NUM - 1));

			int typeNum = type(mt);

			// 生成するビルの種類をランダムに設定
			m_Building[z * BUILDING_HORIZONTAL + x].Type	= typeNum;
			m_Building[z * BUILDING_HORIZONTAL + x].Visible	= true;
			m_Building[z * BUILDING_HORIZONTAL + x].Pos.x	= ((x - (BUILDING_HORIZONTAL / 2)) * BUILDING_MARGIN);
			m_Building[z * BUILDING_HORIZONTAL + x].Pos.y	= 0.0f;
			m_Building[z * BUILDING_HORIZONTAL + x].Pos.z	= ((z - (BUILDING_INSIDE / 2)) * BUILDING_MARGIN);
		}
	}

	// 建物が線路に近接している場合非表示にする
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

	// 初期化が完了
	m_ifInitialize = true;

	// †スレッド起動†
	//m_hTh = (HANDLE)_beginthreadex(NULL, 0, UpdateS, NULL, 0, &m_thID);
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CBuilding::RecalcTexCoord(LPD3DXMESH* mesh)
{
	
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CBuilding::Uninit(void)
{
	// スレッド終了
	CloseHandle(m_hTh);

	for(int i = 0 ; i < BUILDING_TYPE_NUM ; i++)
	{
		SafetyRelease(m_BuildingMesh[i].pBuffMat);
		SafetyRelease(m_BuildingMesh[i].pMesh);
	}

	// 初期化が必要
	m_ifInitialize = false;
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CBuilding::Update(void)
{
	GOALPOS goalPos = CGame::GetGoal()->GetGoalPos();

	if(m_ifInitialize)
	{
		static SPLINE oldSpline = *CGame::GetRailLine()->GetSpline();
		D3DXVECTOR3 playerPos = CGame::GetPlayer1()->GetPos();
		D3DXVECTOR3 moveDir = CGame::GetPlayer1()->GetMoveDir();


		// 位置更新
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

		// スプライン情報が更新された場合
		if(flug || (oldSpline.Length - m_Spline->Length > 1.0f))
		{
			// 建物が線路に近接している場合非表示にする
#pragma omp parallel for
			for(int i = 0 ; i < BUILDING_ALL ; i++)
			{
				bool ifNearBuilding = false;

				// 道中の線路近接判定
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

				// ゴールの線路近接判定
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

		// スプライン情報保存
		oldSpline = (*m_Spline);
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CBuilding::Draw(void)
{
	D3DXMATERIAL	*pMat = NULL;		// マテリアル
	D3DMATERIAL9	matDef;				// デフォルトのマテリアル


	if(!CManager::GetEdhitMode() && m_ifInitialize)
	{
		// Zテスト開始
		CRendererDX::EnableZTest();

		// アルファテスト開始
		CRendererDX::EnableAlphaTest();

		// ライティング設定をオフに
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
		
		// 現在のマテリアルを取得
		D3D_DEVICE->GetMaterial(&matDef);

		for(int i = 0 ; i < BUILDING_ALL ; i++)
		{
			if(m_Building[i].Visible)
			{
				// マトリックス設定
				CRendererDX::SetMatrix(&m_mtxWorld, m_Building[i].Pos, VEC3_ZERO);

				pMat = m_BuildingMat[m_Building[i].Type];

				// 3Dモデル描画
				for(int j = 0 ; j < (int)m_BuildingMesh[m_Building[i].Type].NumMat ; j++)
				{
					// マテリアルセット
					D3D_DEVICE->SetMaterial(&pMat[j].MatD3D);

					// テクスチャ読み込み
					if(pMat[j].pTextureFilename)
					{// テクスチャ有り

						// リストから同名のテクスチャを探索し、セット
						for each(TEXTURE list in m_BuildingMesh[m_Building[i].Type].Tex)
						{
							if(list.FileName == CharPToString(pMat[j].pTextureFilename))
							{
								D3D_DEVICE->SetTexture(0, list.pTexture);
							}
						}
					}
					else
					{// テクスチャ無し

						// テクスチャをセットしない
						D3D_DEVICE->SetTexture(0, NULL);
					}

					// モデル描画
					m_BuildingMesh[m_Building[i].Type].pMesh->DrawSubset(j);
				}
			}
		}

		// マテリアルを元に戻す
		D3D_DEVICE->SetMaterial(&matDef);

		// アルファテスト終了
		CRendererDX::DisableAlphaTest();

		// Zテスト終了
		CRendererDX::DisableZTest();

		// ライティング設定をオンに
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
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