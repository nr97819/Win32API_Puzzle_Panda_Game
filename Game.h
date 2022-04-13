#pragma once

#include "Piece.h"

class CGame
{
private:
	CPiece			m_pieces[4][4]; // 게임 판 (퍼즐 조각들)
	POINT			m_voidPos; // 초기 공백 퍼즐(조각)의 좌표
	static int		RAND_MIX_VALUE; // 랜덤하게 섞을 정도

public:
	CGame();
	~CGame();

public:
	CPiece(*GetPieces())[4] { return m_pieces; };

	bool InitializeGame(int _delta);
	void MovePiece(int _dotX, int _dotY);
	void ShufflePuzzleRandomly();
};

