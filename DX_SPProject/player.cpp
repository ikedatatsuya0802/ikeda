//=============================================================================
//
//	タイトル	プレイヤー
//	ファイル名	player.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/14
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "player.h"
#include "game.h"
#include "meshfield.h"
#include "railLine.h"
#include "cameraDX.h"
#include "driftMark.h"
#include "countdown.h"

//=============================================================================
//	関数名	:CPlayer()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CPlayer::CPlayer(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneXDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CPlayer()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CPlayer::Init(D3DXVECTOR3 pos)
{
	char			*str	= NULL;	// ファイル内容格納配列
	unsigned int	offset	= 0;	// 文字列指定子

	m_NumParts	= 0;
	m_NumMotion	= 0;
	m_Pos		= VEC3_ZERO;
	m_Rot		= VEC3_ZERO;
	m_Move		= VEC3_ZERO;
	m_RotMove	= VEC3_ZERO;
	m_nCntMove	= 0;
	m_bJump		= false;
	m_NumKey	= 0;
	m_Key		= 0;
	m_Frame		= 0;
	m_Pause		= false;

	// モデル・モーションの読み込み
	LoadMotion("./data/motion.txt");
	SetMotion(MT_NONE);
	m_Pause		= false;
	
	m_RailLine	= 0;
	//m_Spline->nNum		= 0;
	m_Per		= PLAYER_START_PER;
	m_PerOld	= PLAYER_START_PER;
	m_PerMove	= 0.0f;
	
	// スプラインの読み込み
	//LoadSpline(m_RailLine);
	m_Spline = CGame::GetRailLine()->GetSpline();

	m_SplineTime = 0.0f;
	m_DriftCurve = 0;
	m_CntDrift = 0;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CPlayer::Uninit(void)
{

}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CPlayer::Update(void)
{
	// 移動処理
	if(m_Pause == false)
	{
		// スプライン位置を保存
		m_PerOld = m_Per;

		if(m_Per <= RAILLINE_GOAL)
		{// ゴールに到着するまで

			// プレイヤー移動
			UpdateMove();

			// 相対回転設定
			UpdateMotion();


			// ドリフトマーク開始判断
			if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).ifDrift)
			{
				if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).Status == -1)
				{// ドリフトの開始

					if(!CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).Curve)
					{// 左カーブ

						if((m_DriftCurve >= 0) && (m_PerMove > PLAYER_CURVESPEED_MAX))
						{// ドリフトしていなかった場合、80km/hを超えていたら減速

							CManager::GetCamera()->SetCameraVibrate(180, 10.0f);
							m_PerMove -= PLAYER_NOT_DRIFT;
							if(m_PerMove < 0.0f) m_PerMove = 0;
						}
					}
					else
					{// 右カーブ

						if((m_DriftCurve <= 0) && (m_PerMove > PLAYER_CURVESPEED_MAX))
						{// ドリフトしていなかった場合、80km/hを超えていたら減速

							CManager::GetCamera()->SetCameraVibrate(180, 10.0f);
							m_PerMove -= PLAYER_NOT_DRIFT;
							if(m_PerMove < 0.0f) m_PerMove = 0;
						}
					}
				}
			}
		}
		else
		{// ゴール到達後

			D3DXVECTOR3 pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);
			D3DXVECTOR3 vecDis = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL + 0.01f);

			// 回転量計算
			m_Spline->Rot.y = atan2f((pos.x - vecDis.x), (pos.z - vecDis.z));

			// 位置反映
			m_Pos.x -= m_RealSpeed * sinf(m_Spline->Rot.y);
			m_Pos.z -= m_RealSpeed * cosf(m_Spline->Rot.y);
		}

		UpdateDrift();
	}
}

//=============================================================================
//	関数名	:UpdateMove
//	引数	:無し
//	戻り値	:無し
//	説明	:オブジェクトを移動・回転させる。
//=============================================================================
void CPlayer::UpdateMove(void)
{
	CCameraDX	*camera		= DX_CAMERA;	// カメラ
	int		posNum = (int)(m_Per * ((int)m_Spline->Pos.size() - 1));
	

	// スプラインのロード
	m_Spline = CGame::GetRailLine()->GetSpline();

	
	if((CGame::GetFrame() > COUNTDOWN_END_TIME) && !DX_CAMERA->GetCameraMode() && KH_W)
	{// 加速

		m_PerMove += PLAYER_MOVEMENT;
	}
	else if(!DX_CAMERA->GetCameraMode() && KH_S)
	{// 減速

		m_PerMove -= PLAYER_MOVEMENT;
		if(m_PerMove < 0.0f)
		{
			m_PerMove = 0.0f;
		}
	}

	// 速度制限
	if(m_PerMove > PLAYER_SPEED_MAX)
	{
		m_PerMove = PLAYER_SPEED_MAX;
	}

	// 移動量反映
	m_Per += m_PerMove;

	// 絶対移動量の計測
	m_RealSpeed = D3DXVec3Length(&(CGame::GetRailLine()->GetSplinePos(m_Per) - m_Pos));

	// 位置反映
	m_Pos.x = CGame::GetRailLine()->GetSplinePos(m_Per).x;
	m_Pos.z = CGame::GetRailLine()->GetSplinePos(m_Per).z;

	
	// 回転反映
	float tDis = m_Per + 0.01f;
	D3DXVECTOR3 vecDis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vecDis.x = CGame::GetRailLine()->GetSplinePos(tDis).x;
	vecDis.z = CGame::GetRailLine()->GetSplinePos(tDis).z;

	// 回転量計算
	m_Spline->Rot.y = atan2f((m_Pos.x - vecDis.x), (m_Pos.z - vecDis.z));
}

//=============================================================================
//	関数名	:UpdateDrift
//	引数	:無し
//	戻り値	:無し
//	説明	:ドリフトの実行管理。
//=============================================================================
void CPlayer::UpdateDrift(void)
{
	// ドリフト
	if(!m_CntDrift && KT_LEFT && !m_bJump && ((m_DriftCurve == 0) || (m_DriftCurve == 2)))
	{
		m_Move.y += PLAYER_JUMP;

		m_bJump = true;

		if(!m_DriftCurve) m_DriftCurve--;
		m_DriftCurve--;
		m_CntDrift = PLAYER_ROT_STEP;
	}
	else if(!m_CntDrift && KT_RIGHT && !m_bJump && ((m_DriftCurve == 0) || (m_DriftCurve == -2)))
	{
		m_Move.y += PLAYER_JUMP;

		m_bJump = true;

		if(!m_DriftCurve) m_DriftCurve++;
		m_DriftCurve++;
		m_CntDrift = PLAYER_ROT_STEP;
	}

	// ドリフトマーク終了判断
	if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).ifDrift)
	{
		if(CGame::GetRailLine()->GetDriftStatus(m_PerOld - DRIFTMARK_FUTURE, m_Per - DRIFTMARK_FUTURE).Status == 1)
		{// ドリフトの終点の場合

			if(m_DriftCurve == 2)
			{
				m_DriftCurve--;
				m_CntDrift = PLAYER_ROT_STEP;
				m_Move.y += PLAYER_JUMP;

				m_bJump = true;
			}
			else if(m_DriftCurve == -2)
			{
				m_DriftCurve++;
				m_CntDrift = PLAYER_ROT_STEP;
				m_Move.y += PLAYER_JUMP;

				m_bJump = true;
			}
		}
	}

	if(m_CntDrift > 0)
	{
		switch(m_DriftCurve)
		{
		case -2:
			m_Rot.y -= PLAYER_ROT_SPEED;
			break;
		case -1:
			m_Rot.y += PLAYER_ROT_SPEED;
			break;
		case 0:
			break;
		case 1:
			m_Rot.y -= PLAYER_ROT_SPEED;
			break;
		case 2:
			m_Rot.y += PLAYER_ROT_SPEED;
			break;
		default:
			break;
		}

		m_CntDrift--;
		if(((m_DriftCurve == -1) || (m_DriftCurve == 1)) && !m_CntDrift)
		{
			m_DriftCurve = 0;
		}
	}

	// ジャンプ量の反映
	m_Pos.y += m_Move.y;

	CMeshfield	*mesh = CGame::GetMeshfield();	// メッシュフィールド
												// プレイヤーの高さを設定
	if(m_Pos.y < (mesh->GetHeight(m_Pos) + 0.0f))
	{
		m_Pos.y = mesh->GetHeight(m_Pos) + 0.0f;
		m_bJump = false;
	}
	else
	{
		// ジャンプ量の減衰
		m_Move.y -= PLAYER_GRAVITY;
	}

	// 回転量補正
	if(m_RotMove.y > D3DX_PI)				// 回転量がプラス方向に逆向きの場合
	{
		// 回転量を逆方向に
		m_RotMove.y -= (D3DX_PI * 2.0f);
	}
	else if(m_RotMove.y < -D3DX_PI)			// 回転量がマイナス方向に逆向きの場合
	{
		// 回転量を逆方向に
		m_RotMove.y += (D3DX_PI * 2.0f);
	}
}

//=============================================================================
//	関数名	:UpdateMotion
//	引数	:無し
//	戻り値	:無し
//	説明	:モーションを更新する。
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		KEY *key, *keyNext;
		float rate;
		D3DXVECTOR3 pos, rot;

		key		= &m_Motion[m_MotionType].KeyInfo[m_Key].Key[i];
		keyNext	= &m_Motion[m_MotionType].KeyInfo[(m_Key + 1) % m_NumKey].Key[i];
		rate = (float)m_Frame / m_Motion[m_MotionType].KeyInfo[m_Key].Frame;
		pos.x = (key->PosX * (1.0f - rate)) + (keyNext->PosX * rate);
		pos.y = (key->PosY * (1.0f - rate)) + (keyNext->PosY * rate);
		pos.z = (key->PosZ * (1.0f - rate)) + (keyNext->PosZ * rate);
		rot.x = (key->RotX * (1.0f - rate)) + (keyNext->RotX * rate);
		rot.y = (key->RotY * (1.0f - rate)) + (keyNext->RotY * rate);
		rot.z = (key->RotZ * (1.0f - rate)) + (keyNext->RotZ * rate);
		m_Model[i]->SetPos(pos);
		m_Model[i]->SetRot(rot);
	}

	m_Frame++;
	if(m_Frame >= m_Motion[m_MotionType].KeyInfo[m_Key].Frame)
	{
		m_Key = (m_Key + 1) % m_NumKey;
		m_Frame = 0;
		
		if(!m_Motion[m_MotionType].Loop && (m_Key == (m_Motion[m_MotionType].NumKey - 1)))
		{
			SetMotion(MT_NEUTRAL);
		}
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CPlayer::Draw(void)
{
	// ワールドマトリックスの設定
	D3DXVECTOR3 rot = D3DXVECTOR3(m_Rot.x, m_Rot.y + m_Spline->Rot.y, m_Rot.z);
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, rot);

	// Zテスト方法更新
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// 描画処理
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		m_Model[i]->Draw();
	}
	
	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	
	// Zテスト方法更新
	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	// デバッグ情報表示
#ifdef _DEBUG/*
	CDebugProc::DebugProc("スプライン座標:(%.4f)\n", m_Per);
	CDebugProc::DebugProc("モデル座標:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	CDebugProc::DebugProc("モデル回転:(%5.2f:%5.2f:%5.2f)\n", m_Rot.x, m_Rot.y, m_Rot.z);*/
	//CDebugProc::DebugProc("Key:%d, Frame:%d\n", m_Key, m_Frame);
#endif
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CPlayer *CPlayer::Create(bool ifListAdd, D3DXVECTOR3 pos)
{
	CPlayer *player;	// インスタンス

	// インスタンス生成
	player = new CPlayer(ifListAdd);

	// 初期化処理
	player->Init(pos);

	// インスタンスをリターン
	return player;
}

//=============================================================================
//	関数名	:LoadMotion
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:モーションを読み込み・セットする。
//=============================================================================
void CPlayer::LoadMotion(char *fileName)
{
	int		nCntData	= 0;	// 配列生成用カウンタ
	char	*str		= NULL;	// ファイル内容格納文字列
	int		offset		= 0;	// オフセット
	

	// バイナリファイル読み込み
	str = CReadFile::ReadBinaryFile(fileName);

	// パーツ数の読み取り
	offset = CReadFile::SearchWordByString(str, offset, "NUM_PARTS ");
	sscanf_s(&str[offset], "NUM_PARTS = %d", &m_NumParts);
	
	// 3Dモデルファイルの読み取り
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		char modelFileName[256] = {NULL};

		// メモリリセット
		memset(modelFileName, NULL, sizeof(modelFileName));

		offset = CReadFile::SearchWordByString(str, offset + 3, "MODEL_FILENAME ");
		sscanf(&str[offset], "MODEL_FILENAME = %s", modelFileName);
		
		// モデルのインスタンスの生成
		m_Model[i] = CModel::Create(modelFileName);
	}

	// モデル初期情報の読み取り・セット
	offset = CReadFile::SearchWordByString(str, offset, "MODEL_INIT");
	for(int i = 0 ; i < m_NumParts ; i++)
	{
		D3DXVECTOR3 posdef, rotdef;

		// 親オブジェクト番号の読み取り
		offset = CReadFile::SearchWordByString(str, offset + 3, "PARENT");
		nCntData = 0;
		sscanf_s(&str[offset], "PARENT = %d", &nCntData);

		// 親オブジェクト番号のセット
		if(nCntData >= 0)
		{
			m_Model[i]->SetParent(m_Model[nCntData]);
		}
		else
		{
			m_Model[i]->SetParent(NULL);
		}

		// 初期相対座標の読み取り
		offset = CReadFile::SearchWordByString(str, offset, "POS");
		sscanf_s(&str[offset], "POS = %f %f %f", &posdef.x, &posdef.y, &posdef.z);

		// 初期相対座標のセット
		m_Model[i]->SetPosDef(posdef);
		
		// 初期相対回転の読み取り
		offset = CReadFile::SearchWordByString(str, offset, "ROT");
		sscanf_s(&str[offset], "ROT = %f %f %f", &rotdef.x, &rotdef.y, &rotdef.z);
		
		// 初期相対回転のセット
		m_Model[i]->SetRotDef(rotdef);
	}

	

	/* モーション情報読み込み */
	// モーション数読み込み
	offset = CReadFile::SearchWordByString(str, offset, "MOTION_NUM");
	nCntData = 0;
	sscanf_s(&str[offset], "MOTION_NUM = %d", &m_NumMotion);
	
	// モーション情報の生成
	m_Motion = new MOTION[m_NumMotion];
	
	for(int i = 0 ; i < m_NumMotion ; i++)
	{
		// ループ読み込み
		offset = CReadFile::SearchWordByString(str, offset, "LOOP");
		nCntData = 0;
		sscanf_s(&str[offset], "LOOP = %d", &nCntData);
		(nCntData == 0) ? m_Motion[i].Loop = false : m_Motion[i].Loop = true;

		// キー数読み込み
		offset = CReadFile::SearchWordByString(str, offset, "NUM_KEY");
		nCntData = 0;
		sscanf_s(&str[offset], "NUM_KEY = %d", &nCntData);
		m_Motion[i].NumKey = nCntData;

		// ポーズ情報生成
		m_Motion[i].KeyInfo = new KEY_INFO[nCntData];
		
		for(int j = 0 ; j < m_Motion[i].NumKey ; j++)
		{
			// フレーム数読み込み
			offset = CReadFile::SearchWordByString(str, offset, "FRAME");
			nCntData = 0;
			sscanf_s(&str[offset], "FRAME = %d", &nCntData);
			m_Motion[i].KeyInfo[j].Frame = nCntData;

			// キー生成
			m_Motion[i].KeyInfo[j].Key = new KEY[m_NumParts];

			// キー情報読み込み
			for(int k = 0 ; k < m_NumParts ; k++)
			{
				// 座標読み込み
				offset = CReadFile::SearchWordByString(str, offset + 1, "POS");
				nCntData = 0;
				sscanf_s(&str[offset], "POS = %f %f %f",
					&m_Motion[i].KeyInfo[j].Key[k].PosX,
					&m_Motion[i].KeyInfo[j].Key[k].PosY,
					&m_Motion[i].KeyInfo[j].Key[k].PosZ);

				// 回転読み込み
				offset = CReadFile::SearchWordByString(str, offset + 1, "ROT");
				nCntData = 0;
				sscanf_s(&str[offset], "ROT = %f %f %f",
					&m_Motion[i].KeyInfo[j].Key[k].RotX,
					&m_Motion[i].KeyInfo[j].Key[k].RotY,
					&m_Motion[i].KeyInfo[j].Key[k].RotZ);
			}
		}
	}
}

//=============================================================================
//	関数名	:SetMotion
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:モーションをセットする。
//=============================================================================
void CPlayer::SetMotion(MOTIONTYPE motionType)
{
	switch(motionType)
	{
	case MT_NEUTRAL:
		{
			m_NumKey = m_Motion[(int)motionType].NumKey;
			break;
		}
	case MT_WALK:
		{
			m_NumKey = m_Motion[(int)motionType].NumKey;
			break;
		}
	default:
		m_NumKey = m_Motion[(int)motionType].NumKey;
		break;
	}
	m_MotionType	= motionType;
	m_Key			= 0;
	m_Frame			= 0;
}