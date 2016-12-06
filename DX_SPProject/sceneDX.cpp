//=============================================================================
//
//	タイトル	シーンファイル
//	ファイル名	scene.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/19
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "sceneDX.h"
#include "main.h"
#include "rendererDX.h"
#include "scene2DDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
//CSceneDX *CSceneDX::m_pTop[PRIORITY_NUM] = {NULL};
//CSceneDX *CSceneDX::m_pCur[PRIORITY_NUM] = {NULL};
CSceneDX *CSceneDX::m_pTop = {NULL};
CSceneDX *CSceneDX::m_pCur = {NULL};
LPDIRECT3DTEXTURE9 CSceneDX::m_pTexture;

//=============================================================================
//	関数名	:CSceneDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSceneDX::CSceneDX(int priority, OBJTYPE objType)
{
	if(m_pTop == NULL)
	{// リストに何も登録されてない場合
		// 自身のポインタをリスト先頭に登録
		m_pTop = this;

		// 先頭へ登録
		m_pPrev = NULL;
	}
	else
	{// リストに他に追加されている場合
		// 前項目を参照先に
		m_pPrev = m_pCur;

		// 前項目の次参照先を自身に
		m_pPrev->m_pNext = this;
	}
		
	// リストの終端ポインタに自身を登録
	m_pCur = this;

	// 次の項目は無い
	m_pNext = NULL;

	// 座標・回転の初期化
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_flgDraw = true;
}

//=============================================================================
//	関数名	:~CSceneDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSceneDX::~CSceneDX()
{
	CSceneDX *pScene = m_pTop;		// 参照先バッファ
	CSceneDX *pSceneNext = NULL;	// 次参照先バッファ
	CSceneDX *pScenePrev = NULL;	// 前参照先バッファ

	while(pScene)
	{
		pSceneNext = pScene->m_pNext;
		pScenePrev = pScene->m_pPrev;

		if(pScene == this)
		{
			if(pScene->m_pPrev)
			{
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			if(pScene->m_pNext)
			{
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			if(pScene == m_pTop)
			{
				m_pTop = pSceneNext;
			}

			if(pScene == m_pCur)
			{
				m_pCur = pScenePrev;
			}
		}
		pScene = pSceneNext;
	}
}

//=============================================================================
//	関数名	:UpdateAll
//	引数	:無し
//	戻り値	:無し
//	説明	:リストに追加されている全ての対象を更新する。
//=============================================================================
void CSceneDX::UpdateAll(void)
{
	CSceneDX *scene = m_pTop;	// リストの先頭ポインタ
	CSceneDX *sceneNext;		// リストの次参照先バッファ

	// リストの最後に到達するまで更新を続ける
	while(scene != NULL)
	{
		sceneNext = scene->m_pNext;

		// 更新
		scene->Update();

		// 次の項目へ
		scene = sceneNext;
	}
}

//=============================================================================
//	関数名	:DrawAll
//	引数	:無し
//	戻り値	:無し
//	説明	:リストに追加されている全ての対象を描画する。
//=============================================================================
void CSceneDX::DrawAll(void)
{
	CSceneDX *scene = m_pTop;	// リストの先頭ポインタ
	
	// リストの最後に到達するまで描画を続ける
	while(scene != NULL)
	{
		// 描画
		scene->Draw();

		// 次の項目へ
		scene = scene->m_pNext;
	}
}

//=============================================================================
//	関数名	:DeleteAll
//	引数	:無し
//	戻り値	:無し
//	説明	:リストに追加されている全ての対象を削除する。
//=============================================================================
void CSceneDX::DeleteAll(void)
{
	CSceneDX *pScene = m_pTop;	// リストの先頭ポインタ
	CSceneDX *pSceneNext;		// 次参照先
	
	while(pScene != NULL)
	{
		// 次参照先を退避
		pSceneNext = pScene->m_pNext;

		// 終了処理
		pScene->Uninit();

		// インスタンス削除
		if(pScene != NULL)
		{
			delete pScene;
		}

		// 次参照先をセット
		pScene = pSceneNext;
	}
	m_pTop = NULL;
}

//=============================================================================
//	関数名	:Release
//	引数	:無し
//	戻り値	:無し
//	説明	:対象を削除する。
//=============================================================================
void CSceneDX::Release(void)
{
	// 終了処理
	Uninit();

	if(this == m_pTop)
	{// リストの先頭の場合
		// リストの先頭を現在の次項目へ
		m_pTop = m_pNext;
	}
	else
	{// リストの先頭でない場合
		// 前項目の次参照先を現在の次参照先へ
		m_pPrev->m_pNext = m_pNext;
	}

	if(this == m_pCur)
	{// リストの終端の場合
		// リストの終端を現在の前項目へ
		m_pCur = m_pPrev;
	}
	else
	{// リストの終端でない場合
		// 次項目の前参照先を現在の前参照先へ
		m_pNext->m_pPrev = m_pPrev;
	}
	// インスタンス削除
	delete this;
}

//=============================================================================
//	関数名	:UnlinkList
//	引数	:無し
//	戻り値	:無し
//	説明	:対象をリストから外す。
//=============================================================================
void CSceneDX::UnlinkList(void)
{
	if(this == m_pTop)
	{// リストの先頭の場合
		// リストの先頭を現在の次項目へ
		m_pTop = m_pNext;
	}
	else
	{// リストの先頭でない場合
		// 前項目の次参照先を現在の次参照先へ
		m_pPrev->m_pNext = m_pNext;
	}

	if(this == m_pCur)
	{// リストの終端の場合
		// リストの終端を現在の前項目へ
		m_pCur = m_pPrev;
	}
	else
	{// リストの終端でない場合
		// 次項目の前参照先を現在の前参照先へ
		m_pNext->m_pPrev = m_pPrev;
	}

	// 対象の前後項目を消去
	this->m_pPrev = NULL;
	this->m_pNext = NULL;
}