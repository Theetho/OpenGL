#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const std::vector<unsigned int>& indices) :
	m_Vertices(vertices),
	m_Textures(textures),
	m_Indices(indices)
{
	this->SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_Vao);
	glGenBuffers(1, &m_Vbo);
	glGenBuffers(1, &m_Ebo);

	glBindVertexArray(m_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
