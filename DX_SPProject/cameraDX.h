#ifndef _CAMERADX_H_
#define _CAMERADX_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene.cpp)
//	�t�@�C����	scene.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/19
//
//=============================================================================
#include "rendererDX.h"
//=============================================================================
//	�}�N����`
//=============================================================================
#define DX_CAMERA					(CManager::GetCamera())

#define	CAMERA_NEARZ				(10.0f)			// NearZ�l
#define	CAMERA_FARZ					(100000.0f)		// FarZ�l
#define	CAMERA_DEFAULT_DISTANCE		(200.0f)		// �����̎��_�\�����_�ԋ���
#define	CAMERA_WHEEL_CHANGE_Y		(150.0f)		// �}�E�X�z�C�[���ɂ�鍂���̕ω���
#define	CAMERA_EDIT_V1				D3DXVECTOR3(0.0, 3000.0f, -10.0f)	// �����_�̈ړ���(�㉺)
#define	CAMERA_EDIT_R1				D3DXVECTOR3(0.0, 0.0f, 0.0f)		// �����_�̈ړ���(�㉺)
#define	CAMERA_EDIT_V2				D3DXVECTOR3(0.0, 150.0f, -500.0f)	// �����_�̈ړ���(�㉺)
#define	CAMERA_EDIT_R2				D3DXVECTOR3(0.0, 100.0f, -300.0f)	// �����_�̈ړ���(�㉺)

#define	CAMERA_POSV_MOVEMENT		(5.0f)			// ���_�̈ړ����x
#define	CAMERA_POSV_MOVEMENT_Y		(5.0f)			// ���_�̈ړ���(�㉺)
#define	CAMERA_POSR_MOVEMENT_X		(0.05f)			// �����_�̈ړ���(���E)
#define	CAMERA_POSR_MOVEMENT_Y		(3.0f)			// �����_�̈ړ���(�㉺)

#define	CAMERA_POSV_TOPLAYER		(200.0f)		// �v���C���[����̋���
#define	CAMERA_POSV_TOHIGHPLAYER	(70.0f)			// �v���C���[����̋���

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	D3DXVECTOR3	vPos;	// �h����W
	int			MaxCnt;	// ��ʂ̗h��J�E���^
	int			Cnt;	// ��ʂ̗h��J�E���^
	float		Width;	// ��ʂ̗h��͈̔�
} CAMERA_VIBRATE;				// �h����

typedef struct{	// �J�������
	D3DXVECTOR3	posV;			// ���_
	D3DXVECTOR3	posR;			// �����_
	D3DXVECTOR3	vecU;			// ������x�N�g��
	D3DXMATRIX	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxView;		// �r���[�}�g���b�N�X
	D3DXVECTOR3 Rot;			// ��]�p
	float		Distance;		// ���_�\�����_�ԋ���
	CAMERA_VIBRATE Vib;			// �h����
} CAMERA;

typedef struct {
	int			Frame;
	D3DXVECTOR3	PosV;	// ���_
	D3DXVECTOR3	PosR;	// �����_
} CAMERA_ANIM_STATUS;	// �J�����̃A�j���[�V�����X�e�[�^�X

typedef struct {
	bool						ifAnim;		// �A�j���[�V�������Ă��邩
	bool						Loop;		// ���[�v���邩
	vector<CAMERA_ANIM_STATUS>	Status;		// �A�j���[�V�������
} CAMERA_ANIM;// �J�����̃A�j���[�V�������

//=============================================================================
//	�N���X��`
//=============================================================================
class CCameraDX
{
public:
	CCameraDX();
	~CCameraDX();

	CAMERA	m_CS;			// �J����
	CAMERA	m_CSEdit;		// �J����(�G�f�B�b�g���[�h)

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

	bool m_flgCameraMode;	// �J�������[�h�Afalse->�ʏ�Atrue->�G�f�B�b�g���[�h
};

#endif