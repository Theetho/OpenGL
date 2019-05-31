#pragma once

class Camera
{
public:
	Camera();
	Camera(const glm::vec3 & eyes, const  glm::vec3 & center, const glm::vec3 & up, const float & speed, const float & sensitivity);
	~Camera();

	void Move();
	void LookAt();

	inline const float & GetSpeed() const { return m_Speed; }
	inline const float & GetSensitivity() const { return m_Sensitivity; }
	inline glm::mat4 GetView() const { return m_View; }
	inline void SetSpeed(const float & speed) { m_Speed = speed; }
	inline void SetSensitivity(const float & sensitivity) { m_Sensitivity = sensitivity; }

private:
	float m_Speed, m_Sensitivity;
	float m_Theta;
	float m_Phi;
	glm::vec3 m_Orientation;
	glm::vec3 m_VerticalAxis;
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_ForwardMotion;
	glm::vec3 m_LateralMotion;
	glm::mat4 m_View;

	void InitAngles();
	void Orientate();
	void UpdateOrientation();
};

