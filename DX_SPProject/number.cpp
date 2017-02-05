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
LPDIRECT3DTEXTURE9	CNumber::m_pTexture = NULL;

//=============================================================================
//	関数名	:CNumber()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CNumber::CNumber(bool ifListAdd, int priority, OBJTYPE objType) : CScene2DDX(ifListAdd, priority, objType)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
	m_pTexture	= NULL;
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
	VERTEX_2D *pVtx;	// 2D頂点情報

	// 閾値チェック
	(value) > 9 ? value = 9 : 0;
	(value) < 0 ? value = 0 : 0;

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
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
	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		pVtx[i].rhw = 1.0f;

		// スコア色設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(((float)value * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(((float)value * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 1.0f);
	
	m_pVtxBuff->Unlock();

	Load();
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
	CRendererDX::EnableAlphaTest();

	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);

	// 頂点フォーマットの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);

	// 描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);

	// アルファテスト終了
	CRendererDX::DisableAlphaTest();
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
	//Unload();
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
	(value) > 9 ? value = 9 : 0;
	(value) < 0 ? value = 0 : 0;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(((float)value * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(((float)value * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 1.0f);

	m_pVtxBuff->Unlock();
}