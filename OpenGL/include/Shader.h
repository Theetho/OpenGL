#pragma once

class Shader
{
public:
	Shader(const std::string & shaderName);
	Shader(const std::string & vertexName, const std::string & fragmentName);
	virtual ~Shader();

	void SetBool(const std::string & name, const bool & value) const;
	void SetInt(const std::string & name, const int & value) const;
	void SetFloat(const std::string & name, const float & value) const;
	void SetMatrix(const std::string & name, const glm::mat4 & data) const;
	void SetVector3(const std::string & name, const glm::vec3 & data) const;
	void SetVector4(const std::string & name, const glm::vec4 & data) const;
	void SetSamplers(const unsigned int & count) const;
	
	inline void					Start() const { glUseProgram(m_ProgramID); }
	inline void					Stop() const { glUseProgram(0); }
	inline const unsigned int	GetProgramID() const { return m_ProgramID; }

private:
	unsigned int m_ProgramID, m_VertexID, m_FragmentID;

	std::string		LoadSourceCode(const std::string & filePath);
	unsigned int	CompileShader(const std::string & filePath, const unsigned int & type);
};

