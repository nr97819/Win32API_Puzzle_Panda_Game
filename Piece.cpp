#include "pch.h"

#include "Piece.h"

CPiece::CPiece() 
	: m_id(0), m_size(0), m_pos{}
{

}

CPiece::CPiece(int _index, POINT _pPos)
	: m_id(_index), m_pos(_pPos), m_size(0)
{

}

CPiece::~CPiece()
{

}
