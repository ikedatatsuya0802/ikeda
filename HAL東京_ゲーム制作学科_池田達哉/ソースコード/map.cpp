//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	map.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "map.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"
#include "game.h"
#include "railLine.h"
#include "player.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CMap::m_pTexture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CMap::CMap(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CMap::Init(D3DXVECTOR3 pos)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	m_Spline = CGame::GetRailLine()->GetSpline();

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[0], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[1], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline->PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[2], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[3], NULL);
	//D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[4], NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"MAP_TEXFILENAME000, &m_pTexture);
	
	// レール情報セット
	SetVtxBuff();

	Load();

	m_SplinePos = VEC3_ZERO;

	D3DXCreateTexture(D3D_DEVICE, (UINT)MAP_WIDTH, (UINT)MAP_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_pMapTexture);
	m_pMapTexture->GetSurfaceLevel(0, &m_pMapSurf);

	D3D_DEVICE->GetRenderTarget(0, &m_BackBufferSurf);
}

//=============================================================================
//	関数名	:SetMapData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CMap::SetVtxBuff(void)
{
	VERTEX_2D		* pVtx;		// 3D頂点情報


	m_pVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(m_Pos.x - (MAP_WIDTH / 2), m_Pos.y - (MAP_WIDTH / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3(m_Pos.x + (MAP_WIDTH / 2), m_Pos.y - (MAP_WIDTH / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(m_Pos.x - (MAP_WIDTH / 2), m_Pos.y + (MAP_WIDTH / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3(m_Pos.x + (MAP_WIDTH / 2), m_Pos.y + (MAP_WIDTH / 2), 0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].rhw = 1.0f;

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	m_pVtxBuff[0]->Unlock();


	m_pVtxBuff[1]->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(- (MAP_WIDTH / 2), - (MAP_WIDTH / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3(+ (MAP_WIDTH / 2), - (MAP_WIDTH / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(- (MAP_WIDTH / 2), + (MAP_WIDTH / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3(+ (MAP_WIDTH / 2), + (MAP_WIDTH / 2), 0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].rhw = 1.0f;

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.5f);

			// テクスチャ座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	m_pVtxBuff[1]->Unlock();


	m_pVtxBuff[2]->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++)
		{
			D3DXVECTOR3 pos = VEC3_ZERO;

			// 座標設定
			if(i == (int)m_Spline->PosHermite.size())
			{
				pos.x = m_Spline->PosHermite[(int)m_Spline->Pos.size() - 1].x;
				pos.y = -m_Spline->PosHermite[(int)m_Spline->Pos.size() - 1].z;
			}
			else
			{
				pos.x = m_Spline->PosHermite[i].x;
				pos.y = -m_Spline->PosHermite[i].z;
			}

			pos *= MAP_POINT_ASPECT;

			pVtx[i].Pos.x = MAP_START_X + pos.x;
			pVtx[i].Pos.y = MAP_START_Y + pos.y;
			pVtx[i].Pos.z = 0.0f;

			// 法線設定
			pVtx[i].rhw = 1.0f;

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標設定
			pVtx[i].tex = VEC2_ZERO;
		}
	}
	m_pVtxBuff[2]->Unlock();

	m_pVtxBuff[3]->Lock(0, 0, (void**)&pVtx, 0);
	{
		float fLength = hypotf(MAP_POINT_WIDTH, MAP_POINT_HEIGHT) * 0.5f;
		float fAngle = atan2f(MAP_POINT_WIDTH, MAP_POINT_HEIGHT);
		float rot = CGame::GetPlayer1()->GetSplineRot().y + D3DX_PI;
		// 座標設定
		/*pVtx[0].Pos = D3DXVECTOR3(m_Pos.x - (MAP_POINT_WIDTH / 2), m_Pos.y - (MAP_POINT_WIDTH / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3(m_Pos.x + (MAP_POINT_WIDTH / 2), m_Pos.y - (MAP_POINT_WIDTH / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(m_Pos.x - (MAP_POINT_WIDTH / 2), m_Pos.y + (MAP_POINT_WIDTH / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3(m_Pos.x + (MAP_POINT_WIDTH / 2), m_Pos.y + (MAP_POINT_WIDTH / 2), 0.0f);*/
		pVtx[0].Pos = D3DXVECTOR3((MAP_START_X + m_SplinePos.x) - (sinf(fAngle - rot) * fLength),
			(MAP_START_Y + m_SplinePos.y) - (cosf(fAngle - rot) * fLength),
			0.0f);
		pVtx[1].Pos = D3DXVECTOR3((MAP_START_X + m_SplinePos.x) - (sinf(-fAngle - rot) * fLength),
			(MAP_START_Y + m_SplinePos.y) - (cosf(-fAngle - rot) * fLength),
			0.0f);
		pVtx[2].Pos = D3DXVECTOR3((MAP_START_X + m_SplinePos.x) - (sinf(-fAngle - rot + D3DX_PI) * fLength),
			(MAP_START_Y + m_SplinePos.y) - (cosf(-fAngle - rot + D3DX_PI) * fLength),
			0.0f);
		pVtx[3].Pos = D3DXVECTOR3((MAP_START_X + m_SplinePos.x) - (sinf(fAngle - rot - D3DX_PI) * fLength),
			(MAP_START_Y + m_SplinePos.y) - (cosf(fAngle - rot - D3DX_PI) * fLength),
			0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].rhw = 1.0f;

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 0.7f);
		}

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuff[3]->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CMap::Uninit(void)
{
	SafetyRelease(m_pVtxBuff[0]);
	SafetyRelease(m_pVtxBuff[1]);
	SafetyRelease(m_pVtxBuff[2]);
	SafetyRelease(m_pVtxBuff[3]);
	SafetyRelease(m_pTexture);

	SafetyRelease(m_BackBufferSurf);
	SafetyRelease(m_pMapSurf);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CMap::Update(void)
{
	m_SplinePos.x = CGame::GetRailLine()->GetSplinePos(CGame::GetPlayer1()->GetPerSpline()).x;
	m_SplinePos.y = -CGame::GetRailLine()->GetSplinePos(CGame::GetPlayer1()->GetPerSpline()).z;
	m_SplinePos *= MAP_POINT_ASPECT;

	// レール情報セット
	SetVtxBuff();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CMap::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト方法更新
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	D3D_DEVICE->SetRenderTarget(0, m_pMapSurf);

	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);	// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, NULL);	// テクスチャの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[1], 0, sizeof(VERTEX_2D));	// 頂点フォーマットの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);		// 描画

	D3D_DEVICE->SetTexture(0, NULL);	// テクスチャの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[2], 0, sizeof(VERTEX_2D));			// 頂点フォーマットの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_LINESTRIP, 0, m_Spline->PosHermite.size() - 1);	// 描画

	D3D_DEVICE->SetTexture(0, m_pTexture);	// テクスチャの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[3], 0, sizeof(VERTEX_2D));	// 頂点フォーマットの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);		// 描画
	D3D_DEVICE->SetRenderTarget(0, m_BackBufferSurf);

	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);	// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pMapTexture);	// テクスチャの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[0], 0, sizeof(VERTEX_2D));	// 頂点フォーマットの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);		// 描画

	/*
	// Zテスト方法更新
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CMap *CMap::Create(D3DXVECTOR3 pos)
{
	CMap *scene3D;

	scene3D = new CMap;
	
	scene3D->Init(pos);

	return scene3D;
}