//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	countdown.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "countdown.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	関数名	:CCountdown()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CCountdown::CCountdown(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	関数名	:~CCountdown()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CCountdown::~CCountdown()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CCountdown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	m_Rot = VEC3_ZERO;
	m_Alpha = 0.0f;
	m_Tex = 0.0f;
	m_SizePow = 1.0f;
	m_Size = size;
	m_fLength	= hypotf((m_Size.x * m_SizePow), (m_Size.y * m_SizePow)) * 0.5f;
	m_fAngle	= atan2f((m_Size.x * m_SizePow), (m_Size.y * m_SizePow));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"COUNTDOWN_TEXFILENAME000, &m_pTexture);
	

	SetVtxBuff();
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CCountdown::SetVtxBuff(void)
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

	for(int i = 0 ; i < VERTEX_NUM ; i++)
	{
		// 除算係数設定
		pVtx[i].rhw = 1.0f;

		// 頂点色設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, m_Alpha);
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, m_Tex);
	pVtx[1].tex = D3DXVECTOR2(1.0f, m_Tex);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_Tex + 0.25f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, m_Tex + 0.25f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CCountdown::Uninit(void)
{
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
void CCountdown::Update(void)
{
	int frame = CGame::GetFrame();


	m_fLength = hypotf((m_Size.x * m_SizePow), (m_Size.y * m_SizePow)) * 0.5f;
	m_fAngle = atan2f((m_Size.x * m_SizePow), (m_Size.y * m_SizePow));
	
	if(frame >= (COUNTDOWN_START + MINUTE * COUNTDOWN_MINUTE))
	{
		if(frame == (COUNTDOWN_START + MINUTE * COUNTDOWN_MINUTE) + 1) m_SizePow = 1.0f;

		m_Tex = 0.75f;

		m_Alpha -= (1.0f / MINUTE);
		if(m_Alpha < 0.0f) m_Alpha = 0.0f;

		m_SizePow += (1.0f / MINUTE * 1.5f);
	}
	else if(frame > COUNTDOWN_START)
	{
		if(frame % MINUTE == 1) m_SizePow = 1.0f;

		if((frame - (MINUTE * COUNTDOWN_MINUTE)) / MINUTE == 0)
		{// カウント3

			m_Tex = 0.0f;

			m_Alpha = ((frame - (MINUTE * COUNTDOWN_MINUTE)) % MINUTE) / (float)MINUTE;

			m_SizePow += (1.0f / MINUTE * COUNTDOWN_SIZE_POW);
		}
		else if((frame - (MINUTE * COUNTDOWN_MINUTE)) / MINUTE == 1)
		{// カウント2

			m_Tex = 0.25f;

			m_Alpha = ((frame - (MINUTE * COUNTDOWN_MINUTE)) % MINUTE) / (float)MINUTE;

			m_SizePow += (1.0f / MINUTE * COUNTDOWN_SIZE_POW);
		}
		else if((frame - (MINUTE * COUNTDOWN_MINUTE)) / MINUTE == 2)
		{// カウント1

			m_Tex = 0.5f;

			m_Alpha = ((frame - (MINUTE * COUNTDOWN_MINUTE)) % MINUTE) / (float)MINUTE;

			m_SizePow += (1.0f / MINUTE * COUNTDOWN_SIZE_POW);
		}
	}

	SetVtxBuff();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CCountdown::Draw(void)
{
	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
	// 頂点バッファの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);
	// プリミティブ描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3	pos		-> 初期位置
//			:D3DXVECTOR2	size	-> ポリゴンのサイズ
//			:char			*str	-> テクスチャのファイルパス
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CCountdown *CCountdown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CCountdown *instance;	// インスタンス

	// インスタンス生成
	instance = new CCountdown;

	// 初期化処理
	instance->Init(pos, size);

	// インスタンスをリターン
	return instance;
}