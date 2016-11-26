//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	gametime.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "gametime.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
FIGURE	CGametime::m_Figure;
int		CGametime::m_NumFigure;

//=============================================================================
//	�֐���	:CGametime()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CGametime::CGametime(int priority, OBJTYPE objtype) : CNumber(priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CGametime()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CGametime::~CGametime()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CGametime::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure)
{
	// �����i�[
	m_Figure = figure;

	// �����ݒ�
	if(figure.Dot)
	{
		m_NumFigure = figure.Upper + figure.Dot + figure.Lower;
	}
	else
	{
		m_NumFigure = figure.Upper;
	}

	// �i���o�[�C���X�^���X�z�񐶐�
	m_Number = new CNumber *[m_NumFigure];

	// ���W������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	
	// ��]������
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���W�ݒ�
	for(int i = 0 ; i < figure.Upper ; i++)
	{// �������ݒ�
		m_Number[i] = CNumber::Create(D3DXVECTOR3((pos.x - ((size.x * 0.5f) / m_NumFigure * 2.0f * (figure.Upper - i))), pos.y, pos.z), D3DXVECTOR2((size.x / m_NumFigure), size.y));
	}
	if(figure.Dot)
	{// �����_�ݒ�
		m_Number[figure.Upper] = CNumber::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR2((size.x / m_NumFigure), size.y), -1);
	}
	for(int i = figure.Upper + figure.Dot ; i < m_NumFigure ; i++)
	{// �������ݒ�
		m_Number[i] = CNumber::Create(D3DXVECTOR3((pos.x + ((size.x * 0.5f) / m_NumFigure * 2.0f * ((i - figure.Upper - figure.Dot) + 1))), pos.y, pos.z), D3DXVECTOR2((size.x / m_NumFigure), size.y));
	}

	// �^�C�}�[������
	m_Time = 0.0f;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CGametime::Uninit(void)
{

}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CGametime::Update(void)
{
	static float value = 0.0f;

	value += 1.0f;
	SetValue(value);
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CGametime::Draw(void)
{
	// �i���o�[�`��
	for(int i = 0 ; i < m_NumFigure ; i++)
	{
		m_Number[i]->Draw();
	}
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CGametime *CGametime::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, FIGURE figure)
{
	CGametime *gametime;	// �C���X�^���X

	// �C���X�^���X����
	gametime = new CGametime;

	// ����������
	gametime->Init(pos, size, figure);

	// �C���X�^���X�����^�[��
	return gametime;
}

//=============================================================================
//	�֐���	:SetValue
//	����	:����
//	�߂�l	:����
//	����	:���l���Z�b�g����B
//=============================================================================
void CGametime::SetValue(float value)
{
	char *str;				// ���l���͗p������
	float valueCal = value;	// ���l
	float valueUpper = 0.0f;
	float valueLower = 0.0f;

	// �����񐶐�
	str = new char[m_NumFigure];

	// �������L���X�g
	if((int)valueCal > pow(10.0f, m_Figure.Upper))
	{
		valueCal -= valueCal - (pow(10.0f, m_Figure.Upper) - 1);
	}
	value = valueCal;

	// �������L���X�g
	if(m_Figure.Dot)
	{
		valueCal = valueCal * pow(10.0f, -m_Figure.Lower - 1);
		value = (float)(int)(valueCal + 0.5f);
		value = value * pow(10.0f, m_Figure.Lower + 1);
	}
	
	valueCal = value;

	if(m_Figure.Dot)
	{// �����_�t��
		sprintf(str, "%d.%d", m_Figure.Upper, m_Figure.Lower);

		valueLower = (value - (int)value) * pow(10.0f, m_Figure.Upper);

		// �������Z�b�g
		for(int i = 0 ; i < m_Figure.Upper ; i++)
		{
			if(valueCal >= pow(10.0f, (m_Figure.Upper - i - 1)))
			{
				m_Number[i]->SetNumber((int)valueCal / (int)pow(10.0f, (m_Figure.Upper - i - 1)));
			}
			else
			{
				m_Number[i]->SetNumber(0);
			}

			// ������
			valueCal = (float)((int)valueCal % (int)pow(10.0f, (m_Figure.Upper - i - 1)));
		}

		// �����_�Z�b�g
		m_Number[m_Figure.Upper]->SetNumber(-1);

		// �������Z�b�g
		for(int i = 0 ; i < m_Figure.Lower ; i++)
		{
			if(valueLower >= pow(10.0f, (m_Figure.Lower - i - 1)))
			{
				m_Number[i]->SetNumber((int)valueLower / (int)pow(10.0f, (m_Figure.Lower - i - 1)));
			}
			else
			{
				m_Number[i]->SetNumber(0);
			}

			// ������
			valueLower = (float)((int)valueLower % (int)pow(10.0f, (m_Figure.Lower - i - 1)));
		}
	}
	else
	{// �����_����
		sprintf(str, "%d", m_Figure.Upper);

		// �������Z�b�g
		for(int i = 0 ; i < m_Figure.Upper ; i++)
		{
			if(valueCal >= pow(10.0f, (m_Figure.Upper - i - 1)))
			{
				m_Number[i]->SetNumber((int)valueCal / (int)pow(10.0f, (m_Figure.Upper - i - 1)));
			}
			else
			{
				m_Number[i]->SetNumber(0);
			}

			// ������
			valueCal = (float)((int)valueCal % (int)pow(10.0f, (m_Figure.Upper - i - 1)));
		}/*
		m_Number[0]->SetNumber(1);
		m_Number[1]->SetNumber(3);
		m_Number[2]->SetNumber(5);
		m_Number[3]->SetNumber(7);
		m_Number[4]->SetNumber(9);*/
	}

	delete[] str;
}