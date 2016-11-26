//=============================================================================
//
//	タイトル	メッシュフィールド
//	ファイル名	skybox.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/05/10
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "skybox.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CSkybox::m_Texture;

//=============================================================================
//	関数名	:CSkybox()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSkybox::CSkybox(int priority, OBJTYPE objtype) : CSceneDX(priority, objtype)
{

}

//=============================================================================
//	関数名	:~CSkybox()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSkybox::~CSkybox()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CSkybox::Init(D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;										// 3D頂点情報
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス

	// 各種初期化処理
	SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(((sizeof(VERTEX_3D) * SKYBOX_VERTEX_NUM)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
			SetSkyboxData(pVtx);
	}
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CSkybox::Uninit(void)
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
void CSkybox::Update(void)
{
	m_Rot.y -= 0.0001f;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSkybox::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// マトリックス
	
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

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);		// ワールドマトリックスの設定

	// ライティング設定をオフに
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		
	// Zテスト方法更新
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテスト開始
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 描画処理
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));		// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);										// 頂点フォーマットの設定
	pDevice->SetTexture(0, m_Texture);									// テクスチャの設定
	for(int i = 0 ; i < SKYBOX_PRIMITIVE_NUM ; i++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * VERTEX_NUM), PRIMITIVE_NUM);	// 描画
	}

	// Zテスト方法更新
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテスト終了
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティング設定をオンに
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名:SetSkyboxData
//	引数:VERTEX_3D *pVtx(頂点データ)
//	戻り値:無し
//	説明:頂点データをセットする。
//=============================================================================
void CSkybox::SetSkyboxData(VERTEX_3D *pVtx)
{
	// 座標設定
	// 天面
	pVtx[0].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[1].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[2].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[3].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	
	// 底面
	pVtx[4].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[5].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[6].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[7].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	
	// 前面
	pVtx[8].Pos		= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[9].Pos		= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[10].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[11].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	
	// 背面
	pVtx[12].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[13].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[14].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[15].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	
	// 左側面
	pVtx[16].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[17].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[18].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[19].Pos	= D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	
	// 右側面
	pVtx[20].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[21].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[22].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[23].Pos	= D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	
	for(int i = 0 ; i < SKYBOX_VERTEX_NUM ; i++)
	{
		// 法線設定
		pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(0.2f, 0.2f, 0.4f, 1.0f);
	}
	
	// テクスチャ貼付座標設定
	// 天面
	pVtx[0].tex		= D3DXVECTOR2((1 * 0.25f), (0 * (1.0f / 3.0f)));
	pVtx[1].tex		= D3DXVECTOR2((2 * 0.25f), (0 * (1.0f / 3.0f)));
	pVtx[2].tex		= D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[3].tex		= D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	// 底面
	pVtx[4].tex		= D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[5].tex		= D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[6].tex		= D3DXVECTOR2((1 * 0.25f), (3 * (1.0f / 3.0f)));
	pVtx[7].tex		= D3DXVECTOR2((2 * 0.25f), (3 * (1.0f / 3.0f)));
	// 前面
	pVtx[8].tex		= D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[9].tex		= D3DXVECTOR2((4 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[10].tex	= D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[11].tex	= D3DXVECTOR2((4 * 0.25f), (2 * (1.0f / 3.0f)));
	// 背面
	pVtx[12].tex	= D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[13].tex	= D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[14].tex	= D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[15].tex	= D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	// 左側面
	pVtx[16].tex	= D3DXVECTOR2((0 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[17].tex	= D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[18].tex	= D3DXVECTOR2((0 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[19].tex	= D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f)));
	// 右側面
	pVtx[20].tex	= D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[21].tex	= D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f)));
	pVtx[22].tex	= D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f)));
	pVtx[23].tex	= D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f)));
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CSkybox *CSkybox::Create(D3DXVECTOR3 pos)
{
	CSkybox *skybox;

	skybox = new CSkybox;

	skybox->Init();

	return skybox;
}