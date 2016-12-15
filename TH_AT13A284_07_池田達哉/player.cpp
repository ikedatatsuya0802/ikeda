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
#include "player.h"
#include "model.h"
#include "main.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "sceneXDX.h"
#include "meshfield.h"

//=============================================================================
//	関数名	:CPlayer()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CPlayer::CPlayer(bool isListAdd, int priority, OBJTYPE objType) : CSceneXDX(isListAdd, priority, objType)
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
	m_NumParts	= 0;
	m_NumMotion	= 0;
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_Move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCntMove	= 0;
	m_bJump		= false;
	m_Pause		= false;

	m_MotionType= MT_NONE;
	m_NumKey	= 0;
	m_Key		= 0;
	m_Frame		= 0;

	m_MotionBlend	= MT_NONE;
	m_NumKeyBlend	= 0;
	m_KeyBlend		= 0;
	m_FrameBlend	= 0;
	
	m_FrameBlendMotion	= 0;

	// モデル・モーションの読み込み
	LoadMotion("data/motion.txt");

	SetMotion(MT_NEUTRAL);
	SetMotion(MT_NEUTRAL);
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
	// プレイヤー移動
	UpdateMove();
	
	// モーション設定
	//UpdateMotion();
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
	CMeshfield	*mesh		= CManager::GetMeshfield();	// メッシュフィールド


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
	// パーツ分だけモーションを更新する
	if(m_MotionType != MT_NONE)
	{// モーションが実行中

		for(int i = 0 ; i < m_NumParts ; i++)
		{
			KEY *key, *keyNext;		// キー情報
			D3DXVECTOR3 pos, rot;	// 座標・回転バッファ
			float rate;				// モーションブレンド割合

			KEY *keyBlend, *keyNextBlend;		// キー情報
			D3DXVECTOR3 posBlend, rotBlend;		// 座標・回転バッファ
			float rateBlend;					// モーションブレンド割合

			D3DXVECTOR3 posBlendMotion, rotBlendMotion;		// 座標・回転バッファ
			float rateBlendMotion;							// モーションブレンド割合

			
			// キー情報の設定
			key		= &m_Motion[m_MotionType].KeyInfo[m_Key].Key[i];
			keyNext	= &m_Motion[m_MotionType].KeyInfo[(m_Key + 1) % m_NumKey].Key[i];

			// ブレンド比率の設定
			rate = (float)m_Frame / m_Motion[m_MotionType].KeyInfo[m_Key].Frame;

			// 座標のブレンド
			pos.x = (key->PosX * (1.0f - rate)) + (keyNext->PosX * rate);
			pos.y = (key->PosY * (1.0f - rate)) + (keyNext->PosY * rate);
			pos.z = (key->PosZ * (1.0f - rate)) + (keyNext->PosZ * rate);

			// 回転のブレンド
			rot.x = (key->RotX * (1.0f - rate)) + (keyNext->RotX * rate);
			rot.y = (key->RotY * (1.0f - rate)) + (keyNext->RotY * rate);
			rot.z = (key->RotZ * (1.0f - rate)) + (keyNext->RotZ * rate);

			// モーションが同時実行中かどうか
			if(m_MotionBlend != MT_NONE)
			{// モーションが同時実行中

				// キー情報の設定
				keyBlend		= &m_Motion[m_MotionBlend].KeyInfo[m_KeyBlend].Key[i];
				keyNextBlend	= &m_Motion[m_MotionBlend].KeyInfo[(m_KeyBlend + 1) % m_NumKeyBlend].Key[i];
				
				// ブレンド比率の設定
				rateBlend = (float)m_FrameBlend / m_Motion[m_MotionBlend].KeyInfo[m_KeyBlend].Frame;

				// 座標のブレンド
				posBlend.x = (keyBlend->PosX * (1.0f - rateBlend)) + (keyNextBlend->PosX * rateBlend);
				posBlend.y = (keyBlend->PosY * (1.0f - rateBlend)) + (keyNextBlend->PosY * rateBlend);
				posBlend.z = (keyBlend->PosZ * (1.0f - rateBlend)) + (keyNextBlend->PosZ * rateBlend);

				// 回転のブレンド
				rotBlend.x = (keyBlend->RotX * (1.0f - rateBlend)) + (keyNextBlend->RotX * rateBlend);
				rotBlend.y = (keyBlend->RotY * (1.0f - rateBlend)) + (keyNextBlend->RotY * rateBlend);
				rotBlend.z = (keyBlend->RotZ * (1.0f - rateBlend)) + (keyNextBlend->RotZ * rateBlend);
				
				// ブレンド比率の設定
				rateBlendMotion = (float)m_FrameBlendMotion / MOTION_CHANGE_FRAME;

				// 座標のブレンド
				posBlendMotion.x = (pos.x * (1.0f - rateBlendMotion)) + (posBlend.x * rateBlendMotion);
				posBlendMotion.y = (pos.y * (1.0f - rateBlendMotion)) + (posBlend.y * rateBlendMotion);
				posBlendMotion.z = (pos.z * (1.0f - rateBlendMotion)) + (posBlend.z * rateBlendMotion);

				// 回転のブレンド
				rotBlendMotion.x = (rot.x * (1.0f - rateBlendMotion)) + (rotBlend.x * rateBlendMotion);
				rotBlendMotion.y = (rot.y * (1.0f - rateBlendMotion)) + (rotBlend.y * rateBlendMotion);
				rotBlendMotion.z = (rot.z * (1.0f - rateBlendMotion)) + (rotBlend.z * rateBlendMotion);

				// ブレンドモーションの座標・回転を設定
				m_Model[i]->SetPos(posBlendMotion);
				m_Model[i]->SetRot(rotBlendMotion);
			}
			else
			{
				// モーションの座標・回転を設定
				m_Model[i]->SetPos(pos);
				m_Model[i]->SetRot(rot);
			}
		}

		// フレームカウンタの増加
		if(m_Frame++ >= m_Motion[m_MotionType].KeyInfo[m_Key].Frame)
		{// フレームカウンタがキーフレームのフレーム数を超えた場合

			// キー情報の再設定
			m_Key = (m_Key + 1) % m_NumKey;
			// フレームカウンタの初期化
			m_Frame = 0;
			
			// モーションはループするかどうか
			if(!m_Motion[m_MotionType].Loop && (m_Key == (m_Motion[m_MotionType].NumKey - 1)))
			{
				SetMotion(MT_NEUTRAL);
			}
		}
		
		
		// モーションが同時実行中かどうか
		if(m_MotionBlend != MT_NONE)
		{// モーションが同時実行中

			if(m_FrameBlend++ >= m_Motion[m_MotionBlend].KeyInfo[m_KeyBlend].Frame)
			{// フレームカウンタがキーフレームのフレーム数を超えた場合

				// キー情報の再設定
				m_KeyBlend = (m_KeyBlend + 1) % m_NumKeyBlend;
				// フレームカウンタの初期化
				m_FrameBlend = 0;
				
				// モーションはループするかどうか
				if(!m_Motion[m_MotionBlend].Loop && (m_KeyBlend == (m_Motion[m_MotionBlend].NumKey - 1)))
				{
					SetMotion(MT_NEUTRAL);
				}
			}

			// ブレンドカウンタの加算
			if(m_FrameBlendMotion++ >= MOTION_CHANGE_FRAME)
			{// ブレンドを終了する
				m_FrameBlendMotion	= 0;
				m_MotionType		= m_MotionBlend;
				m_NumKey			= m_NumKeyBlend;
				m_Key				= m_KeyBlend;
				m_Frame				= m_FrameBlend;
				
				m_MotionBlend	= MT_NONE;
				m_NumKeyBlend	= 0;
				m_KeyBlend		= 0;
				m_FrameBlend	= 0;
			}
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
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
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
	CDebugProc::DebugProc("モデル座標:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	CDebugProc::DebugProc("Key:%d, Frame:%d\n", m_Key, m_Frame);
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
//	関数名	:SetMotion
//	引数	:MOTIONTYPE motionType(モーションタイプ)
//	戻り値	:無し
//	説明	:モーションをセットする。
//=============================================================================
void CPlayer::SetMotion(MOTIONTYPE motionType)
{
	// モーションを同時実行するかどうか
	if(m_MotionType == MT_NONE)
	{// モーションを同時実行しない

		m_MotionType	= motionType;
		m_NumKey		= m_Motion[(int)motionType].NumKey;
		m_Key			= 0;
		m_Frame			= 0;
	}
	else
	{// モーションを同時実行する

		if(m_FrameBlendMotion == 0)
		{
			m_MotionBlend	= motionType;
			m_NumKeyBlend	= m_Motion[(int)motionType].NumKey;
			m_KeyBlend		= 0;
			m_FrameBlend	= 0;
		}
	}

	// モーションブレンドカウンタの初期化
	m_FrameBlendMotion	= 0;
}