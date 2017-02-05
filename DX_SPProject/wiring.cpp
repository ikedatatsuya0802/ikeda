//=============================================================================
//
//	タイトル	高架
//	ファイル名	wiring.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "wiring.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "game.h"
#include "railLine.h"
#include "player.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CWiring::CWiring(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneXDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CWiring::~CWiring()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CWiring::Init(D3DXVECTOR3 pos)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	// スプライン情報の取得
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 3Dモデル読み込み
	m_ModelStatus	= new MODELSTATUS;
	//m_pTexture		= new TEXTURE;
	LoadModel(".\\data\\MODEL\\wiring.x", m_ModelStatus);
}

//=============================================================================
//	関数名	:SetWiringData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CWiring::SetVtxBuff(void)
{

}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
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
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
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
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CWiring::Draw(void)
{
	D3DXMATERIAL		*pMat = NULL;		// マテリアル
	D3DMATERIAL9		matDef;				// デフォルトのマテリアル

	if(!CManager::GetEdhitMode())
	{
		// アルファ・Zテスト開始
		CRendererDX::EnableAlphaTest();
		CRendererDX::EnableZTest();

		// 現在のマテリアルを取得
		D3D_DEVICE->GetMaterial(&matDef);

		for(int i = 2 ; i < (int)m_Spline->PosHermite.size() ; i += 3)
		{
			float rot = 0.0f;

			// 架線柱の回転値計算
			if((i > 0) && (i != (int)m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[i + 1].x - m_Spline->PosHermite[i - 1].x),
					(m_Spline->PosHermite[i + 1].z - m_Spline->PosHermite[i - 1].z));
			}
			else
			{
				rot = 0.0f;
			}

			// マトリックス設定
			CRendererDX::SetMatrix(&m_mtxWorld,
				D3DXVECTOR3(m_Spline->PosHermite[i].x, m_Spline->PosHermite[i].y, m_Spline->PosHermite[i].z),
				D3DXVECTOR3(0.0f, rot, 0.0f));

			// マテリアル変換
			pMat = (D3DXMATERIAL *)m_ModelStatus->pBuffMat->GetBufferPointer();

			// 3Dモデル描画
			for(int mat = 0 ; mat < (int)m_ModelStatus->NumMat ; mat++)
			{
				float			playerPos = CGame::GetPlayer1()->GetPerSpline();	// プレイヤのスプライン座標
				D3DXMATERIAL	pMatAlpha = *pMat;									// 透明マテリアル


				// プレイヤーに近づいたら透過させる
				float length = (1.0f / m_Spline->PosHermite.size() * i) - playerPos;

				if(length < WIRING_CLEAR_BORDER)
				{
					(&pMatAlpha)[mat].MatD3D.Diffuse.a = (1.0f + (length - WIRING_CLEAR_BORDER) / 0.005f);

					if((&pMatAlpha)[mat].MatD3D.Diffuse.a < 0.0f)
					{
						(&pMatAlpha)[mat].MatD3D.Diffuse.a = 0.0f;
					}

					// マテリアルセット
					D3D_DEVICE->SetMaterial(&(&pMatAlpha)[mat].MatD3D);
				}
				else
				{
					// マテリアルセット
					D3D_DEVICE->SetMaterial(&pMat[mat].MatD3D);
				}

				// テクスチャをセットしない
				D3D_DEVICE->SetTexture(0, NULL);

				// モデル描画
				m_ModelStatus->pMesh->DrawSubset(mat);
			}

			// マテリアルを元に戻す
			D3D_DEVICE->SetMaterial(&matDef);
		}

		// アルファ・Zテスト終了
		CRendererDX::DisableAlphaTest();
		CRendererDX::DisableZTest();
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CWiring *CWiring::Create(D3DXVECTOR3 pos)
{
	CWiring *instance;

	instance = new CWiring;
	
	instance->Init(pos);

	return instance;
}