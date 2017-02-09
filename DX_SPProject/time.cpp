//=============================================================================
//
//	タイトル	経過時間
//	ファイル名	time.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2017/01/21
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "time.h"

//=============================================================================
//	関数名	:CTime()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CTime::CTime(bool ifListAdd, int priority, OBJTYPE objType) : CScene2DDX(ifListAdd, priority, objType)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	関数名	:~CTime()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CTime::~CTime()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CTime::Init(DWORD time, cVec3 pos, cVec2 size)
{
	m_ifCountStart	= false;
	m_StartTime		= 0;
	m_TimeValue		= time;
	m_Time.SetTime(0, 0, 0);
	TimeConvert(&m_Time, m_TimeValue);


	// 時刻のセット
	m_Instance[0] = CNumber::Create((m_Time.minute / 10), D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 3.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[1] = CNumber::Create((m_Time.minute % 10), D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 2.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[2] = CScene2DDX::Create(".\\data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 1.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y), 0.0f);
	m_Instance[3] = CNumber::Create((m_Time.second / 10), D3DXVECTOR3(pos.x + (-size.x / TIME_FIGURE * 0.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[4] = CNumber::Create((m_Time.second % 10), D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 0.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[5] = CScene2DDX::Create(".\\data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 1.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y), 0.0f);
	m_Instance[6] = CNumber::Create((m_Time.millisec / 10), D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 2.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
	m_Instance[7] = CNumber::Create((m_Time.millisec % 10), D3DXVECTOR3(pos.x + (size.x / TIME_FIGURE * 3.5f), pos.y, 0.0f),
		D3DXVECTOR2((size.x / TIME_FIGURE), size.y));
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CTime::Uninit(void)
{

}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CTime::Update(void)
{
	// カウント中の場合
	if(m_ifCountStart)
	{
		// 現在時間取得
		m_TimeValue = timeGetTime() - m_StartTime;

		// 時間変換
		TimeConvert(&m_Time, m_TimeValue);

		// 時間をセット
		((CNumber*)m_Instance[0])->SetNumber((m_Time.minute <= 0) ? 0 : (m_Time.minute / 10));
		((CNumber*)m_Instance[1])->SetNumber((m_Time.minute <= 0) ? 0 : (m_Time.minute % 10));
		((CNumber*)m_Instance[3])->SetNumber((m_Time.second <= 0) ? 0 : (m_Time.second / 10));
		((CNumber*)m_Instance[4])->SetNumber((m_Time.second <= 0) ? 0 : (m_Time.second % 10));
		((CNumber*)m_Instance[6])->SetNumber((m_Time.millisec <= 0) ? 0 : (m_Time.millisec / 10));
		((CNumber*)m_Instance[7])->SetNumber((m_Time.millisec <= 0) ? 0 : (m_Time.millisec % 10));
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CTime::Draw(void)
{

}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CTime *CTime::Create(DWORD time, cVec3 pos, cVec2 size)
{
	CTime *instance;	// インスタンス
	
	// インスタンス生成
	instance = new CTime();
	
	// 初期化処理
	instance->Init(time, pos, size);
	
	// インスタンスをリターン
	return instance;
}

//=============================================================================
//	関数名	:SaveTime
//	引数	:無し
//	戻り値	:無し
//	説明	:ランキングに引き渡す時間を保存。
//=============================================================================
void CTime::SaveTime(void)
{
	FILE* fp;

	fopen_s(&fp, TIME_FILENAME, "w");

	fprintf_s(fp, "%d", m_TimeValue);

	fclose(fp);
}

//=============================================================================
//	関数名	:TimeConvert
//	引数	:無し
//	戻り値	:無し
//	説明	:ミリ秒で表された時間を分秒ミリ秒で表される時間に変換。
//=============================================================================
void CTime::TimeConvert(TIME* timeStr, DWORD time)
{
	timeStr->minute = time / 1000 / 60;
	timeStr->second = time / 1000 % 60;
	timeStr->millisec = time % 1000 / 10;

	if(timeStr->minute > 99) timeStr->minute = 99;
	if(timeStr->minute > 59) timeStr->minute = 59;
	if(timeStr->minute > 99) timeStr->minute = 99;
}

//=============================================================================
//	関数名	:SetColor
//	引数	:float	a		-> アルファ値
//			:float	r		-> R値
//			:float	g		-> G値
//			:float	b		-> B値
//	戻り値	:無し
//	説明	:ポリゴン色を設定する。
//=============================================================================
void CTime::SetColor(cfloat a, cfloat r, cfloat g, cfloat b)
{
	for(int i = 0 ; i < TIME_FIGURE ; i++)
	{
		m_Instance[i]->SetColor(a, r, g, b);
	}
}