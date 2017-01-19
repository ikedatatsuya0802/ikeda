//=============================================================================
//
//	タイトル	プレイヤー
//	ファイル名	model.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/14
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include <stdio.h>
#include "model.h"
#include "main.h"
#include "player.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "scene3DDX.h"
#include "meshfield.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================

//=============================================================================
//	関数名	:CModel()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CModel::CModel(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneXDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CModel()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CModel::Init(char *filename, D3DXVECTOR3 pos)
{
	// 各種初期化処理
	m_Parent	= NULL;
	m_PosDef	= D3DXVECTOR3(pos.x, pos.y, pos.z);
	m_RotDef	= VEC3_ZERO;
	m_Pos		= VEC3_ZERO;
	m_Rot		= VEC3_ZERO;
	D3DXMatrixIdentity(&m_mtxWorld);

	// モデル読み込み
	LoadModel(filename);
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CModel::Uninit(void)
{
	SafetyRelease(m_ModelStatus.pMesh);
	SafetyRelease(m_ModelStatus.pBuffMat);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CModel::Draw(void)
{
	D3DXMATRIX		mtxView, mtxScl, mtxRot, mtxTrans;	// マトリックス
	D3DXMATRIX		parentMatrix;						// 親オブジェクトのマトリックス
	D3DXMATERIAL	*pMat		= NULL;					// マテリアル
	D3DMATERIAL9	matDef;								// デフォルトのマテリアル

	
	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケール設定
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	if(m_Parent != NULL)
	{
		parentMatrix = m_Parent->GetWorldMatrix();
	}
	else
	{
		D3D_DEVICE->GetTransform(D3DTS_WORLD, &parentMatrix);
	}

	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotDef.y, m_RotDef.x, m_RotDef.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, m_PosDef.x, m_PosDef.y, m_PosDef.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	D3DXMatrixIdentity(&mtxTrans);
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &parentMatrix);
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// Zテスト開始
	CRendererDX::EnableZTest();
	
	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// 描画処理
	D3D_DEVICE->GetMaterial(&matDef);	// 現在のマテリアルを取得

	// マテリアル変換
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();

	// 3Dモデル描画
	for(int i = 0 ; i < (int)m_ModelStatus.NumMat ; i++)
	{
		// マテリアルセット
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// テクスチャ読み込み
		if(pMat[i].pTextureFilename)
		{// テクスチャ有り

			// リストから同名のテクスチャを探索し、セット
			for each(TEXTURE list in m_pTexture)
			{
				if(list.FileName == CharPToString(pMat[i].pTextureFilename))
				{
					D3D_DEVICE->SetTexture(0, list.pTexture);
				}
			}
		}
		else
		{// テクスチャ無し

		 // テクスチャをセットしない
			D3D_DEVICE->SetTexture(0, NULL);
		}

		// モデル描画
		m_ModelStatus.pMesh->DrawSubset(i);
	}

	// マテリアルを元に戻す
	D3D_DEVICE->SetMaterial(&matDef);

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// Zテスト終了
	CRendererDX::DisableZTest();
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CModel *CModel::Create(char *filename, D3DXVECTOR3 pos)
{
	CModel *model;	// インスタンス

	// インスタンス生成
	model = new CModel();

	// 初期化処理
	model->Init(filename, pos);

	// インスタンスをリターン
	return model;
}

//=============================================================================
//	関数名	:LoadModel
//	引数	:char *filename -> ファイル名
//			:LPDIRECT3DDEVICE9 D3D_DEVICE -> 3Dデバイス
//			:MODELSTATUS ms -> 3Dモデルの各種情報
//	戻り値	:無し
//	説明	:モデルを読み込む。エラー回避付き。
//=============================================================================
void CModel::LoadModel(char *filename)
{
	FILE *fp;	// ファイルポインタ


	// もし3Dモデルファイルのファイル名が間違っていた場合、ダミーのモデルを読み込む。
	if(fopen_s(&fp, filename, "r") == NULL)
	{// ファイル名が正常
		fclose(fp);
		D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}
	else
	{// 指定したファイルが存在していない
		D3DXLoadMeshFromX("./data/MODEL/dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}

	AutomaticSetTexture();
}

//=============================================================================
//	関数名	:AutomaticSetTexture
//	引数	:無し
//	戻り値	:無し
//	説明	:マテリアル情報より自動でテクスチャを追加する。
//=============================================================================
void CModel::AutomaticSetTexture(void)
{
	D3DXMATERIAL	*pMat = NULL;	// マテリアル

	// マテリアル変換
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();

	// プレイヤー描画
	for(int i = 0 ; i < (int)m_ModelStatus.NumMat ; i++)
	{
		// テクスチャ読み込み
		if(pMat[i].pTextureFilename)
		{
			AddTexture(m_pTexture, pMat[i].pTextureFilename);
		}
	}
}

//=============================================================================
//	関数名	:AddTexture
//	引数	:無し
//	戻り値	:無し
//	説明	:テクスチャを追加する。
//=============================================================================
void CModel::AddTexture(vector<TEXTURE> &texture, char* fileName)
{
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);


	D3DXCreateTextureFromFile(D3D_DEVICE, fileName, &texture[texture.size() - 1].pTexture);
}