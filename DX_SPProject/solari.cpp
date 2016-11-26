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
#include "solari.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSolari::CSolari(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSolari::~CSolari()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CSolari::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;										// 3D頂点情報
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\"SOLARI_TEXFILENAME000, &m_Texture);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), (SOLARI_HEIGHT * 0.5f), 0.0f);
	pVtx[1].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), (SOLARI_HEIGHT * 0.5f), 0.0f);
	pVtx[2].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	pVtx[3].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	
	pVtx[4].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	pVtx[5].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), 0.0f, 0.0f);
	pVtx[6].Pos = D3DXVECTOR3(-(SOLARI_WIDTH * 0.5f), -(SOLARI_HEIGHT * 0.5f), 0.0f);
	pVtx[7].Pos = D3DXVECTOR3( (SOLARI_WIDTH * 0.5f), -(SOLARI_HEIGHT * 0.5f), 0.0f);
	
	for(int nCntInit = 0 ; nCntInit < (VERTEX_NUM * 2) ; nCntInit++)
	{	
		// 法線設定
		pVtx[nCntInit].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		pVtx[nCntInit].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// テクスチャ貼付座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f);
	
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
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
void CSolari::Uninit(void)
{
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if(m_Texture != NULL)
	{
		m_Texture->Release();
		m_Texture = NULL;
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CSolari::Update(void)
{
	if(CInput::GKP(DIK_SPACE))
	{
		m_Rot.x += 0.02f;
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSolari::Draw(void)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;	// マトリックス
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();	// 3Dデバイス
	CCameraDX	*camera = CManager::GetCamera();	// 3Dデバイス

	
	// 射影変換行列
	/*camera->m_CS.mtxProjection	= D3DXMATRIX(
     2/SCREEN_WIDTH , 0.0f, 0.0f, 0.0f,
     0.0f, 2/SCREEN_HEIGHT , 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f
	);*/
	D3DXMATRIX proj;
	D3DXMATRIX view;

	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&proj);

	// Orthoでの射影行列の作成
	D3DXMatrixOrthoLH(&proj, SCREEN_WIDTH, SCREEN_HEIGHT, CAMERA_NEARZ, CAMERA_FARZ);

	// プロジェクションマトリクスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&view);

	// ビューマトリクスの作成
	D3DXMatrixLookAtLH(&view, &camera->m_CS.posV, &camera->m_CS.posR, &camera->m_CS.vecU);

	// ビューマトリクスの設定
	pDevice->SetTransform(D3DTS_VIEW, &view);


	
	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	
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

	// カリング設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画処理
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	pDevice->SetTexture(0, m_Texture);								// テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, PRIMITIVE_NUM);	// 描画

	// カリング設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ライティング設定をオンに
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CSolari *CSolari::Create(D3DXVECTOR3 pos)
{
	CSolari *scene3D;	// インスタンス

	// インスタンス生成
	scene3D = new CSolari;

	// 初期化処理
	scene3D->Init(pos);

	// インスタンスを返す
	return scene3D;
}