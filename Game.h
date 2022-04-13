#pragma once

#include "Piece.h"

class CGame
{
private:
	CPiece			m_pieces[4][4]; // ���� �� (���� ������)
	POINT			m_voidPos; // �ʱ� ���� ����(����)�� ��ǥ
	static int		RAND_MIX_VALUE; // �����ϰ� ���� ����

public:
	CGame();
	~CGame();

public:
	CPiece(*GetPieces())[4] { return m_pieces; };

	bool InitializeGame(int _delta);
	void MovePiece(int _dotX, int _dotY);
	void ShufflePuzzleRandomly();
};

