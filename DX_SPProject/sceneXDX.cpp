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
CSceneXDX::CSceneXDX(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
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
void CSceneXDX::Init(char* fileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	LoadModel(fileName);
}

//=============================================================================
//	関数名	:Load
//	引数	:無し
//	戻り値	:無し
//	説明	:。
//=============================================================================
void CSceneXDX::Load(void)
{

}

//=============================================================================
//	関数名	:Load
//	引数	:無し
//	戻り値	:無し
//	説明	:。
//=============================================================================
void CSceneXDX::Unload(void)
{

}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CSceneXDX::Uninit(void)
{

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
	D3DXMATERIAL		*pMat = NULL;		// マテリアル
	D3DMATERIAL9		matDef;				// デフォルトのマテリアル


	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

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
CSceneXDX *CSceneXDX::Create(char* fileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CSceneXDX *instance;

	instance = new CSceneXDX(true);

	instance->Init(fileName, pos, rot);

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
void CSceneXDX::LoadModel(char *filename)
{
	FILE *fp;	// ファイルポインタ
	char str[1024] = ".\\data\\MODEL\\";
	strcat(str, filename);

	// もし3Dモデルファイルのファイル名が間違っていた場合、ダミーのモデルを読み込む。
	if(fopen_s(&fp, str, "r") == NULL)
	{// ファイル名が正常
		fclose(fp);
		D3DXLoadMeshFromX(str, D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}
	else
	{// 指定したファイルが存在していない
		D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
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
void CSceneXDX::AutomaticSetTexture(void)
{
	D3DXMATERIAL	*pMat = NULL;	// マテリアル

									// マテリアル変換
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();

	// プレイヤー描画
	for(int i = 0 ; i < (int)m_ModelStatus.NumMat ; i++)
	{
		// マテリアルセット
		//D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

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
void CSceneXDX::AddTexture(vector<TEXTURE> &texture, char* fileName)
{
	char optional[1024] = ".\\data\\MODEL\\n700\\";
	char* fName = strcat(optional, fileName);
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);


	D3DXCreateTextureFromFile(D3D_DEVICE, fName, &texture[texture.size() - 1].pTexture);
}