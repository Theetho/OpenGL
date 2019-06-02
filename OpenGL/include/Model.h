#pragma once

class Model
{
public:
	Model(const unsigned int & vaoID, const unsigned int & count, const bool & indiced);
	~Model();

	inline const unsigned int	GetVaoID() const { return m_VaoID; }
	inline const unsigned int	GetCount() const { return m_Count; }
	inline const bool			GetIndiced() const { return m_Indiced; }

private:
	unsigned int m_VaoID, m_Count;
	bool m_Indiced;
};


