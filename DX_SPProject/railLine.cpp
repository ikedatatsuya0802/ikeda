//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	railLine.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include <stdio.h>
#include "readfile.h"
#include "railLine.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3DTEXTURE9	CRailLine::m_pTexture;
LPDIRECT3DTEXTURE9	CRailLine::m_pTexturePoints;

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CRailLine::CRailLine(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CRailLine::~CRailLine()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CRailLine::Init(int line, D3DXVECTOR3 pos)
{
	char			*str	= NULL;	// ファイル内容格納配列
	unsigned int	offset	= 0;	// 文字列指定子

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_RailLineLine = line;
	
	LoadSpline(line);
	CalcSpline(line);

	// 頂点データセット
	SetSplineVtx();
	SetSplineVtxSPoints();
	SetSplineVtxLPoints();

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, "./data/TEXTURE/effect000.jpg", &m_pTexturePoints);

	//Load();
}

//=============================================================================
//	関数名	:SetSplineVtx
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインの頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtx(int line)
{
	VERTEX_3D	*pVtx;	// 3D頂点情報


	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * (((m_Spline.nNum - 1) * RAILLINE_SET) + 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
		{
			// 座標設定
			pVtx[i].Pos = D3DXVECTOR3(m_Spline.PosHermite[i].x, 1.0f, m_Spline.PosHermite[i].z);

			// 法線設定
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ貼付座標設定
			pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}
	m_pVtxBuff->Unlock();	
}

//=============================================================================
//	関数名	:SetSplineVtx
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインの頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtxSPoints(int line)
{
	VERTEX_3D	*pVtx;	// 頂点情報
	

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffSPoints, NULL);

	m_pVtxBuffSPoints->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
		}

		// テクスチャ貼付座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffSPoints->Unlock();
}

//=============================================================================
//	関数名	:SetSplineVtx
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインの頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtxLPoints(int line)
{
	VERTEX_3D	*pVtx;	// 頂点情報
	
	
	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLPoints, NULL);

	m_pVtxBuffLPoints->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);
		}

		// テクスチャ貼付座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffLPoints->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CRailLine::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);

	//Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CRailLine::Update(void)
{
	for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
	{
		m_Spline.Pos[i];
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CRailLine::Draw(void)
{
	// マトリックス設定
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 描画処理
	D3D_DEVICE->SetTexture(0, NULL);	// テクスチャの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);	// 頂点フォーマットの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->DrawPrimitive(D3DPT_LINESTRIP, 0, m_Spline.PosHermite.size());	// 描画

	// 加算合成設定
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテスト開始
	//D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	//D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
	//D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	D3D_DEVICE->SetTexture(0, m_pTexturePoints);	// テクスチャの設定
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuffSPoints, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
	{
		if((i % RAILLINE_SET) != 0)
		{
			// マトリックス設定
			CRendererDX::SetMatrixBB(&m_mtxWorld, m_Spline.PosHermite[i]);

			// 描画
			D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);
		}
	}

	D3D_DEVICE->SetStreamSource(0, m_pVtxBuffLPoints, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
	{
		// マトリックス設定
		CRendererDX::SetMatrixBB(&m_mtxWorld, m_Spline.Pos[i]);

		// 描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);
	}
	
	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zテスト終了
	//D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	//D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// レンダーステート設定を戻す
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	// デバッグ情報表示
#ifdef _DEBUG
	for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
	{
		CDebugProc::DebugProc("スプライン座標[%d]:(%5.2f:%5.2f:%5.2f)\n", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
	}
#endif
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CRailLine *CRailLine::Create(int line, D3DXVECTOR3 pos)
{
	CRailLine *scene3D;

	scene3D = new CRailLine;
	
	scene3D->Init(line, pos);

	return scene3D;
}

//=============================================================================
//	関数名	:LoadSpline
//	引数	:なし
//	戻り値	:なし
//	説明	:ファイルからスプライン情報を読み込む。
//=============================================================================
void CRailLine::LoadSpline(int line)
{
	FILE	*fp = NULL;	// ファイルポインタ

	fp = fopen("./data/spline.txt", "r");

	// ファイル終了まで読み込み
	while(feof(fp) != EOF)
	{
		char str[65535] = {NULL};
		memset(str, NULL, sizeof(str));

		fscanf(fp, "%s", str);

		if(strcmp(str, "NUM_POSITION") == 0)
		{
			fscanf(fp, " = %d", &m_Spline.nNum);

			if(m_Spline.nNum > 0)
			{
				m_Spline.PosHermite.resize((m_Spline.nNum - 1) * RAILLINE_SET);
			}
		}
		else if(strcmp(str, "POS") == 0)
		{
			D3DXVECTOR3 pos;
			fscanf(fp, " = %f %f %f\n", &pos.x, &pos.y, &pos.z);
			m_Spline.Pos.push_back(pos);
			m_Spline.ifHold.push_back(false);
		}
		else if(strcmp(str, "VEC") == 0)
		{
			D3DXVECTOR3 vec;
			fscanf(fp, " = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			m_Spline.Vec.push_back(vec);
		}
		else if(strcmp(str, "END_SCRIPT") == 0)
		{
			break;
		}
	}

	fclose(fp);
}

//=============================================================================
//	関数名	:CalcSpline
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインを計算する。
//=============================================================================
void CRailLine::CalcSpline(int line)
{
	float t		= 0.0f;
	float rot	= 0.0f;
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if((int)m_Spline.Pos.size() >= 2)
	{
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++, t += (1.0f / RAILLINE_SET))
		{
			float nowt = (t - ((int)t));

			m_Spline.PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[(int)t + 1].x)
								+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[(int)t + 1].x);
			m_Spline.PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[(int)t + 1].z)
								+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[(int)t + 1].z);	
			m_Spline.PosHermite[i].y = 1.0f;
		}
	}

	
	for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++, t += (1.0f / RAILLINE_SET))
	{
		float nowt = (t - ((int)t));

		if((int)t == 0)
		{
			vec = D3DXVECTOR3((m_Spline.PosHermite[(int)t + 1].x - m_Spline.PosHermite[(int)t].x), 0.0f, (m_Spline.PosHermite[(int)t + 1].z - m_Spline.PosHermite[(int)t].z));
		}
		else if(i == (RAILLINE_SET - 1))
		{
			vec = D3DXVECTOR3((m_Spline.PosHermite[(int)t].x - m_Spline.PosHermite[(int)t - 1].x), 0.0f, (m_Spline.PosHermite[(int)t].z - m_Spline.PosHermite[(int)t - 1].z));
		}
		else
		{
			vec = D3DXVECTOR3((m_Spline.PosHermite[(int)t + 1].x - m_Spline.PosHermite[(int)t - 1].x), 0.0f, (m_Spline.PosHermite[(int)t + 1].z - m_Spline.PosHermite[(int)t - 1].z));
		}
		D3DXVec3Normalize(&vec, &vec);
		vec *= RAILLINE_MARGIN;

		if(line > 0)
		{
			m_Spline.PosHermite[i].x -= cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline.PosHermite[i].z -= sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}
		else if(line < 0)
		{
			m_Spline.PosHermite[i].x += cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline.PosHermite[i].z += sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}

		t += (1.0f / RAILLINE_SET);
	}
}