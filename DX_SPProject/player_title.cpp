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
#include "player_title.h"
#include "cameraDX.h"
#include "meshfield.h"
#include "sceneXDX.h"
#include "model.h"
#include "manager.h"

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CPlayer_Title::Init(D3DXVECTOR3 pos)
{
	m_Pos = pos;
	m_Move = VEC3_ZERO;
	m_Move.z = PLAYER_TITLE_SPEED;

	// モデル・モーションの読み込み
	LoadMotion("./data/motion.txt");
	m_Pause = false;

	m_Frame = 0;
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CPlayer_Title::Update(void)
{
	// 移動してくる感じ
	m_Pos.z += m_Move.z;

	m_Frame++;
	if(CManager::GetCamera()->ifCameraAnimInitialized())
	{
		if(m_Frame > CManager::GetCamera()->GetCameraAnimFrame())
		{
			m_Frame = 0;
			m_Pos.z = -(MESHFIELD_TOTALHEIGHT * 0.5f);
		}
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CPlayer_Title::Draw(void)
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

#ifdef _DEBUG
	CDebugProc::DebugProc("モデル座標:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
#endif
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CPlayer_Title *CPlayer_Title::Create(bool ifListAdd, int priority, OBJTYPE objtype, D3DXVECTOR3 pos)
{
	CPlayer_Title *player;	// インスタンス

	// インスタンス生成
	player = new CPlayer_Title;

	// 初期化処理
	player->Init(pos);

	// インスタンスをリターン
	return player;
}