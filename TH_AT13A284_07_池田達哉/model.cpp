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
CModel::CModel(int priority)
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
	m_RotDef	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);

	// モデル読み込み
	LoadModel(filename);

	AutomaticSetTexture();

	/*
	AddTexture(m_Texture, "hakatanozomi.bmp");
	AddTexture(m_Texture, "n700.bmp");
	AddTexture(m_Texture, "n700mado.bmp");
	AddTexture(m_Texture, "n700rogo.bmp");
	AddTexture(m_Texture, "n700yane.bmp");
	AddTexture(m_Texture, "ta.bmp");*/
}

void CModel::AutomaticSetTexture(void)
{
	D3DXMATERIAL	*pMat = NULL;	// マテリアル

	// マテリアル変換
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();

	// プレイヤー描画
	for(int i = 0 ; i < (int)m_ModelStatus.NumMat ; i++)
	{
		// マテリアルセット
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// テクスチャ読み込み
		if(pMat[i].pTextureFilename)
		{
			AddTexture(m_Texture, pMat[i].pTextureFilename);
		}
	}
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CModel::AddTexture(vector<TEXTURE> &texture, char* fileName)
{
	char optional[] = "./data/MODEL/n700";
	char* fName = strcat(optional, fileName);
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);


	D3DXCreateTextureFromFile(D3D_DEVICE, fName, &texture[texture.size() - 1].pTexture);
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
	D3DXMATRIX		mtxView, mtxScl, mtxRot, mtxTrans;			// マトリックス
	D3DXMATRIX		parentMatrix;								// 親オブジェクトのマトリックス
	D3DXMATERIAL	*pMat		= NULL;							// マテリアル
	D3DMATERIAL9	matDef;										// デフォルトのマテリアル

	
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
	
	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// 描画処理
	D3D_DEVICE->GetMaterial(&matDef);	// 現在のマテリアルを取得

	// マテリアル変換
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();	
	
	// プレイヤー描画
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelStatus.NumMat ; nCntMat++)
	{
		D3D_DEVICE->SetMaterial(&pMat[nCntMat].MatD3D);	// マテリアルセット

		// テクスチャ読み込み
		if(pMat[nCntMat].pTextureFilename)
		{// テクスチャ有り

			D3D_DEVICE->SetTexture(0, NULL);
		}
		else
		{// テクスチャ無し

			D3D_DEVICE->SetTexture(0, NULL);
		}

		m_ModelStatus.pMesh->DrawSubset(nCntMat);
	}

	// マテリアルを元に戻す
	D3D_DEVICE->SetMaterial(&matDef);

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
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
	model = new CModel;

	// 初期化処理
	model->Init(filename, pos);

	// インスタンスを返す
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
		D3DXLoadMeshFromX("data/MODEL/dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}

}