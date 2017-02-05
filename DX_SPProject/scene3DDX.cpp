//=============================================================================
//
//	タイトル	シーンファイル(3D)
//	ファイル名	scene3D.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "scene3DDX.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "meshfield.h"

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CScene3DDX::CScene3DDX(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneDX(ifListAdd, priority, objtype)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CScene3DDX::~CScene3DDX()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CScene3DDX::Init(cchar *str, cVec3 pos, cVec3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点バッファ設定
	SetVtxBuff();

	// テクスチャ読み込み
	Load(str);
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CScene3DDX::SetVtxBuff(void)
{
	VERTEX_3D	*pVtx;	// 3D頂点情報


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = -(POLYGON3DDX_WIDTH * 0.5f);
	pVtx[0].Pos.y = (POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[0].Pos.z = 0.0f;

	pVtx[1].Pos.x = (POLYGON3DDX_WIDTH * 0.5f);
	pVtx[1].Pos.y = (POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[1].Pos.z = 0.0f;

	pVtx[2].Pos.x = -(POLYGON3DDX_WIDTH * 0.5f);
	pVtx[2].Pos.y = -(POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[2].Pos.z = 0.0f;

	pVtx[3].Pos.x = (POLYGON3DDX_WIDTH * 0.5f);
	pVtx[3].Pos.y = -(POLYGON3DDX_HEIGHT * 0.5f);
	pVtx[3].Pos.z = 0.0f;

	for(int nCntSet = 0 ; nCntSet < VERTEX_SQUARE ; nCntSet++)
	{
		// 法線設定
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
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
void CScene3DDX::Uninit(void)
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
void CScene3DDX::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CScene3DDX::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// アルファ・Zテスト開始
	CRendererDX::EnableAlphaTest();
	CRendererDX::EnableZTest();
	
	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);

	// 頂点フォーマットの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// テクスチャの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);

	// 描画
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);

	// アルファ・Zテスト終了
	CRendererDX::DisableAlphaTest();
	CRendererDX::DisableZTest();
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CScene3DDX *CScene3DDX::Create(cchar *str, cVec3 pos, cVec3 rot)
{
	CScene3DDX *instance;	// インスタンス

	// インスタンス生成
	instance = new CScene3DDX();

	// 初期化処理
	instance->Init(str, pos, rot);

	// インスタンスをリターン
	return instance;
}

//=============================================================================
//	関数名	:SetMeshIndex
//	引数	:LPDIRECT3DINDEXBUFFER9 *idx	->	インデックス
//			:int x							->	メッシュの横ブロック数
//			:int y							->	メッシュの縦ブロック数
//	戻り値	:無し
//	説明	:メッシュのインデックスをセットする。
//=============================================================================
void CScene3DDX::SetMeshIndex(LPDIRECT3DINDEXBUFFER9 *idxBuff, cint horizontal, cint vertical)
{
	WORD	*pIdx;
	int		idxNum		= ((horizontal + 1) * 2) * vertical + ((vertical - 1) * 2);
	int		idxStd		= ((horizontal + 1) * 2 + 2);
	int		idxVtxNum	= (horizontal + 1) * (vertical + 1);

	// インデックスバッファの確保
	D3D_DEVICE->CreateIndexBuffer((sizeof(WORD) * idxNum), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, idxBuff, NULL);


	(*idxBuff)->Lock(0, 0, (void**)&pIdx, 0);
	{
		int buff = (idxVtxNum / (vertical + 1));

		for(int nCntVertical = 0 ; nCntVertical < vertical ; nCntVertical++)
		{
			for(int nCntIdx = 0 ; nCntIdx < idxStd ; nCntIdx++)
			{
				if((nCntVertical == (vertical - 1)) && (nCntIdx == (idxStd - 2)))
				{// 処理打ち切り
					break;
				}

				if(nCntIdx == (idxStd - 1))
				{
					buff += (horizontal + 2);
					pIdx[nCntVertical * idxStd + nCntIdx] = buff;
				}
				else if(nCntIdx == (idxStd - 2))
				{
					pIdx[nCntVertical * idxStd + nCntIdx] = buff;
				}
				else
				{
					pIdx[nCntVertical * idxStd + nCntIdx] = buff;

					if(nCntIdx % 2 == 0)
					{
						buff -= (horizontal + 1);
					}
					else
					{
						if(nCntIdx != (idxStd - 3))
						{
							buff += (horizontal + 2);
						}
					}
				}
			}
		}
	}
	(*idxBuff)->Unlock();
}