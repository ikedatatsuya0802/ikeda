//=============================================================================
//
//	タイトル	シーンファイル(2DDX)
//	ファイル名	gametime.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "gametime.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
FIGURE	CGametime::m_Figure;
int		CGametime::m_NumFigure;

//=============================================================================
//	関数名	:CGametime()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CGametime::CGametime(int priority, OBJTYPE objtype) : CNumber(priority, objtype)
{

}

//=============================================================================
//	関数名	:~CGametime()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CGametime::~CGametime()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CGametime::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure)
{
	// 桁情報格納
	m_Figure = figure;

	// 桁数設定
	if(figure.Dot)
	{
		m_NumFigure = figure.Upper + figure.Dot + figure.Lower;
	}
	else
	{
		m_NumFigure = figure.Upper;
	}

	// ナンバーインスタンス配列生成
	m_Number = new CNumber *[m_NumFigure];

	// 座標初期化
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	
	// 回転初期化
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 座標設定
	for(int i = 0 ; i < figure.Upper ; i++)
	{// 整数部設定
		m_Number[i] = CNumber::Create(D3DXVECTOR3((pos.x - ((size.x * 0.5f) / m_NumFigure * 2.0f * (figure.Upper - i))), pos.y, pos.z), D3DXVECTOR2((size.x / m_NumFigure), size.y));
	}
	if(figure.Dot)
	{// 小数点設定
		m_Number[figure.Upper] = CNumber::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR2((size.x / m_NumFigure), size.y), -1);
	}
	for(int i = figure.Upper + figure.Dot ; i < m_NumFigure ; i++)
	{// 小数部設定
		m_Number[i] = CNumber::Create(D3DXVECTOR3((pos.x + ((size.x * 0.5f) / m_NumFigure * 2.0f * ((i - figure.Upper - figure.Dot) + 1))), pos.y, pos.z), D3DXVECTOR2((size.x / m_NumFigure), size.y));
	}

	// タイマー初期化
	m_Time = 0.0f;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CGametime::Uninit(void)
{

}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CGametime::Update(void)
{
	static float value = 0.0f;

	value += 1.0f;
	SetValue(value);
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CGametime::Draw(void)
{
	// ナンバー描画
	for(int i = 0 ; i < m_NumFigure ; i++)
	{
		m_Number[i]->Draw();
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CGametime *CGametime::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure)
{
	CGametime *gametime;	// インスタンス

	// インスタンス生成
	gametime = new CGametime;

	// 初期化処理
	gametime->Init(pos, size, figure);

	// インスタンスをリターン
	return gametime;
}

//=============================================================================
//	関数名	:SetValue
//	引数	:無し
//	戻り値	:無し
//	説明	:数値をセットする。
//=============================================================================
void CGametime::SetValue(float value)
{
	char *str;				// 数値入力用文字列
	float valueCal = value;	// 数値
	float valueUpper = 0.0f;
	float valueLower = 0.0f;

	// 文字列生成
	str = new char[m_NumFigure];

	// 整数部キャスト
	if((int)valueCal > pow(10.0f, m_Figure.Upper))
	{
		valueCal -= valueCal - (pow(10.0f, m_Figure.Upper) - 1);
	}
	value = valueCal;

	// 小数部キャスト
	if(m_Figure.Dot)
	{
		valueCal = valueCal * pow(10.0f, -m_Figure.Lower - 1);
		value = (float)(int)(valueCal + 0.5f);
		value = value * pow(10.0f, m_Figure.Lower + 1);
	}
	
	valueCal = value;

	if(m_Figure.Dot)
	{// 小数点付き
		sprintf(str, "%d.%d", m_Figure.Upper, m_Figure.Lower);

		valueLower = (value - (int)value) * pow(10.0f, m_Figure.Upper);

		// 整数部セット
		for(int i = 0 ; i < m_Figure.Upper ; i++)
		{
			if(valueCal >= pow(10.0f, (m_Figure.Upper - i - 1)))
			{
				m_Number[i]->SetNumber((int)valueCal / (int)pow(10.0f, (m_Figure.Upper - i - 1)));
			}
			else
			{
				m_Number[i]->SetNumber(0);
			}

			// 桁下げ
			valueCal = (float)((int)valueCal % (int)pow(10.0f, (m_Figure.Upper - i - 1)));
		}

		// 小数点セット
		m_Number[m_Figure.Upper]->SetNumber(-1);

		// 小数部セット
		for(int i = 0 ; i < m_Figure.Lower ; i++)
		{
			if(valueLower >= pow(10.0f, (m_Figure.Lower - i - 1)))
			{
				m_Number[i]->SetNumber((int)valueLower / (int)pow(10.0f, (m_Figure.Lower - i - 1)));
			}
			else
			{
				m_Number[i]->SetNumber(0);
			}

			// 桁下げ
			valueLower = (float)((int)valueLower % (int)pow(10.0f, (m_Figure.Lower - i - 1)));
		}
	}
	else
	{// 小数点無し
		sprintf(str, "%d", m_Figure.Upper);

		// 整数部セット
		for(int i = 0 ; i < m_Figure.Upper ; i++)
		{
			if(valueCal >= pow(10.0f, (m_Figure.Upper - i - 1)))
			{
				m_Number[i]->SetNumber((int)valueCal / (int)pow(10.0f, (m_Figure.Upper - i - 1)));
			}
			else
			{
				m_Number[i]->SetNumber(0);
			}

			// 桁下げ
			valueCal = (float)((int)valueCal % (int)pow(10.0f, (m_Figure.Upper - i - 1)));
		}/*
		m_Number[0]->SetNumber(1);
		m_Number[1]->SetNumber(3);
		m_Number[2]->SetNumber(5);
		m_Number[3]->SetNumber(7);
		m_Number[4]->SetNumber(9);*/
	}

	delete[] str;
}