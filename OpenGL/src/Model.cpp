#include "pch.h"
#include "Model.h"

Model::Model(const unsigned int & vaoID, const unsigned int & count, const bool & indiced) :
	m_VaoID(vaoID),
	m_Count(count),
	m_Indiced(indiced)
{}

Model::~Model()
{}
