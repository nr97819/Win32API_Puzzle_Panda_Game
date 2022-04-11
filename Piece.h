#pragma once

#include <Windows.h>

class CPiece
{
private:
	int m_id;
	int m_size;
	POINT m_pos;

public:
	CPiece();
	CPiece(int index, POINT pPos);
	~CPiece();

	int GetId() { return m_id; }
	void SetId(int _id) { this->m_id = _id; }
	POINT GetPos() { return m_pos; }
};

