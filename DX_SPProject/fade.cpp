//=============================================================================
//
//	タイトル	フェード処理
//	ファイル名	fade.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/07/12
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "input.h"
#include "scene2DDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
CMode		*CFade::m_NextMode;
FADESTATUS	CFade::m_FadeState;
FADE		CFade::m_Fade[2];		// フェード
LPDIRECT3DVERTEXBUFFER9	CFade::m_pVtxBuff;
LPDIRECT3DTEXTURE9		CFade::m_pTexture[2];

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CFade::Init(void)
{
	VERTEX_2D			*pVtx;	// 頂点情報

	
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\fade000.jpg", &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\fade001.jpg", &m_pTexture[1]);

	// フェード情報の初期設定
	m_Fade[0].Pos		= D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f);
	m_Fade[0].Rot		= VEC3_ZERO;
	m_Fade[0].fLength	= hypotf((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT) * 0.5f;
	m_Fade[0].fAngle	= atan2f((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT);
	
	m_Fade[1].Pos		= D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f);
	m_Fade[1].Rot		= VEC3_ZERO;
	m_Fade[1].fLength	= hypotf((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT) * 0.5f;
	m_Fade[1].fAngle	= atan2f((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT);

	m_FadeState.State			= FS_FIRST;
	m_FadeState.nCntFade		= FADE_CLOSETIME;
	m_FadeState.fLength			= (SCREEN_WIDTH * 0.25f);	// フェード同士の距離の初期設定
	m_FadeState.nCntPhaseNone	= PHASETIME_NONE;			// 初期フェーズカウントの初期化

	// フェードの初期設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < 2 ; i++)
	{
		// 描画座標設定
		pVtx[i * VERTEX_NUM + 0].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 0].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 0].Pos.z = 0.0f;
	
		pVtx[i * VERTEX_NUM + 1].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.z = 0.0f;
	
		pVtx[i * VERTEX_NUM + 2].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.z = 0.0f;
	
		pVtx[i * VERTEX_NUM + 3].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.z = 0.0f;
	
		// 除算係数設定
		for(int nCntRhw = 0 ; nCntRhw < VERTEX_NUM ; nCntRhw++)
		{
			pVtx[i * VERTEX_NUM + nCntRhw].rhw = 1.0f;
		}
	
		// フェード色設定
		for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
		{
			pVtx[i * VERTEX_NUM + nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}
	
		// テクスチャ座標設定
		pVtx[i * VERTEX_NUM + 0].tex = VEC2_ZERO;
		pVtx[i * VERTEX_NUM + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i * VERTEX_NUM + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[i * VERTEX_NUM + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CFade::Uninit(void)
{
	for(int i = 0 ; i < 2 ; i++)
	{
		if(m_pTexture[i] != NULL)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = NULL;
		}
	}
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CFade::Update(void)
{
	VERTEX_2D	*pVtx;	// 頂点情報

	if(m_FadeState.State == FS_FIRST)
	{// 初期フェーズ
		m_FadeState.nCntPhaseNone--;

		if(m_FadeState.nCntPhaseNone <= 0)
		{// フェードを初期化
			m_FadeState.nCntPhaseNone = PHASETIME_NONE;
			m_FadeState.fLength = (SCREEN_WIDTH * 0.25f);
			m_FadeState.State = FS_IN;
		}
		else if(m_FadeState.nCntPhaseNone == FADE_CLOSETIME * 0.5f)
		{// モードを変更
			if(m_NextMode != NULL)
			{
				CManager::SetMode(m_NextMode);
				m_NextMode = NULL;
			}
		}
	}
	else if(m_FadeState.State == FS_IN)
	{// フェードインの時
		
		if(m_FadeState.nCntFade == FADE_CLOSETIME)
		{// フェードの間隔を広げる
			m_FadeState.fLength += FADE_CLOSESPEED;
			//KeyStateLock(FADE_CLOSETIME);
		}

		if(m_FadeState.fLength >= SCREEN_WIDTH / cosf(m_Fade[0].Rot.z))
		{// ２つのフェードが無くなった場合
			m_FadeState.fLength = SCREEN_WIDTH / cosf(m_Fade[0].Rot.z);
			m_FadeState.State = FS_NONE;
		}
	}
	else if(m_FadeState.State == FS_OUT)
	{// フェードアウトの時
		
		if(m_FadeState.nCntFade == FADE_CLOSETIME)
		{// フェードの間隔を狭める

			m_FadeState.fLength -= FADE_CLOSESPEED;
		}

		if(m_FadeState.fLength <= (SCREEN_WIDTH * 0.25f))
		{// ２つのフェードがくっついた場合
			
			m_FadeState.nCntFade--;
			if(m_FadeState.nCntFade <= 0)
			{// フェードを初期化

				//PlaySound(SOUND_LABEL_SE002);
				m_FadeState.nCntFade = FADE_CLOSETIME;
				m_FadeState.fLength = (SCREEN_WIDTH * 0.25f);
				m_FadeState.State = FS_IN;
			}
			else if(m_FadeState.nCntFade == FADE_CLOSETIME * 0.5f)
			{// モードを変更
				if(m_NextMode != NULL)
				{
					CManager::SetMode(m_NextMode);
					m_NextMode = NULL;
				}
			}
		}
	}
	
	// フェードの座標更新
	m_Fade[0].Pos.x = (SCREEN_WIDTH * 0.5f) - m_FadeState.fLength * cosf(m_Fade[0].Rot.z);
	m_Fade[0].Pos.y = (SCREEN_HEIGHT * 0.5f) - m_FadeState.fLength * sinf(m_Fade[0].Rot.z);
	m_Fade[1].Pos.x = (SCREEN_WIDTH * 0.5f) + m_FadeState.fLength * cosf(m_Fade[1].Rot.z);
	m_Fade[1].Pos.y = (SCREEN_HEIGHT * 0.5f) + m_FadeState.fLength * sinf(m_Fade[1].Rot.z);

	// フェードの座標反映
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < 2 ; i++)
	{
		pVtx[i * VERTEX_NUM + 0].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 0].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CFade::Draw(void)
{
	if(m_FadeState.State != FS_NONE)
	{
		// 頂点フォーマットの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
		// 頂点フォーマットの設定
		D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		// フェード描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (VERTEX_NUM * 0), PRIMITIVE_NUM);
		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, m_pTexture[1]);
		// フェード描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (VERTEX_NUM * 1), PRIMITIVE_NUM);
	}
}

//=============================================================================
//	関数名	:Start
//	引数	:無し
//	戻り値	:無し
//	説明	:フェードを開始する。
//=============================================================================
void CFade::Start(CMode *nextMode, FADE_STATE fadeState)
{
	if((nextMode != NULL) && (m_FadeState.State == FS_NONE))
	{
		m_NextMode = nextMode;
		m_FadeState.State = fadeState;
	}
}