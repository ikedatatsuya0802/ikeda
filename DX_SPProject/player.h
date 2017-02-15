#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(player.cpp)
//	ファイル名	player.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/14
//
//=============================================================================
#include "sceneDX.h"
#include "sceneXDX.h"
#include "model.h"
#include "game.h"
#include "rail.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PLAYER_PARTS			(1)			// プレイヤーのパーツ数

#define PLAYER_MOVEMENT			(0.05f / CGame::GetRailLine()->GetSpline()->LengthMin[0])	// 加速度
#define PLAYER_SPEED_MAX		(PLAYER_MOVEMENT * 1200.0f)									// 最高速
#define	PLAYER_CURVESPEED_MAX	(PLAYER_SPEED_MAX * (1.0f / 12.0f))							// カーブの限界スピード

#define	PLAYER_START_PER		(0.02f)		// プレイヤーのスタート地点

#define	PLAYER_DRIFT_ROT		(0.4f)						// ドリフト時の回転量
#define	PLAYER_ROT_STEP			(8)							// プレイヤーの回転に掛かるフレーム数
#define	PLAYER_NOT_DRIFT		(PLAYER_SPEED_MAX * 0.2f)	// カーブでドリフトしていなかった際の減速量

#define	PLAYER_SPEED_DOWN		(0.05f)			// プレイヤーの回転ステップ
#define	PLAYER_JUMP				(10.0f)			// プレイヤーの回転ステップ
#define	PLAYER_GRAVITY			(0.5f)			// プレイヤーの回転ステップ
#define	PLAYER_ROTMOTION		(0.2f)			// プレイヤーの回転ステップ
#define	PLAYER_MOTION_SPEED		(1.0f / 15)		// プレイヤーのモーションスピード

#define	PLAYER_CAMERA_DISVEC	(10.0f)			// カメラ距離の変更量

//=============================================================================
//	構造体
//=============================================================================
typedef struct{
	float PosX, PosY, PosZ;
	float RotX, RotY, RotZ;
} KEY;

typedef struct{
	int	Frame;
	KEY* Key;
} KEY_INFO;

typedef struct{
	bool		Loop;
	int			NumKey;
	KEY_INFO* KeyInfo;
} MOTION;

typedef enum{
	MT_NONE,
	MT_NEUTRAL,
	MT_WALK,
	MT_PUNCH,
	MT_MAX
} MOTIONTYPE;

//=============================================================================
//	クラス定義
//=============================================================================
class CModel;
class CPlayer : public CSceneXDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CPlayer* Create(bool ifListAdd = true, D3DXVECTOR3 pos = VEC3_ZERO);

	//SPLINE	GetSpline(void) { return m_Spline; }
	D3DXVECTOR3	GetPlayerPos(void) { return CGame::GetRailLine()->GetSplinePos(m_Per); }	// 現在座標をリターン
	D3DXVECTOR3	GetSplineRot(void) { return m_Spline->Rot; }		// スプラインの回転情報をリターン
	D3DXVECTOR3	GetMoveDir(void) { return m_MoveVec; }				// 進行方向を取得
	float	GetSpeed(void) { return m_PerMove; }					// スプライン情報を取得
	float	GetOldPerSpline(void) { return m_PerOld; }				// スプライン情報を取得
	float	GetPerSpline(void) { return m_Per; }					// スプライン情報を取得
	void	SetPerSpline(float perSpline) { m_Per = perSpline; }	// スプライン位置をセット

protected:
	CPlayer(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_PLAYER);
	~CPlayer();

	CModel *m_Model[PLAYER_PARTS];	// 3Dモデルのパーツ情報

	void UpdateMove(void);
	void UpdateDrift(void);
	
	int				m_RailLine;		// どの線路を走っているか
	bool			m_Pause;		// ポーズ
	int				m_NumParts;		// パーツ数
	int				m_NumMotion;	// モーション数
	
	D3DXVECTOR3		m_Move;			// モデルの相対移動量
	D3DXVECTOR3		m_RotMove;		// モデルの相対回転量
	int				m_CntRot;		// 移動カウンタ
	bool			m_bJump;		// ジャンプ状態
	D3DXVECTOR3		m_MoveVec;		// 進行方向
	D3DXVECTOR3		m_VecQuat;		// 傾斜クォータニオンの回転ベクトル
	float			m_RotQuat;		// 傾斜クォータニオンの回転角度

	// モーション関連
	void		LoadMotion(char *fileName = "./data/motion.txt");

	MOTION*		m_Motion;		// モーション情報

	SPLINE*		m_Spline;		// スプライン情報
	float		m_Per;			// スプライン上位置
	float		m_PerOld;		// スプライン上位置(旧)
	float		m_PerMove;		// スプライン上位置の移動量
	float		m_SplineTime;	// スプライン上位置の移動量
	float		m_RealSpeed;	// 絶対移動量
	int			m_DriftState;	// ドリフトしているか、またどちらにドリフトしているか
	int			m_DriftCurve;	// どちらにドリフト中か
	int			m_CntDrift;		// ドリフトカウンタ
};

#endif