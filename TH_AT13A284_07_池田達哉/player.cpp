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
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Pause		= false;

	m_MotionType= MT_NONE;
	m_Key		= 0;
	m_Frame		= 0;

	// モデル・モーションの読み込み
	LoadMotion("motion.txt");
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

}

//=============================================================================
//	関数名	:UpdateMotion
//	引数	:無し
//	戻り値	:無し
//	説明	:モーションを更新する。
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	for(int i = 0 ; i < (int)m_Model.size() ; i++)
	{
		KEY *key, *keyNext;
		float rate;
		D3DXVECTOR3 pos, rot;

		key = &m_Motion.KeyInfo[m_Key].Key[i];
		keyNext = &m_Motion.KeyInfo[(m_Key + 1) % m_Motion.KeyInfo.size()].Key[i];
		rate = (float)m_Frame / m_Motion.KeyInfo[m_Key].Frame;
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
	if(m_Frame >= m_Motion.KeyInfo[m_Key].Frame)
	{
		m_Key = (m_Key + 1) % m_Motion.KeyInfo.size();
		m_Frame = 0;

		if(!m_Motion.Loop && (m_Key == (m_Motion.KeyInfo.size() - 1)))
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
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// 描画処理
	for(int i = 0 ; i < (int)m_Model.size() ; i++)
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
	FILE	*fp			= NULL;	// ファイルポインタ
	FILE	*fpBuff		= NULL;	// ファイルポインタ
	string	optional	= ".\\data\\";
	optional += fileName;

	fp = fopen(optional.c_str(), "r");

	// END_SCRIPTまで読み込み
	while(!feof(fp))
	{
		string word;
		char wordBuff[1024] = { NULL };

		// 単語を取得
		fscanf(fp, "%s", wordBuff);
		word += wordBuff;

		if(word == "MODEL_DIR")
		{// モデル情報を読み込み

			char buff[127] = { NULL };
			char buff2[127] = { NULL };
			string buf;
			vector<string> filename;

						
			// ファイルディレクトリ、ファイル名取得
			fscanf(fp, " = %s\n", buff);
			fscanf(fp, "MODEL_FILENAME = %s\n", buff2);

			// モデルインスタンス生成
			m_Model.push_back(CModel::Create(buff, buff2));

			int parent = 0;
			D3DXVECTOR3 vec = VEC3_ZERO;

			// 親オブジェクトの読み込み
			fscanf(fp, "PARENT = %d\n", &parent);
			// 親オブジェクト番号のセット
			m_Model[m_Model.size() - 1]->SetParent((parent >= 0) ? m_Model[parent] : NULL);

			// 座標読み込み
			fscanf(fp, "SCL = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			// 座標のセット
			m_Model[m_Model.size() - 1]->SetScl(vec);

			// 座標読み込み
			fscanf(fp, "DEFPOS = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			// 座標のセット
			m_Model[m_Model.size() - 1]->SetPosDef(vec);

			// 回転読み込み
			fscanf(fp, "DEFROT = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			// 回転のセット
			m_Model[m_Model.size() - 1]->SetRotDef(vec);

		}
		else if(word == "LOOP")
		{// モーションのループ情報読み込み

			fscanf(fp, " = %d\n", &m_Motion.Loop);
		}
		else if(word == "FRAME")
		{// キー情報読み込み

			// キー情報配列生成
			m_Motion.KeyInfo.push_back(KEY_INFO());

			// フレーム数格納
			fscanf(fp, " = %d\n", &m_Motion.KeyInfo[m_Motion.KeyInfo.size() - 1].Frame);
		}
		else if(word == "POS")
		{// キー配列生成

			KEY key;
			// 座標情報格納
			fscanf(fp, " = %f %f %f\n", &key.PosX, &key.PosY, &key.PosZ);
			// 回転情報格納
			fscanf(fp, "ROT = %f %f %f\n", &key.RotX, &key.RotY, &key.RotZ);

			// キー配列生成＆情報格納
			m_Motion.KeyInfo[m_Motion.KeyInfo.size() - 1].Key.push_back(key);
		}
	}

	// ファイルクローズ
	fclose(fp);
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
		m_NumKey		= m_Motion.KeyInfo.size();
		m_Key			= 0;
		m_Frame			= 0;
	}
	else
	{// モーションを同時実行する

		if(m_FrameBlendMotion == 0)
		{
			m_MotionBlend	= motionType;
			m_NumKeyBlend	= m_Motion.KeyInfo.size();
			m_KeyBlend		= 0;
			m_FrameBlend	= 0;
		}
	}

	// モーションブレンドカウンタの初期化
	m_FrameBlendMotion	= 0;
}