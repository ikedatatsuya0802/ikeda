//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	EStructure.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "EStructure.h"
#include "manager.h"
#include "rendererDX.h"
#include "manager.h"
#include "cameraDX.h"
#include "game.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CEStructure::m_pTexture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CEStructure::CEStructure(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CEStructure::~CEStructure()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CEStructure::Init(int line, D3DXVECTOR3 pos)
{	
	char			*str	= NULL;	// ファイル内容格納配列
	unsigned int	offset	= 0;	// 文字列指定子

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_EStructureLine = line;
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * (m_Spline->PosHermite.size() * 2 + 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ESTRUCTURE_TEXFILENAME000, &m_pTexture);
	
	// レール情報セット
	SetVtxBuff();

	Load();
}

//=============================================================================
//	関数名	:SetEStructureData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CEStructure::SetVtxBuff(void)
{
	VERTEX_3D			*pVtx;		// 3D頂点情報

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;
		//D3DXVECTOR3 a[ESTRUCTURE_VERTEX];

		// 描画座標設定
		rot = atan2f((m_Spline->PosHermite[1].x - m_Spline->PosHermite[0].x), (m_Spline->PosHermite[1].z - m_Spline->PosHermite[0].z));
		pVtx[0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z - sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
		pVtx[1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z + sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
		for(int i = 1 ; i < (int)m_Spline->PosHermite.size() ; i++)
		{
			// 角度設定
			if(i != ((int)m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[i + 1].x - m_Spline->PosHermite[i].x), (m_Spline->PosHermite[i + 1].z - m_Spline->PosHermite[i].z));
			}
			else
			{
				rot = atan2f((m_Spline->PosHermite[i].x - m_Spline->PosHermite[i - 1].x), (m_Spline->PosHermite[i].z - m_Spline->PosHermite[i - 1].z));
			}

			if(i == ((int)m_Spline->PosHermite.size() - 1))
			{
				pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
					0.1f,
					m_Spline->PosHermite[0].z - sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
				pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f),
					0.1f,
					m_Spline->PosHermite[0].z + sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
			}
			else
			{
				pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[i].x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
					0.1f,
					m_Spline->PosHermite[i].z - sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
				pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[i].x - cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f),
					0.1f,
					m_Spline->PosHermite[i].z + sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
			}
		}

		for(int i = 0 ; i < ((int)m_Spline->PosHermite.size() * 2 + 2) ; i++)
		{
			// 法線設定
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		for(int i = 1 ; i < (int)m_Spline->PosHermite.size() ; i++)
		{
			pVtx[i * 2 + 0].tex = D3DXVECTOR2(0.0f, (float)i * 0.75f);
			pVtx[i * 2 + 1].tex = D3DXVECTOR2(1.0f, (float)i * 0.75f);
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
void CEStructure::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CEStructure::Update(void)
{
	static SPLINE spline = *CGame::GetRailLine()->GetSpline();

	// 描画フラグ設定
	m_flgDraw = DX_CAMERA->GetCameraMode() ? false : true;

	// レールがエディットされた場合、更新
	if(fabsf(spline.Length - m_Spline->Length) < 0.01f)
	{
		// レール情報セット
		SetVtxBuff();
	}

	spline = (*m_Spline);
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CEStructure::Draw(void)
{
	if(m_flgDraw)
	{
		// マトリックス設定
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// Zテスト開始
		CRendererDX::EnableZTest();

		// ライティング設定をオフに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 描画処理
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
		D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, ((int)m_Spline->PosHermite.size() * 2));	// 描画

		// ライティング設定をオンに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Zテスト終了
		CRendererDX::DisableZTest();
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CEStructure *CEStructure::Create(int line, D3DXVECTOR3 pos)
{
	CEStructure *scene3D;

	scene3D = new CEStructure;
	
	scene3D->Init(line, pos);

	return scene3D;
}