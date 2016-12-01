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
#include "d3dx9.h"
//=============================================================================
//	�}�N����`
//=============================================================================
#define	CAMERA_NEARZ				(10.0f)			// NearZ�l
#define	CAMERA_FARZ					(100000.0f)		// FarZ�l
#define	CAMERA_DEFAULT_DISTANCE		(200.0f)		// �����̎��_�\�����_�ԋ���

#define	CAMERA_POSV_MOVEMENT		(5.0f)			// ���_�̈ړ����x
#define	CAMERA_POSR_MOVEMENT_X		(0.05f)			// �����_�̈ړ���(���E)
#define	CAMERA_POSR_MOVEMENT_Y		(2.0f)			// �����_�̈ړ���(�㉺)

#define	CAMERA_POSV_TOPLAYER		(200.0f)		// �v���C���[����̋���
#define	CAMERA_POSV_TOHIGHPLAYER	(70.0f)		// �v���C���[����̋���

//=============================================================================
//	�\����
//=============================================================================
typedef struct{	// �J�������
	D3DXVECTOR3	posV;			// ���_
	D3DXVECTOR3	posR;			// �����_
	D3DXVECTOR3	vecU;			// ������x�N�g��
	D3DXMATRIX	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxView;		// �r���[�}�g���b�N�X
	D3DXVECTOR3 Rot;			// ��]�p
	float		fDistance;		// ���_�\�����_�ԋ���
} CAMERA;

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

	bool	GetCameraMode(void) { return m_flgCameraMode; }
	void	ChangeCameraMode(void) { m_flgCameraMode = m_flgCameraMode ? false : true; }

protected:
	void CameraMove(void);

	bool m_flgCameraMode;	// �J�������[�h�Afalse->�ʏ�Atrue->�G�f�B�b�g���[�h
};

#endif