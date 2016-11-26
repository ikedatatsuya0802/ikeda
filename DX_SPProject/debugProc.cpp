//=============================================================================
//
//	タイトル	デバッグ情報処理
//	ファイル名	debugProc.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/26
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "debugProc.h"
#include "manager.h"
#include "rendererDX.h"
#include <stdio.h>

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPD3DXFONT	CDebugProc::m_pFont;
char		CDebugProc::m_aStrDebug[DEBUGSTR_MAX];

//=============================================================================
//	関数名	:InitDebugProc
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CDebugProc::Init(void)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス

	// フォント設定
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
	
	// メモリ初期化
	memset(m_aStrDebug, 0, sizeof(m_aStrDebug));

	return S_OK;
}

//=============================================================================
//	関数名	:UnInitDebugProc
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CDebugProc::Uninit(void)
{
	if(m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
//	関数名	:UpdateDebugProc
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CDebugProc::Update(void)
{

}

//=============================================================================
//	関数名	:DrawDebugProc
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	
	// デバッグ情報描画
	m_pFont->DrawText(NULL, &m_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	// メモリ初期化
	memset(m_aStrDebug, 0, sizeof(m_aStrDebug));
}

//=============================================================================
//	関数名:	PrintfDebugProc
//	引数	:char *fmt(可変引数、描画する文字列)
//	戻り値	:無し
//	説明	:疑似printf。
//=============================================================================
void CDebugProc::DebugProc(char *fmt, ...)
{
	va_list list;
	char str[256];

	va_start(list, fmt);

	vsprintf_s(str, fmt, list);

	strcat_s(m_aStrDebug, str);

	va_end(list);
}