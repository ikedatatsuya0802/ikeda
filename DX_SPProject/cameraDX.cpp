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
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	関数名	:CCameraDX()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CCameraDX::CCameraDX()
{

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

	// 通常時カメラ設定
	m_CS.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CS.posR = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	m_CS.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot = D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

	// エディット時カメラ設定
	m_CSEdit.posV = D3DXVECTOR3(0.0f, 3000.0f, -10.0f);
	m_CSEdit.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CSEdit.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CSEdit.Rot = D3DXVECTOR3(0.0f, atan2f((m_CSEdit.posR.x - m_CSEdit.posV.x), (m_CSEdit.posR.z - m_CSEdit.posV.z)), 0.0f);
	m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
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
#ifdef _DEBUG
	if(m_flgCameraMode)
	{
		CameraMove();
	}

	if(KT_L)
	{
		ChangeCameraMode();
	}
#endif

	CPlayer	*player	= NULL;			// プレイヤーインスタンス
		
	const type_info& this_id = typeid(*CManager::GetMode());
	//const type_info& client_id = typeid(CGame);
	if(this_id == typeid(CGame))
	{// ゲームモードの場合のみ処理実行

		player	= CGame::GetPlayer1();	// プレイヤー情報の取得
	}
	else
	{
		player	= NULL;
	}
	
	if(player != NULL)
	{
		// 視点設定
		m_CS.posR.x = player->GetPos().x;
		m_CS.posR.z = player->GetPos().z;
		m_CS.posR.y = player->GetPos().y + (CAMERA_POSV_TOHIGHPLAYER / 2);

		// 視点設定
		m_CS.posV.x = player->GetPos().x + (sinf(m_CS.Rot.y + player->GetRot().y) * CAMERA_POSV_TOPLAYER);
		m_CS.posV.z = player->GetPos().z + (cosf(m_CS.Rot.y + player->GetRot().y) * CAMERA_POSV_TOPLAYER);
		m_CS.posV.y = player->GetPos().y + CAMERA_POSV_TOHIGHPLAYER;

		// 注視点設定
		//m_CS.posR.x = player->GetPos().x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
		//m_CS.posR.z = player->GetPos().z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}

	if(CInput::GetMouseNotch() > 0)
	{
		// ↑に回転（チルト）した

		if(CInput::GetMouseNotch() > 1)
		{
			m_CSEdit.posV.y -= CAMERA_WHEEL_CHANGE_Y;
		}
		m_CSEdit.posV.y		-= CAMERA_WHEEL_CHANGE_Y;

		CInput::SetMouseNotch(0);
	}
	else if(CInput::GetMouseNotch() < 0)
	{
		// ↓に回転（チルト）した

		if(CInput::GetMouseNotch() < -1)
		{
			m_CSEdit.posV.y += CAMERA_WHEEL_CHANGE_Y;
		}
		m_CSEdit.posV.y		+= CAMERA_WHEEL_CHANGE_Y;

		CInput::SetMouseNotch(0);
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
	if(CInput::GetKeyPress(DIK_UP) && !CInput::GetKeyPress(DIK_LEFT) && !CInput::GetKeyPress(DIK_RIGHT))		// カメラ移動(奥)
	{
		// 視点移動
		m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(CInput::GetKeyPress(DIK_DOWN) && !CInput::GetKeyPress(DIK_LEFT) && !CInput::GetKeyPress(DIK_RIGHT))		// カメラ移動(手前)
	{
		// 視点移動
		m_CSEdit.posV.x -= sinf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CSEdit.posV.z -= cosf(m_CSEdit.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(CInput::GetKeyPress(DIK_LEFT))		// カメラ移動(左)
	{
		if(CInput::GetKeyPress(DIK_UP))			// 左奥
		{
			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else if(CInput::GetKeyPress(DIK_DOWN))	// 左手前
		{
			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else								// 左
		{
			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
	}
	else if(CInput::GetKeyPress(DIK_RIGHT))		// カメラ移動(右)
	{
		if(CInput::GetKeyPress(DIK_UP))			// 右奥
		{
			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else if(CInput::GetKeyPress(DIK_DOWN))	// 右手前
		{
			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
		else								// 右
		{
			// 視点移動
			m_CSEdit.posV.x += sinf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CSEdit.posV.z += cosf(m_CSEdit.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
			m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		}
	}

	
	// 視点移動
	if(KH_Y && !KH_Z && !KH_C)			// 視点移動(上)
	{
		// 視点設定
		m_CSEdit.posV.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)	// 視点移動(下)
	{
		// 視点設定
		m_CSEdit.posV.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Z)		// 視点移動(左)
	{
		if(KH_Y)			// 左上
		{
			// 視点変更
			m_CSEdit.posV.y += CAMERA_POSR_MOVEMENT_Y;
			
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		else if(KH_N)	// 左下
		{
			// 視点変更
			m_CSEdit.posV.y -= CAMERA_POSR_MOVEMENT_Y;
			
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		else
		{
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		
		// 視点注視点間距離更新
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// 視点設定
		m_CSEdit.posV.x = m_CSEdit.posR.x - (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posV.z = m_CSEdit.posR.z - (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(KH_C)		// 視点移動(右)
	{
		if(KH_Y)			// 右上
		{
			// 視点変更
			m_CSEdit.posV.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_N)	// 右下
		{
			// 視点変更
			m_CSEdit.posV.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}
			
		// 視点注視点間距離更新
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// 視点設定
		m_CSEdit.posV.x = m_CSEdit.posR.x - (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posV.z = m_CSEdit.posR.z - (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}

	// 注視点移動
	if(KH_T && !KH_Q && !KH_E)			// 注視点移動(上)
	{
		// 注視点設定
		m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)	// 注視点移動(下)
	{
		// 注視点設定
		m_CSEdit.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)		// 注視点移動(左)
	{
		if(KH_T)			// 左上
		{
			// 視点変更
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)	// 左下
		{
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
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
		
		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
	}
	else if(KH_E)		// 注視点移動(右)
	{
		if(KH_T)			// 右上
		{
			// 視点変更
			m_CSEdit.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);
			
			// 角度増減
			m_CSEdit.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)	// 右下
		{
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
		m_CSEdit.fDistance = hypotf((m_CSEdit.posR.z - m_CSEdit.posV.z), (m_CSEdit.posR.x - m_CSEdit.posV.x));
			
		// 注視点設定
		m_CSEdit.posR.x = m_CSEdit.posV.x + (sinf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
		m_CSEdit.posR.z = m_CSEdit.posV.z + (cosf(m_CSEdit.Rot.y) * m_CSEdit.fDistance);
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
	D3DXMatrixLookAtLH(&camera->mtxView, &camera->posV, &camera->posR, &camera->vecU);
	// ビューマトリクスの設定
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &camera->mtxView);


	// デバッグ情報表示
#ifdef _DEBUG
	CDebugProc::DebugProc("カメラ視点　(%5.2f:%5.2f:%5.2f)\n", camera->posV.x, camera->posV.y, camera->posV.z);
	CDebugProc::DebugProc("カメラ注視点(%5.2f:%5.2f:%5.2f)\n", camera->posR.x, camera->posR.y, camera->posR.z);
#endif
}