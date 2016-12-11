//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	farGoal.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "farGoal.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	関数名	:CFarGoal()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CFarGoal::CFarGoal(bool ifListAdd, int priority, OBJTYPE objtype)
{

}

//=============================================================================
//	関数名	:~CFarGoal()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CFarGoal::~CFarGoal()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CFarGoal::Init(uint defValue, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	uint valueCal = defValue;	// 初期値の退避
	m_FarLength = (float)defValue;

	if(valueCal > 0)
	{
		// 桁数ぶんだけ配列を追加する	
		while(valueCal != 0)
		{
			// 配列追加
			m_Number.push_back(0);
			valueCal /= 10;
		}

		valueCal = defValue;
		for(int i = 0 ; i < (int)m_Number.size() ; i++)
		{
			D3DXVECTOR3 numberPos = pos;

			// 相対X座標計算
			numberPos.x = pos.x - ((i * 2 - (int)m_Number.size()) * (size.x / (float)m_Number.size() / 2.0f));

			// 値セット
			m_Number[i] = CNumber::Create((valueCal % 10), numberPos, D3DXVECTOR2((size.x / m_Number.size()), size.y));
			valueCal /= 10;
		}
	}
	else
	{
		m_Number.push_back(0);
		m_Number[0] = CNumber::Create((valueCal % 10), pos, D3DXVECTOR2((size.x / m_Number.size()), size.y));
	}

	CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.62f), (SCREEN_HEIGHT * 0.05f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((250.0f * WINDOW_ASPECT_X), (60.0f * WINDOW_ASPECT_Y)), "farGoal000.png");
	CScene2DDX::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.95f), (SCREEN_HEIGHT * 0.05f), 0.0f), VEC3_ZERO,
		D3DXVECTOR2((70.0f * WINDOW_ASPECT_X), (60.0f * WINDOW_ASPECT_Y)), "farGoal001.png");
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CFarGoal::Uninit(void)
{
	// リスト開放
	m_Number.clear();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CFarGoal::Update(void)
{
	int value = 0;
	float per = CGame::GetPlayer1()->GetPerSpline();

	float length = ((per / RAILLINE_GOAL) * m_FarLength);
	value = (int)(m_FarLength - length);

	SetValue(value);
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CFarGoal::Draw(void)
{

}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CFarGoal *CFarGoal::Create(uint defValue, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CFarGoal *farGoal;	// インスタンス

	// インスタンス生成
	farGoal = new CFarGoal;

	// 初期化処理
	farGoal->Init(defValue, pos, size);

	// インスタンスをリターン
	return farGoal;
}

//=============================================================================
//	関数名	:SetValue
//	引数	:無し
//	戻り値	:無し
//	説明	:数値をセットする。
//=============================================================================
void CFarGoal::SetValue(int value)
{
	int valueCal = value;

	SIMPLE_FOR((int)m_Number.size())
	{
		// 値をセット
		m_Number[i]->SetNumber(valueCal % 10);

		valueCal /= 10;
	}
}