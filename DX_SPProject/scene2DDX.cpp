//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	scene2DDX.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "scene2DDX.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	関数名	:CScene2DDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CScene2DDX::CScene2DDX(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneDX(ifListAdd, priority, objtype)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
	m_pTexture	= NULL;
	m_pVtxBuff	= NULL;
}

//=============================================================================
//	関数名	:~CScene2DDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CScene2DDX::~CScene2DDX()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CScene2DDX::Init(cchar *str, cVec3 pos, cVec2 size, cfloat rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, rot));
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// 頂点バッファ設定
	SetVtxBuff();

	// テクスチャのロード
	Load(str);
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CScene2DDX::SetVtxBuff(void)
{
	VERTEX_2D	*pVtx;	// 3D頂点情報


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

	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		// 除算係数設定
		pVtx[i].rhw = 1.0f;

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CScene2DDX::Uninit(void)
{
	Unload();

	// インスタンス削除
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CScene2DDX::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CScene2DDX::Draw(void)
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
//	関数名	:Create
//	引数	:D3DXVECTOR3	pos		-> 初期位置
//			:D3DXVECTOR2	size	-> ポリゴンのサイズ
//			:char			*str	-> テクスチャのファイルパス
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CScene2DDX *CScene2DDX::Create(cchar *str, cVec3 pos, cVec2 size, cfloat rot)
{
	CScene2DDX *instance;	// インスタンス

	// インスタンス生成
	instance = new CScene2DDX();

	// 初期化処理
	instance->Init(str, pos, size, rot);

	// インスタンスをリターン
	return instance;
}

//=============================================================================
//	関数名	:SetColor
//	引数	:bool	mode	-> 変更モード、trueで倍率指定、falseで値指定
//			:float	x		-> 幅(倍率)
//			:float	y		-> 高さ(倍率)
//	戻り値	:無し
//	説明	:ポリゴン色を設定する。
//=============================================================================
void CScene2DDX::SetSize(cbool mode, cfloat x, cfloat y)
{
	VERTEX_2D	*pVtx;	// 2D頂点情報

	if(mode)
	{// 倍率指定
		
		float nx = (x * sinf(m_fAngle - m_Rot.z) * m_fLength);
		float ny = (y * cosf(m_fAngle - m_Rot.z) * m_fLength);

		m_fLength = hypotf(nx, ny);
		m_fAngle = atan2f(nx, ny);
	}
	else
	{// 値指定

		m_fLength = hypotf(x, y) * 0.5f;
		m_fAngle = atan2f(x, y);
	}


	// 頂点バッファに反映
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));

	pVtx[1].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));

	pVtx[2].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));

	pVtx[3].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));

	m_pVtxBuff->Unlock();
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
void CScene2DDX::SetColor(cfloat a, cfloat r, cfloat g, cfloat b)
{
	VERTEX_2D	*pVtx;	// 2D頂点情報

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		pVtx[i].col = D3DCOLOR_COLORVALUE(r, g, b, a);
	}
	
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:SetUV
//	引数	:cVec2	leftTop		-> 左上UV座標
//			:cVec2	rightButtom	-> 右下UV座標
//	戻り値	:無し
//	説明	:ポリゴンのUV座標を設定する。
//=============================================================================
void CScene2DDX::SetUV(cVec2 leftTop, cVec2 rightButtom)
{
	VERTEX_2D	*pVtx;	// 2D頂点情報

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(leftTop.x, leftTop.y);
	pVtx[1].tex = D3DXVECTOR2(rightButtom.x, leftTop.y);
	pVtx[2].tex = D3DXVECTOR2(leftTop.x, rightButtom.y);
	pVtx[3].tex = D3DXVECTOR2(rightButtom.x, rightButtom.y);

	m_pVtxBuff->Unlock();
}