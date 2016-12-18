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
#include "manager.h"

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
	D3DMATERIAL9	mtrl;	// マテリアルオブジェクト

	// 各種初期化処理
	m_FileDir = fileDir;
	m_Pos		= VEC3_ZERO;
	m_Rot		= VEC3_ZERO;

	m_MorphCountAll = 0;
	m_MorphCount	= 0;
	m_MorphStatus	= 0;

	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);			// Ｚバッファ有効化
	D3D_DEVICE->SetRenderState(D3DRS_AMBIENT, 0x00303030);	// 環境光の設定

	// マテリアルの設定
	ZeroMemory(&mtrl, sizeof(mtrl));			// いったんゼロでクリア
	mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0;
	mtrl.Ambient.r = mtrl.Ambient.g = mtrl.Ambient.b = mtrl.Ambient.a = 1.0;
	D3D_DEVICE->SetMaterial(&mtrl);

	// モデル読み込み
	LoadModel(m_FileDir, filename);

	// カウント総数を計測
	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		m_MorphCountAll += m_ModelStatus[i].MorphTime;
	}
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

	SafetyRelease(m_pTexture);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CModel::Update(void)
{
	MODELSTATUS_MORPH*		pt;	// オブジェクトへのポインタ
	pt = &m_ModelStatus[0];
	int nextArray = 0;	// 次のモーフィング配列

	VERTEX		*spt0, *spt1, *dpt;					// 各頂点データへのポインタ
	float		ratio;								// 時間から求めた変形の割合

	//変形前ポインタを取得
	spt0 = pt[m_MorphStatus].pt_vertex;
	//変形後ポインタを取得
	nextArray = (m_MorphStatus + 1) % m_ModelStatus.size();
	spt1 = pt[nextArray].pt_vertex;
	//モーフィング中ポインタを取得
	dpt = m_MorphVertex;

	// 割合を求める
	ratio = (float)m_MorphCount / (float)pt[nextArray].MorphTime;

	// ２つのデータから座標を保管してモーフィング中データとして格納
	//それぞれのポインタを１つすすめる
	for(int i = 0 ; i < pt[0].num_vertex ; i++, spt0++, spt1++, dpt++)
	{
		dpt->x = (spt1->x - spt0->x) * ratio + spt0->x;
		dpt->y = (spt1->y - spt0->y) * ratio + spt0->y;
		dpt->z = (spt1->z - spt0->z) * ratio + spt0->z;
	}

	m_MorphCount++;
	if(m_MorphCount >= pt[nextArray].MorphTime)
	{
		m_MorphStatus = (m_MorphStatus + 1) % m_ModelStatus.size();
		m_MorphCount = 0;
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CModel::Draw(void)
{
	D3D_DEVICE->SetFVF(FVF_VERTEX);		// 頂点フォーマット指定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos);

	// テクスチャのセット
	D3D_DEVICE->SetTexture(0, m_pTexture);

	// モーフィングモデルを描画
	if(m_ModelStatus.size() > 0)
	{
		D3D_DEVICE->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
			0, m_ModelStatus[0].num_vertex, m_ModelStatus[0].num_face, m_ModelStatus[0].pt_index,
			D3DFMT_INDEX16, m_MorphVertex, sizeof(VERTEX));
	}

#ifdef _DEBUG
	CDebugProc::DebugProc("FRAME:%d, Key:%d\n", m_MorphCount, m_MorphStatus);
	CDebugProc::DebugProc("TIME:%.2fsec", (m_MorphCountAll / 60.0f));
#endif
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
	FILE		*fp;	// ファイルポインタ
	string str = ".\\data\\MODEL\\";
	str += fileDir;
	string morphFile = str + "morph.txt";

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

	LPDIRECT3DVERTEXBUFFER9	vertex_buffer;	// 頂点バッファ
	LPDIRECT3DINDEXBUFFER9	index_buffer;	// インデックスバッファ
	VERTEX*		pVertices;		// 頂点データへのポインタ
	WORD*		pIndices;		// ポリゴン構成（頂点リンク）データへのポインタ

	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		string strbuff = str;

		strbuff += filename;
		strbuff += to_string(i);
		strbuff += ".x";
		// もし3Dモデルファイルのファイル名が間違っていた場合、ダミーのモデルを読み込む。
		if(fopen_s(&fp, strbuff.c_str(), "r") == NULL)
		{// ファイル名が正常
			fclose(fp);
			D3DXLoadMeshFromX(strbuff.c_str(), D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);

			m_ModelStatus[i].pMesh->GetVertexBuffer(&vertex_buffer);	// 頂点バッファオブジェクトへのポインタをゲット
			m_ModelStatus[i].pMesh->GetIndexBuffer(&index_buffer);		// インデックスバッファオブジェクトへのポインタをゲット
			m_ModelStatus[i].num_vertex = m_ModelStatus[i].pMesh->GetNumVertices();	// 頂点数をゲット
			m_ModelStatus[i].num_face = m_ModelStatus[i].pMesh->GetNumFaces();		// 面数をゲット

			// 頂点データ、インデックスデータをメモリにコピー
			m_ModelStatus[i].pt_vertex = new VERTEX[m_ModelStatus[i].num_vertex];	// 頂点ワーク領域を確保
			m_ModelStatus[i].pt_index = new WORD[m_ModelStatus[0].num_face * 3];	// インデックスワーク領域を確保

			vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);											// 頂点バッファをロック
			memcpy(m_ModelStatus[i].pt_vertex, pVertices, sizeof(VERTEX)*m_ModelStatus[0].num_vertex);	// 頂点データをワークにコピー
			vertex_buffer->Unlock();																	// 頂点バッファをアンロック

			index_buffer->Lock(0, 0, (void**)&pIndices, 0);												// インデックスバッファをロック
			memcpy(m_ModelStatus[i].pt_index, pIndices, sizeof(WORD)*m_ModelStatus[0].num_face * 3);	// インデックスデータをワークにコピー
			index_buffer->Unlock();																		// インデックスバッファをアンロック
		}
		else
		{// 指定したファイルが存在していない
			D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);
		}
	}

	// モーフィング用に頂点バッファ、インデックスバッファを作成
	m_MorphVertex = new VERTEX[m_ModelStatus[0].num_vertex];		// モーフィング用頂点ワーク領域を確保
	m_MorphIndex = new WORD[m_ModelStatus[0].num_face * 3];			// モーフィング用インデックスワーク領域を確保

	vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);								// 頂点バッファをロック
	memcpy(m_MorphVertex, pVertices, sizeof(VERTEX)*m_ModelStatus[0].num_vertex);	// 頂点データをワークにコピー
	vertex_buffer->Unlock();														// 頂点バッファをアンロック

	index_buffer->Lock(0, 0, (void**)&pIndices, 0);								// インデックスバッファをロック
	memcpy(m_MorphIndex, pIndices, sizeof(WORD)*m_ModelStatus[0].num_face * 3);	// インデックスデータをワークにコピー
	index_buffer->Unlock();														// インデックスバッファをアンロック

	// テクスチャ読み込み
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\MODEL\\tex.jpg", &m_pTexture);
}

string CModel::to_string(int val)
{
	char buffer[numeric_limits<int>::digits10 + 2]; // '-' + NULL
	sprintf(buffer, "%d", val);
	return buffer;
}