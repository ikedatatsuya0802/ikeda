//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	pause.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "pause.h"
#include "main.h"
#include "game.h"
#include "player.h"
#include "fade.h"

//=============================================================================
//	関数名	:CPause()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CPause::CPause(bool ifListAdd, int priority, OBJTYPE objtype) : CScene2DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CPause()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CPause::Init(void)
{

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[0], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[1], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff[2], NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME000, &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME001, &m_pTexture[1]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME002, &m_pTexture[2]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME003, &m_pTexture[3]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PAUSE_TEXFILENAME004, &m_pTexture[4]);

	m_Pos		= D3DXVECTOR3(PAUSE_POSX, PAUSE_POSY, 0.0f);
	m_fLength	= hypotf(PAUSE_WIDTH, PAUSE_HEIGHT) * 0.5f;
	m_fAngle	= atan2f(PAUSE_WIDTH, PAUSE_HEIGHT);
	SetVtxBuff(&m_pVtxBuff[0]);

	m_Pos = D3DXVECTOR3(PAUSE_MENU_POSX, PAUSE_MENU_POSY, 0.0f);
	m_fLength = hypotf(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT) * 0.5f;
	m_fAngle = atan2f(PAUSE_MENU_WIDTH, PAUSE_MENU_HEIGHT);
	SetVtxBuff(&m_pVtxBuff[1]);

	CRendererDX::SetFullScreenVtx(&m_pVtxBuff[2]);
	VERTEX_2D	*pVtx;	// 3D頂点情報
	m_pVtxBuff[2]->Lock(0, 0, (void**)&pVtx, 0);
	{
		SIMPLE_FOR(4)
		{
			pVtx[i].col = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.7f);
		}
	}
	m_pVtxBuff[2]->Unlock();

	m_flgPause	= false;
	m_PauseMenu	= 0;
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CPause::SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff)
{
	VERTEX_2D	*pVtx;	// 3D頂点情報


	(*vtxBuff)->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ貼付座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	(*vtxBuff)->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CPause::Uninit(void)
{
	// インスタンス削除
	SIMPLE_FOR(PAUSE_TEXTURE_NUM)
	{
		SafetyRelease(m_pTexture[i]);
	}
	SafetyRelease(m_pVtxBuff[0]);
	SafetyRelease(m_pVtxBuff[1]);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CPause::Update(void)
{
	VERTEX_2D	*pVtx;	// 3D頂点情報

	if(KT_P)
	{
		ChangePause();
	}

	if(m_flgPause)
	{
		// ポーズメニュー操作
		if(KT_W || KRP_W)
		{// カーソルを上に

			m_PauseMenu--;
			if(m_PauseMenu < 0)
			{
				m_PauseMenu += (uint)PAUSE_MENU_MAX;
			}
		}
		else if(KT_S || KRP_S)
		{// カーソルを下に

			m_PauseMenu++;
			m_PauseMenu = m_PauseMenu % (uint)PAUSE_MENU_MAX;
		}

		// カーソル位置調整
		m_pVtxBuff[1]->Lock(0, 0, (void**)&pVtx, 0);
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, (m_PauseMenu / (float)PAUSE_MENU_MAX));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (m_PauseMenu / (float)PAUSE_MENU_MAX));
			pVtx[2].tex = D3DXVECTOR2(0.0f, ((m_PauseMenu / (float)PAUSE_MENU_MAX) + (1.0f / (float)PAUSE_MENU_MAX)));
			pVtx[3].tex = D3DXVECTOR2(1.0f, ((m_PauseMenu / (float)PAUSE_MENU_MAX) + (1.0f / (float)PAUSE_MENU_MAX)));
		}
		m_pVtxBuff[1]->Unlock();

		// ポーズメニュー決定
		if(KT_ENTER)
		{
			switch(m_PauseMenu)
			{
			case PAUSE_MENU_CONTINUE:	// 続ける
				ChangePause();
				break;
			case PAUSE_MENU_RETRY:		// 最初から
				ChangePause();
				CFade::Start(new CGame, FS_OUT);
				break;
			case PAUSE_MENU_QUIT:		// 終了
				ChangePause();
				CFade::Start(new CTitle, FS_OUT);
				break;
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
void CPause::Draw(void)
{
	if(m_flgPause)
	{
		// アルファテスト開始
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

		// 頂点フォーマットの設定
		D3D_DEVICE->SetFVF(FVF_VERTEX_2D);

		// 頂点バッファの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[2], 0, sizeof(VERTEX_2D));
		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, NULL);
		// プリミティブ描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

		// 頂点バッファの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[0], 0, sizeof(VERTEX_2D));
		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		// プリミティブ描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, m_pTexture[m_PauseMenu + 2]);
		// プリミティブ描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

		// 頂点バッファの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[1], 0, sizeof(VERTEX_2D));
		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, m_pTexture[1]);
		// プリミティブ描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

		// アルファテスト終了
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:無し
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *instance;	// インスタンス

	// インスタンス生成
	instance = new CPause;

	// 初期化処理
	instance->Init();

	// インスタンスをリターン
	return instance;
}