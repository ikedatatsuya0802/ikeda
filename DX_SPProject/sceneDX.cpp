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
#include "manager.h"
#include "cameraDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
list<CSceneDX*>	CSceneDX::m_List[PRIORITY_MAX];

//=============================================================================
//	関数名	:CSceneDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSceneDX::CSceneDX(bool ifListAdd, int priority, OBJTYPE objType)
{
	// リスト追加フラグがオンの場合、リストに自身を追加
	if(ifListAdd) m_List[priority].push_back(this);

	// オブジェクトタイプを初期化
	m_ObjType = objType;

	// 座標・回転の初期化
	m_Pos = VEC3_ZERO;
	m_Rot = VEC3_ZERO;

	m_flgDraw		= true;
	m_DebugProcCnt	= 0;
}

//=============================================================================
//	関数名	:~CSceneDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSceneDX::~CSceneDX()
{

}

//=============================================================================
//	関数名	:UpdateAll
//	引数	:無し
//	戻り値	:無し
//	説明	:リストに追加されている全ての対象を更新する。
//=============================================================================
void CSceneDX::UpdateAll(void)
{
	// 全リストを検索
	for(int i = (PRIORITY_MAX - 1) ; i >= 0 ; i--)
	{
		// リストに登録されている全ての要素に更新処理を行う
		for each(CSceneDX* list in m_List[i])
		{
			// 更新処理
			list->Update();
		}
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
	// 全リストを検索
	for(int i = (PRIORITY_MAX - 1) ; i >= 0 ; i--)
	{
		// リストに登録されている全ての要素に描画処理を行う
		for each(CSceneDX* list in m_List[i])
		{
			// 描画フラグがオンの場合のみ描画
			if(list->GetDrawFrag())
			{
				if(i == PRIORITY_3D)
				{
					if(!CManager::GetEdhitMode())
					{
						// フォグを有効にする
						D3D_DEVICE->SetRenderState(D3DRS_FOGENABLE, TRUE);
					}
					else
					{
						// フォグを無効にする
						D3D_DEVICE->SetRenderState(D3DRS_FOGENABLE, FALSE);
					}
				}

				// 描画処理
				list->Draw();

				// フォグを無効にする
				D3D_DEVICE->SetRenderState(D3DRS_FOGENABLE, FALSE);
			}
		}
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
	list<CSceneDX*>::iterator itr;	// リストのイテレータ

	// 全リストを検索
	for(int i = 0 ; i < PRIORITY_MAX ; i++)
	{
		// リストに登録されている全ての要素を削除する
		for(itr = m_List[i].begin() ; itr != m_List[i].end() ; )
		{
			// インスタンスが存在している場合のみ処理
			if(*itr)
			{
				// 終了処理
				(*itr)->Uninit();

				// インスタンス削除
				delete (*itr);

				(*itr) = NULL;
			}

			// リストから削除
			itr = m_List[i].erase(itr);
		}
	}
}

//=============================================================================
//	関数名	:Release
//	引数	:無し
//	戻り値	:無し
//	説明	:対象を削除する。
//=============================================================================
void CSceneDX::Release(void)
{
	list<CSceneDX*>::iterator itr;	// リストのイテレータ

	// 全リストを検索
	for(int i = 0 ; i < PRIORITY_MAX ; i++)
	{
		// リストから自身のインスタンスを探索する
		for(itr = m_List[i].begin() ; itr != m_List[i].end() ; itr++)
		{
			// 自身のインスタンスを見つけ、リストから削除
			if(*itr == this)
			{
				if(*itr)
				{
					// 終了処理
					(*itr)->Uninit();

					// インスタンス削除
					delete (*itr);
				}

				// リスト削除
				itr = m_List[i].erase(itr);

				// 処理終了
				break;
			}
		}
	}
}

//=============================================================================
//	関数名	:UnlinkList
//	引数	:無し
//	戻り値	:無し
//	説明	:対象をリストから外す。
//=============================================================================
void CSceneDX::UnlinkList(void)
{
	list<CSceneDX*>::iterator itr;	// リストのイテレータ

	// 全リストを検索
	for(int i = 0 ; i < PRIORITY_MAX ; i++)
	{
		// リストから自身のインスタンスを探索する
		for(itr = m_List[i].begin() ; itr != m_List[i].end() ; itr++)
		{
			// 自身のインスタンスを見つけ、リストから削除
			if(*itr == this)
			{
				// リスト削除
				itr = m_List[i].erase(itr);

				// 処理終了
				break;
			}
		}
	}
}

//=============================================================================
//	関数名	:AddVtxBuffer
//	引数	:vector<LPDIRECT3DVERTEXBUFFER9> &pVtxBuff	->	頂点バッファ
//			:int size	->	バッファサイズ
//	戻り値	:無し
//	説明	:頂点バッファを追加する。
//=============================================================================
void CSceneDX::AddVtxBuffer(vector<LPDIRECT3DVERTEXBUFFER9> &pVtxBuff, const int size)
{
	// サイズ変更
	pVtxBuff.resize(pVtxBuff.size() + 1);

	// バッファ生成
	D3D_DEVICE->CreateVertexBuffer(size,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&pVtxBuff[pVtxBuff.size() - 1],
		NULL);
}