//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	driftMark.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "driftMark.h"
#include "railLine.h"
#include "player.h"
#include "game.h"

bool		CDriftMark::m_VisibleType	= false;
bool		CDriftMark::m_flgCounter	= true;
bool		CDriftMark::m_Curve			= true;
int			CDriftMark::m_Count			= 0;
int			CDriftMark::m_SetCounter	= 0;

//=============================================================================
//	関数名	:CScene2DDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CDriftMark::CDriftMark(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
}

//=============================================================================
//	関数名	:~CDriftMark()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CDriftMark::~CDriftMark()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CDriftMark::Init(void)
{
	// 各種初期化処理
	m_DriftMark.Pos = D3DXVECTOR3((SCREEN_WIDTH * 0.3f), (SCREEN_HEIGHT * 0.3f), 0.0f);
	m_DriftMark.Size = D3DXVECTOR2(DRIFTMARK_WIDTH, DRIFTMARK_HEIGHT);
	m_DriftMark.Col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	
	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(DRIFTMARK_TEXNAME000, TEX_FILEPASS), &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(DRIFTMARK_TEXNAME001, TEX_FILEPASS), &m_pTexture[1]);

	SetVtxBuff();
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CDriftMark::SetVtxBuff(void)
{
	VERTEX_2D	*pVtx;	// 3D頂点情報


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[1].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));
	pVtx[2].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[3].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));
	pVtx[4].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[5].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));
	pVtx[6].Pos.x = (m_DriftMark.Pos.x - (m_DriftMark.Size.x * 0.5f));
	pVtx[7].Pos.x = (m_DriftMark.Pos.x + (m_DriftMark.Size.x * 0.5f));

	pVtx[0].Pos.y = (m_DriftMark.Pos.y - (m_DriftMark.Size.y * 0.5f));
	pVtx[1].Pos.y = (m_DriftMark.Pos.y - (m_DriftMark.Size.y * 0.5f));
	pVtx[2].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));
	pVtx[3].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));

	pVtx[5].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));
	pVtx[4].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.1f));
	pVtx[6].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.5f));
	pVtx[7].Pos.y = (m_DriftMark.Pos.y + (m_DriftMark.Size.y * 0.5f));

	for(int i = 0 ; i < VERTEX_SQUARE * 2 ; i++)
	{
		pVtx[i].Pos.z = 0.0f;

		// 除算係数設定
		pVtx[i].rhw = 1.0f;

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 0.0f);
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.6f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.6f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.6f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.6f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CDriftMark::Uninit(void)
{
	// インスタンス削除
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture[0]);
	SafetyRelease(m_pTexture[1]);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CDriftMark::Update(void)
{
	static float tex = 0.0f;
	// テクスチャ座標移動
	VERTEX_2D	*pVtx;	// 3D頂点情報
	float oldt	= CGame::GetPlayer1()->GetOldPerSpline();
	float t		= CGame::GetPlayer1()->GetPerSpline();
	float futureoldt = CGame::GetPlayer1()->GetOldPerSpline() + DRIFTMARK_FUTURE;
	float futuret = CGame::GetPlayer1()->GetPerSpline() + DRIFTMARK_FUTURE;

	// ドリフトマーク出現判断
	if(CGame::GetRailLine()->GetDriftStatus(futureoldt, futuret).ifDrift)
	{
		if(CGame::GetRailLine()->GetDriftStatus(futureoldt, futuret).Status == -1)
		{// ドリフトの始点の場合

			if(CGame::GetRailLine()->GetDriftStatus(futureoldt, futuret).Curve)
			{// 右カーブの警告

				m_DriftMark.Pos = D3DXVECTOR3((SCREEN_WIDTH * 0.2f), (SCREEN_HEIGHT * 0.3f), 0.0f);
				SetVtxBuff();
				CDriftMark::VisibleDriftMark(true, true, 60);
			}
			else
			{// 左カーブの警告
				m_DriftMark.Pos = D3DXVECTOR3((SCREEN_WIDTH * 0.8f), (SCREEN_HEIGHT * 0.3f), 0.0f);
				SetVtxBuff();
				CDriftMark::VisibleDriftMark(true, false, 60);
			}
		}
	}

	// ドリフトマーク消去判断
	if(CGame::GetRailLine()->GetDriftStatus(oldt, t).ifDrift)
	{
		if(CGame::GetRailLine()->GetDriftStatus(oldt, t).Status == 1)
		{// ドリフトの終点の場合

			CDriftMark::InvisibleDriftMark(30);
		}
	}

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(tex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(tex + 1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(tex, 0.6f);
	pVtx[3].tex = D3DXVECTOR2(tex + 1.0f, 0.6f);
	m_pVtxBuff->Unlock();
	tex += m_Curve ? -DRIFTMARK_SRIDESPEED : DRIFTMARK_SRIDESPEED;

	if(m_VisibleType)
	{// 点滅

		float rad = D3DX_PI / m_SetCounter * (m_SetCounter - m_Count);
		float alpha = sinf(rad);

		if(m_flgCounter)
		{// カウンタを減らす

			// カウンタに応じてアルファ値を変動
			SetColor(alpha, WHITE);

			// カウンタを0に近づける
			m_Count--;

			if(m_Count == 0)
			{
				m_flgCounter = false;
			}
		}
		else
		{// カウンタを増やす

			// カウンタに応じてアルファ値を変動
			SetColor(alpha, WHITE);

			// カウンタを0に近づける
			m_Count++;

			if(m_Count == m_SetCounter)
			{
				m_flgCounter = true;
			}
		}
	}
	else
	{// 点灯

		if(m_Count != 0)
		{
			if(m_Count > 0)
			{// カウンタが正数

				// カウンタに応じてアルファ値を変動
				SetColor((1.0f / (float)m_SetCounter * (float)(m_SetCounter - m_Count)), WHITE);

				// カウンタを0に近づける
				m_Count--;
			}
			else
			{// カウンタが負数

				// カウンタに応じてアルファ値を変動
				SetColor((1.0f / fabsf((float)m_SetCounter) * fabsf((float)m_Count)), WHITE);

				// カウンタを0に近づける
				m_Count++;

				// カウンタが0になった場合、アルファ値を0に
				if(m_Count == 0)
				{
					SetColor(0.0f, WHITE);
				}
			}
		}
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CDriftMark::Draw(void)
{
	// Zテスト方法更新
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// 頂点フォーマットの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, (m_Curve ? m_pTexture[1] : m_pTexture[0]));
	// メーター描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);
	// メーター描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, PRIMITIVE_SQUARE);

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	/*
	// Zテスト方法更新
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3	pos		-> 初期位置
//			:D3DXVECTOR2	size	-> ポリゴンのサイズ
//			:char			*str	-> テクスチャのファイルパス
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CDriftMark* CDriftMark::Create(void)
{
	CDriftMark *instance;	// インスタンス

	// インスタンス生成
	instance = new CDriftMark;

	// 初期化処理
	instance->Init();

	// インスタンスをリターン
	return instance;
}

//=============================================================================
//	関数名	:SetColor
//	引数	:float	a		-> アルファ値
//			:float	r		-> R値
//			:float	g		-> G値
//			:float	b		-> B値
//	戻り値	:無し
//	説明	:ポリゴン色を設定する。
//=============================================================================
void CDriftMark::SetColor(float a, float r, float g, float b)
{
	VERTEX_2D	*pVtx;	// 2D頂点情報

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < 8 ; i++)
	{
		pVtx[i].col = D3DCOLOR_COLORVALUE(r, g, b, a);
	}

	m_pVtxBuff->Unlock();

}

//=============================================================================
//	関数名	:VisibleDriftMark
//	引数	:float	a		-> アルファ値
//	戻り値	:無し
//	説明	:ドリフトマークを表示する。
//=============================================================================
void CDriftMark::VisibleDriftMark(bool type, bool curve, int time)
{
	m_VisibleType = type;
	m_Curve = curve;
	m_Count = time;
	m_SetCounter = type ? (int)fabsf((float)time + 0.1f) : time;
}

//=============================================================================
//	関数名	:InvisibleDriftMark
//	引数	:float	a		-> アルファ値
//	戻り値	:無し
//	説明	:ドリフトマークを表示する。
//=============================================================================
void CDriftMark::InvisibleDriftMark(int time)
{
	// 絶対値を取得
	time = (int)fabsf((float)time + 0.01f);

	m_Count = -time;
	m_VisibleType = false;
}