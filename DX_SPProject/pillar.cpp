//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	pillar.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "pillar.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "game.h"
#include "railLine.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CPillar::m_pTexture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CPillar::CPillar(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CPillar::~CPillar()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CPillar::Init(D3DXVECTOR3 pos)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	// スプライン情報の取得
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 頂点バッファ追加
	for each (auto itr in m_Spline->Pos)
	{
		AddVtxBuffer(m_pVtxBuff, (sizeof(VERTEX_3D) * PILLAR_VERTEX_NUM));
	}

	// 頂点情報セット
	SetVtxBuff();

	// 法線情報セット
	//SetMeshNor();

	// インデックスセット
	SetMeshIndex(&m_pIdxBuff, PILLAR_HORIZONTAL, PILLAR_VERTICAL);

	Load();
}

//=============================================================================
//	関数名	:SetPillarData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CPillar::SetVtxBuff(void)
{
	VERTEX_3D *pVtx = NULL;


	for(int i = 0 ; i < (int)m_Spline->Pos.size() ; i++)
	{
		m_pVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);

		// 座標設定
		for(int y = 0 ; y <= PILLAR_VERTICAL ; y++)
		{
			for(int x = 0 ; x <= PILLAR_HORIZONTAL ; x++)
			{
				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos.x
					= cosf(D3DX_PI * 2.0f / PILLAR_HORIZONTAL * x) * PILLAR_RADIUS;

				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos.y
					= ((PILLAR_HEIGHT * 1.0f * PILLAR_VERTICAL)) - ((m_Spline->Pos[i].y - 10.0f) * y);

				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos.z
					= sinf(D3DX_PI * 2.0f / PILLAR_HORIZONTAL * x) * PILLAR_RADIUS;

				m_MeshPos[y * (PILLAR_HORIZONTAL + 1) + x] = pVtx[y * (PILLAR_HORIZONTAL + 1) + x].Pos;
			}
		}

		for(int j = 0 ; j < PILLAR_VERTEX_NUM ; j++)
		{
			// 法線設定
			pVtx[j].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 色設定
			pVtx[j].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標設定
		for(int y = 0 ; y <= PILLAR_VERTICAL ; y++)
		{
			for(int x = 0 ; x <= PILLAR_HORIZONTAL ; x++)
			{
				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].tex.x = (1.0f / 8.0f * x);
				pVtx[y * (PILLAR_HORIZONTAL + 1) + x].tex.y = (1.0f / PILLAR_VERTICAL * y);
			}
		}

		m_pVtxBuff[i]->Unlock();
	}
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CPillar::Uninit(void)
{
	for(int i = 0 ; i < (int)m_pVtxBuff.size() ; i++)
	{
		SafetyRelease(m_pVtxBuff[i]);
	}
	SafetyRelease(m_pTexture);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CPillar::Update(void)
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
void CPillar::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, VEC3_ZERO, VEC3_ZERO);

	// Zテスト開始
	CRendererDX::EnableZTest();

	// 描画処理
	D3D_DEVICE->SetIndices(m_pIdxBuff);		// インデックスバッファのバインド
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);		// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);	// テクスチャの設定

	for(int i = 0 ; i < (int)m_Spline->Pos.size() ; i++)
	{
		// 最低ラインの高さには柱を出さない
		if(m_Spline->Pos[i].y > 2.0f)
		{
			// 設置位置・サイズの設定
			CRendererDX::SetMatrix(&m_mtxWorld,
				D3DXVECTOR3(m_Spline->Pos[i].x, 0.0f, m_Spline->Pos[i].z),
				VEC3_ZERO,
				D3DXVECTOR3(1.0f, (m_Spline->Pos[i].y / PILLAR_HEIGHT - 0.1f), 1.0f));

			// 頂点バッファの設定
			D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[i], 0, sizeof(VERTEX_3D));

			// 描画
			D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, PILLAR_VERTEX_NUM, 0, PILLAR_POLYGON_NUM);
		}
	}

	// Zテスト終了
	CRendererDX::DisableZTest();
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CPillar *CPillar::Create(D3DXVECTOR3 pos)
{
	CPillar *instance;

	instance = new CPillar;
	
	instance->Init(pos);

	return instance;
}

//=============================================================================
//	関数名	:SetMeshNor
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:。
//=============================================================================
void CPillar::SetMeshNor(void)
{
	VERTEX_3D* pVtx = NULL;

	for(int i = 0 ; i < (int)m_pVtxBuff.size() ; i++)
	{
		m_pVtxBuff[i]->Lock(0, 0, (void**)&pVtx, 0);
		memcpy(pVtx, m_pVtxBuff[i], sizeof(m_pVtxBuff[i]));

		for(int j = 0 ; j < PILLAR_VERTEX_NUM ; j++)
		{
			// 法線設定
			D3DXVECTOR3 nor = VEC3_ZERO;

			if(j == 0)
			{// 最初の点

				D3DXVECTOR3 v1, v2;

				v1 = m_MeshPos[1] - m_MeshPos[0];
				v2 = m_MeshPos[PILLAR_HORIZONTAL] - m_MeshPos[0];

				D3DXVec3Cross(&nor, &v1, &v2);
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j < PILLAR_HORIZONTAL)
			{// 上端の点

				D3DXVECTOR3 n1, n2, v1, v2, v3;
				
				// 基準点からのベクトル算出
				v1 = m_MeshPos[j + 1] - m_MeshPos[j];
				v2 = m_MeshPos[j + PILLAR_HORIZONTAL] - m_MeshPos[j];
				v3 = m_MeshPos[j - 1] - m_MeshPos[j];

				// 外積により2つのベクトルにおける法線算出
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// 最終的なベクトル算出
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j > (PILLAR_VERTEX_NUM - PILLAR_HORIZONTAL))
			{// 下端の点

				D3DXVECTOR3 n1, n2, v1, v2, v3;

				// 基準点からのベクトル算出
				v1 = m_MeshPos[j - 1] - m_MeshPos[j];
				v2 = m_MeshPos[j - PILLAR_HORIZONTAL] - m_MeshPos[j];
				v3 = m_MeshPos[j + 1] - m_MeshPos[j];

				// 外積により2つのベクトルにおける法線算出
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// 最終的なベクトル算出
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j % PILLAR_HORIZONTAL == 0)
			{// 左端の点

				D3DXVECTOR3 n1, n2, v1, v2, v3;

				// 基準点からのベクトル算出
				v1 = m_MeshPos[j - PILLAR_HORIZONTAL] - m_MeshPos[j];
				v2 = m_MeshPos[j + 1] - m_MeshPos[j];
				v3 = m_MeshPos[j + PILLAR_HORIZONTAL] - m_MeshPos[j];

				// 外積により2つのベクトルにおける法線算出
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// 最終的なベクトル算出
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else if(j % PILLAR_HORIZONTAL == (PILLAR_HORIZONTAL - 1))
			{// 左端の点

				D3DXVECTOR3 n1, n2, v1, v2, v3;

				// 基準点からのベクトル算出
				v1 = m_MeshPos[j - PILLAR_HORIZONTAL] - m_MeshPos[j];
				v2 = m_MeshPos[j + 1] - m_MeshPos[j];
				v3 = m_MeshPos[j + PILLAR_HORIZONTAL] - m_MeshPos[j];

				// 外積により2つのベクトルにおける法線算出
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);

				// 最終的なベクトル算出
				nor = (n1 + n2) / 2.0f;
				D3DXVec3Normalize(&nor, &nor);
			}
			else
			{// 接点が6つの点

				D3DXVECTOR3 n1, n2, n3, n4, n5, n6, v1, v2, v3, v4, v5, v6;

				v1 = m_MeshPos[0] - m_MeshPos[0];
				v2 = m_MeshPos[0] - m_MeshPos[0];
				v3 = m_MeshPos[0] - m_MeshPos[0];
				v4 = m_MeshPos[0] - m_MeshPos[0];
				v5 = m_MeshPos[0] - m_MeshPos[0];
				v6 = m_MeshPos[0] - m_MeshPos[0];
				D3DXVec3Cross(&n1, &v1, &v2);
				D3DXVec3Normalize(&n1, &n1);
				D3DXVec3Cross(&n2, &v2, &v3);
				D3DXVec3Normalize(&n2, &n2);
				D3DXVec3Cross(&n3, &v3, &v4);
				D3DXVec3Normalize(&n3, &n3);
				D3DXVec3Cross(&n4, &v4, &v5);
				D3DXVec3Normalize(&n4, &n4);
				D3DXVec3Cross(&n5, &v5, &v6);
				D3DXVec3Normalize(&n5, &n5);
				D3DXVec3Cross(&n6, &v6, &v1);
				D3DXVec3Normalize(&n6, &n6);
				nor = (n1 + n2 + n3 + n4 + n5 + n6) / 6.0f;
				D3DXVec3Normalize(&nor, &nor);
			}

			// 法線をセット
			pVtx[j].Nor = nor;


			m_pVtxBuff[i]->Unlock();
		}
	}
}

void CPillar::Vector3Round(D3DXVECTOR3 *vec)
{
	(*vec).x = (float)(int)(*vec).x;
	(*vec).y = (float)(int)(*vec).y;
	(*vec).z = (float)(int)(*vec).z;
}