#pragma once
#include <glm/glm.hpp>
#include <iostream>

class Body; //forward declaration

class Force
{
public:
	Force() {}
	~Force() {}

	virtual glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &val);
};

//Gravity class
class Gravity : public Force
{
public:
	//constructors
	Gravity() {}
	Gravity(const glm::vec3 &gravity) { m_gravity = gravity; }

	//get and set methods
	glm::vec3 getGravity() const { return m_gravity; }
	void setGravity(glm::vec3 gravity) { m_gravity = gravity; }

	//physics
	glm::vec3  apply(float mass, const glm::vec3 &pos, const glm::vec3 &val);

private:
	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);
};

//Drag class
class Drag : public Force 
{
public:
	Drag() {}

	//physics
	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &val);

private:

};

class Hook : public Force 
{
public: 
	Hook() {}
	Hook(Body* b1, Body* b2, float ks, float kd, float rest)
	{
		m_ks = ks; m_kd = kd; m_rest = rest; m_b1 = b1; m_b2 = b2;
	}

	//gets and sets
	float getM_ks() const { return m_ks; }
	void setM_ks(float ks) { m_ks = ks; }

	float getM_kd() const { return m_kd; }
	void setM_kd(float kd) { m_kd = kd; }

	float getM_rest() const { return m_rest; }
	void setM_rest(float rest) { m_rest = rest; }

	Body* getB1() const { return m_b1; }
	Body* setB1(Body* body) { m_b1 = body; }

	Body* getB2() const { return m_b2; }
	Body* setB2(Body* body) { m_b2 = body; }

	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);

private:
	float m_ks; //spring stiffness
	float m_kd; //damping coefficient
	float m_rest; //spring rest length

	Body* m_b1; //pointer to the body connected to one extremety of the spring
	Body* m_b2; //pointer to the body connected to the other extremety
};