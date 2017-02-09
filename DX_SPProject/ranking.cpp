//=============================================================================
//
//	タイトル	経過時間
//	ファイル名	ranking.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2017/01/21
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "ranking.h"

//=============================================================================
//	関数名	:CRanking()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CRanking::CRanking(bool ifListAdd, int priority, OBJTYPE objType) : CScene2DDX(ifListAdd, priority, objType)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
}

//=============================================================================
//	関数名	:~CRanking()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:const D3DXVECTOR3	->	pos(初期位置)
//			:const D3DXVECTOR2	->	size(サイズ)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CRanking::Init(cVec3 pos, cVec2 size, cfloat margin)
{
	DWORD value;
	m_RankIn = -1;

	// ランキング読み込み
	LoadRanking();
	value = m_NowValue;

	for(int i = 0, j = 0 ; i < RANKING_NUM ; i++)
	{
		if(m_Ranking[j] < value)
		{
			m_NewRanking.push_back(m_Ranking[j]);
			j++;
		}
		else
		{
			m_NewRanking.push_back(value);
			m_RankIn = i;
			value = 1000000;
		}
	}

	// ランキングを保存
	SaveRanking();


	// ランキングを表示
	for(int i = 0 ; i < RANKING_NUM ; i++)
	{
		float height = (size.y - (RANKING_NUM - 1) * margin) / RANKING_NUM;
		float y = pos.y + (i - (RANKING_NUM / 2)) * (height + margin);

		CScene2DDX::Create(PRIORITY_2D, ".\\data\\TEXTURE\\ranking000.png", D3DXVECTOR3((pos.x - SCREEN_WIDTH * 0.3f), y, 0.0f), D3DXVECTOR2(height * 2, height))
			->SetUV(D3DXVECTOR2(0.0f, (i / 10.0f)), D3DXVECTOR2(1.0f, (i / 10.0f + 0.1f)));
		m_Instance[i] = CTime::Create(m_NewRanking[i], D3DXVECTOR3(pos.x, y, 0.0f), D3DXVECTOR2(size.x, height));
	}
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CRanking::Uninit(void)
{

}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CRanking::Update(void)
{
	static float alpha = 0.0f;

	if(m_RankIn >= 0)
	{
		m_Instance[m_RankIn]->SetColor((cosf(alpha) / 2 + 0.5f));
	}

	alpha += 0.1f;
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CRanking *CRanking::Create(cVec3 pos, cVec2 size, cfloat margin)
{
	CRanking *instance;	// インスタンス
	
	// インスタンス生成
	instance = new CRanking();
	
	// 初期化処理
	instance->Init(pos, size, margin);
	
	// インスタンスをリターン
	return instance;
}

//=============================================================================
//	関数名	:SaveRanking
//	引数	:無し
//	戻り値	:無し
//	説明	:ランキングを保存。
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE* fp;

	fopen_s(&fp, RANKING_FILENAME, "w");

	// 順位数ぶんランキング情報を保存
	for(uint i = 0 ; i < RANKING_NUM ; i++)
	{
		fprintf_s(fp, "%d\n", m_NewRanking[i]);
	}

	fclose(fp);
}

//=============================================================================
//	関数名	:LoadRanking
//	引数	:無し
//	戻り値	:無し
//	説明	:ランキングを読み込み。
//=============================================================================
void CRanking::LoadRanking(void)
{
	FILE* fp;


	// 今回のスコアを読み込み
	fopen_s(&fp, TIME_FILENAME, "r");
	fscanf_s(fp, "%d", &m_NowValue);
	fclose(fp);


	fopen_s(&fp, RANKING_FILENAME, "r");

	// 順位数ぶんランキング情報を読み込み
	for(uint i = 0 ; i < RANKING_NUM ; i++)
	{
		DWORD value;
		fscanf_s(fp, "%d\n", &value);
		m_Ranking.push_back(value);
	}

	fclose(fp);
}