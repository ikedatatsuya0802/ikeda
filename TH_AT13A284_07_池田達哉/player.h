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

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PLAYER_PARTS			(10)			// プレイヤーのパーツ数
#define	PLAYER_TEXTURENUM		(4)
#define	PLAYER_TEXFILENAME000	"player000.png"	// テクスチャのファイル名
#define	PLAYER_TEXFILENAME001	"player001.png"	// テクスチャのファイル名
#define	PLAYER_TEXFILENAME002	"player002.jpg"	// テクスチャのファイル名
#define	PLAYER_TEXFILENAME003	"player003.jpg"	// テクスチャのファイル名

#define	PLAYER_MOVEMENT			(0.00005f)			// テクスチャのファイル名
#define	PLAYER_ROT_STEP			(8)				// プレイヤーの回転ステップ

#define	PLAYER_SPEED_DOWN		(0.05f)			// プレイヤーの回転ステップ
#define	PLAYER_JUMP				(15.0f)			// プレイヤーの回転ステップ
#define	PLAYER_GRAVITY			(0.5f)			// プレイヤーの回転ステップ
#define	PLAYER_ROTMOTION		(0.2f)			// プレイヤーの回転ステップ
#define	MOTION_CHANGE_FRAME		(30)		// モーションブレンドフレーム数

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
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CPlayer	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CModel *m_Model[PLAYER_PARTS];	// 3Dモデルのパーツ情報

private:
	CPlayer(bool isListAdd = true, int priority = 1, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	void UpdateMove(void);

	bool			m_Pause;		// ポーズ
	int				m_NumParts;		// パーツ数
	int				m_NumMotion;	// モーション数
	D3DXMATRIX		m_mtxWorld;		// ワールドマトリックス
	
	int				m_nCntMove;		// 移動カウンタ
	D3DXVECTOR3		m_Move;			// モデルの相対移動量
	D3DXVECTOR3		m_RotMove;		// モデルの相対回転量
	bool			m_bJump;		// ジャンプ状態

	// モーション関連
	void		LoadMotion(char *fileName = "data/motion.txt");
	void		SetMotion(MOTIONTYPE motionType);
	void		UpdateMotion(void);
	MOTION		*m_Motion;	// モーション

	MOTIONTYPE	m_MotionType;	// 実行中のモーション
	int			m_NumKey;		// キー値
	int			m_Key;			// キー
	int			m_Frame;		// フレーム

	MOTIONTYPE	m_MotionBlend;	// 切替前前のモーション
	int			m_NumKeyBlend;	// キー値
	int			m_KeyBlend;		// キー
	int			m_FrameBlend;	// フレーム
	
	int			m_FrameBlendMotion;// モーション切り替え中カウンタ
};

#endif