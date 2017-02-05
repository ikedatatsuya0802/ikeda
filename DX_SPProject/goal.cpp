//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	goal.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "goal.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CGoal::m_pTexture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CGoal::CGoal(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CGoal::~CGoal()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CGoal::Init(void)
{	
	// 各種初期化処理
	m_Pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);
	D3DXVec3Normalize(&m_Vec, &(CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL + 0.01f) - m_Pos));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[0], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[1], NULL);

	// レール情報セット
	SetVtxBuff();

	Load();

	CSceneXDX::Create(".\\data\\MODEL\\ekisha.x", NULL,
		D3DXVECTOR3(CGame::GetPlayer1()->GetPos().x, 0.0f, CGame::GetPlayer1()->GetPos().z + 300.0f),
		D3DXVECTOR3(0.0, D3DX_PI, 0.0f));
	CSceneXDX::Create(".\\data\\MODEL\\ekisha.x", NULL, D3DXVECTOR3(m_Pos.x, 0.0f, m_Pos.z));
}

//=============================================================================
//	関数名	:SetGoalData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CGoal::SetVtxBuff(void)
{
	VERTEX_3D	*pVtx;	// 3D頂点情報
	SPLINE*		spline = CGame::GetRailLine()->GetSpline();		// スプライン情報
	D3DXVECTOR3 posFar = VEC3_ZERO;
	float rot = 0.0f;

	posFar.x = m_Pos.x + m_Vec.x * RAIL_LENGTH;
	posFar.y = m_Pos.y;
	posFar.z = m_Pos.z + m_Vec.z * RAIL_LENGTH;

	rot = atan2f(m_Vec.x, m_Vec.z);

	m_pVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = posFar.x + (sinf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[0].Pos.y = 1.0f;
	pVtx[0].Pos.z = posFar.z + (cosf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	pVtx[1].Pos.x = posFar.x + (sinf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[1].Pos.y = 1.0f;
	pVtx[1].Pos.z = posFar.z + (cosf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	pVtx[2].Pos.x = m_Pos.x + (sinf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[2].Pos.y = 1.0f;
	pVtx[2].Pos.z = m_Pos.z + (cosf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	pVtx[3].Pos.x = m_Pos.x + (sinf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[3].Pos.y = 1.0f;
	pVtx[3].Pos.z = m_Pos.z + (cosf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	for(int nCntSet = 0 ; nCntSet < VERTEX_SQUARE ; nCntSet++)
	{
		// 法線設定
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 100.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 100.0f);

	m_pVtxBuff[0]->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CGoal::Uninit(void)
{
	SafetyRelease(m_pVtxBuff[0]);
	SafetyRelease(m_pVtxBuff[1]);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CGoal::Update(void)
{
	m_Pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);
	D3DXVec3Normalize(&m_Vec, &(CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL + 0.01f) - m_Pos));

	SetVtxBuff();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CGoal::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, VEC3_ZERO, m_Rot);

	// アルファ・Zテスト開始
	CRendererDX::EnableAlphaTest();
	CRendererDX::EnableZTest();

	// 描画処理
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);										// 頂点フォーマットの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[0], 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);									// テクスチャの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);	// 描画

	// アルファ・Zテスト終了
	CRendererDX::DisableAlphaTest();
	CRendererDX::DisableZTest();
}

//=============================================================================
//	関数名	:Create
//	引数	:無し
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CGoal *CGoal::Create(void)
{
	CGoal *instance;

	instance = new CGoal;
	
	instance->Init();

	return instance;
}