#pragma once

class Model
{
public:
	Model(const unsigned int & vaoID, const unsigned int & count);
	~Model();

	inline const unsigned int GetVaoID() const { return m_VaoID; }
	inline const unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_VaoID, m_Count;
};


