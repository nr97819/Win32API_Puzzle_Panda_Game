#include "pch.h"

#include "Game.h"

int CGame::RAND_MIX_VALUE = 3000;

CGame::CGame() 
	: m_pieces{}
	, m_voidPos{ 3, 3 }

{

}

CGame::~CGame()
{

}

bool CGame::InitializeGame(int _delta)
{
	int x, y, idxCnt = 0;
	for (y = 0; y < 4; ++y)
	{
		for (x = 0; x < 4; ++x)
		{
			if (idxCnt >= 15) // 마지막 칸은 비워두기 (게임용)
			{
				m_pieces[y][x] = CPiece(-1, POINT{ x * _delta, y * _delta });
				break;
			}

			m_pieces[y][x] = CPiece(idxCnt, POINT{ x * _delta, y * _delta });
			idxCnt++;
		}
	}
	return true;
}

void CGame::ShufflePuzzleRandomly()
{
	for (int k = 0; k < RAND_MIX_VALUE; k++)
	{
		int iX = static_cast<int>(rand() % 4);
		int iY = static_cast<int>(rand() % 4);

		if ((abs(iX - m_voidPos.x) + abs(iY - m_voidPos.y)) == 1)
		{
			CPiece(*pPieces)[4] = GetPieces();

			CPiece temp = pPieces[iY][iX];
			pPieces[iY][iX] = pPieces[m_voidPos.y][m_voidPos.x];
			pPieces[m_voidPos.y][m_voidPos.x] = temp;

			m_voidPos.x = iX;
			m_voidPos.y = iY;
		}
	}
}

void CGame::MovePiece(int _dotX, int _dotY)
{
	CPiece(*pPieces)[4] = GetPieces();

	if ((abs(_dotX - m_voidPos.x) + abs(_dotY - m_voidPos.y)) == 1)
	{
		CPiece temp = pPieces[_dotY][_dotX];
		pPieces[_dotY][_dotX] = pPieces[m_voidPos.y][m_voidPos.x];
		pPieces[m_voidPos.y][m_voidPos.x] = temp;

		m_voidPos.x = _dotX;
		m_voidPos.y = _dotY;
	}
}