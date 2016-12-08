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
#include "rail.h"
#include "railLine.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PLAYER_PARTS			(1)			// プレイヤーのパーツ数
#define	PLAYER_TEXTURENUM		(4)
#define	PLAYER_TEXFILENAME000	"player000.png"	// テクスチャのファイル名
#define	PLAYER_TEXFILENAME001	"player001.png"	// テクスチャのファイル名
#define	PLAYER_TEXFILENAME002	"player002.jpg"	// テクスチャのファイル名
#define	PLAYER_TEXFILENAME003	"player003.jpg"	// テクスチャのファイル名

#define	PLAYER_MOVEMENT			(0.0003f)		// プレイヤーのスピード
#define	PLAYER_ROT_STEP			(8)				// プレイヤーの回転ステップ

#define	PLAYER_SPEED_DOWN		(0.05f)			// プレイヤーの回転ステップ
#define	PLAYER_JUMP				(15.0f)			// プレイヤーの回転ステップ
#define	PLAYER_GRAVITY			(0.5f)			// プレイヤーの回転ステップ
#define	PLAYER_ROTMOTION		(0.2f)			// プレイヤーの回転ステップ
#define	PLAYER_MOTION_SPEED		(1.0f / 15)		// プレイヤーのモーションスピード

//=============================================================================
//	構造体
//=============================================================================
typedef struct{
	float PosX, PosY, PosZ;
	float RotX, RotY, RotZ;
} KEY;

typedef struct{
	int	Frame;
	KEY	*Key;
} KEY_INFO;

typedef struct{
	bool		Loop;
	int			NumKey;
	KEY_INFO	*KeyInfo;
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

	static CPlayer	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

	//SPLINE	GetSpline(void) { return m_Spline; }
	float	GetPerSpline(void) { return m_Per; }					// スプライン情報を取得
	void	SetPerSpline(float perSpline) { m_Per = perSpline; }	// スプライン位置をセット

protected:
	CModel *m_Model[PLAYER_PARTS];	// 3Dモデルのパーツ情報

private:
	CPlayer(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_PLAYER);
	~CPlayer();

	void UpdateMove(void);
	
	int				m_RailLine;		// どの線路を走っているか
	bool			m_Pause;		// ポーズ
	int				m_NumParts;		// パーツ数
	int				m_NumMotion;	// モーション数
	
	int				m_nCntMove;		// 移動カウンタ
	D3DXVECTOR3		m_Move;			// モデルの相対移動量
	D3DXVECTOR3		m_RotMove;		// モデルの相対回転量
	bool			m_bJump;		// ジャンプ状態

	// モーション関連
	void		LoadMotion(char *fileName = "./data/motion.txt");
	void		SetMotion(MOTIONTYPE motionType);
	void		UpdateMotion(void);

	MOTION		*m_Motion;		// モーション情報
	MOTIONTYPE	m_MotionType;	// モーションタイプ
	int			m_NumKey;		// キー数
	int			m_Key;			// 現在のキー
	int			m_Frame;		// 現在のフレーム値

	SPLINE*		m_Spline;		// スプライン情報
	float		m_Per;			// スプライン上位置
	float		m_PerMove;		// スプライン上位置の遷移量
};

#endif