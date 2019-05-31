#include "pch.h"
#include "Shader.h"

Shader::Shader(const std::string & shaderName)
{
	m_ProgramID		= glCreateProgram();
	m_VertexID		= CompileShader("res/shaders/" + shaderName + ".vert", GL_VERTEX_SHADER);
	m_FragmentID	= CompileShader("res/shaders/" + shaderName + ".frag", GL_FRAGMENT_SHADER);
	glAttachShader(m_ProgramID, m_VertexID);
	glAttachShader(m_ProgramID, m_FragmentID);
	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);

	// Checking the linking status
	int success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(m_ProgramID, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	glDeleteShader(m_VertexID);
	glDeleteShader(m_FragmentID);
}

Shader::Shader(const std::string & vertexName, const std::string & fragmentName)
{
	m_ProgramID = glCreateProgram();
	m_VertexID = CompileShader("res/shaders/" + vertexName + ".vert", GL_VERTEX_SHADER);
	m_FragmentID = CompileShader("res/shaders/" + fragmentName + ".frag", GL_FRAGMENT_SHADER);
	glAttachShader(m_ProgramID, m_VertexID);
	glAttachShader(m_ProgramID, m_FragmentID);
	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);

	// Checking the linking status
	int success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetProgramInfoLog(m_ProgramID, 512, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
	}

	glDeleteShader(m_VertexID);
	glDeleteShader(m_FragmentID);
}


Shader::~Shader()
{
	Stop();
	glDeleteProgram(m_ProgramID);
}

void Shader::SetBool(const std::string & name, const bool & value) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string & name, const int & value) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string & name, const float & value) const
{
	glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value);
}

void Shader::SetMatrix(const std::string & name, const glm::mat4 & data) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(data));
}

void Shader::SetVector3(const std::string & name, const glm::vec3 & data) const
{
	glUniform3fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, value_ptr(data));
}

void Shader::SetVector4(const std::string & name, const glm::vec4 & data) const
{
	glUniform4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, value_ptr(data));
}

void Shader::SetSamplers(const unsigned int & count) const 
{
	for (unsigned int i = 1; i <= count; ++i)
	{
		SetInt("uniform_texture_" + std::to_string(i), i - 1);
	}
}

std::string Shader::LoadSourceCode(const std::string & filePath)
{
	std::ifstream file(filePath);
	std::string line, source = "";
	if (file.is_open())
	{
		while (std::getline(file, line))
			source += line + "\n";
		file.close();
	}
	return source;
}

unsigned int Shader::CompileShader(const std::string & filePath, const unsigned int & type)
{
	std::string source = LoadSourceCode(filePath);
	const char * c_source = source.c_str();
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &c_source, NULL);
	glCompileShader(id);

	// Checking the compilation status
	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char info_log[512];
		glGetShaderInfoLog(id, 512, NULL, info_log);
		std::cout << "File path : " << filePath << ", Source code :\n" << source << std::endl;
		std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << info_log << std::endl;
		return 0;
	}

	return id;
}
