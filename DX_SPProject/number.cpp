//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	number.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "number.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CNumber::m_pTexture;

//=============================================================================
//	関数名	:CNumber()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CNumber::CNumber(int value) : CScene2DDX(true)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	関数名	:~CNumber()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CNumber::Init(int value, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	VERTEX_2D			*pVtx;										// 2D頂点情報


	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.z = 0.0f;
	
	pVtx[1].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.z = 0.0f;
	
	pVtx[2].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.z = 0.0f;
	
	pVtx[3].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.z = 0.0f;
	
	// 除算係数設定
	for(int cntRhw = 0 ; cntRhw < VERTEX_NUM ; cntRhw++)
	{
		pVtx[cntRhw].rhw = 1.0f;
	}
	
	// スコア色設定
	for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
	{
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 閾値チェック
	if(value < -1)
	{
		value = 0;
	}
	else if(value > 9)
	{
		value = 9;
	}
	
	if(value >= 0)
	{// 値が数の場合

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(((float)value * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(((float)value * 0.1f), 0.5f);
		pVtx[3].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.5f);
	}
	else
	{// 小数点の場合

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVtxBuff->Unlock();

	Load();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CNumber::Uninit(void)
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
void CNumber::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CNumber::Draw(void)
{
	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// 頂点フォーマットの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);
	// メーター描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CNumber *CNumber::Create(int value, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CNumber *number;	// インスタンス
	
	// インスタンス生成
	number = new CNumber();
	
	// 初期化処理
	number->Init(value, pos, size);
	
	// インスタンスをリターン
	return number;
}

//=============================================================================
//	関数名	:Create
//	引数	:int value(数値)
//	戻り値	:無し
//	説明	:正数値、あるいは小数点その他の記号をセットする。
//=============================================================================
void CNumber::SetNumber(int value)
{
	VERTEX_2D	*pVtx;	// 2D頂点情報

	// 閾値チェック
	if(value < -1)
	{
		value = 0;
	}
	else if(value > 9)
	{
		value = 9;
	}

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	if(value != -1)
	{// 値が数の場合

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(((float)value * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(((float)value * 0.1f), 0.5f);
		pVtx[3].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.5f);
	}
	else
	{// 小数点の場合

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVtxBuff->Unlock();
}