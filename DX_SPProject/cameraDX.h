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
#include "d3dx9.h"
//=============================================================================
//	マクロ定義
//=============================================================================
#define	CAMERA_NEARZ				(10.0f)			// NearZ値
#define	CAMERA_FARZ					(100000.0f)		// FarZ値
#define	CAMERA_DEFAULT_DISTANCE		(200.0f)		// 初期の視点―注視点間距離

#define	CAMERA_POSV_MOVEMENT		(5.0f)			// 視点の移動速度
#define	CAMERA_POSR_MOVEMENT_X		(0.05f)			// 注視点の移動量(左右)
#define	CAMERA_POSR_MOVEMENT_Y		(2.0f)			// 注視点の移動量(上下)

#define	CAMERA_POSV_TOPLAYER		(200.0f)		// プレイヤーからの距離
#define	CAMERA_POSV_TOHIGHPLAYER	(70.0f)		// プレイヤーからの距離

//=============================================================================
//	構造体
//=============================================================================
typedef struct{	// カメラ情報
	D3DXVECTOR3	posV;			// 視点
	D3DXVECTOR3	posR;			// 注視点
	D3DXVECTOR3	vecU;			// 上方向ベクトル
	D3DXMATRIX	mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	mtxView;		// ビューマトリックス
	D3DXVECTOR3 Rot;			// 回転角
	float		fDistance;		// 視点―注視点間距離
} CAMERA;

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

	bool	GetCameraMode(void) { return m_flgCameraMode; }
	void	ChangeCameraMode(void) { m_flgCameraMode = m_flgCameraMode ? false : true; }

protected:
	void CameraMove(void);

	bool m_flgCameraMode;	// カメラモード、false->通常、true->エディットモード
};

#endif