//=============================================================================
//
//	タイトル	メッシュフィールド
//	ファイル名	meshfield.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/05/10
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "meshfield.h"
#include "sceneXDX.h"

static D3DXVECTOR3 g_PosBuff[MESHFIELD_VERTEX_NUM];
static D3DXVECTOR3 g_NorBuff[MESHFIELD_VERTEX_NUM];

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CMeshfield::m_pTexture;

//=============================================================================
//	関数名	:CMeshfield()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CMeshfield::CMeshfield(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CMeshfield()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CMeshfield::~CMeshfield()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CMeshfield::Init(cVec3 pos, cVec3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// 頂点情報セット
	SetVtxBuff();

	// インデックスセット
	SetMeshIndex(&m_pIdxBuff, MESHFIELD_HORIZONTAL, MESHFIELD_VERTICAL);

	Load();
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CMeshfield::SetVtxBuff(void)
{
	VERTEX_3D *pVtx;
	static float rot = 1.0f;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= MESHFIELD_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= MESHFIELD_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].Pos.x
				= (m_Pos.x - (MESHFIELD_WIDTH * 0.5f * MESHFIELD_HORIZONTAL)) + (MESHFIELD_WIDTH * nCntVtxHorizontal);
			g_PosBuff[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].x
				= (m_Pos.x - (MESHFIELD_WIDTH * 0.5f * MESHFIELD_HORIZONTAL)) + (MESHFIELD_WIDTH * nCntVtxHorizontal);

			//pVtx[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].Pos.y = g_HeightMap[nCntVtxVertical][nCntVtxHorizontal];
			//g_PosBuff[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].y = g_HeightMap[nCntVtxVertical][nCntVtxHorizontal];
			pVtx[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].Pos.y = 0.0f;
			g_PosBuff[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].y = 0.0f;

			pVtx[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].Pos.z
				= (m_Pos.z + (MESHFIELD_HEIGHT * 0.5f * MESHFIELD_VERTICAL)) - (MESHFIELD_HEIGHT * nCntVtxVertical);
			g_PosBuff[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].z
				= (m_Pos.z + (MESHFIELD_HEIGHT * 0.5f * MESHFIELD_VERTICAL)) - (MESHFIELD_HEIGHT * nCntVtxVertical);
		}
	}

	/*
	for(int x = 0 ; x <= MESHFIELD_VERTICAL ; x++)
	{
	for(int y = 0 ; y <= MESHFIELD_HORIZONTAL ; y++)
	{
	pVtx[(y * (MESHFIELD_VERTICAL + 1) + x)].Pos.y = (sinf(rot) * MESHFIELD_WAVE);
	g_PosBuff[(y * (MESHFIELD_VERTICAL + 1) + x)].y = (sinf(rot) * MESHFIELD_WAVE);
	rot += MESHFIELD_WAVEROT;
	}
	}*/


	// 法線設定
	for(int y = 0 ; y <= MESHFIELD_VERTICAL ; y++)
	{
		for(int x = 0 ; x <= MESHFIELD_HORIZONTAL ; x++)
		{
			if((y != 0) && (y != (MESHFIELD_VERTICAL))
				&& (x != 0) && (x != (MESHFIELD_HORIZONTAL)))
			{
				//pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				// 法線設定
				D3DXVECTOR3 n0, n1, n2, n3, n4, n5, n6, v01, v02, v03, v04, v05, v06;
				v01 = pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
				v02 = pVtx[((y - 1) * (MESHFIELD_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
				v03 = pVtx[((y - 1) * (MESHFIELD_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
				v04 = pVtx[((y) * (MESHFIELD_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
				v05 = pVtx[((y + 1) * (MESHFIELD_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
				v06 = pVtx[((y + 1) * (MESHFIELD_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
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
				pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Nor = n0;
			}
			else
			{
				pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
		}
	}

	// 色設定
	for(int nCntVertex = 0 ; nCntVertex < MESHFIELD_VERTEX_NUM ; nCntVertex++)
	{
		//pVtx[nCntVertex].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntVertex].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標設定
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= MESHFIELD_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= MESHFIELD_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].tex.x = (float)nCntVtxHorizontal * 10.0f;
			pVtx[(nCntVtxVertical * (MESHFIELD_HORIZONTAL + 1) + nCntVtxHorizontal)].tex.y = (float)nCntVtxVertical * 10.0f;
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
void CMeshfield::Uninit(void)
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
void CMeshfield::Update(void)
{
	VERTEX_3D *pVtx;	// 頂点バッファ
	WORD *pIdx;			// インデックスバッファ


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 法線設定
	for(int y = 0 ; y <= MESHFIELD_VERTICAL ; y++)
	{
		for(int x = 0 ; x <= MESHFIELD_HORIZONTAL ; x++)
		{
			if((y != 0) && (y != (MESHFIELD_VERTICAL))
				&& (x != 0) && (x != (MESHFIELD_HORIZONTAL)))
			{
				// 法線設定
				D3DXVECTOR3 n0;
				if(1)
				{// ちゃんとした法線設定
					D3DXVECTOR3 n1, n2, n3, n4, n5, n6, v01, v02, v03, v04, v05, v06;
					v01 = pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
					v02 = pVtx[((y - 1) * (MESHFIELD_HORIZONTAL + 1) + (x - 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
					v03 = pVtx[((y - 1) * (MESHFIELD_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
					v04 = pVtx[((y) * (MESHFIELD_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
					v05 = pVtx[((y + 1) * (MESHFIELD_HORIZONTAL + 1) + x)].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
					v06 = pVtx[((y + 1) * (MESHFIELD_HORIZONTAL + 1) + (x + 1))].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Pos;
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
					v1 = pVtx[(y * (MESHFIELD_HORIZONTAL + 1)) + (x + 1)].Pos - pVtx[(y * (MESHFIELD_HORIZONTAL + 1)) + (x - 1)].Pos;
					v2 = pVtx[((y - 1) * (MESHFIELD_HORIZONTAL + 1)) + x].Pos - pVtx[((y + 1) * (MESHFIELD_HORIZONTAL + 1)) + x].Pos;
					n1 = D3DXVECTOR3(-v1.y, v1.x, 0.0f);
					D3DXVec3Normalize(&n1, &n1);
					n2 = D3DXVECTOR3(0.0f, v2.z, -v2.y);
					D3DXVec3Normalize(&n2, &n2);
					n0 = n1 + n2;
					D3DXVec3Normalize(&n0, &n0);
				}
				pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Nor = n0;
				g_NorBuff[(y * (MESHFIELD_HORIZONTAL + 1) + x)] = n0;
			}
			else
			{
				pVtx[(y * (MESHFIELD_HORIZONTAL + 1) + x)].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				g_NorBuff[(y * (MESHFIELD_HORIZONTAL + 1) + x)] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
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
void CMeshfield::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// Zテスト方法更新
	CRendererDX::EnableZTest();
	
	// 描画処理
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetIndices(m_pIdxBuff);								// インデックスバッファのバインド
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
	D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESHFIELD_VERTEX_NUM, 0, MESHFIELD_POLYGON_NUM);	// メッシュフィールド描画
	
	// Zテスト終了
	CRendererDX::DisableZTest();
}

//=============================================================================
//	関数名	:GetFrontMesh
//	引数	:D3DXVECTOR3 pos
//	戻り値	:無し
//	説明	:メッシュ上のどの点に乗っているか判別する。
//=============================================================================
inline int	CMeshfield::GetFrontMesh(D3DXVECTOR3 pos)
{
	VERTEX_3D	*pVtx;						// 頂点バッファ
	WORD		*pIdx;						// インデックスバッファ
	int			onPolygon = 0;				// どのポリゴンに乗っているか


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for(int i = 0 ; i < (MESHFIELD_INDEX_NUM - 2) ; i++)
	{
		if(i % 2 == 0)
		{
			D3DXVECTOR3 n0, v01, v02;
			v01 = pVtx[pIdx[i + 1]].Pos - pVtx[pIdx[i]].Pos;
			v01.y = 0.0f;
			v02 = pos - pVtx[pIdx[i]].Pos;
			v02.y = 0.0f;
			D3DXVec3Cross(&n0, &v01, &v02);
			if(n0.y >= 0.0f)
			{
				v01 = pVtx[pIdx[i + 2]].Pos - pVtx[pIdx[i + 1]].Pos;
				v01.y = 0.0f;
				v02 = pos - pVtx[pIdx[i + 1]].Pos;
				v02.y = 0.0f;
				D3DXVec3Cross(&n0, &v01, &v02);
				if(n0.y >= 0.0f)
				{
					v01 = pVtx[pIdx[i]].Pos - pVtx[pIdx[i + 2]].Pos;
					v01.y = 0.0f;
					v02 = pos - pVtx[pIdx[i + 2]].Pos;
					v02.y = 0.0f;
					D3DXVec3Cross(&n0, &v01, &v02);
					if(n0.y >= 0.0f)
					{// 点はポリゴンの中にある
						return i;
					}
				}
			}
		}
		else
		{
			D3DXVECTOR3 n0, v01, v02;
			v01 = pVtx[pIdx[i + 2]].Pos - pVtx[pIdx[i]].Pos;
			v01.y = 0.0f;
			v02 = pos - pVtx[pIdx[i]].Pos;
			v02.y = 0.0f;
			D3DXVec3Cross(&n0, &v01, &v02);
			if(n0.y >= 0.0f)
			{
				v01 = pVtx[pIdx[i + 1]].Pos - pVtx[pIdx[i + 2]].Pos;
				v01.y = 0.0f;
				v02 = pos - pVtx[pIdx[i + 2]].Pos;
				v02.y = 0.0f;
				D3DXVec3Cross(&n0, &v01, &v02);
				if(n0.y >= 0.0f)
				{
					v01 = pVtx[pIdx[i]].Pos - pVtx[pIdx[i + 1]].Pos;
					v01.y = 0.0f;
					v02 = pos - pVtx[pIdx[i + 1]].Pos;
					v02.y = 0.0f;
					D3DXVec3Cross(&n0, &v01, &v02);
					if(n0.y >= 0.0f)
					{// 点はポリゴンの中にある
						return i;
					}
				}
			}
		}
	}

	m_pIdxBuff->Unlock();
	m_pVtxBuff->Unlock();

	return -1;
}

//=============================================================================
//	関数名	:GetHeight
//	引数	:D3DXVECTOR3 pos
//	戻り値	:無し
//	説明	:与えられた座標の乗っかっているポリゴンの高さを返す。
//=============================================================================
float CMeshfield::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D	*pVtx;							// 頂点バッファ
	WORD		*pIdx;							// インデックスバッファ
	float		height		= 0.0f;
	int			onPolygon	= GetFrontMesh(pos);// どのポリゴンに乗っているか
	D3DXVECTOR3	vc1, vc2, vc3;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	if(onPolygon >= 0)
	{
		D3DXVECTOR3	v1, v2, v3;
		D3DXVECTOR3	vp1, vp2, vp3;
		
		if(onPolygon % 2 == 0)
		{
			// ベクトル算出
			v1	= pVtx[pIdx[onPolygon + 1]].Pos - pVtx[pIdx[onPolygon]].Pos;
			vp1	= pos - pVtx[pIdx[onPolygon]].Pos;
			v2	= pVtx[pIdx[onPolygon + 2]].Pos - pVtx[pIdx[onPolygon + 1]].Pos;
			vp2	= pos - pVtx[pIdx[onPolygon + 1]].Pos;
			v3	= pVtx[pIdx[onPolygon]].Pos - pVtx[pIdx[onPolygon + 2]].Pos;
			vp3	= pos - pVtx[pIdx[onPolygon + 2]].Pos;
		}
		else
		{
			// ベクトル算出
			v1	= pVtx[pIdx[onPolygon + 2]].Pos - pVtx[pIdx[onPolygon]].Pos;
			vp1	= pos - pVtx[pIdx[onPolygon]].Pos;
			v2	= pVtx[pIdx[onPolygon + 1]].Pos - pVtx[pIdx[onPolygon + 2]].Pos;
			vp2	= pos - pVtx[pIdx[onPolygon + 1]].Pos;
			v3	= pVtx[pIdx[onPolygon]].Pos - pVtx[pIdx[onPolygon + 1]].Pos;
			vp3	= pos - pVtx[pIdx[onPolygon + 1]].Pos;
		}

		// 指定座標の外積算出
		D3DXVec3Cross(&vc1, &v1, &vp1);
		D3DXVec3Cross(&vc2, &v2, &vp2);
		D3DXVec3Cross(&vc3, &v3, &vp3);
	}
	else
	{
		//height = 0.0f;
	}

	// 指定座標がポリゴン内かどうか
	if((vc1.y >= 0.0f) && (vc2.y >= 0.0f) && (vc3.y >= 0.0f))
	{
		D3DXVECTOR3	nor, v1, v2;

		// 法線算出
		v1 = pVtx[pIdx[onPolygon + 1]].Pos - pVtx[pIdx[onPolygon]].Pos;
		v2 = pVtx[pIdx[onPolygon + 2]].Pos - pVtx[pIdx[onPolygon]].Pos;
		D3DXVec3Cross(&nor, &v1, &v2);

		// 高さを求める
		height = pVtx[pIdx[onPolygon]].Pos.y
			- (nor.x * (pos.x - pVtx[pIdx[onPolygon]].Pos.x)
			+ nor.z * (pos.z - pVtx[pIdx[onPolygon]].Pos.z)) / nor.y;
	}

	m_pIdxBuff->Unlock();
	m_pVtxBuff->Unlock();

	return height;
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CMeshfield *CMeshfield::Create(cVec3 pos, cVec3 rot)
{
	CMeshfield *instance;	// インスタンス

	// インスタンス生成
	instance = new CMeshfield();

	// 初期化処理
	instance->Init(pos, rot);

	// インスタンスをリターン
	return instance;
}

//=============================================================================
//	関数名	:GetVtxPos
//	引数	:int nNumVtx(頂点番号)
//	戻り値	:無し
//	説明	:指定した番号の頂点座標を返す。
//=============================================================================
D3DXVECTOR3 CMeshfield::GetVtxPos(int nNumVtx)
{
	return g_PosBuff[nNumVtx];
}

//=============================================================================
//	関数名	:GetVtxNor
//	引数	:int nNumVtx(頂点番号)
//	戻り値	:無し
//	説明	:指定した番号の頂点の法線を返す。
//=============================================================================
D3DXVECTOR3 CMeshfield::GetVtxNor(int nNumVtx)
{
	return g_NorBuff[nNumVtx];
}