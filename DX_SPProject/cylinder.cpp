//=============================================================================
//
//	タイトル	メッシュフィールド
//	ファイル名	cylinder.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/05/10
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "cylinder.h"
#include "cameraDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CCylinder::m_pTexture;

//=============================================================================
//	関数名	:CCylinder()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CCylinder::CCylinder(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CCylinder()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CCylinder::~CCylinder()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * CYLINDER_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// 頂点情報セット
	SetVtxBuff();

	// インデックスバッファの確保
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
				{// 処理打ち切り
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
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CCylinder::SetVtxBuff(void)
{
	VERTEX_3D *pVtx;
	static float rot = 1.0f;


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
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
		// 法線設定
		pVtx[nCntVertex].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 色設定
		pVtx[nCntVertex].col = D3DCOLOR_COLORVALUE(0.8f, 0.8f, 0.8f, 0.8f);
	}

	// テクスチャ貼付座標設定
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
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CCylinder::Uninit(void)
{
	// インスタンス削除
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
	SafetyRelease(m_pIdxBuff);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CCylinder::Update(void)
{
	// カメラ座標に追従
	m_Pos = CManager::GetCamera()->GetCameraPosV();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CCylinder::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
		
	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// 描画処理
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetIndices(m_pIdxBuff);								// インデックスバッファのバインド
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
	D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CYLINDER_VERTEX_NUM, 0, CYLINDER_POLYGON_NUM);	// メッシュフィールド描画
		
	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CCylinder *CCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CCylinder *cylinder;
	
	cylinder = new CCylinder;

	cylinder->Init(pos, rot);

	return cylinder;
}