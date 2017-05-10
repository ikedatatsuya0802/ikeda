//=============================================================================
//
//	タイトル	メッシュフィールド
//	ファイル名	skydome.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/05/10
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "skydome.h"
#include "cameraDX.h"
#include "manager.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CSkydome::m_pTexture;

//=============================================================================
//	関数名	:CSkydome()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSkydome::CSkydome(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CSkydome()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSkydome::~CSkydome()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CSkydome::Init(bool ifLight, cVec3 pos, cVec3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer(((sizeof(VERTEX_3D) * SKYDOME_VERTEX_NUM)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// 頂点バッファの設定
	SetVtxBuff(ifLight);

	// インデックスセット
	SetMeshIndex(&m_pIdxBuff, SKYDOME_HORIZONTAL, SKYDOME_VERTICAL);

	Load();
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CSkydome::SetVtxBuff(bool ifLight)
{
	VERTEX_3D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float radius = 0.0f;
		float radianMax = atan2f((SKYDOME_HEIGHT * SKYDOME_VERTICAL), SKYDOME_RADIUS);

		// 座標設定
		for(int y = 0 ; y <= SKYDOME_VERTICAL ; y++)
		{
			radius = cosf(radianMax / SKYDOME_VERTICAL * (SKYDOME_VERTICAL - y)) * SKYDOME_RADIUS;

			for(int x = 0 ; x <= SKYDOME_HORIZONTAL ; x++)
			{
				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].Pos.x
					= sinf(D3DX_PI * 2.0f / SKYDOME_HORIZONTAL * x) * radius;

				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].Pos.y
					= ((SKYDOME_HEIGHT * 1.0f * SKYDOME_VERTICAL)) - (SKYDOME_HEIGHT * y);

				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].Pos.z
					= cosf(D3DX_PI * 2.0f / SKYDOME_HORIZONTAL * x) * radius;
			}
		}

		// 法線設定
		for(int i = 0 ; i < SKYDOME_VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ貼付座標設定
		for(int y = 0 ; y <= SKYDOME_VERTICAL ; y++)
		{
			for(int x = 0 ; x <= SKYDOME_HORIZONTAL ; x++)
			{
				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].tex.x = (1.0f / SKYDOME_HORIZONTAL * x);
				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].tex.y = (1.0f / SKYDOME_VERTICAL * y);
			}
		}
	}
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CSkydome::Uninit(void)
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
void CSkydome::Update(void)
{
	// カメラ座標に追従
	m_Pos.x = DX_CAMERA->GetCameraPosV().x;
	m_Pos.z = DX_CAMERA->GetCameraPosV().z;

	m_Rot.y -= SKYDOME_MOVEROT;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSkydome::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト開始
	CRendererDX::EnableZTest();

	// 描画処理(円柱部)
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	D3D_DEVICE->SetIndices(m_pIdxBuff);									// インデックスバッファもバインド
	D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定

	// ドーム(円柱部)描画
	D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SKYDOME_VERTEX_NUM, 0, SKYDOME_POLYGON_NUM);

	// 描画処理(天井部)
	D3D_DEVICE->SetTexture(0, NULL);														// テクスチャの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLEFAN, SKYDOME_VERTEX_NUM, SKYDOME_HORIZONTAL);	// 描画

	// Zテスト終了
	CRendererDX::DisableZTest();

	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行う。
//=============================================================================
CSkydome *CSkydome::Create(bool ifLight, cVec3 pos, cVec3 rot)
{
	CSkydome *instance;

	instance = new CSkydome();

	instance->Init(ifLight, pos, rot);

	return instance;
}