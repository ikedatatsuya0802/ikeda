//=============================================================================
//
//	タイトル	カメラファイル
//	ファイル名	camera.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/26
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "cameraDX.h"
#include <typeinfo>
#include "input.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "mode.h"

//=============================================================================
//	関数名	:CCameraDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CCameraDX::CCameraDX()
{
	// アニメーション情報初期化
	LoadCameraAnim();
	m_Anim.ifAnim = true;
	//m_Anim2.ifAnim = true;
}

//=============================================================================
//	関数名	:~CCameraDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CCameraDX::~CCameraDX()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CCameraDX::Init(void)
{
	// カメラモード設定
	m_flgCameraMode = false;

	if(CManager::MatchMode(CTitle()))
	{
		// 通常時カメラ設定
		m_CS.posV = D3DXVECTOR3(0.0f, 50.0f, -100.0f);
		m_CS.posR = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	}
	else if(CManager::MatchMode(CGame()))
	{
		// 通常時カメラ設定
		m_CS.posV = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		m_CS.posR = D3DXVECTOR3(0.0f, 30.0f, 0.0f);
	}
	m_CS.vecU			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot			= D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.Distance		= CAMERA_POSV_TOPLAYER;
	m_CS.Vib.vPos		= VEC3_ZERO;
	m_CS.Vib.Cnt		= 0;
	m_CS.Vib.Width	= 0.0f;

	// エディット時カメラ設定
	m_CSEdit.posV		= CAMERA_EDIT_V1;
	m_CSEdit.posR		= CAMERA_EDIT_R1;
	m_CSEdit.vecU		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CSEdit.Rot		= D3DXVECTOR3(0.0f, atan2f((m_CSEdit.posR.x - m_CSEdit.posV.x), (m_CSEdit.posR.z - m_CSEdit.posV.z)), 0.0f);
	m_CSEdit.Distance	= hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
	m_CSEdit.Vib.vPos	= VEC3_ZERO;
	m_CSEdit.Vib.Cnt	= 0;
	m_CSEdit.Vib.Width	= 0.0f;

	m_Key		= 0;
	m_Frame		= 0;
	m_DisVec	= 0;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CCameraDX::Uninit(void)
{

}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CCameraDX::Update(void)
{
	if(CManager::MatchMode(CTitle()))
	{// タイトルの場合のみ処理実行

		CameraAnimation();
	}		
	else if(CManager::MatchMode(CGame()))
	{// ゲームモードの場合のみ処理実行

		CPlayer	*player = NULL;	// プレイヤーインスタンス

		if(m_flgCameraMode)
		{
			CameraMove();

			// 視点をいい感じに
			if(CInput::GetKeyTrigger(DIK_2))
			{
				m_CSEdit.posV = CAMERA_EDIT_V1;
				m_CSEdit.posR = CAMERA_EDIT_R1;
				m_CSEdit.Rot = VEC3_ZERO;
				m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			}
			else if(CInput::GetKeyTrigger(DIK_3))
			{
				m_CSEdit.posV = CAMERA_EDIT_V2;
				m_CSEdit.posR = CAMERA_EDIT_R2;
				m_CSEdit.Rot = VEC3_ZERO;
				m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			}
			else if(CInput::GetKeyTrigger(DIK_4))
			{
				D3DXVECTOR3 pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);

				m_CSEdit.posV = D3DXVECTOR3(pos.x, pos.y + 3000.0f, pos.z - 10.0f);
				m_CSEdit.posR = D3DXVECTOR3(pos.x, pos.y, pos.z);
				m_CSEdit.Rot = VEC3_ZERO;
				m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			}


			if(CInput::GetMouseNotch() > 0)
			{
				// ↑に回転（チルト）した

				if(CInput::GetMouseNotch() > 1)
				{
					m_CSEdit.posV.y -= CAMERA_WHEEL_CHANGE_Y;
				}
				m_CSEdit.posV.y -= CAMERA_WHEEL_CHANGE_Y;

				CInput::SetMouseNotch(0);
			}
			else if(CInput::GetMouseNotch() < 0)
			{
				// ↓に回転（チルト）した

				if(CInput::GetMouseNotch() < -1)
				{
					m_CSEdit.posV.y += CAMERA_WHEEL_CHANGE_Y;
				}
				m_CSEdit.posV.y += CAMERA_WHEEL_CHANGE_Y;

				CInput::SetMouseNotch(0);
			}
		}
		else
		{
#ifdef _DEBUG
			if(KH_Q)
			{// 注視点移動(左)

			 // 角度増減
				m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * 0.25f;
			}
			else if(KH_E)
			{// 注視点移動(右)

			 // 角度増減
				m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * 0.25f;
			}
#endif

			player = CGame::GetPlayer1();	// プレイヤー情報の取得

			CameraVibrate();

			if(player != NULL)
			{
				// 視点設定
				/*m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;*/

				// 注視点設定
				m_CS.posR.x = player->GetPos().x;
				m_CS.posR.z = player->GetPos().z;
				m_CS.posR.y = player->GetPos().y + (CAMERA_POSV_TOHIGHPLAYER / 2);

				// カメラ距離設定
				m_CS.Distance += m_DisVec;

				// カメラ距離変化量減衰
				m_DisVec *= 0.8f;

				// 視点設定
				m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->GetSplineRot().y) * m_CS.Distance);
				m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;
			}
		}



		if(KT_L)
		{
			ChangeCameraMode();
		}
	}
}

//=============================================================================
//	関数名	:CameraMove
//	引数	:無し
//	戻り値	:無し
//	説明	:カメラの操作を独立。
//=============================================================================
void CCameraDX::CameraMove(void)
{
	if(KH_W && !KH_A && !KH_D)
	{// カメラ移動(奥)

		// 視点移動
		m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
	else if(KH_S && !KH_A && !KH_D)
	{// カメラ移動(手前)

		// 視点移動
		m_CSEdit.posV.x -= sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z -= cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
	else if(KH_A)
	{// カメラ移動(左)

		if(CInput::GetKeyPress(DIK_UP))
		{// 左奥

			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else if(KH_S)
		{// 左手前

			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else
		{// 左

			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
	}
	else if(KH_D)
	{// カメラ移動(右)

		if(KH_W)
		{// 右奥

			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else if(KH_S)
		{// 右手前

			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
		else
		{// 右

			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		}
	}

	
	// 視点移動
	if(KH_Y && !KH_Z && !KH_C)
	{// 視点移動(上)

		// 視点設定
		m_CSEdit.posV.y += CAMERA_POSV_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)
	{// 視点移動(下)

		// 視点設定
		m_CSEdit.posV.y -= CAMERA_POSV_MOVEMENT_Y;
	}

	// 注視点移動
	if(KH_T && !KH_Q && !KH_E)
	{// 注視点移動(上)

		// 注視点設定
		m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)
	{// 注視点移動(下)

		// 注視点設定
		m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)
	{// 注視点移動(左)

		if(KH_T)
		{// 左上

			// 視点変更
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// 左下

			// 視点変更
			m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}
			
		// 視点注視点間距離更新
		m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
	else if(KH_E)
	{// 注視点移動(右)

		if(KH_T)
		{// 右上

			// 視点変更
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// 右下

			// 視点変更
			m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
			
		// 視点注視点間距離更新
		m_CSEdit.Distance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			
		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.Distance);
	}
}

//=============================================================================
//	関数名	:CameraAnimation
//	引数	:無し
//	戻り値	:無し
//	説明	:カメラをアニメーションさせる。
//=============================================================================
void CCameraDX::CameraAnimation(void)
{
	if(1)
	{// posVとposR指定のアニメーション

		D3DXVECTOR3 posV;
		D3DXVECTOR3 posR;
		D3DXVECTOR3 nextVec;
		int keyNext = 0;
		float rate = 0.0f;

		if(m_Key != ((int)m_Anim.Status.size() - 1))
		{
			keyNext = m_Key + 1;
		}
		else
		{
			keyNext = 0;
		}

		rate = (m_Frame / (float)m_Anim.Status[keyNext].Frame);


		// 現在フレームでの視点の計算
		nextVec = m_Anim.Status[keyNext].PosV - m_Anim.Status[m_Key].PosV;
		posV.x = m_Anim.Status[m_Key].PosV.x + (nextVec.x * rate);
		posV.y = m_Anim.Status[m_Key].PosV.y + (nextVec.y * rate);
		posV.z = m_Anim.Status[m_Key].PosV.z + (nextVec.z * rate);

		// 現在フレームでの注視点の計算
		nextVec = m_Anim.Status[keyNext].PosR - m_Anim.Status[m_Key].PosR;
		posR.x = m_Anim.Status[m_Key].PosR.x + (nextVec.x * rate);
		posR.y = m_Anim.Status[m_Key].PosR.y + (nextVec.y * rate);
		posR.z = m_Anim.Status[m_Key].PosR.z + (nextVec.z * rate);

		// 座標反映
		m_CS.posV = posV;
		m_CS.posR = posR;

		// フレームの増加
		m_Frame++;
		if(m_Frame >= m_Anim.Status[keyNext].Frame)
		{
			m_Key = (m_Key + 1) % m_Anim.Status.size();
			m_Frame = 0;

			if(!m_Anim.Loop && (m_Key == (m_Anim.Status.size() - 1)))
			{
				m_Anim.ifAnim = false;
			}
		}
	}
	else
	{// posVと角度指定のアニメーション

		D3DXVECTOR3 posV;
		D3DXVECTOR3 posR;
		D3DXVECTOR3 nextVec;
		int keyNext = 0;
		float rate = 0.0f;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 vec;

		if(m_Key != ((int)m_Anim2.Status.size() - 1))
		{
			keyNext = m_Key + 1;
		}
		else
		{
			keyNext = 0;
		}

		rate = (m_Frame / (float)m_Anim2.Status[keyNext].Frame);


		// 現在フレームでの視点の計算
		nextVec = m_Anim2.Status[keyNext].PosV - m_Anim2.Status[m_Key].PosV;
		posV.x = m_Anim2.Status[m_Key].PosV.x + (nextVec.x * rate);
		posV.y = m_Anim2.Status[m_Key].PosV.y + (nextVec.y * rate);
		posV.z = m_Anim2.Status[m_Key].PosV.z + (nextVec.z * rate);

		// 現在フレームでの角度計算
		rot = m_Anim2.Status[keyNext].Rot - m_Anim2.Status[m_Key].Rot;
		rot.x = m_Anim2.Status[m_Key].Rot.x + (rot.x * rate);
		rot.y = m_Anim2.Status[m_Key].Rot.y + (rot.y * rate);
		rot.z = m_Anim2.Status[m_Key].Rot.z + (rot.z * rate);
		CDebugProc::DebugProc("rot:%f %f %f\n", rot.x, rot.y, rot.z);
		CDebugProc::DebugProc("KEY:%d, FRAME:%d\n", m_Key, m_Frame);
		

		// 角度を元に注視ベクトル計算
		vec.x = CAMERA_VIEW_FAR * sinf(rot.y);
		vec.y = CAMERA_VIEW_FAR * sinf(rot.x);
		vec.z = CAMERA_VIEW_FAR * cosf(rot.y) + CAMERA_VIEW_FAR * cosf(rot.x);

		// 注視ベクトルを正規化
		D3DXVec3Normalize(&vec, &vec);
		vec * CAMERA_VIEW_FAR;

		posR.x = posV.x + vec.x;
		posR.y = posV.y + vec.y;
		posR.z = posV.z + vec.z;


		// 座標反映
		m_CS.posV = posV;
		m_CS.posR = posR;

		// フレームの増加
		m_Frame++;
		if(m_Frame >= m_Anim2.Status[keyNext].Frame)
		{
			m_Key = (m_Key + 1) % m_Anim2.Status.size();
			m_Frame = 0;

			if(!m_Anim2.Loop && (m_Key == (m_Anim2.Status.size() - 1)))
			{
				m_Anim2.ifAnim = false;
			}
		}
	}
}

//=============================================================================
//	関数名	:LoadCameraAnim
//	引数	:無し
//	戻り値	:無し
//	説明	:カメラのアニメーション情報をロードする。
//=============================================================================
void CCameraDX::LoadCameraAnim(void)
{
	FILE	*fp = NULL;	// ファイルポインタ

	fp = fopen("./data/camera_anim.txt", "r");

	m_Anim.ifAnim	= false;
	m_Anim.Loop		= false;

	// END_SCRIPTまで読み込み
	while(!feof(fp))
	{
		char str[65535] = { NULL };
		memset(str, NULL, sizeof(str));

		// 単語を取得
		fscanf(fp, "%s", str);

		if(strcmp(str, "LOOP") == 0)
		{// ループ情報を読み込み

			int a = 0;
			fscanf(fp, " = %d\n", &a);
			m_Anim.Loop = (bool)a;
		}
		else if(strcmp(str, "FRAME") == 0)
		{// スプライン制御点を追加

			// アニメーションデータを追加
			m_Anim.Status.push_back(CAMERA_ANIM_STATUS());

			uint aNum = m_Anim.Status.size() - 1;	// 終端の配列番号

			// フレーム数を読み込み
			fscanf(fp, " = %d\n", &m_Anim.Status[aNum].Frame);
			// 視点情報を読み込み
			fscanf(fp, "POSV = %f %f %f\n", &m_Anim.Status[aNum].PosV.x, &m_Anim.Status[aNum].PosV.y, &m_Anim.Status[aNum].PosV.z);
			// 注視点情報を読み込み
			fscanf(fp, "POSR = %f %f %f\n", &m_Anim.Status[aNum].PosR.x, &m_Anim.Status[aNum].PosR.y, &m_Anim.Status[aNum].PosR.z);
		}
		else if(strcmp(str, "END_SCRIPT") == 0)
		{// スプライン読み込みを終了

			break;
		}
	}

	// ファイルクローズ
	fclose(fp);
}

//=============================================================================
//	関数名	:CameraVibrate
//	引数	:無し
//	戻り値	:無し
//	説明	:カメラの振動を実行。
//=============================================================================
void CCameraDX::CameraVibrate(void)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<float> width(-(m_CS.Vib.Width / 2), (m_CS.Vib.Width / 2));

	// エディットモードでない場合のみ実行
	if(!m_flgCameraMode)
	{
		// カウンタが設定されている場合実行
		if(m_CS.Vib.Cnt != 0)
		{
			// ランダムに揺れを決定
			m_CS.Vib.vPos.x = width(mt) * (m_CS.Vib.Cnt / (float)m_CS.Vib.MaxCnt);
			m_CS.Vib.vPos.y = width(mt) * (m_CS.Vib.Cnt / (float)m_CS.Vib.MaxCnt);

			if(m_CS.Vib.Cnt)
			{
				// カウンタ減少
				m_CS.Vib.Cnt--;
			}
		}
		else
		{
			m_CS.Vib.vPos	= VEC3_ZERO;
			m_CS.Vib.MaxCnt = 0;
			m_CS.Vib.Cnt	= 0;
			m_CS.Vib.Width	= 0.0f;
		}
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CCameraDX::SetCamera(void)
{
	CAMERA* camera = m_flgCameraMode ? &m_CSEdit : &m_CS;

	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxProjection);
	
	// プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&camera->mtxProjection,						// プロジェクションマトリクス
								(D3DX_PI * 0.25),								// 視野角
								((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// アスペクト比
								CAMERA_NEARZ,									// NearZ値
								CAMERA_FARZ);									// FarZ値

	// プロジェクションマトリクスの設定
	D3D_DEVICE->SetTransform(D3DTS_PROJECTION, &camera->mtxProjection);
	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&camera->mtxView);
	// ビューマトリクスの作成
	if(m_flgCameraMode)
	{
		D3DXMatrixLookAtLH(&camera->mtxView, &camera->posV, &camera->posR, &camera->vecU);
	}
	else
	{
		D3DXMatrixLookAtLH(&camera->mtxView, &camera->posV, &(camera->posR + camera->Vib.vPos), &camera->vecU);
	}
	// ビューマトリクスの設定
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &camera->mtxView);


	// デバッグ情報表示
#ifdef _DEBUG
	//CDebugProc::DebugProc("カメラ視点　(%5.2f:%5.2f:%5.2f)\n", camera->posV.x, camera->posV.y, camera->posV.z);
	//CDebugProc::DebugProc("カメラ注視点(%5.2f:%5.2f:%5.2f)\n", camera->posR.x, camera->posR.y, camera->posR.z);
#endif
}

//=============================================================================
//	関数名	:GetCameraAnimFrame
//	引数	:無し
//	戻り値	:無し
//	説明	:アニメーションの総フレーム数を返す。
//=============================================================================
int CCameraDX::GetCameraAnimFrame(void)
{
	int frame = 0;

	for(int i = 0 ; i < (int)m_Anim.Status.size() ; i++)
	{
		frame += m_Anim.Status[i].Frame;
	}

	return frame;
}