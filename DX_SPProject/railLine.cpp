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
#include "railLine.h"
#include "main.h"
#include <time.h>
#include "input.h"
#include "manager.h"
#include "cameraDX.h"

//=============================================================================
//	静的メンバ
//=============================================================================
LPDIRECT3DTEXTURE9 CRailLine::m_pTextures[2];

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CRailLine::CRailLine(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
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
	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_RailLine = line;
	m_EditType = ET_POSVEC;
	
	LoadSpline(line);
	CalcSpline(line);

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffSPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPointer, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffDrift, NULL);

	// 頂点データセット
	SetSplineVtx();
	SetSplineVtxVec();
	SetSplineVtxSPoints();
	SetSplineVtxLPoints();
	SetSplineVtxPointer();
	SetSplineVtxDrift();

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
	VERTEX_3D* pVtx;	// 3D頂点情報
	
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

			// テクスチャ座標設定
			pVtx[i].tex = VEC2_ZERO;
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
	VERTEX_3D* pVtx;	// 3D頂点情報

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

			// テクスチャ座標設定
			pVtx[i * 2 + 0].tex = VEC2_ZERO;
			pVtx[i * 2 + 1].tex = VEC2_ZERO;
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
	VERTEX_3D* pVtx;	// 頂点情報
	

	m_pVtxBuffSPoints->Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
	pVtx[1].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
	pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);
	pVtx[3].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);

	// 法線設定
	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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
	VERTEX_3D* pVtx;	// 頂点情報
	
	
	m_pVtxBuffLPoints->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);
		}

		// テクスチャ座標設定
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
	VERTEX_3D* pVtx;	// 頂点情報


	m_pVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_DRAG_SIZE / 2), 0.5f, (RAILLINE_DRAG_SIZE / 2));
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_DRAG_SIZE / 2), 0.5f, (RAILLINE_DRAG_SIZE / 2));
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_DRAG_SIZE / 2), 0.5f, -(RAILLINE_DRAG_SIZE / 2));
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_DRAG_SIZE / 2), 0.5f, -(RAILLINE_DRAG_SIZE / 2));

		// 法線設定
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffPointer->Unlock();
}

//=============================================================================
//	関数名	:SetSplineVtxDrift
//	引数	:なし
//	戻り値	:なし
//	説明	:ドリフトの開始・終点のマーク。
//=============================================================================
void CRailLine::SetSplineVtxDrift(int line)
{
	VERTEX_3D* pVtx;	// 頂点情報


	m_pVtxBuffDrift->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 座標設定
		pVtx[0].Pos = D3DXVECTOR3(-75.0f, 150.0f, 0.0f);
		pVtx[1].Pos = D3DXVECTOR3(75.0f, 150.0f, 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-75.0f, 0.0f, 0.0f);
		pVtx[3].Pos = D3DXVECTOR3(75.0f, 0.0f, 0.0f);

		// 法線設定
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色設定
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
	}
	m_pVtxBuffDrift->Unlock();
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
	SafetyRelease(m_pVtxBuffVec);
	SafetyRelease(m_pVtxBuffSPoints);
	SafetyRelease(m_pVtxBuffLPoints);
	SafetyRelease(m_pVtxBuffPointer);
	SafetyRelease(m_pVtxBuffDrift);

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
	m_flgDraw = CManager::GetEdhitMode() ? true : false;

	if(CManager::GetEdhitMode())
	{// エディットモードの場合のみ処理

		// エディットモード変更
		if(CInput::GetMouseTrigger(MBTN_CENTER))
		{
			if(m_EditType == ET_POSVEC) m_EditType = ET_DRIFT;
			else if(m_EditType == ET_DRIFT) m_EditType = ET_POSVEC;
		}

		// カメラの高さに応じたスケールを設定
		m_YScale = (DX_CAMERA->GetCameraPosV().y / 2000.0f);
		
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
	static POINT holdPos = {0};	// ホールド開始時のマウスのスクリーン座標

	if(CInput::GetMousePress(MBTN_LEFT))
	{// マウスが押されている状態

		switch(m_EditType)
		{
		case ET_POSVEC:
			if(CInput::GetMouseTrigger(MBTN_LEFT))
			{// ドラッグフラグ設定

				// 後の頂点を優先的に選択し、ドラッグフラグをオンに
				for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos(), b = m_Spline.Pos[i];
					float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(b.x, 1.0f, b.z)));

					if(length < RAILLINE_DRAG_SIZE)
					{
						holdPos = CInput::GetMouseScreenPos();
						m_Spline.ifHold[i] = 1;
						break;
					}
				}
			}

			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					if(KH_LSHIFT)
					{
						// 高さエディット
						m_Spline.Pos[i].y = ((float)holdPos.y - CInput::GetMouseScreenPos().y) * 2.0f;
						
						// 高さの下限値設定
						if(m_Spline.Pos[i].y < 1.0f)
						{
							m_Spline.Pos[i].y = 1.0f;
						}
					}
					else
					{
						m_Spline.Pos[i].x = CInput::GetMouseWorldPos().x;
						m_Spline.Pos[i].z = CInput::GetMouseWorldPos().z;
					}
				}
			}
			break;
		case ET_DRIFT:
			if(CInput::GetMouseTrigger(MBTN_LEFT))
			{// ドラッグフラグ設定

				// 後の頂点を優先的に選択し、ドラッグフラグをオンに
				for(int i = ((int)m_Spline.Drift.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos();
					D3DXVECTOR3 b = GetSplinePos(m_Spline.Drift[i].Begin);
					D3DXVECTOR3 c = GetSplinePos(m_Spline.Drift[i].End);
					float lengthBegin = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(b.x, 1.0f, b.z)));
					float lengthEnd = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(c.x, 1.0f, c.z)));

					if(lengthBegin < RAILLINE_DRAG_SIZE)
					{
						if(lengthBegin < lengthEnd) m_Spline.Drift[i].ifHoldBegin = true;
						break;
					}
					else if(lengthEnd < RAILLINE_DRAG_SIZE)
					{
						if(lengthEnd < lengthBegin) m_Spline.Drift[i].ifHoldEnd = true;
						break;
					}
				}
			}

			// ドリフト方向切り替え
			if(CInput::GetKeyTrigger(DIK_F4))
			{
#pragma omp parallel for
				for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
				{
					if(m_Spline.Drift[i].ifHoldBegin || m_Spline.Drift[i].ifHoldEnd)
					{
						//ChangeFlag(m_Spline.Drift[i].Curve);
						m_Spline.Drift[i].Curve = m_Spline.Drift[i].Curve ? false : true;
					}
				}
			}

			for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
			{
				// スプラインの最近点探索
				float per = 0.0f;
				int maxHermite = 0;

#pragma omp parallel for
				for(int j = 0 ; j < RAILLINE_DRIFT_SEARCH ; j++)
				{
					float tMax = (1.0f / RAILLINE_DRIFT_SEARCH * maxHermite);
					float t = (1.0f / RAILLINE_DRIFT_SEARCH * j);
					float lengthMin = D3DXVec3Length(&(CInput::GetMouseWorldPos() - GetSplinePos(tMax)));
					float length = D3DXVec3Length(&(CInput::GetMouseWorldPos() - GetSplinePos(t)));
					if(lengthMin > length)
					{
						maxHermite = j;
					}
				}

				if(m_Spline.Drift[i].ifHoldBegin)
				{
					m_Spline.Drift[i].Begin = ((1.0f / RAILLINE_DRIFT_SEARCH) * maxHermite);

					// 可動範囲設定
					if(i > 0)
					{
						if(m_Spline.Drift[i].Begin <= m_Spline.Drift[i - 1].End + RAILLINE_DRIFT_DRAG)
						{
							m_Spline.Drift[i].Begin = m_Spline.Drift[i - 1].End + RAILLINE_DRIFT_DRAG;
						}
					}

					if(i < (int)m_Spline.Drift.size() - 1)
					{
						if(m_Spline.Drift[i].Begin >= m_Spline.Drift[i].End - RAILLINE_DRIFT_DRAG)
						{
							m_Spline.Drift[i].Begin = m_Spline.Drift[i].End - RAILLINE_DRIFT_DRAG;
						}
					}
				}
				else if(m_Spline.Drift[i].ifHoldEnd)
				{
					m_Spline.Drift[i].End = ((1.0f / RAILLINE_DRIFT_SEARCH) * maxHermite);

					// 可動範囲設定
					if(i < (int)m_Spline.Drift.size() - 1)
					{
						if(m_Spline.Drift[i].End >= m_Spline.Drift[i + 1].Begin - RAILLINE_DRIFT_DRAG)
						{
							m_Spline.Drift[i].End = m_Spline.Drift[i + 1].Begin - RAILLINE_DRIFT_DRAG;
						}
					}

					if(m_Spline.Drift[i].End <= m_Spline.Drift[i].Begin + RAILLINE_DRIFT_DRAG)
					{
						m_Spline.Drift[i].End = m_Spline.Drift[i].Begin + RAILLINE_DRIFT_DRAG;
					}
				}
			}
			break;
		default:
			break;
		}

		if(KT_X)
		{// X座標を0に
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Pos[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}
		else if(KT_Z)
		{// Z座標を0に
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Pos[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}

		// スプラインの再計算
		CalcSpline();
	}
	else if(CInput::GetMousePress(MBTN_RIGHT))
	{
		switch(m_EditType)
		{
		case ET_POSVEC:
			if(CInput::GetMouseTrigger(MBTN_RIGHT))
			{// ドラッグフラグ設定

				// 後の頂点を優先的に選択し、ドラッグフラグをオンに
				for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos();
					D3DXVECTOR3 b = m_Spline.Pos[i] + m_Spline.Vec[i];
					float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(b.x, 1.0f, b.z)));

					if(length < (RAILLINE_DRAG_SIZE * m_YScale))
					{
						m_Spline.ifHold[i] = 1;
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
			break;
		default:
			break;
		}

		
		if(KT_X)
		{// X座標を0に
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Vec[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}		
		else if(KT_Z)
		{// Z座標を0に
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Vec[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}

		// スプラインの再計算
		CalcSpline();
	}
	else if(CInput::GetMouseRelease(MBTN_LEFT) || CInput::GetMouseRelease(MBTN_RIGHT))
	{// マウスがリリースされた場合

		// 全頂点のホールドを解除
#pragma omp parallel for
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			m_Spline.ifHold[i] = 0;
		}
#pragma omp parallel for
		for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
		{
			m_Spline.Drift[i].ifHoldBegin	= false;
			m_Spline.Drift[i].ifHoldEnd		= false;
		}

		// スプラインの再計算
		CalcSpline();
	}


	// 線形の再計算
	SetSplineVtx();
	SetSplineVtxVec();
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
	m_Spline.LengthMin.push_back(0);

	// 制御ベクトルを追加
	D3DXVECTOR3 vec = m_Spline.Vec[m_Spline.Vec.size() - 1];
	m_Spline.Vec.push_back(D3DXVECTOR3(vec.x, vec.y, vec.z));

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
		m_Spline.LengthMin.pop_back();

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
	static int dpCnt = 0;
	dpCnt++;
	if(dpCnt > 120 * 100000)
	{
		dpCnt = 0;
	}

	if(m_flgDraw)
	{
		// マウス座標に対応するスケール
		D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		if(m_YScale < 1.0f) m_YScale = 1.0f;
		scl *= m_YScale;

		// マトリックス設定
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// Zテスト開始
		D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		// アルファテスト開始
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

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

		// テクスチャの設定
		D3D_DEVICE->SetTexture(0, m_pTextures[0]);


		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffSPoints, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
		{
			if((i % RAILLINE_SET) != 0)
			{
				// マトリックス設定
				CRendererDX::SetMatrixBB(&m_mtxWorld, m_Spline.PosHermite[i], VEC3_ZERO, scl);

				// 描画
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);
			}
		}

		// 頂点フォーマットの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffLPoints, 0, sizeof(VERTEX_3D));
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			D3DXVECTOR3 pos = m_Spline.Pos[i];
			pos.y += 1.0f;

			// マトリックス設定
			CRendererDX::SetMatrixBB(&m_mtxWorld, pos, VEC3_ZERO, scl);

			// 描画
			D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);
		}

		// 頂点フォーマットの設定
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffPointer, 0, sizeof(VERTEX_3D));
		// マトリックス設定
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * (m_YScale / 2);
		CRendererDX::SetMatrix(&m_mtxWorld, CInput::GetMouseWorldPos(), VEC3_ZERO, scl);
		// 描画
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);

		if(m_EditType == ET_DRIFT)
		{
			// テクスチャの設定
			D3D_DEVICE->SetTexture(0, m_pTextures[1]);
			for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
			{
				D3DXVECTOR3 pos;
				VERTEX_3D *pVtx;
				float tex;

				// テクスチャ座標の設定
				m_pVtxBuffDrift->Lock(0, 0, (void**)&pVtx, 0);
				tex = m_Spline.Drift[i].Curve ? 0.25f : 0.0f;
				for(int texture = 0 ; texture < VERTEX_SQUARE ; texture++)
					pVtx[texture].tex = D3DXVECTOR2(tex + ((texture % 2) * 0.25f), ((float)(texture / 2)));
				m_pVtxBuffDrift->Unlock();
				// 頂点フォーマットの設定
				D3D_DEVICE->SetStreamSource(0, m_pVtxBuffDrift, 0, sizeof(VERTEX_3D));
				// スプライン座標取得
				pos = GetSplinePos(m_Spline.Drift[i].Begin);
				// マトリックス設定
				CRendererDX::SetMatrixBB(&m_mtxWorld, pos, VEC3_ZERO, scl);
				// 描画
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);

				// テクスチャ座標の設定
				m_pVtxBuffDrift->Lock(0, 0, (void**)&pVtx, 0);
				tex = m_Spline.Drift[i].Curve ? 0.75f : 0.5f;
				for(int texture = 0 ; texture < VERTEX_SQUARE ; texture++)
					pVtx[texture].tex = D3DXVECTOR2(tex + ((texture % 2) * 0.25f), ((float)(texture / 2)));
				m_pVtxBuffDrift->Unlock();
				// 頂点フォーマットの設定
				D3D_DEVICE->SetStreamSource(0, m_pVtxBuffDrift, 0, sizeof(VERTEX_3D));
				// スプライン座標取得
				pos = GetSplinePos(m_Spline.Drift[i].End);
				// マトリックス設定
				CRendererDX::SetMatrixBB(&m_mtxWorld, pos, VEC3_ZERO, scl);
				// 描画
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);
			}
		}

		// ライティング設定をオンに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// レンダーステート設定を戻す
		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Z・アルファテスト終了
		CRendererDX::DisableZTest();
		CRendererDX::DisableAlphaTest();
	}

	// デバッグ情報表示
#ifdef _DEBUG
	if(CManager::GetEdhitMode())
	{
		CDebugProc::DebugProc("エディットタイプ:%d\n", m_EditType);
		if(dpCnt / 120 % 2 == 0)
		{
			for(int i = 0 ; i < (int)m_Spline.Pos.size() / 2 ; i++)
			{
				CDebugProc::DebugProc("spl_pos[%2d]:(%5.1f:%5.1f:%5.1f) ", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
				CDebugProc::DebugProc("vec:(%5.1f:%5.1f:%5.1f)\n", m_Spline.Vec[i].x, m_Spline.Vec[i].y, m_Spline.Vec[i].z);
			}
			if((int)m_Spline.Pos.size() % 2 == 1)
			{
				CDebugProc::DebugProc("\n");
			}
		}
		else
		{
			for(int i = (int)m_Spline.Pos.size() / 2 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				CDebugProc::DebugProc("spl_pos[%2d]:(%5.1f:%5.1f:%5.1f) ", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
				CDebugProc::DebugProc("vec:(%5.1f:%5.1f:%5.1f)\n", m_Spline.Vec[i].x, m_Spline.Vec[i].y, m_Spline.Vec[i].z);
			}
		}
	}
	CDebugProc::DebugProc("スプライン長:%.1f\n", m_Spline.Length);

	CDebugProc::DebugProc("スプライン区間長\n");
	for(int i = 0 ; i < (int)m_Spline.LengthMin.size() - 1 ; i++)
	{
		CDebugProc::DebugProc("[%2d->%2d]:%4.0f, ", i, (i + 1), m_Spline.LengthMin[i]);

		if(i % 4 == 3)
		{
			CDebugProc::DebugProc("\n");
		}
	}
	CDebugProc::DebugProc("\n");

	if(m_DebugProcCnt > 0)
	{
		CDebugProc::DebugProc("スプライン情報をセーブしました\n");
		m_DebugProcCnt--;
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
//	関数名	:SaveSpline
//	引数	:なし
//	戻り値	:なし
//	説明	:スプライン情報をファイルにセーブする。
//=============================================================================
void CRailLine::SaveSpline(void)
{
	FILE* fp = NULL;					// ファイルポインタ
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


	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	fprintf_s(fp, "// ドリフト点情報\n");
	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
	{
		fprintf_s(fp, "DRIFT = %.4f %.4f %d\n", m_Spline.Drift[i].Begin, m_Spline.Drift[i].End, (int)m_Spline.Drift[i].Curve);
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
	FILE* fp = NULL;	// ファイルポインタ
	int posnum = 0;

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

			posnum++;
			D3DXVECTOR3 pos;
			fscanf(fp, " = %f %f %f\n", &pos.x, &pos.y, &pos.z);
			m_Spline.Pos.push_back(pos);
			m_Spline.ifHold.push_back(0);
			m_Spline.LengthMin.push_back(0.0f);
		}
		else if(strcmp(str, "VEC") == 0)
		{// スプライン制御ベクトルを追加
			
			D3DXVECTOR3 vec;
			fscanf(fp, " = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			m_Spline.Vec.push_back(vec);
		}
		else if(strcmp(str, "DRIFT") == 0)
		{// スプライン制御ベクトルを追加

			DRIFT_POINT drift;
			fscanf(fp, " = %f %f %d\n", &drift.Begin, &drift.End, &drift.Curve);
			m_Spline.Drift.push_back(drift);
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
	D3DXVECTOR3 vec = VEC3_ZERO;
	
	if((int)m_Spline.Pos.size() >= 2)
	{
		// スプラインを計算する
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
		{
			int		posNum	= (int)(t * ((int)m_Spline.Pos.size() - 1));
			float	nowt	= t * ((int)m_Spline.Pos.size() - 1) - posNum;

			m_Spline.PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].x)
				+ (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].x)
				+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].x)
				+ ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].x);

			m_Spline.PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].z)
				+ (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].z)
				+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].z)
				+ ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].z);
			
			// 高さ設定
			if(posNum < (int)m_Spline.Pos.size())
			{
				// 次点への高さの差分計算
				float high = m_Spline.Pos[posNum + 1].y - m_Spline.Pos[posNum].y;

				if(high > 0.0f)
				{// 上り坂

					float rad = nowt * D3DX_PI;
					float pow = (-cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

					m_Spline.PosHermite[i].y = m_Spline.Pos[posNum].y + high * pow;
					rad = 0;
				}
				else if(high < 0.0f)
				{// 下り坂

					high *= -1.0f;
					float rad = nowt * D3DX_PI;
					float pow = (cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

					m_Spline.PosHermite[i].y = (m_Spline.Pos[posNum].y - high) + high * pow;
				}
				else
				{
					m_Spline.PosHermite[i].y = m_Spline.Pos[posNum].y;
				}
			}
			else
			{
				m_Spline.PosHermite[i].y = m_Spline.Pos[posNum].y;
			}

			// 実時間を進める
			t += (1.0f / RAILLINE_SET);
		}

		// スプライン長を計算する
		m_Spline.Length = 0.0f;
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for(int i = 1 ; i < RAILLINE_DRIFT_SEARCH ; i++)
		{
			float oldT = (1.0f / RAILLINE_DRIFT_SEARCH * (i - 1));	// i-1のスプライン位置
			float t = (1.0f / RAILLINE_DRIFT_SEARCH * i);			// iのスプライン位置

			// 1区間の長さ算出
			float length = D3DXVec3Length(&(GetSplinePos(oldT) - GetSplinePos(t)));

			// 長さ加算
			m_Spline.Length += length;
		}

		// スプライン区間長を計算する
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for(int i = 0 ; i < ((int)m_Spline.LengthMin.size() - 1) ; i++)
		{
			m_Spline.LengthMin[i] = 0.0f;
			float prevT = (1.0f / (m_Spline.Pos.size() - 1) * i);
			float nextT = (1.0f / (m_Spline.Pos.size() - 1) * (i + 1));
			for(int j = 1 ; j < RAILLINE_DRIFT_SEARCH ; j++)
			{
				float oldT = ((nextT - prevT) / RAILLINE_DRIFT_SEARCH * (j - 1)) + prevT;	// i-1のスプライン位置
				float t = ((nextT - prevT) / RAILLINE_DRIFT_SEARCH * j) + prevT;			// iのスプライン位置

				// 1区間の長さ算出
				float length = D3DXVec3Length(&(GetSplinePos(oldT) - GetSplinePos(t)));

				// 長さ加算
				m_Spline.LengthMin[i] += length;
			}
		}
	}
}

//=============================================================================
//	関数名	:GetSplinePos
//	引数	:float t	->	スプライン媒介変数
//	戻り値	:なし
//	説明	:媒介変数の値に応じたスプライン上の点を返す。
//=============================================================================
D3DXVECTOR3 CRailLine::GetSplinePos(float t)
{
	D3DXVECTOR3 pos;

	if(((int)m_Spline.Pos.size() >= 2) && ((t >= 0.0f) && (t <= 1.0f)))
	{
		int		posNum = (int)(t * ((int)m_Spline.Pos.size() - 1));
		float	nowt = t * ((int)m_Spline.Pos.size() - 1) - posNum;

		pos.x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].x)
			+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].x);
		pos.z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].z)
			+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].z);
		

		if(posNum < (int)m_Spline.Pos.size())
		{
			// 次点への高さの差分計算
			float high = m_Spline.Pos[posNum + 1].y - m_Spline.Pos[posNum].y;

			if(high > 0.0f)
			{// 上り坂

				float rad = nowt * D3DX_PI;
				float pow = (-cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

				pos.y = m_Spline.Pos[posNum].y + high * pow;
			}
			else if(high < 0.0f)
			{// 下り坂

				high *= -1.0f;
				float rad = nowt * D3DX_PI;
				float pow = (cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

				pos.y = (m_Spline.Pos[posNum].y - high) + high * pow;
			}
			else
			{
				pos.y = m_Spline.Pos[posNum].y;
			}
		}
		else
		{
			pos.y = m_Spline.Pos[posNum].y;
		}
	}

	return pos;
}

//=============================================================================
//	関数名	:GetDriftStatus
//	引数	:float oldt	->	スプライン媒介変数(旧)
//			:float t	->	スプライン媒介変数(新)
//	戻り値	:なし
//	説明	:ドリフト中か否か、またどの状態かを返す。
//=============================================================================
DRIFT_STATUS CRailLine::GetDriftStatus(float oldt, float t)
{
	DRIFT_STATUS ds = { false, false, 0 };	// ドリフトステータス
	bool ifOldInDrift = false;		// 前回スプライン座標がドリフト地点に入っているか
	bool ifNewInDrift = false;		// 今回スプライン座標がドリフト地点に入っているか

	if((int)m_Spline.Pos.size() >= 2)
	{
		if((t >= 0.0f) && (t <= 1.0f) && (oldt >= 0.0f) && (oldt <= 1.0f))
		{
			for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
			{
				if((oldt >= m_Spline.Drift[i].Begin) && (oldt <= m_Spline.Drift[i].End))
				{// 前回座標がドリフト区間の中にある

					ifOldInDrift = true;
					ds.Curve = m_Spline.Drift[i].Curve;
				}
				if((t >= m_Spline.Drift[i].Begin) && (t <= m_Spline.Drift[i].End))
				{// 今回座標がドリフト区間の中にある

					ifNewInDrift = true;
					ds.Curve = m_Spline.Drift[i].Curve;
				}
			}
		}

		if(!ifOldInDrift && !ifNewInDrift)
		{// ドリフト区間に入っていない

			ds.ifDrift	= false;
			ds.Status	= 0;
		}
		else if(!ifOldInDrift && ifNewInDrift)
		{// ドリフトの始点

			ds.ifDrift = true;
			ds.Status = -1;
		}
		else if(ifOldInDrift && !ifNewInDrift)
		{// ドリフトの終点

			ds.ifDrift = true;
			ds.Status = 1;
		}
		else if(ifOldInDrift && ifNewInDrift)
		{// ドリフト区間の中

			ds.ifDrift = true;
			ds.Status = 0;
		}
	}

	return ds;
}

//=============================================================================
//	関数名	:GetMoveVec
//	引数	:float t	->	スプライン媒介変数
//	戻り値	:なし
//	説明	:tの位置での進行方向を返す。
//=============================================================================
D3DXVECTOR3 CRailLine::GetMoveVec(float t)
{
	D3DXVECTOR3 vec = GetSplinePos(t + 0.01f) - GetSplinePos(t);

	return vec;
}

//=============================================================================
//	関数名	:AngleOf2Vector
//	引数	:D3DXVECTOR3 a	->	ベクトル1
//			:D3DXVECTOR3 b	->	ベクトル2
//	戻り値	:なし
//	説明	:2つのベクトルのなす角を返す。
//=============================================================================
float CRailLine::AngleOf2Vector(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	//内積とベクトル長さを使ってcosθを求める
	float length = D3DXVec3Length(&a) * D3DXVec3Length(&b);

	if(length > 0.0f)
	{
		float rad = D3DXVec3Dot(&a, &b) / length;

		if(rad > 1.0f)
		{
			rad = 1.0f;
		}

		//cosθからθを求める
		float angle = acos(rad);

		// 角度補正
		if(a.y < 0.0f)
		{
			angle *= -1;
		}

		return angle;
	}
	else
	{
		return 0;
	}
}