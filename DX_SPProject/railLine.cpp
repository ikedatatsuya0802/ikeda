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
#include <time.h>
#include "readfile.h"
#include "railLine.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"

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

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffSPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPointer, NULL);


	// 頂点データセット
	SetSplineVtx();
	SetSplineVtxVec();
	SetSplineVtxSPoints();
	SetSplineVtxLPoints();
	SetSplineVtxPointer();

	// テクスチャのロード
	Load();
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
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i <= (int)m_Spline.PosHermite.size() ; i++)
		{
			// 座標設定
			if(i == (int)m_Spline.PosHermite.size())
			{
				pVtx[i].Pos = D3DXVECTOR3(m_Spline.Pos[(int)m_Spline.Pos.size() - 1].x, 1.0f, m_Spline.Pos[(int)m_Spline.Pos.size() - 1].z);
			}
			else
			{
				pVtx[i].Pos = D3DXVECTOR3(m_Spline.PosHermite[i].x, 1.0f, m_Spline.PosHermite[i].z);
			}

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
//	関数名	:SetSplineVtxVec
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインの制御ベクトルの頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtxVec(int line)
{
	VERTEX_3D	*pVtx;	// 3D頂点情報

	m_pVtxBuffVec->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			// 座標設定
			pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline.Pos[i].x, 1.0f, m_Spline.Pos[i].z);
			pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline.Pos[i].x + m_Spline.Vec[i].x, 1.0f, m_Spline.Pos[i].z + m_Spline.Vec[i].z);

			// 法線設定
			pVtx[i * 2 + 0].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i * 2 + 1].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i * 2 + 0].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[i * 2 + 1].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);

			// テクスチャ貼付座標設定
			pVtx[i * 2 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[i * 2 + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}
	m_pVtxBuffVec->Unlock();
}

//=============================================================================
//	関数名	:SetSplineVtxSPoints
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインの頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtxSPoints(int line)
{
	VERTEX_3D	*pVtx;	// 頂点情報
	

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
//	関数名	:SetSplineVtxLPoints
//	引数	:なし
//	戻り値	:なし
//	説明	:スプラインの頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtxLPoints(int line)
{
	VERTEX_3D	*pVtx;	// 頂点情報
	
	
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
//	関数名	:SetSplineVtxPointer
//	引数	:なし
//	戻り値	:なし
//	説明	:マウスのドラッグ範囲の頂点データをセットする。
//=============================================================================
void CRailLine::SetSplineVtxPointer(int line)
{
	VERTEX_3D	*pVtx;	// 頂点情報


	m_pVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_DRAG_SIZE / 2), 0.5f, (RAILLINE_DRAG_SIZE / 2));
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_DRAG_SIZE / 2), 0.5f, (RAILLINE_DRAG_SIZE / 2));
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_DRAG_SIZE / 2), 0.5f, -(RAILLINE_DRAG_SIZE / 2));
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_DRAG_SIZE / 2), 0.5f, -(RAILLINE_DRAG_SIZE / 2));

		// 法線設定
		for(int i = 0 ; i < VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ貼付座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffPointer->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CRailLine::Uninit(void)
{
	SafetyRelease(m_pTexture);
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pVtxBuffVec);
	SafetyRelease(m_pVtxBuffSPoints);
	SafetyRelease(m_pVtxBuffLPoints);
	SafetyRelease(m_pVtxBuffPointer);

	Unload();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CRailLine::Update(void)
{
	// 描画フラグ設定
	m_flgDraw = CManager::GetCamera()->GetCameraMode() ? true : false;

	if(CManager::GetCamera()->GetCameraMode())
	{// エディットモードの場合のみ処理

		// カメラの高さに応じたスケールを設定
		m_YScale = (CManager::GetCamera()->GetCameraPosV().y / 2000.0f);
		
		// スプラインをセーブ
		if(CInput::GetKeyTrigger(DIK_F1))
		{
			// スプライン情報をセーブ
			SaveSpline();

			// デバッグ情報表示カウントをセット
			m_DebugProcCnt = DEBUGPROC_CNT;
		}
		
		// スプラインの制御点を増やす
		if(CInput::GetKeyTrigger(DIK_F2))
		{
			AddPoints();
		}
		else if(CInput::GetKeyTrigger(DIK_F3))
		{
			DeletePoints();
		}


		// マウスによるエディット動作
		MouseEdit();

		// 線形の再計算
		SetSplineVtx();
		SetSplineVtxVec();
	}
}

//=============================================================================
//	関数名	:MouseEdit
//	引数	:無し
//	戻り値	:無し
//	説明	:マウスによるエディット動作。
//=============================================================================
void CRailLine::MouseEdit(void)
{
	if(CInput::GetMousePress(MS_LB))
	{// マウスが押されている状態

		if(CInput::GetMouseTrigger(MS_LB))
		{// ドラッグフラグ設定

			// 後の頂点を優先的に選択し、ドラッグフラグをオンに
			for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
			{
				D3DXVECTOR3 a = CInput::GetMouseWorldPos(), b = m_Spline.Pos[i];
				float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 0.0f, a.z) - D3DXVECTOR3(b.x, 0.0f, b.z)));

				if(length < RAILLINE_DRAG_SIZE)
				{
					m_Spline.ifHold[i] = true;
					break;
				}
			}
		}

		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			if(m_Spline.ifHold[i])
			{
				m_Spline.Pos[i] = CInput::GetMouseWorldPos();
			}
		}

		// スプラインの再計算
		CalcSpline();
	}
	else if(CInput::GetMouseRelease(MS_LB))
	{// マウスがリリースされた場合

		// 全頂点のホールドを解除
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			m_Spline.ifHold[i] = false;
		}
	}

	if(CInput::GetMousePress(MS_RB))
	{// マウスが押されている状態

		if(CInput::GetMouseTrigger(MS_RB))
		{// ドラッグフラグ設定

			// 後の頂点を優先的に選択し、ドラッグフラグをオンに
			for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
			{
				D3DXVECTOR3 a = CInput::GetMouseWorldPos();
				D3DXVECTOR3 b = m_Spline.Pos[i] + m_Spline.Vec[i];
				float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 0.0f, a.z) - D3DXVECTOR3(b.x, 0.0f, b.z)));

				if(length < (RAILLINE_DRAG_SIZE * m_YScale))
				{
					m_Spline.ifHold[i] = true;
					break;
				}
			}
		}

		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			if(m_Spline.ifHold[i])
			{
				m_Spline.Vec[i] = CInput::GetMouseWorldPos() - m_Spline.Pos[i];
			}
		}

		// スプラインの再計算
		CalcSpline();
	}
	else if(CInput::GetMouseRelease(MS_RB))
	{// マウスがリリースされた場合

		// 全頂点のホールドを解除
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			m_Spline.ifHold[i] = false;
		}
	}
}

//=============================================================================
//	関数名	:AddPoints
//	引数	:無し
//	戻り値	:無し
//	説明	:スプライン制御点を増やす。
//=============================================================================
void CRailLine::AddPoints(void)
{
	// 制御座標を追加
	D3DXVECTOR3 pos = m_Spline.Pos[m_Spline.Pos.size() - 1];
	m_Spline.Pos.push_back(D3DXVECTOR3(pos.x, pos.y, pos.z));
	m_Spline.ifHold.push_back(false);

	// 制御ベクトルを追加
	D3DXVECTOR3 vec = m_Spline.Vec[m_Spline.Vec.size() - 1];
	m_Spline.Vec.push_back(D3DXVECTOR3(0.0f, 0.0f, 500.0f));

	// 頂点バッファのリサイズ
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pVtxBuffVec);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
	SetSplineVtx();
	SetSplineVtxVec();

	// スプラインの再計算
	CalcSpline();
}

//=============================================================================
//	関数名	:DeletePoints
//	引数	:無し
//	戻り値	:無し
//	説明	:スプライン制御点を削除する。
//=============================================================================
void CRailLine::DeletePoints(void)
{
	// 最低限2個の制御点は保持する
	if(m_Spline.Pos.size() > 2)
	{
		// 制御座標を削除
		m_Spline.Pos.pop_back();
		m_Spline.ifHold.pop_back();

		// 制御ベクトルを削除
		m_Spline.Vec.pop_back();

		// 頂点バッファのリサイズ
		SafetyRelease(m_pVtxBuff);
		SafetyRelease(m_pVtxBuffVec);
		D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
		D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
		SetSplineVtx();
		SetSplineVtxVec();

		// スプラインの再計算
		CalcSpline();
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
	if(m_flgDraw)
	{
		// マウス座標に対応するスケール
		D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		scl *= m_YScale;

		// マトリックス設定
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// ライティング設定をオフに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 線形描画
		D3D_DEVICE->SetTexture(0, NULL);	// テクスチャの設定
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);	// 頂点フォーマットの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
		D3D_DEVICE->DrawPrimitive(D3DPT_LINESTRIP, 0, m_Spline.PosHermite.size());	// 描画


		// ベクトル描画
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffVec, 0, sizeof(VERTEX_3D));		// 頂点フォーマットの設定
		D3D_DEVICE->DrawPrimitive(D3DPT_LINELIST, 0, (m_Spline.Pos.size() * 2));	// 描画

		// 加算合成設定
		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		D3D_DEVICE->SetTexture(0, m_pTexture);	// テクスチャの設定
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

		// 頂点フォーマットの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffLPoints, 0, sizeof(VERTEX_3D));
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			D3DXVECTOR3 pos = m_Spline.Pos[i];
			pos.y += 1.0f;

			// マトリックス設定
			CRendererDX::SetMatrixBB(&m_mtxWorld, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), scl);

			// 描画
			D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);
		}

		// 頂点フォーマットの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffPointer, 0, sizeof(VERTEX_3D));
		// マトリックス設定
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * (m_YScale / 2);
		CRendererDX::SetMatrix(&m_mtxWorld, CInput::GetMouseWorldPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), scl);
		// 描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);

		// ライティング設定をオンに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// レンダーステート設定を戻す
		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		// デバッグ情報表示
#ifdef _DEBUG
		if(CManager::GetCamera()->GetCameraMode())
		{
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				CDebugProc::DebugProc("スプライン座標[%d]:(%5.2f:%5.2f:%5.2f)\n", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
			}
		}

		if(m_DebugProcCnt > 0)
		{
			CDebugProc::DebugProc("スプライン情報をセーブしました\n");
			m_DebugProcCnt--;
		}
#endif
	}
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
//	関数名	:SaveSpline
//	引数	:なし
//	戻り値	:なし
//	説明	:スプライン情報をファイルにセーブする。
//=============================================================================
void CRailLine::SaveSpline(void)
{
	FILE	*fp = NULL;					// ファイルポインタ
	time_t now = time(NULL);			// 現在時刻
	struct tm *pnow = localtime(&now);	// 現在時刻

	fp = fopen("./data/spline.txt", "w");

	fprintf_s(fp, "//==============================================================================\n");
	fprintf_s(fp, "//\n");
	fprintf_s(fp, "//	スプラインデータ用スクリプトファイル\n");
	fprintf_s(fp, "//\n");
	fprintf_s(fp, "//	データ出力日時：%2d/%2d/%2d %2d:%2d:%2d\n", pnow->tm_year, pnow->tm_mon, pnow->tm_mday, pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
	fprintf_s(fp, "//\n");
	fprintf_s(fp, "//==============================================================================\n");
	fprintf_s(fp, "SCRIPT				// 消すなよ！絶対消すなよ！\n\n");

	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	fprintf_s(fp, "// 座標情報\n");
	fprintf_s(fp, "//------------------------------------------------------------------------------\n");

	for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
	{
		fprintf_s(fp, "POS = %.1f %.1f %.1f\n", m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
	}
	fprintf_s(fp, "\n");


	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	fprintf_s(fp, "// ベクトル情報\n");
	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	for(int i = 0 ; i < (int)m_Spline.Vec.size() ; i++)
	{
		fprintf_s(fp, "VEC = %.1f %.1f %.1f\n", m_Spline.Vec[i].x, m_Spline.Vec[i].y, m_Spline.Vec[i].z);
	}
	fprintf_s(fp, "\n");

	fprintf_s(fp, "END_SCRIPT			// 消すなよ！絶対消すなよ！");

	fclose(fp);
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

	// END_SCRIPTまで読み込み
	while(!feof(fp))
	{
		char str[65535] = {NULL};
		memset(str, NULL, sizeof(str));

		// 単語を取得
		fscanf(fp, "%s", str);

		if(strcmp(str, "POS") == 0)
		{// スプライン制御点を追加

			D3DXVECTOR3 pos;
			fscanf(fp, " = %f %f %f\n", &pos.x, &pos.y, &pos.z);
			m_Spline.Pos.push_back(pos);
			m_Spline.ifHold.push_back(false);
		}
		else if(strcmp(str, "VEC") == 0)
		{// スプライン制御ベクトルを追加
			
			D3DXVECTOR3 vec;
			fscanf(fp, " = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			m_Spline.Vec.push_back(vec);
		}
		else if(strcmp(str, "END_SCRIPT") == 0)
		{// スプライン読み込みを終了

			break;
		}
	}

	// エルミート座標を追加
	m_Spline.PosHermite.resize(RAILLINE_SET);

	// ファイルクローズ
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
		// スプラインを計算する
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++, t += (1.0f / RAILLINE_SET))
		{
			int		posNum	= (int)(t * ((int)m_Spline.Pos.size() - 1));
			float	nowt	= t * ((int)m_Spline.Pos.size() - 1) - posNum;

			m_Spline.PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].x)
				+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].x);
			m_Spline.PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].z)
				+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].z);
			m_Spline.PosHermite[i].y = 1.0f;
		}
	}

	/*
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
	}*/
}