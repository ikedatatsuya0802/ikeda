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
//	関数名	:CSceneX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CSceneXDX::CSceneXDX(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneDX(ifListAdd, priority, objtype)
{
	D3DXMatrixIdentity(&m_mtxWorld);
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
void CSceneXDX::Init(cchar* fileName, MODELSTATUS* mesh, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	if(mesh)
	{
		m_ModelStatus = mesh;
	}
	else
	{
		m_ModelStatus = new MODELSTATUS;
		LoadModel(fileName, m_ModelStatus);
	}
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CSceneXDX::Uninit(void)
{
	/*
	SafetyRelease(m_ModelStatus->pBuffMat);
	SafetyRelease(m_ModelStatus->pMesh);*/
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
	D3DXMATERIAL	*pMat = NULL;		// マテリアル
	D3DMATERIAL9	matDef;				// デフォルトのマテリアル


	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// Zテスト開始
	CRendererDX::EnableZTest();

	// アルファテスト開始
	CRendererDX::EnableAlphaTest();

	// 描画処理
	D3D_DEVICE->GetMaterial(&matDef);	// 現在のマテリアルを取得

	// マテリアル変換
	pMat = (D3DXMATERIAL *)m_ModelStatus->pBuffMat->GetBufferPointer();

	// 3Dモデル描画
	for(int i = 0 ; i < (int)m_ModelStatus->NumMat ; i++)
	{
		// マテリアルセット
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// テクスチャ読み込み
		if(pMat[i].pTextureFilename)
		{// テクスチャ有り

			// リストから同名のテクスチャを探索し、セット
			for each(TEXTURE list in m_ModelStatus->Tex)
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
		m_ModelStatus->pMesh->DrawSubset(i);
	}

	// マテリアルを元に戻す
	D3D_DEVICE->SetMaterial(&matDef);

	// アルファテスト終了
	CRendererDX::DisableAlphaTest();

	// Zテスト終了
	CRendererDX::DisableZTest();
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CSceneXDX *CSceneXDX::Create(cchar* fileName, MODELSTATUS* mesh, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CSceneXDX *instance;

	instance = new CSceneXDX(true);

	instance->Init(fileName, mesh, pos, rot);

	return instance;
}

//=============================================================================
//	関数名	:LoadModel
//	引数	:char *filename -> ファイル名
//			:LPDIRECT3DDEVICE9 D3D_DEVICE -> 3Dデバイス
//			:MODELSTATUS ms -> 3Dモデルの各種情報
//	戻り値	:無し
//	説明	:モデルを読み込む。エラー回避付き。
//=============================================================================
void CSceneXDX::LoadModel(cchar* filename, MODELSTATUS* modelStatus)
{
	FILE *fp;	// ファイルポインタ

	// もし3Dモデルファイルのファイル名が間違っていた場合、ダミーのモデルを読み込む。
	if(fopen_s(&fp, filename, "r") == NULL)
	{// ファイル名が正常
		fclose(fp);
		D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&modelStatus->pBuffMat, NULL, &modelStatus->NumMat, &modelStatus->pMesh);
	}
	else
	{// 指定したファイルが存在していない
		D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&modelStatus->pBuffMat, NULL, &modelStatus->NumMat, &modelStatus->pMesh);
	}

	AutomaticSetTexture(modelStatus);
}

//=============================================================================
//	関数名	:AutomaticSetTexture
//	引数	:無し
//	戻り値	:無し
//	説明	:マテリアル情報より自動でテクスチャを追加する。
//=============================================================================
void CSceneXDX::AutomaticSetTexture(MODELSTATUS* modelStatus)
{
	D3DXMATERIAL* pMat = NULL;	// マテリアル

	// マテリアル変換
	pMat = (D3DXMATERIAL *)modelStatus->pBuffMat->GetBufferPointer();

	// プレイヤー描画
	for(int i = 0 ; i < (int)modelStatus->NumMat ; i++)
	{
		// テクスチャ読み込み
		if(pMat[i].pTextureFilename)
		{
			AddTexture(modelStatus->Tex, pMat[i].pTextureFilename);
		}
	}
}

//=============================================================================
//	関数名	:AddTexture
//	引数	:無し
//	戻り値	:無し
//	説明	:テクスチャを追加する。
//=============================================================================
void CSceneXDX::AddTexture(vector<TEXTURE> &texture, char* fileName)
{
	char optional[1024] = ".\\";
	string fName;
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);

	if(fileName[0] != '.')
	{
		fName = strcat(optional, fileName);
	}
	else
	{
		fName.append(fileName);
	}

	D3DXCreateTextureFromFile(D3D_DEVICE, fName.c_str(), &texture[texture.size() - 1].pTexture);
}