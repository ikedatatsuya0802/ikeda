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
#include "cameraDX.h"
#include "manager.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CSkybox::m_pTexture;

//=============================================================================
//	関数名	:CSkybox()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSkybox::CSkybox(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
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
void CSkybox::Init(bool ifLight, cVec3 pos, cVec3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer(((sizeof(VERTEX_3D) * SKYBOX_VERTEX_NUM)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	SetVtxBuff(ifLight);

	Load();
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CSkybox::SetVtxBuff(bool ifLight)
{
	VERTEX_3D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	// 天面
	pVtx[0].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[1].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[2].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[3].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));

	// 底面
	pVtx[4].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[5].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[6].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[7].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));

	// 前面
	pVtx[8].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[9].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[10].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[11].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));

	// 背面
	pVtx[12].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[13].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[14].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[15].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));

	// 左側面
	pVtx[16].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[17].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[18].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[19].Pos = D3DXVECTOR3(-(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));

	// 右側面
	pVtx[20].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[21].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));
	pVtx[22].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), (SKYBOX_SIZE * 0.5f));
	pVtx[23].Pos = D3DXVECTOR3((SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f), -(SKYBOX_SIZE * 0.5f));

	for(int i = 0 ; i < SKYBOX_VERTEX_NUM ; i++)
	{
		// 法線設定
		pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		if(1)
		{
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			pVtx[i].col = D3DCOLOR_COLORVALUE(0.2f, 0.2f, 0.4f, 1.0f);
		}
	}
	if(1)
	{
		// テクスチャ座標設定
		// 天面
		pVtx[0].tex = D3DXVECTOR2((1 * 0.25f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((2 * 0.25f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1 * 0.25f), 0.25f);
		pVtx[3].tex = D3DXVECTOR2((2 * 0.25f), 0.25f);
		// 底面
		pVtx[4].tex = D3DXVECTOR2((1 * 0.25f), 0.5f);
		pVtx[5].tex = D3DXVECTOR2((2 * 0.25f), 0.5f);
		pVtx[6].tex = D3DXVECTOR2((1 * 0.25f), 0.75f);
		pVtx[7].tex = D3DXVECTOR2((2 * 0.25f), 0.75f);
		// 前面
		pVtx[8].tex = D3DXVECTOR2((3 * 0.25f), 0.25f);
		pVtx[9].tex = D3DXVECTOR2((4 * 0.25f), 0.25f);
		pVtx[10].tex = D3DXVECTOR2((3 * 0.25f), 0.5f);
		pVtx[11].tex = D3DXVECTOR2((4 * 0.25f), 0.5f);
		// 背面
		pVtx[12].tex = D3DXVECTOR2((1 * 0.25f), 0.25f);
		pVtx[13].tex = D3DXVECTOR2((2 * 0.25f), 0.25f);
		pVtx[14].tex = D3DXVECTOR2((1 * 0.25f), 0.5f);
		pVtx[15].tex = D3DXVECTOR2((2 * 0.25f), 0.5f);
		// 左側面
		pVtx[16].tex = D3DXVECTOR2((0 * 0.25f), 0.25f);
		pVtx[17].tex = D3DXVECTOR2((1 * 0.25f), 0.25f);
		pVtx[18].tex = D3DXVECTOR2((0 * 0.25f), 0.5f);
		pVtx[19].tex = D3DXVECTOR2((1 * 0.25f), 0.5f);
		// 右側面
		pVtx[20].tex = D3DXVECTOR2((2 * 0.25f), 0.25f);
		pVtx[21].tex = D3DXVECTOR2((3 * 0.25f), 0.25f);
		pVtx[22].tex = D3DXVECTOR2((2 * 0.25f), 0.5f);
		pVtx[23].tex = D3DXVECTOR2((3 * 0.25f), 0.5f);
	}
	else
	{
		// テクスチャ座標設定
		// 天面
		pVtx[0].tex = D3DXVECTOR2((1 * 0.25f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((2 * 0.25f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[3].tex = D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		// 底面
		pVtx[4].tex = D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f + 0.0001f)));
		pVtx[5].tex = D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f + 0.0001f)));
		pVtx[6].tex = D3DXVECTOR2((1 * 0.25f), 1.0f);
		pVtx[7].tex = D3DXVECTOR2((2 * 0.25f), 1.0f);
		// 前面
		pVtx[8].tex = D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[9].tex = D3DXVECTOR2((4 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[10].tex = D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f - 0.0001f)));
		pVtx[11].tex = D3DXVECTOR2((4 * 0.25f), (2 * (1.0f / 3.0f - 0.0001f)));
		// 背面
		pVtx[12].tex = D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[13].tex = D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[14].tex = D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f - 0.0001f)));
		pVtx[15].tex = D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f - 0.0001f)));
		// 左側面
		pVtx[16].tex = D3DXVECTOR2((0 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[17].tex = D3DXVECTOR2((1 * 0.25f), (1 * (1.0f / 3.0f + 0.0001f)));
		pVtx[18].tex = D3DXVECTOR2((0 * 0.25f), (2 * (1.0f / 3.0f - 0.0001f)));
		pVtx[19].tex = D3DXVECTOR2((1 * 0.25f), (2 * (1.0f / 3.0f - 0.0001f)));
		// 右側面
		pVtx[20].tex = D3DXVECTOR2((2 * 0.25f), (1 * (1.0f / 3.0f + 0.00001f)));
		pVtx[21].tex = D3DXVECTOR2((3 * 0.25f), (1 * (1.0f / 3.0f + 0.00001f)));
		pVtx[22].tex = D3DXVECTOR2((2 * 0.25f), (2 * (1.0f / 3.0f - 0.00001f)));
		pVtx[23].tex = D3DXVECTOR2((3 * 0.25f), (2 * (1.0f / 3.0f - 0.00001f)));
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
	SafetyRelease(m_pVtxBuff);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CSkybox::Update(void)
{
	// カメラ座標に追従
	m_Pos = DX_CAMERA->GetCameraPosV();

	m_Rot.y -= 0.00001f;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSkybox::Draw(void)
{
	if(!CManager::GetEdhitMode())
	{
	// マトリックス設定
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// ライティング設定をオフに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// Zテスト方法更新
		D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// アルファテスト開始
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

		// 描画処理
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));		// 頂点フォーマットの設定
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);										// 頂点フォーマットの設定
		D3D_DEVICE->SetTexture(0, m_pTexture);									// テクスチャの設定
		for(int i = 0 ; i < SKYBOX_PRIMITIVE_SQUARE ; i++)
		{
			D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * VERTEX_SQUARE), PRIMITIVE_SQUARE);	// 描画
		}
		/*
		// Zテスト方法更新
		D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
		D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/

		// アルファテスト終了
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

		// ライティング設定をオンに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CSkybox *CSkybox::Create(bool ifLight, cVec3 pos, cVec3 rot)
{
	CSkybox *instance;

	instance = new CSkybox();

	instance->Init(ifLight, pos, rot);

	return instance;
}