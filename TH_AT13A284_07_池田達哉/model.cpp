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
#include "model.h"
#include "main.h"

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
	D3DXMatrixIdentity(&m_mtxWorld);
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
void CModel::Init(string fileDir, string filename)
{
	// 各種初期化処理
	m_FileDir = fileDir;
	m_Parent	= NULL;
	m_PosDef	= VEC3_ZERO;
	m_RotDef	= VEC3_ZERO;
	m_Pos		= VEC3_ZERO;
	m_Rot		= VEC3_ZERO;

	m_MorphCount	= 0;
	m_MorphStatus	= 0;

	// モデル読み込み
	LoadModel(m_FileDir, filename);
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CModel::Uninit(void)
{
	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		SafetyRelease(m_ModelStatus[i].pMesh);
		SafetyRelease(m_ModelStatus[i].pBuffMat);
	}
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


	// 親オブジェクトのワールドマトリックス取得
	if(m_Parent != NULL)
	{
		parentMatrix = *m_Parent->GetWMatrix();
	}
	else
	{
		D3D_DEVICE->GetTransform(D3DTS_WORLD, &parentMatrix);
	}
	
	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケール設定
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotDef.y + m_Rot.y, m_RotDef.x + m_Rot.x, m_RotDef.z + m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, m_PosDef.x + m_Pos.x, m_PosDef.y + m_Pos.y, m_PosDef.z + m_Pos.z);
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
	pMat = (D3DXMATERIAL *)m_ModelStatus[m_MorphStatus].pBuffMat->GetBufferPointer();

	// 3Dモデル描画
	for(int i = 0 ; i < (int)m_ModelStatus[m_MorphStatus].NumMat ; i++)
	{
		// マテリアルセット
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// テクスチャ読み込み
		if(pMat[i].pTextureFilename)
		{// テクスチャ有り

		 // リストから同名のテクスチャを探索し、セット
			for each(TEXTURE list in m_pTexture)
			{
				if(list.FileName == pMat[i].pTextureFilename)
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
		m_ModelStatus[m_MorphStatus].pMesh->DrawSubset(i);
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
CModel *CModel::Create(string fileDir, string filename)
{
	CModel *model;	// インスタンス

	// インスタンス生成
	model = new CModel();

	// 初期化処理
	model->Init(fileDir, filename);

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
void CModel::LoadModel(string fileDir, string filename)
{
	FILE *fp;	// ファイルポインタ
	string str = ".\\data\\MODEL\\";
	str += fileDir;
	string morphFile = str + "\\morph.txt";

	fopen_s(&fp, morphFile.c_str(), "r");

	// END_SCRIPTまで読み込み
	while(!feof(fp))
	{
		string word;
		char wordBuff[1024] = { NULL };

		// 単語を取得
		fscanf(fp, "%s", wordBuff);
		word += wordBuff;

		if(word == "MORPH_COUNT")
		{// モデル情報を読み込み

			m_ModelStatus.push_back(MODELSTATUS_MORPH());
			fscanf(fp, " = %d", &m_ModelStatus[m_ModelStatus.size() - 1].MorphTime);
		}
	}

	fclose(fp);


	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		string strbuff = str;

		strbuff += "\\";
		strbuff += to_string(i);
		strbuff += "\\";
		strbuff += filename;
		// もし3Dモデルファイルのファイル名が間違っていた場合、ダミーのモデルを読み込む。
		if(fopen_s(&fp, strbuff.c_str(), "r") == NULL)
		{// ファイル名が正常
			fclose(fp);
			D3DXLoadMeshFromX(strbuff.c_str(), D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);
		}
		else
		{// 指定したファイルが存在していない
			D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);
		}
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

	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		// マテリアル変換
		pMat = (D3DXMATERIAL *)m_ModelStatus[i].pBuffMat->GetBufferPointer();

		// プレイヤー描画
		for(int i = 0 ; i < (int)m_ModelStatus[i].NumMat ; i++)
		{
			// テクスチャ読み込み
			if(pMat[i].pTextureFilename)
			{
				// テクスチャ追加
				AddTexture(m_pTexture, pMat[i].pTextureFilename);
			}
		}
	}
}

//=============================================================================
//	関数名	:AddTexture
//	引数	:無し
//	戻り値	:無し
//	説明	:テクスチャを追加する。
//=============================================================================
void CModel::AddTexture(vector<TEXTURE> &texture, string fileName)
{
	string optional = ".\\data\\MODEL\\";
	optional += m_FileDir;
	optional += "\\";
	optional += "TEXTURE\\";
	optional += fileName;
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);
	const char* file = optional.c_str();
	string texname = "./data/MODEL/sample/TEXTURE/side01.png";

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(D3D_DEVICE, fileName.c_str(), &texture[texture.size() - 1].pTexture);
}

string CModel::to_string(int val)
{
	char buffer[numeric_limits<int>::digits10 + 2]; // '-' + NULL
	sprintf(buffer, "%d", val);
	return buffer;
}