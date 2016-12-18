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
#include "input.h"

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
	//m_Anim.ifAnim = true;
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
	// 通常時カメラ設定
	m_CS.posV = D3DXVECTOR3(150.0f, 200.0f, -1000.0f);
	m_CS.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CS.vecU			= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_CS.Rot			= D3DXVECTOR3(0.0f, atan2f((m_CS.posR.x - m_CS.posV.x), (m_CS.posR.z - m_CS.posV.z)), 0.0f);
	m_CS.Distance		= hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));
	m_CS.Vib.vPos		= VEC3_ZERO;
	m_CS.Vib.Cnt		= 0;
	m_CS.Vib.Width		= 0.0f;

	m_Key	= 0;
	m_Frame = 0;
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
	//CameraAnimation();
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
		m_CS.posV.x += sinf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CS.posV.z += cosf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
	}
	else if(KH_S && !KH_A && !KH_D)
	{// カメラ移動(手前)

	 // 視点移動
		m_CS.posV.x -= sinf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;
		m_CS.posV.z -= cosf(m_CS.Rot.y) * CAMERA_POSV_MOVEMENT;

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
	}
	else if(KH_A)
	{// カメラ移動(左)

		if(CInput::GetKeyPress(DIK_UP))
		{// 左奥

		 // 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else if(KH_S)
		{// 左手前

		 // 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else
		{// 左

		 // 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y - (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
	}
	else if(KH_D)
	{// カメラ移動(右)

		if(KH_W)
		{// 右奥

		 // 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.25f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else if(KH_S)
		{// 右手前

		 // 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.75f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
		else
		{// 右

		 // 視点移動
			m_CS.posV.x += sinf(m_CS.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;
			m_CS.posV.z += cosf(m_CS.Rot.y + (D3DX_PI * 0.5f)) * CAMERA_POSV_MOVEMENT;

			// 注視点設定
			m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
			m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
		}
	}


	// 視点移動
	if(KH_Y && !KH_Z && !KH_C)
	{// 視点移動(上)

	 // 視点設定
		m_CS.posV.y += CAMERA_POSV_MOVEMENT_Y;
	}
	else if(KH_N && !KH_Z && !KH_C)
	{// 視点移動(下)

	 // 視点設定
		m_CS.posV.y -= CAMERA_POSV_MOVEMENT_Y;
	}

	// 注視点移動
	if(KH_T && !KH_Q && !KH_E)
	{// 注視点移動(上)

	 // 注視点設定
		m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_B && !KH_Q && !KH_E)
	{// 注視点移動(下)

	 // 注視点設定
		m_CS.posR.y -= CAMERA_POSR_MOVEMENT_Y;
	}
	else if(KH_Q)
	{// 注視点移動(左)

		if(KH_T)
		{// 左上

		 // 視点変更
			m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y -= CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// 左下

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
		m_CS.Distance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
	}
	else if(KH_E)
	{// 注視点移動(右)

		if(KH_T)
		{// 右上

		 // 視点変更
			m_CS.posR.y += CAMERA_POSR_MOVEMENT_Y * sinf(D3DX_PI * 0.25);

			// 角度増減
			m_CS.Rot.y += CAMERA_POSR_MOVEMENT_X * sinf(D3DX_PI * 0.25);
		}
		else if(KH_B)
		{// 右下

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
		m_CS.Distance = hypotf((m_CS.posR.z - m_CS.posV.z), (m_CS.posR.x - m_CS.posV.x));

		// 注視点設定
		m_CS.posR.x = m_CS.posV.x + (sinf(m_CS.Rot.y) * m_CS.Distance);
		m_CS.posR.z = m_CS.posV.z + (cosf(m_CS.Rot.y) * m_CS.Distance);
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
	if(m_Anim.ifAnim)
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
	D3DXMatrixLookAtLH(&m_CS.mtxView, &m_CS.posV, &(m_CS.posR + m_CS.Vib.vPos), &m_CS.vecU);
	// ビューマトリクスの設定
	D3D_DEVICE->SetTransform(D3DTS_VIEW, &m_CS.mtxView);


	// デバッグ情報表示
#ifdef _DEBUG
	//CDebugProc::DebugProc("カメラ視点　(%5.2f:%5.2f:%5.2f)\n", m_CS.posV.x, m_CS.posV.y, m_CS.posV.z);
	//CDebugProc::DebugProc("カメラ注視点(%5.2f:%5.2f:%5.2f)\n", m_CS.posR.x, m_CS.posR.y, m_CS.posR.z);
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