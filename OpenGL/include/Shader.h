#pragma once

#include "Material.h"
#include "Light.h"

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
	void SetMaterial(const Material & material);
	void SetTexturedMaterial();
	void SetDirectionalLight(const std::vector<DirectionalLight> & light);
	void SetPointLight(const std::vector<PointLight> & light);
	void SetSpotLight(const std::vector<SpotLight> & light);
	
	inline void					Start() const { glUseProgram(m_ProgramID); }
	inline void					Stop() const { glUseProgram(0); }
	inline const unsigned int	GetProgramID() const { return m_ProgramID; }

private:
	unsigned int m_ProgramID, m_VertexID, m_FragmentID;

	std::string		LoadSourceCode(const std::string & filePath);
	unsigned int	CompileShader(const std::string & filePath, const unsigned int & type);
	void			SetLight(const std::string & lightName, const std::vector<Light*> & light);
	template<typename T>
	std::vector<Light *> GetLightVector(const std::vector<T> & light)
	{
		std::vector<Light *> result;
		for (unsigned int i = 0; i < light.size(); ++i)
		{
			result.push_back((Light *)&light[i]);
		}
		return result;
	}
};

