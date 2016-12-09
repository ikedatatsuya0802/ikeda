#ifndef _CAMERADX_H_
#define _CAMERADX_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene.cpp)
//	ファイル名	scene.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/19
//
//=============================================================================
#include "rendererDX.h"
//=============================================================================
//	マクロ定義
//=============================================================================
#define DX_CAMERA					(CManager::GetCamera())

#define	CAMERA_NEARZ				(10.0f)			// NearZ値
#define	CAMERA_FARZ					(100000.0f)		// FarZ値
#define	CAMERA_DEFAULT_DISTANCE		(200.0f)		// 初期の視点―注視点間距離
#define	CAMERA_WHEEL_CHANGE_Y		(150.0f)		// マウスホイールによる高さの変化量
#define	CAMERA_EDIT_V1				D3DXVECTOR3(0.0, 3000.0f, -10.0f)	// 注視点の移動量(上下)
#define	CAMERA_EDIT_R1				D3DXVECTOR3(0.0, 0.0f, 0.0f)		// 注視点の移動量(上下)
#define	CAMERA_EDIT_V2				D3DXVECTOR3(0.0, 150.0f, -500.0f)	// 注視点の移動量(上下)
#define	CAMERA_EDIT_R2				D3DXVECTOR3(0.0, 100.0f, -300.0f)	// 注視点の移動量(上下)

#define	CAMERA_POSV_MOVEMENT		(5.0f)			// 視点の移動速度
#define	CAMERA_POSV_MOVEMENT_Y		(5.0f)			// 視点の移動量(上下)
#define	CAMERA_POSR_MOVEMENT_X		(0.05f)			// 注視点の移動量(左右)
#define	CAMERA_POSR_MOVEMENT_Y		(3.0f)			// 注視点の移動量(上下)

#define	CAMERA_POSV_TOPLAYER		(200.0f)		// プレイヤーからの距離
#define	CAMERA_POSV_TOHIGHPLAYER	(70.0f)			// プレイヤーからの距離

//=============================================================================
//	構造体
//=============================================================================
typedef struct {
	D3DXVECTOR3	vPos;	// 揺れ座標
	int			MaxCnt;	// 画面の揺れカウンタ
	int			Cnt;	// 画面の揺れカウンタ
	float		Width;	// 画面の揺れの範囲
} CAMERA_VIBRATE;				// 揺れ情報

typedef struct{	// カメラ情報
	D3DXVECTOR3	posV;			// 視点
	D3DXVECTOR3	posR;			// 注視点
	D3DXVECTOR3	vecU;			// 上方向ベクトル
	D3DXMATRIX	mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	mtxView;		// ビューマトリックス
	D3DXVECTOR3 Rot;			// 回転角
	float		Distance;		// 視点―注視点間距離
	CAMERA_VIBRATE Vib;			// 揺れ情報
} CAMERA;

typedef struct {
	int			Frame;
	D3DXVECTOR3	PosV;	// 視点
	D3DXVECTOR3	PosR;	// 注視点
} CAMERA_ANIM_STATUS;	// カメラのアニメーションステータス

typedef struct {
	bool						ifAnim;		// アニメーションしているか
	bool						Loop;		// ループするか
	vector<CAMERA_ANIM_STATUS>	Status;		// アニメーション情報
} CAMERA_ANIM;// カメラのアニメーション情報

//=============================================================================
//	クラス定義
//=============================================================================
class CCameraDX
{
public:
	CCameraDX();
	~CCameraDX();

	CAMERA	m_CS;			// カメラ
	CAMERA	m_CSEdit;		// カメラ(エディットモード)

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	SetCamera(void);
	
	void	SetCameraPosV(D3DXVECTOR3 v) { m_CS.posV = v; }
	void	SetCameraPosR(D3DXVECTOR3 r) { m_CS.posR = r; }
	void	SetCameraPos(D3DXVECTOR3 v, D3DXVECTOR3 r) { m_CS.posV = v; m_CS.posR = r; }
	void	SetCameraVibrate(int time, float width) { m_CS.Vib.MaxCnt = time; m_CS.Vib.Cnt = time; m_CS.Vib.Width = width; }
	void	DisableCameraVibrate(void) { m_CS.Vib.vPos = VEC3_ZERO; m_CS.Vib.Cnt = 0; m_CS.Vib.Width = 0.0f; }

	bool	GetCameraMode(void) { return m_flgCameraMode; }
	void	ChangeCameraMode(void) { m_flgCameraMode = m_flgCameraMode ? false : true; }

	D3DXVECTOR3	GetCameraPosV(void) { return m_flgCameraMode ? m_CSEdit.posV : m_CS.posV; }
	D3DXVECTOR3	GetCameraPosR(void) { return m_flgCameraMode ? m_CSEdit.posR : m_CS.posR; }

protected:
	void CameraMove(void);
	void CameraAnimation(void);
	void CameraVibrate(void);

	void LoadCameraAnim(void);
	CAMERA_ANIM	m_Anim;
	int m_Key;
	int m_Frame;

	bool m_flgCameraMode;	// カメラモード、false->通常、true->エディットモード
};

#endif