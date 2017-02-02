#ifndef _GAME_H_
#define _GAME_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	game.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
#include "mode.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	GOAL_COUNT	(180)	// �S�[�����Ă���t�F�[�h�܂ł̎���

//=============================================================================
//	�O���錾
//=============================================================================
class CMeshfield;
class CPlayer;
class CRailLine;
class CDriftMark;
class CPause;

//=============================================================================
//	�N���X��`
//=============================================================================
class CGame : public CMode
{
public:
	CGame(){}
	~CGame(){}

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static int		GetFrame(void) { return m_Frame; }

	static CMeshfield	*GetMeshfield(void){ return m_Meshfield; }
	static CRailLine	*GetRailLine(void) { return m_RailLine; }
	static CPlayer		*GetPlayer1(void){ return m_Player1; }
	static CPlayer		*GetPlayer2(void){ return m_Player2; }
	static CDriftMark	*GetDriftMark(void) { return m_DriftMark; }
	static CScene2DDX	*GetHakushin(void) { return m_Hakushin; }
	static CPause		*GetPause(void) { return m_Pause; }

private:
	static CMeshfield	*m_Meshfield;	// ���b�V���t�B�[���h�̃C���X�^���X
	static CRailLine	*m_RailLine;	// ���[�����̃C���X�^���X
	static CPlayer		*m_Player1;		// �v���C���[�̃C���X�^���X
	static CPlayer		*m_Player2;		// �v���C���[�̃C���X�^���X
	static CDriftMark	*m_DriftMark;	// �h���t�g�}�[�N�̃C���X�^���X
	static CScene2DDX	*m_Hakushin;	// �W�����̃C���X�^���X
	static CPause		*m_Pause;		// �|�[�Y��ʂ̃C���X�^���X

	static int	m_Frame;	// �ėp�J�E���^

	static int	m_GoalCount;
};

#endif