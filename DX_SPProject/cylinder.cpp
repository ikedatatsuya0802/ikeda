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
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "sceneXDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CCylinder::m_Texture;

//=============================================================================
//	関数名	:CCylinder()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CCylinder::CCylinder(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
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
void CCylinder::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer((sizeof(VERTEX_3D) * CYLINDER_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// 頂点情報セット
	SetCylinderData();

	// インデックスバッファの確保
	pDevice->CreateIndexBuffer((sizeof(WORD) * CYLINDER_INDEX_NUM), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

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
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CCylinder::Uninit(void)
{
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if(m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CCylinder::Update(void)
{
	VERTEX_3D *pVtx;	// 頂点バッファ
	WORD *pIdx;			// インデックスバッファ


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 法線設定
	for(int y = 0 ; y <= CYLINDER_VERTICAL ; y++)
	{
		for(int x = 0 ; x <= CYLINDER_HORIZONTAL ; x++)
		{
			if((y != 0) && (y != (CYLINDER_VERTICAL))
				&& (x != 0) && (x != (CYLINDER_HORIZONTAL)))
			{
				// 法線設定
				D3DXVECTOR3 n0;
				if(1)
				{// ちゃんとした法線設定
					D3DXVECTOR3 n1, n2, n3, n4, n5, n6, v01, v02, v03, v04, v05, v06;
					v01 = pVtx[(y * (CYLINDER_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v02 = pVtx[((y - 1) * (CYLINDER_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v03 = pVtx[((y - 1) * (CYLINDER_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v04 = pVtx[((y) * (CYLINDER_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v05 = pVtx[((y + 1) * (CYLINDER_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					v06 = pVtx[((y + 1) * (CYLINDER_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Pos;
					D3DXVec3Cross(&n1, &v01, &v02);
					D3DXVec3Normalize(&n1, &n1);
					D3DXVec3Cross(&n2, &v02, &v03);
					D3DXVec3Normalize(&n2, &n2);
					D3DXVec3Cross(&n3, &v03, &v04);
					D3DXVec3Normalize(&n3, &n3);
					D3DXVec3Cross(&n4, &v04, &v05);
					D3DXVec3Normalize(&n4, &n4);
					D3DXVec3Cross(&n5, &v05, &v06);
					D3DXVec3Normalize(&n5, &n5);
					D3DXVec3Cross(&n6, &v06, &v01);
					D3DXVec3Normalize(&n6, &n6);
					n0 = (n1 + n2 + n3 + n4 + n5 + n6) / 6.0f;
					D3DXVec3Normalize(&n0, &n0);
				}
				else
				{// 楽～な法線設定
					D3DXVECTOR3 n1, n2;
					D3DXVECTOR3 v1, v2;
					v1 = pVtx[(y * (CYLINDER_HORIZONTAL + 1)) + (x + 1)].Pos - pVtx[(y * (CYLINDER_HORIZONTAL + 1)) + (x - 1)].Pos;
					v2 = pVtx[((y - 1) * (CYLINDER_HORIZONTAL + 1)) + x].Pos - pVtx[((y + 1) * (CYLINDER_HORIZONTAL + 1)) + x].Pos;
					n1 = D3DXVECTOR3(-v1.y, v1.x, 0.0f);
					D3DXVec3Normalize(&n1, &n1);
					n2 = D3DXVECTOR3(0.0f, v2.z, -v2.y);
					D3DXVec3Normalize(&n2, &n2);
					n0 = n1 + n2;
					D3DXVec3Normalize(&n0, &n0);
				}
				pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Nor = n0;
			}
			else
			{
				pVtx[(y * (CYLINDER_HORIZONTAL + 1) + x)].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
		}
	}
	
	m_pIdxBuff->Unlock();
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// マトリックス

	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// スケール設定
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
	
	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	
	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);		// ワールドマトリックスの設定
		
	// ライティング設定をオフに
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// 描画処理
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	pDevice->SetIndices(m_pIdxBuff);								// インデックスバッファのバインド
	pDevice->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	pDevice->SetTexture(0, m_Texture);								// テクスチャの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, CYLINDER_VERTEX_NUM, 0, CYLINDER_POLYGON_NUM);	// メッシュフィールド描画
		
	// ライティング設定をオンに
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名:SetCylinderData
//	引数:VERTEX_3D *pVtx(頂点データ)
//	戻り値:無し
//	説明:頂点データをセットする。
//=============================================================================
void CCylinder::SetCylinderData(void)
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
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CCylinder *CCylinder::Create(D3DXVECTOR3 pos)
{
	CCylinder *cylinder;
	
	cylinder = new CCylinder;

	cylinder->Init();

	return cylinder;
}