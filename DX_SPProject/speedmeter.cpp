//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	speedmeter.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "speedmeter.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	関数名	:CSpeedmeter()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSpeedmeter::CSpeedmeter(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	関数名	:~CSpeedmeter()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSpeedmeter::~CSpeedmeter()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CSpeedmeter::Init(cVec3 pos, cVec3 rot, cVec2 size)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"SPEEDMETER_TEXFILENAME000, &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"SPEEDMETER_TEXFILENAME001, &m_pTexture[1]);
	
	SetVtxBuff();
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, SPEEDMETER_ROT_LOW);
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CSpeedmeter::SetVtxBuff(void)
{
	VERTEX_2D	*pVtx;	// 3D頂点情報


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[2].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[3].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));

	pVtx[4].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[4].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[5].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[5].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[6].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[6].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[7].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[7].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));


	for(int i = 0 ; i < VERTEX_SQUARE * 2 ; i++)
	{
		pVtx[i].Pos.z = 0.0f;

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
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
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
void CSpeedmeter::Uninit(void)
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
void CSpeedmeter::Update(void)
{
	VERTEX_2D	*pVtx;	// 3D頂点情報
	CPlayer* player = CGame::GetPlayer1();


	// メーター角度の補正
	//m_Rot.z = player->GetSpeed() * (4.0f);
	m_Rot.z = ((player->GetSpeed() / PLAYER_SPEED_MAX) * 4.0f);
	m_Rot.z -= 2.0f;

	// 各種スコア情報の更新
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標の更新
		pVtx[4].Pos.x = m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength);
		pVtx[4].Pos.y = m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength);

		pVtx[5].Pos.x = m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength);
		pVtx[5].Pos.y = m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength);

		pVtx[6].Pos.x = m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength);
		pVtx[6].Pos.y = m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength);

		pVtx[7].Pos.x = m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength);
		pVtx[7].Pos.y = m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength);
	}
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSpeedmeter::Draw(void)
{
	// アルファテスト開始
	CRendererDX::EnableAlphaTest();

	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// 頂点バッファの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, m_pTexture[1]);
	// プリミティブ描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, PRIMITIVE_SQUARE);

	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, m_pTexture[0]);
	// プリミティブ描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);

	// アルファテスト終了
	CRendererDX::DisableAlphaTest();
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3	pos		-> 初期位置
//			:D3DXVECTOR3	rot		-> 回転量
//			:D3DXVECTOR2	size	-> ポリゴンのサイズ
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CSpeedmeter *CSpeedmeter::Create(cVec3 pos, cVec3 rot, cVec2 size)
{
	CSpeedmeter *instance;	// インスタンス

	// インスタンス生成
	instance = new CSpeedmeter();

	// 初期化処理
	instance->Init(pos, rot, size);

	// インスタンスをリターン
	return instance;
}