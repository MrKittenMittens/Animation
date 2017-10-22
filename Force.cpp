#include "Force.h"

glm::vec3 Gravity::apply(float mass, const glm::vec3 & pos, const glm::vec3 & val)
{
	
	return glm::vec3(0.0f, -9.8f, 0.0f) /mass;
}

glm::vec3 Drag::apply(float mass, const glm::vec3 & pos, const glm::vec3 & val)
{
	return glm::vec3(val.x*0.95f, val.y * 0.95f, val.z * 0.95f);
}

glm::vec3 Force::apply(float mass, const glm::vec3 & pos, const glm::vec3 & val)
{
	return glm::vec3();
}

glm::vec3 Hook::apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel)
{

	glm::vec3 fd = -(m_kd) * vel;
	int test = fd.y;
	printf(test + "");
	glm::vec3 fn = fd;
	glm::vec3 a = fn;
	return a;
}
