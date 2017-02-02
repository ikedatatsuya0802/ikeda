//=============================================================================
//
//	タイトル	高架
//	ファイル名	EStructure.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "EStructure.h"
#include "manager.h"
#include "rendererDX.h"
#include "manager.h"
#include "cameraDX.h"
#include "game.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CEStructure::m_pTexture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CEStructure::CEStructure(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CEStructure::~CEStructure()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CEStructure::Init(D3DXVECTOR3 pos)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ESTRUCTURE_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\wall000.jpg", &m_pTexture);
	
	// レール情報セット
	SetVtxBuff();

	// インデックスセット
	SetMeshIndex(&m_pIdxBuff, ESTRUCTURE_HORIZONTAL, ESTRUCTURE_VERTICAL);

	Load();
}

//=============================================================================
//	関数名	:SetEStructureData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CEStructure::SetVtxBuff(void)
{
	VERTEX_3D *pVtx;		// 3D頂点情報

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;

		for(int y = 0 ; y < ESTRUCTURE_VERTICAL ; y++)
		{			
			D3DXVECTOR3 splPos = m_Spline->PosHermite[y];			
			splPos.y = (m_Spline->PosHermite[y].y < 2.0f) ? -100.0f : (splPos.y - 1.0f);
			D3DXVECTOR3 vec = VEC3_ZERO;

			// 角度設定
			if(y != ((int)m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[y + 1].x - m_Spline->PosHermite[y].x), (m_Spline->PosHermite[y + 1].z - m_Spline->PosHermite[y].z));
			}
			else
			{
				rot = atan2f((m_Spline->PosHermite[y].x - m_Spline->PosHermite[y - 1].x), (m_Spline->PosHermite[y].z - m_Spline->PosHermite[y - 1].z));
			}

			float cosWidth = (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f));	// 線路幅のX成分
			float sinWidth = (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));	// 線路幅のZ成分
			float cosWidthOuter = cosWidth * 1.1f;						// 外壁幅のX成分
			float sinWidthOuter = sinWidth * 1.1f;						// 外壁幅のZ成分


			for(int x = 0 ; x <= ESTRUCTURE_HORIZONTAL ; x++)
			{
				D3DXVECTOR3 pos = VEC3_ZERO;

				switch(x)
				{
				case 0:		// 基準点
					pos = D3DXVECTOR3(splPos.x, splPos.y, splPos.z);
					break;
				case 1:		// 基準左・内壁下部
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						splPos.y,
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 2:		// 基準左・内壁上部
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 3:		// 基準左・外壁上部
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 4:		// 基準左・外壁下部
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y - (ESTRUCTURE_WALL_HEIGHT * 0.2f)),
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 5:		// 基準真下の点
					pos = D3DXVECTOR3(splPos.x,	(splPos.y - (ESTRUCTURE_WALL_HEIGHT * 0.3f)), splPos.z);
					break;
				case 6:		// 基準右・外壁下部
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y - (ESTRUCTURE_WALL_HEIGHT * 0.2f)),
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 7:		// 基準右・外壁上部
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 8:		// 基準右・内壁上部
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 9:		// 基準右・内壁下部
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						splPos.y,
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 10:	// 基準点
					pos = D3DXVECTOR3(splPos.x, splPos.y, splPos.z);
					break;
				default:
					break;
				}

				int i = y * (ESTRUCTURE_HORIZONTAL + 1) + x;
				pVtx[i].Pos = pos;
			}
		}

		for(int i = 0 ; i < ESTRUCTURE_VERTEX_NUM ; i++)
		{
			// 法線設定
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標設定
			pVtx[i].tex = D3DXVECTOR2((float)(i % 2), (float)i * 0.75f);
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
void CEStructure::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CEStructure::Update(void)
{
	static SPLINE spline = *CGame::GetRailLine()->GetSpline();

	// 描画フラグ設定
	m_flgDraw = DX_CAMERA->GetCameraMode() ? false : true;

	// レールがエディットされた場合、更新
	if(fabsf(spline.Length - m_Spline->Length) < 0.01f)
	{
		// レール情報セット
		SetVtxBuff();
	}

	spline = (*m_Spline);
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CEStructure::Draw(void)
{
	//if(m_flgDraw)
	{
		// マトリックス設定
		CRendererDX::SetMatrix(&m_mtxWorld, VEC3_ZERO);

		// Zテスト開始
		CRendererDX::EnableZTest();

		// ライティング設定をオフに
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 描画処理
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
		D3D_DEVICE->SetIndices(m_pIdxBuff);									// インデックスバッファのバインド
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
		D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
		D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ESTRUCTURE_VERTEX_NUM, 0, ESTRUCTURE_POLYGON_NUM);

		// ライティング設定をオンに
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Zテスト終了
		CRendererDX::DisableZTest();
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CEStructure *CEStructure::Create(D3DXVECTOR3 pos)
{
	CEStructure *instance;

	instance = new CEStructure;
	
	instance->Init(pos);

	return instance;
}