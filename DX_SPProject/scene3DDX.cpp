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
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CScene3DDX::m_Texture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CScene3DDX::CScene3DDX(int priority, OBJTYPE objtype) : CSceneDX(priority, objtype)
{

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
void CScene3DDX::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;										// 3D頂点情報
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
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
	
	for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
	{
		// 法線設定
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		// 色設定
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// テクスチャ貼付座標設定
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
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();	// 3Dデバイス
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;	// マトリックス

	
	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビルボード設定
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;
	
	// スケール設定
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);
		
	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
		
	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// ライティング設定をオフに
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	pDevice->SetTexture(0, m_Texture);								// テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// 描画

	// ライティング設定をオンに
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名	:Create
//	引数	:無し
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CScene3DDX *CScene3DDX::Create(void)
{
	CScene3DDX *scene3D;

	scene3D = new CScene3DDX;

	scene3D->Init();

	return scene3D;
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CScene3DDX *CScene3DDX::Create(D3DXVECTOR3 pos)
{
	CScene3DDX *scene3D;

	scene3D = new CScene3DDX;

	scene3D->Init(pos);

	return scene3D;
}