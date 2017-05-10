//=============================================================================
//
//	タイトル	軌跡エフェクト
//	ファイル名	orbit.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/11/10
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "orbit.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	COrbit::m_pTexture;

//=============================================================================
//	関数名	:COrbit()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
COrbit::COrbit(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~COrbit()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
COrbit::~COrbit()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void COrbit::Init(D3DXVECTOR3 pos)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_LocPos[0] = VEC3_ZERO;
	m_LocPos[1] = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ORBIT_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// 頂点情報セット
	SetOrbitData();

	// インデックスバッファの確保
	D3D_DEVICE->CreateIndexBuffer((sizeof(WORD) * ORBIT_INDEX_NUM), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	WORD *pIdx;

	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	{
		int buff = (ORBIT_VERTEX_NUM / (ORBIT_VERTICAL + 1));

		for(int nCntVertical = 0 ; nCntVertical < ORBIT_VERTICAL ; nCntVertical++)
		{
			for(int nCntIdx = 0 ; nCntIdx < ORBIT_INDEX_STANDARD ; nCntIdx++)
			{
				if((nCntVertical == (ORBIT_VERTICAL - 1)) && (nCntIdx == (ORBIT_INDEX_STANDARD - 2)))
				{// 処理打ち切り
					break;
				}

				if(nCntIdx == (ORBIT_INDEX_STANDARD - 1))
				{
					buff += (ORBIT_HORIZONTAL + 2);
					pIdx[nCntVertical * ORBIT_INDEX_STANDARD + nCntIdx] = buff;
				}
				else if(nCntIdx == (ORBIT_INDEX_STANDARD - 2))
				{
					pIdx[nCntVertical * ORBIT_INDEX_STANDARD + nCntIdx] = buff;
				}
				else
				{
					pIdx[nCntVertical * ORBIT_INDEX_STANDARD + nCntIdx] = buff;
				
					if(nCntIdx % 2 == 0)
					{
						buff -= (ORBIT_HORIZONTAL + 1);
					}
					else
					{
						if(nCntIdx != (ORBIT_INDEX_STANDARD - 3))
						{
							buff += (ORBIT_HORIZONTAL + 2);
						}
					}
				}
			}
		}
	}
	m_pIdxBuff->Unlock();

	// リソースのロード
	Load();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void COrbit::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pIdxBuff);

	// リソースのアンロード
	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void COrbit::Update(void)
{
	VERTEX_3D *pVtx;	// 頂点バッファ
	CPlayer *player = CGame::GetPlayer1();

	if(player != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for(int i = 0 ; i < (ORBIT_VERTEX_NUM / 2 - 1) ; i++)
		{
			m_OldPos[i * 2] = m_OldPos[(i + 1) * 2];
			m_OldPos[i * 2 + 1] = m_OldPos[(i + 1) * 2 + 1];
		}

		D3DXVECTOR3 locPos1 = m_LocPos[0];
		D3DXVECTOR3 locPos2 = m_LocPos[1];

		D3DXVec3TransformCoord(&locPos1, &locPos1, player->GetWMatrix());
		D3DXVec3TransformCoord(&locPos2, &locPos2, player->GetWMatrix());

		m_OldPos[ORBIT_VERTEX_NUM - 1] = locPos1;
		m_OldPos[ORBIT_VERTEX_NUM - 2] = locPos2;

		for(int i = 0 ; i < ORBIT_VERTEX_NUM ; i++)
		{
			pVtx[i].Pos = m_OldPos[i];
		}

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void COrbit::Draw(void)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;			// マトリックス
	static float starttime = 20;


	// マトリックスの設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// カリング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// 描画処理
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetIndices(m_pIdxBuff);								// インデックスバッファのバインド
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, NULL);									// テクスチャの設定

	if(starttime <= 0)
	{
		D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ORBIT_VERTEX_NUM, 0, ORBIT_POLYGON_NUM);	// 描画
	}

	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

	// カリング設定を戻す
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if(starttime > 0) starttime--;
}

//=============================================================================
//	関数名:SetOrbitData
//	引数:VERTEX_3D *pVtx(頂点データ)
//	戻り値:無し
//	説明:頂点データをセットする。
//=============================================================================
void COrbit::SetOrbitData(void)
{
	VERTEX_3D *pVtx;
	static float rot = 1.0f;
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= ORBIT_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= ORBIT_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[(nCntVtxVertical * (ORBIT_HORIZONTAL + 1) + nCntVtxHorizontal)].Pos = VEC3_ZERO;
		}
	}
	
	
	// 法線・色設定
	for(int nCntVertex = 0 ; nCntVertex < ORBIT_VERTEX_NUM ; nCntVertex++)
	{
		pVtx[nCntVertex].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCntVertex].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.7f, (((float)nCntVertex / (float)ORBIT_VERTEX_NUM) * 0.5f));
	}
	
	// テクスチャ座標設定
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= ORBIT_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= ORBIT_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[(nCntVtxVertical * (ORBIT_HORIZONTAL + 1) + nCntVtxHorizontal)].tex.x = (float)nCntVtxHorizontal * 0.5f;
			pVtx[(nCntVtxVertical * (ORBIT_HORIZONTAL + 1) + nCntVtxHorizontal)].tex.y = (float)nCntVtxVertical * 0.5f;
		}
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
COrbit *COrbit::Create(bool ifListAdd, int priority, OBJTYPE objtype, D3DXVECTOR3 pos)
{
	COrbit *orbit;
	
	orbit = new COrbit(ifListAdd, priority, objtype);

	orbit->Init();

	return orbit;
}