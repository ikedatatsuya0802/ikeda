//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	rail.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "rail.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"
#include "game.h"
#include "railLine.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CRail::m_pTexture;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CRail::CRail(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CRail::~CRail()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CRail::Init(int line, D3DXVECTOR3 pos)
{
	VERTEX_3D			*pVtx;										// 3D頂点情報
	
	char			*str	= NULL;	// ファイル内容格納配列
	unsigned int	offset	= 0;	// 文字列指定子

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_RailLine = line;
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * (m_Spline->PosHermite.size() * 2 + 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAIL_TEXFILENAME000, &m_pTexture);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;
		//D3DXVECTOR3 a[RAIL_VERTEX];

		// 描画座標設定
		rot = atan2f((m_Spline->PosHermite[1].x - m_Spline->PosHermite[0].x), (m_Spline->PosHermite[1].z - m_Spline->PosHermite[0].z));
		pVtx[0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
		pVtx[1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z + sinf(rot) * (RAIL_WIDTH * 0.5f));
		for(int nCntInit = 1 ; nCntInit < (int)m_Spline->Pos.size() ; nCntInit++)
		{
			// 角度設定
			if(nCntInit != (m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[nCntInit + 1].x - m_Spline->PosHermite[nCntInit].x), (m_Spline->PosHermite[nCntInit + 1].z - m_Spline->PosHermite[nCntInit].z));
			}
			else
			{
				rot = atan2f((m_Spline->PosHermite[nCntInit].x - m_Spline->PosHermite[nCntInit - 1].x), (m_Spline->PosHermite[nCntInit].z - m_Spline->PosHermite[nCntInit - 1].z));
			}

			pVtx[nCntInit * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[nCntInit].x + (cosf(rot) * (RAIL_WIDTH * 0.5f)),
													1.0f,
													m_Spline->PosHermite[nCntInit].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
			pVtx[nCntInit * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[nCntInit].x - cosf(rot) * (RAIL_WIDTH * 0.5f),
													1.0f,
													m_Spline->PosHermite[nCntInit].z + sinf(rot) * (RAIL_WIDTH * 0.5f));

			//a[nCntInit * 2 + 0] = D3DXVECTOR3(cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, sinf(rot) * (RAIL_WIDTH * 0.5f));
			//a[nCntInit * 2 + 1] = D3DXVECTOR3(-cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, -sinf(rot) * (RAIL_WIDTH * 0.5f));
		}
	
		for(int nCntInit = 0 ; nCntInit < ((int)m_Spline->PosHermite.size() * 2 + 2) ; nCntInit++)
		{
			// 法線設定
			pVtx[nCntInit].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[nCntInit].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}
	
		// テクスチャ貼付座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		for(int nCntInit = 1 ; nCntInit < (int)m_Spline->PosHermite.size() ; nCntInit++)
		{
			pVtx[nCntInit * 2 + 0].tex = D3DXVECTOR2(0.0f, (float)nCntInit * 2.0f);
			pVtx[nCntInit * 2 + 1].tex = D3DXVECTOR2(1.0f, (float)nCntInit * 2.0f);
		}

	}
	m_pVtxBuff->Unlock();

	Load();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CRail::Uninit(void)
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
void CRail::Update(void)
{
	/*static float t = 0.0f;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float nowt = (t - ((int)t));

	pos.x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].x)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].x);
	pos.z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].z)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].z);	
	pos.y = 20.0f;

	CManager::GetCamera()->SetCameraPos(pos, D3DXVECTOR3(0.0f, pos.y + 100.0f, pos.z - 200.0f));


	t += (1.0f / RAIL_SET) * 0.1f;
	if(t > (m_Spline->nNum - 1))
	{
		t -= m_Spline->nNum;
	}*/
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CRail::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, ((int)m_Spline->Pos.size() * 2));	// 描画

	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

	// デバッグ情報表示
#ifdef _DEBUG
	for(int i = 0 ; i < (int)m_Spline->Pos.size() ; i++)
	{
		CDebugProc::DebugProc("スプライン座標[%d]:(%5.2f:%5.2f:%5.2f)\n", i, m_Spline->Pos[i].x, m_Spline->Pos[i].y, m_Spline->Pos[i].z);
	}
#endif
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CRail *CRail::Create(int line, D3DXVECTOR3 pos)
{
	CRail *scene3D;

	scene3D = new CRail;
	
	scene3D->Init(line, pos);

	return scene3D;
}