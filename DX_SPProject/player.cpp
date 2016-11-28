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
#include <stdio.h>
#include "main.h"
#include "player.h"
#include "model.h"
#include "game.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "scene3DDX.h"
#include "meshfield.h"
#include "railLine.h"

//=============================================================================
//	関数名	:CPlayer()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CPlayer::CPlayer(int priority) : CSceneDX(priority)
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
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_Move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	
	m_RailLine			= 0;
	//m_Spline->nNum		= 0;
	m_Per		= 0.0f;
	m_PerMove	= 0.0f;
	
	// スプラインの読み込み
	//LoadSpline(m_RailLine);
	m_Spline = CGame::GetRailLine()->GetSpline();
	CalcSpline(0);
	//SetPos(D3DXVECTOR3(m_Spline->PosHermite[0].x, m_Spline->PosHermite[0].y, m_Spline->PosHermite[0].z));
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CPlayer::Uninit(void)
{
	for(int i = 0 ; i < m_NumMotion ; i++)
	{
		for(int j = 0 ; j < m_Motion[i].NumKey ; j++)
		{
			if(m_Motion[i].KeyInfo[j].Key != NULL)
			{
				m_Motion[i].KeyInfo[j].Key = NULL;
			}
		}
		if(m_Motion[i].KeyInfo != NULL)
		{
			m_Motion[i].KeyInfo = NULL;
		}
	}
	if(m_Motion != NULL)
	{
		m_Motion = NULL;
	}
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CPlayer::Update(void)
{
	if(KT_P)
	{
		m_Pause = (m_Pause ? false : true);
	}

	if(m_Pause == false)
	{
		// プレイヤー移動
		UpdateMove();
	
		// 相対回転設定
		UpdateMotion();
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
	CCameraDX	*camera		= CManager::GetCamera();	// カメラ
	CMeshfield	*mesh		= CGame::GetMeshfield();		// メッシュフィールド

	float nowt = (m_Per - ((int)m_Per));
	
	if(CInput::GetKeyboardPress(DIK_W))				// 奥
	{
		// 移動量を設定
		m_PerMove += PLAYER_MOVEMENT;
	}

	// 移動量反映
	if((m_Per <= (m_Spline->nNum - 1)) && (m_Per >= 0.0f))
	{
		m_Per += m_PerMove;
		if(m_Per > (m_Spline->nNum - 1))
		{
			m_Per -= (m_Spline->nNum - 1);
		}
	}
	else if(m_Per < 0.0f)
	{
		m_Per = 0.0f;
	}

	// 位置反映
	m_Pos.x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)m_Per + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)m_Per + 1].x)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)m_Per + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)m_Per + 1].x);
	m_Pos.z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)m_Per + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)m_Per + 1].z)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)m_Per + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)m_Per + 1].z);	
	//m_Pos.y = 1.0f;

	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if((int)(nowt * RAIL_SET) > 0)
	{
		vec = D3DXVECTOR3((m_Spline->PosHermite[(int)(nowt * RAIL_SET) + 1].x - m_Spline->PosHermite[(int)(nowt * RAIL_SET) - 1].x),
			0.0f,
			(m_Spline->PosHermite[(int)(nowt * RAIL_SET) + 1].z - m_Spline->PosHermite[(int)(nowt * RAIL_SET) - 1].z));
	}
	D3DXVec3Normalize(&vec, &vec);
	vec *= RAIL_MARGIN;

	if(m_RailLine > 0)
	{
		m_Pos.x -= cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) - sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
		m_Pos.z -= sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) + cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
	}
	else if(m_RailLine < 0)
	{
		m_Pos.x += cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) - sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
		m_Pos.z += sinf(D3DX_PI * 0.5f) * (m_RailLine * vec.x) + cosf(D3DX_PI * 0.5f) * (m_RailLine * vec.z);
	}
	
	// 回転反映
	float tDis = nowt + 0.01f;
	D3DXVECTOR3 vecDis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vecDis.x = (pow((tDis - 1), 2) * (2 * tDis + 1) * m_Spline->Pos[(int)m_Per + 0].x) + (powf(tDis, 2) * (3 - 2 * tDis) * m_Spline->Pos[(int)m_Per + 1].x)
		+ (pow((1 - tDis), 2) * tDis * m_Spline->Vec[(int)m_Per + 0].x) + ((tDis - 1) * powf(tDis, 2) * m_Spline->Vec[(int)m_Per + 1].x);
	vecDis.z = (pow((tDis - 1), 2) * (2 * tDis + 1) * m_Spline->Pos[(int)m_Per + 0].z) + (powf(tDis, 2) * (3 - 2 * tDis) * m_Spline->Pos[(int)m_Per + 1].z)
		+ (pow((1 - tDis), 2) * tDis * m_Spline->Vec[(int)m_Per + 0].z) + ((tDis - 1) * powf(tDis, 2) * m_Spline->Vec[(int)m_Per + 1].z);

	m_Rot.y = atan2f((m_Pos.x - vecDis.x), (m_Pos.z - vecDis.z));

	m_PerMove += (-m_PerMove * PLAYER_SPEED_DOWN);

	// ジャンプ
	if(KT_SPACE && !m_bJump)
	{
		m_Move.y += PLAYER_JUMP;

		m_bJump = true;
	}

	// ジャンプ量の反映
	m_Pos.y += m_Move.y;

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

	// カウンタが有効の場合回転する
	if(m_nCntMove > 0)
	{
		m_Rot.y += (m_RotMove.y / PLAYER_ROT_STEP);

		m_nCntMove--;
	}

	// 回転角の境界補正
	if(m_Rot.y > D3DX_PI)
	{
		m_Rot.y = (-D3DX_PI + (m_Rot.y - D3DX_PI));
	}
	else if(m_Rot.y < -D3DX_PI)
	{
		m_Rot.y = (D3DX_PI + (m_Rot.y + D3DX_PI));
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
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// マトリックス

	
	// マトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケール設定
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	//D3DXMatrixRotationQuaternion(&mtxRot, &m_Quaternion);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
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
	
	// デバッグ情報表示
#ifdef _DEBUG
	CDebugProc::DebugProc("スプライン座標:(%.4f)\n", m_Per);
	CDebugProc::DebugProc("モデル座標:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	//CDebugProc::DebugProc("Key:%d, Frame:%d\n", m_Key, m_Frame);
#endif
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *player;	// インスタンス

	// インスタンス生成
	player = new CPlayer;

	// 初期化処理
	player->Init(pos);

	// インスタンスを返す
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
//	関数名	:LoadMotion
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:モーションを読み込み・セットする。
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

//=============================================================================
//	関数名	:LoadSpline
//	引数	:なし
//	戻り値	:なし
//	説明	:なし
//=============================================================================
void CPlayer::LoadSpline(int line)
{/*
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
			fscanf(fp, " = %d", &m_Spline->nNum);

			if(m_Spline->nNum > 0)
			{
				m_Spline->PosHermite = new D3DXVECTOR3[(m_Spline->nNum - 1) * RAIL_SET];
				m_Spline->Pos = new D3DXVECTOR3[m_Spline->nNum];
				m_Spline->Vec = new D3DXVECTOR3[m_Spline->nNum];
			}
		}
		else if(strcmp(str, "POS") == 0)
		{
			// エルミート座標読み取り
			for(int i = 0 ; i < m_Spline->nNum ; i++)
			{
				if(i > 0)
				{
					if(fscanf(fp, "POS = %f %f %f\n", &m_Spline->Pos[i].x, &m_Spline->Pos[i].y, &m_Spline->Pos[i].z) == EOF)
					{
						m_Spline->Pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					if(fscanf(fp, " = %f %f %f\n", &m_Spline->Pos[i].x, &m_Spline->Pos[i].y, &m_Spline->Pos[i].z) == EOF)
					{
						m_Spline->Pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
		else if(strcmp(str, "VEC") == 0)
		{
			// エルミートベクトル読み取り
			for(int i = 0 ; i < m_Spline->nNum ; i++)
			{
				if(i > 0)
				{
					if(fscanf(fp, "VEC = %f %f %f\n", &m_Spline->Vec[i].x, &m_Spline->Vec[i].y, &m_Spline->Vec[i].z) == EOF)
					{
						m_Spline->Vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					if(fscanf(fp, " = %f %f %f\n", &m_Spline->Vec[i].x, &m_Spline->Vec[i].y, &m_Spline->Vec[i].z) == EOF)
					{
						m_Spline->Vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
			}

			// 読み込み終了
			break;
		}
	}

	fclose(fp);*/
}

//=============================================================================
//	関数名	:CalcSpline
//	引数	:なし
//	戻り値	:なし
//	説明	:なし
//=============================================================================
void CPlayer::CalcSpline(int line)
{
	float t		= 0.0f;
	float rot	= 0.0f;
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if(m_Spline->nNum >= 2)
	{
		for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++, t += (1.0f / RAIL_SET))
		{
			float nowt = (t - ((int)t));

			m_Spline->PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].x)
								+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].x);
			m_Spline->PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].z)
								+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].z);	
			m_Spline->PosHermite[i].y = 1.0f;
		}
	}

	
	for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++, t += (1.0f / RAIL_SET))
	{
		float nowt = (t - ((int)t));

		if((int)t == 0)
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t + 1].x - m_Spline->PosHermite[(int)t].x), 0.0f, (m_Spline->PosHermite[(int)t + 1].z - m_Spline->PosHermite[(int)t].z));
		}
		else if(i == (RAIL_SET - 1))
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t].x - m_Spline->PosHermite[(int)t - 1].x), 0.0f, (m_Spline->PosHermite[(int)t].z - m_Spline->PosHermite[(int)t - 1].z));
		}
		else
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t + 1].x - m_Spline->PosHermite[(int)t - 1].x), 0.0f, (m_Spline->PosHermite[(int)t + 1].z - m_Spline->PosHermite[(int)t - 1].z));
		}
		D3DXVec3Normalize(&vec, &vec);
		vec *= RAIL_MARGIN;

		if(line > 0)
		{
			m_Spline->PosHermite[i].x -= cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline->PosHermite[i].z -= sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}
		else if(line < 0)
		{
			m_Spline->PosHermite[i].x += cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline->PosHermite[i].z += sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}

		t += (1.0f / RAIL_SET);
	}
}