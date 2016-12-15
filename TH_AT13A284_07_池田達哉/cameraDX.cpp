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
	m_CS.posV = D3DXVECTOR3(0.0f, 150.0f, -300.0f);
	m_CS.posR = D3DXVECTOR3(0.0f, 70.0f, 0.0f);
	m_CS.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot = D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));
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
	CameraMove();
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
		m_CS.posV.x += sinf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CS.posV.z += cosf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}
	else if(CInput::GetKeyPress(DIK_DOWN) && !CInput::GetKeyPress(DIK_LEFT) && !CInput::GetKeyPress(DIK_RIGHT))		// カメラ移動(手前)
	{
		// 視点移動
		m_CS.posV.x -= sinf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CS.posV.z -= cosf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}
	else if(CInput::GetKeyPress(DIK_LEFT))		// カメラ移動(左)
	{
		if(CInput::GetKeyPress(DIK_UP))			// 左奥
		{
			// 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
		}
		else if(CInput::GetKeyPress(DIK_DOWN))	// 左手前
		{
			// 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
		}
		else								// 左
		{
			// 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
		}
	}
	else if(CInput::GetKeyPress(DIK_RIGHT))		// カメラ移動(右)
	{
		if(CInput::GetKeyPress(DIK_UP))			// 右奥
		{
			// 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
		}
		else if(CInput::GetKeyPress(DIK_DOWN))	// 右手前
		{
			// 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
		}
		else								// 右
		{
			// 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
		}
	}


	// 視点移動
	if(KH_Y && !KH_Z && !KH_C)			// 視点移動(上)
	{
		// 視点設定
		m_CS.posV.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)	// 視点移動(下)
	{
		// 視点設定
		m_CS.posV.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Z)		// 視点移動(左)
	{
		if(KH_Y)			// 左上
		{
			// 視点変更
			m_CS.posV.y += CAMERA_POSR_MOVEMENT_Y;

			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		else if(KH_N)	// 左下
		{
			// 視点変更
			m_CS.posV.y -= CAMERA_POSR_MOVEMENT_Y;

			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}
		else
		{
			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}

		// 視点注視点間距離更新
		m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// 視点設定
		m_CS.posV.x = m_CS.posR.x - (sinf(m_CS.Rot.y) * m_CS.fDistance);
		m_CS.posV.z = m_CS.posR.z - (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}
	else if(KH_C)		// 視点移動(右)
	{
		if(KH_Y)			// 右上
		{
			// 視点変更
			m_CS.posV.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_N)	// 右下
		{
			// 視点変更
			m_CS.posV.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}

		// 視点注視点間距離更新
		m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// 視点設定
		m_CS.posV.x = m_CS.posR.x - (sinf(m_CS.Rot.y) * m_CS.fDistance);
		m_CS.posV.z = m_CS.posR.z - (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}

	// 注視点移動
	if(KH_T && !KH_Q && !KH_E)			// 注視点移動(上)
	{
		// 注視点設定
		m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)	// 注視点移動(下)
	{
		// 注視点設定
		m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)		// 注視点移動(左)
	{
		if(KH_T)			// 左上
		{
			// 視点変更
			m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)	// 左下
		{
			// 視点変更
			m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X;
		}

		// 視点注視点間距離更新
		m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
	}
	else if(KH_E)		// 注視点移動(右)
	{
		if(KH_T)			// 右上
		{
			// 視点変更
			m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)	// 右下
		{
			// 視点変更
			m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else
		{
			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X;
		}

		// 視点注視点間距離更新
		m_CS.fDistance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.fDistance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.fDistance);
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
	// プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&m_CS.mtxProjection);
	
	// プロジェクションマトリクスの作成
	D3DXMatrixPerspectiveFovLH(&m_CS.mtxProjection,						// プロジェクションマトリクス
								(D3DX_PI * 0.25),								// 視野角
								((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// アスペクト比
								CAMERA_NEARZ,									// NearZ値
								CAMERA_FARZ);									// FarZ値

	// プロジェクションマトリクスの設定
	D3D_DEVICE->SetTransform(D3DTS_PROJECTION, &m_CS.mtxProjection);
	// ビューマトリクスの初期化
	D3DXMatrixIdentity(&m_CS.mtxView);
	// ビューマトリクスの作成
	D3DXMatrixLookAtLH(&m_CS.mtxView, &m_CS.posV, &m_CS.posR, &m_CS.vecU);
	// ビューマトリクスの設定
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &m_CS.mtxView);


	// デバッグ情報表示
#ifdef _DEBUG
	CDebugProc::DebugProc("カメラ視点　(%5.2f:%5.2f:%5.2f)\n", m_CS.posV.x, m_CS.posV.y, m_CS.posV.z);
	CDebugProc::DebugProc("カメラ注視点(%5.2f:%5.2f:%5.2f)\n", m_CS.posR.x, m_CS.posR.y, m_CS.posR.z);
#endif
}