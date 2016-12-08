#ifndef _PLAYER_H_
#define _PLAYER_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(player.cpp)
//	�t�@�C����	player.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/14
//
//=============================================================================
#include "sceneDX.h"
#include "sceneXDX.h"
#include "rail.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	PLAYER_PARTS			(1)			// �v���C���[�̃p�[�c��
#define	PLAYER_TEXTURENUM		(4)
#define	PLAYER_TEXFILENAME000	"player000.png"	// �e�N�X�`���̃t�@�C����
#define	PLAYER_TEXFILENAME001	"player001.png"	// �e�N�X�`���̃t�@�C����
#define	PLAYER_TEXFILENAME002	"player002.jpg"	// �e�N�X�`���̃t�@�C����
#define	PLAYER_TEXFILENAME003	"player003.jpg"	// �e�N�X�`���̃t�@�C����

#define	PLAYER_MOVEMENT			(0.0003f)		// �v���C���[�̃X�s�[�h
#define	PLAYER_ROT_STEP			(8)				// �v���C���[�̉�]�X�e�b�v

#define	PLAYER_SPEED_DOWN		(0.05f)			// �v���C���[�̉�]�X�e�b�v
#define	PLAYER_JUMP				(15.0f)			// �v���C���[�̉�]�X�e�b�v
#define	PLAYER_GRAVITY			(0.5f)			// �v���C���[�̉�]�X�e�b�v
#define	PLAYER_ROTMOTION		(0.2f)			// �v���C���[�̉�]�X�e�b�v
#define	PLAYER_MOTION_SPEED		(1.0f / 15)		// �v���C���[�̃��[�V�����X�s�[�h

//=============================================================================
//	�\����
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
//	�N���X��`
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
	float	GetPerSpline(void) { return m_Per; }					// �X�v���C�������擾
	void	SetPerSpline(float perSpline) { m_Per = perSpline; }	// �X�v���C���ʒu���Z�b�g

protected:
	CModel *m_Model[PLAYER_PARTS];	// 3D���f���̃p�[�c���

private:
	CPlayer(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_PLAYER);
	~CPlayer();

	void UpdateMove(void);
	
	int				m_RailLine;		// �ǂ̐��H�𑖂��Ă��邩
	bool			m_Pause;		// �|�[�Y
	int				m_NumParts;		// �p�[�c��
	int				m_NumMotion;	// ���[�V������
	
	int				m_nCntMove;		// �ړ��J�E���^
	D3DXVECTOR3		m_Move;			// ���f���̑��Έړ���
	D3DXVECTOR3		m_RotMove;		// ���f���̑��Ή�]��
	bool			m_bJump;		// �W�����v���

	// ���[�V�����֘A
	void		LoadMotion(char *fileName = "./data/motion.txt");
	void		SetMotion(MOTIONTYPE motionType);
	void		UpdateMotion(void);

	MOTION		*m_Motion;		// ���[�V�������
	MOTIONTYPE	m_MotionType;	// ���[�V�����^�C�v
	int			m_NumKey;		// �L�[��
	int			m_Key;			// ���݂̃L�[
	int			m_Frame;		// ���݂̃t���[���l

	SPLINE*		m_Spline;		// �X�v���C�����
	float		m_Per;			// �X�v���C����ʒu
	float		m_PerMove;		// �X�v���C����ʒu�̑J�ڗ�
};

#endif