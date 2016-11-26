//=============================================================================
//
//	タイトル	シーンファイル(Xファイル)
//	ファイル名	sceneX.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "sceneXDX.h"
#include "manager.h"
#include "main.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "scene3DDX.h"
#include "meshfield.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CSceneXDX::m_Texture[MODEL_TEXTURENUM];
LPD3DXMESH			CSceneXDX::m_pMesh;
LPD3DXBUFFER		CSceneXDX::m_pBuffMat;
DWORD				CSceneXDX::m_NumMat;

//=============================================================================
//	関数名	:CSceneX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSceneXDX::CSceneXDX(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pMesh		= NULL;
	m_pBuffMat	= NULL;
	m_NumMat	= 0;
}

//=============================================================================
//	関数名	:~CSceneX()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CSceneXDX::~CSceneXDX()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CSceneXDX::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3Dデバイス

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nCntMove = 0;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CSceneXDX::Uninit(void)
{
	if(m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if(m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CSceneXDX::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CSceneXDX::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice		= CRendererDX::GetDevice();		// 3Dデバイス
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// マトリックス
	D3DXMATERIAL		*pMat		= NULL;							// マテリアル
	D3DMATERIAL9		matDef;										// デフォルトのマテリアル


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
	
	// アルファテスト開始
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

	// 描画処理
	pDevice->GetMaterial(&matDef);	// 現在のマテリアルを取得

	// マテリアル変換
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();	
	
	// プレイヤー描画
	for(int nCntMat = 0 ; nCntMat < (int)m_NumMat ; nCntMat++)
	{
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// マテリアルセット

		// テクスチャ読み込み
		if(pMat[nCntMat].pTextureFilename)
		{
			if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player000.png") == 0)
			{// レールテクスチャ
				pDevice->SetTexture(0, m_Texture[0]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player001.png") == 0)
			{// レールテクスチャ
				pDevice->SetTexture(0, m_Texture[1]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player002.jpg") == 0)
			{// レールテクスチャ
				pDevice->SetTexture(0, m_Texture[2]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player003.jpg") == 0)
			{// レールテクスチャ
				pDevice->SetTexture(0, m_Texture[3]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}
		}
		else
		{// テクスチャ無し
			pDevice->SetTexture(0, NULL);
		}

		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	// アルファテスト終了
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	
	// デバッグ情報表示
#ifdef _DEBUG
	/*CDebugProc::DebugProc("プレイヤー座標:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	int i = mesh->GetFrontMesh(m_Pos);
	CDebugProc::DebugProc("乗っているポリゴン:(%2d:%2d:%2d)\n", i + 0, i + 1, i + 2);*/
#endif
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CSceneXDX *CSceneXDX::Create(D3DXVECTOR3 pos)
{
	CSceneXDX *sceneX;

	sceneX = new CSceneXDX;

	sceneX->Init(pos);

	return sceneX;
}