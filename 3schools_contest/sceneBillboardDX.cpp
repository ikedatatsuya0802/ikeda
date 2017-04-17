//=============================================================================
//
//	タイトル	シーンファイル(ビルボード)
//	ファイル名	sceneBillboard.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "sceneBillboardDX.h"
#include "main.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CSceneBillboardDX::m_pTexture;

//=============================================================================
//	関数名	:CSceneBillboard()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSceneBillboardDX::CSceneBillboardDX(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{
	
}

//=============================================================================
//	関数名	:~CSceneBillboard()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSceneBillboardDX::~CSceneBillboardDX()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CSceneBillboardDX::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;									// 3D頂点情報
	

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"POLYGONBILLBOARDDX_TEXFILENAME000, &m_pTexture);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = -(POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[0].Pos.y = (POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[0].Pos.z = 0.0f;
	
	pVtx[1].Pos.x = (POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[1].Pos.y = (POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[1].Pos.z = 0.0f;
	
	pVtx[2].Pos.x = -(POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[2].Pos.y = -(POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[2].Pos.z = 0.0f;
	
	pVtx[3].Pos.x = (POLYGONBILLBOARDDX_WIDTH * 0.5f);
	pVtx[3].Pos.y = -(POLYGONBILLBOARDDX_HEIGHT * 0.5f);
	pVtx[3].Pos.z = 0.0f;
	
	for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
	{
		// 法線設定
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		
		// 色設定
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);
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
void CSceneBillboardDX::Uninit(void)
{
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CSceneBillboardDX::Update(void)
{

#ifdef _DEBUG
	//CDebugProc::DebugProc("参照頂点:%d(%5.2f:%5.2f:%5.2f)\n", num, vec.x, vec.y, vec.z);
#endif
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSceneBillboardDX::Draw(void)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;				// マトリックス
	
	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// ビルボード設定
	D3D_DEVICE->GetTransform(D3DTS_VIEW, &mtxView);
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
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 加算合成の設定
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// ビルボード描画

	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	// レンダーステート設定を戻す
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CSceneBillboardDX *CSceneBillboardDX::Create(D3DXVECTOR3 pos)
{
	CSceneBillboardDX *sceneBillboard;

	sceneBillboard = new CSceneBillboardDX;

	sceneBillboard->Init(pos);

	return sceneBillboard;
}