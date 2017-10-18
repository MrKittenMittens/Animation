#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>


// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"


// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Body.h"
#include "Particle.h"

// time
GLfloat frameTime = 0.0f;
GLfloat lastFrame = 0.0f;



// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));

	// create ground plane
	Mesh plane = Mesh::Mesh();


	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	//plane.translate(glm::vec3(0.0f, -5.0f, 0.0f));
	plane.setShader(Shader("resources/shaders/core.vert", "resources/shaders/core.frag"));

	//create cone
	Mesh cone = Mesh::Mesh();

	//initiate random seed
	srand(time(NULL));
	// create particle
	Particle particles[20];
	
	for (int i=0; i<20; i++)
	{
		particles[i] = Particle::Particle();
		particles[i].setVel(glm::vec3(rand() % 61 - 10, rand() % 50, rand() % 11 - 10));
		//particles[i].setVel(glm::vec3(0.0f, 0.0f, 0.0f));
		particles[i].setPos(glm::vec3(0.0f, 2.5f, 0.0f));
	}
	
	// time
	GLfloat firstTime = (GLfloat)glfwGetTime();
	glm::vec3 F;
	glm::vec3 Fg = glm::vec3(0.0f, -9.8f, 0.0f);
	glm::vec3 Fa = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Ff = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 a;
	float m = 0.05;
	
	//box dimensions
	glm::vec3 box = glm::vec3(-2.5f, 5.0f, -2.5f);
	glm::vec3 d = glm::vec3(5.0f, 5.0f, 5.0f);

	//cone dimensions
	glm::vec3 conePos = glm::vec3(0.0f, 0.0f, 0.0f);
	int coneHeight = 2;
	int radTop = 1.5;
	double radBot = .5;
	int radX = 0;
	double heightX = 0;
	int radPart = 0;
	double t = 0.0f;
	const double dt = 0.01f;
	double accumulator = 0.0f;

	/*
	*create a firstFrame
	*
	*/
	// GLfloat firstFrame = (GLfloat)glfwGetTime();



	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat newTime = (GLfloat)glfwGetTime() - firstTime;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.

		newTime *= .5f;


		//I think I understand what's happening here, getting how long it takes to go from one frame to the next
		frameTime = newTime - firstTime;
		firstTime = newTime;

		accumulator += frameTime;

		/*
		**  INTERACTION
		*/
		// Manage interaction
		app.doMovement(frameTime);


		/*
		**  SIMULATION
		*/
		while (accumulator >= dt)
		{
			// Simulate particle's motion

			// Calculate forces on particle to get acceleration
			//F = Fg / 10 + Fa + Ff;
			a = Fg / m;

			// Calculate new velocity and position
			/*
			*  new_vel = old_vel + h * a
			*  new_pos = old_pos + h * new_vel
			*  where h is the time step 
			*/
			//repeat for every particle


			
			for (int j = 0; j < 20; j++)
			{

				// Remember the Particle class has a velocity and position stored in it
				// so you don't need to be making new variables to store them


				//forward euler?
				particles[j].setVel(particles[j].getVel() + dt * a);
				particles[j].setPos(particles[j].getPos() + dt * particles[j].getVel());
				// Just use the setVel and setPos functions instead of this

				


				
				
					// Collision detection goes here
					//getTranslate()[3][1] gets the x position of part
					//If particle hits wall reverse x velocity and decrease by energyDegration
					if ((particles[j].getTranslate()[3][0] > (box.x + d.x)) || (particles[j].getTranslate()[3][0] < (box.x)))
					{
						if (particles[j].getVel().x <= 0)
							particles[j].setPos(glm::vec3(box.x, particles[j].getTranslate()[3][1], particles[j].getTranslate()[3][2]));
						else if (particles[j].getVel().x > 0)
							particles[j].setPos(glm::vec3(box.x + d.x, particles[j].getTranslate()[3][1], particles[j].getTranslate()[3][2]));
						particles[j].setVel(glm::vec3(-particles[j].getVel().x * 0.75, particles[j].getVel().y * 0.9, particles[j].getVel().z * 0.75));
	
					}
					//if particle hits wall reverse z velocity and decrease by energyDegration
					if ((particles[j].getTranslate()[3][2] > (box.z + d.z)) || (particles[j].getTranslate()[3][2] < (box.z)))
					{
						if (particles[j].getVel().z < 0)
							particles[j].setPos(glm::vec3(particles[j].getTranslate()[3][0], particles[j].getTranslate()[3][1], box.z));
						else if (particles[j].getVel().z > 0)
							particles[j].setPos(glm::vec3(particles[j].getTranslate()[3][0], particles[j].getTranslate()[3][1], box.z + d.z));
						particles[j].setVel(glm::vec3(particles[j].getVel().x * 0.75, particles[j].getVel().y * 0.9, -particles[j].getVel().z * 0.75));
	
					}
					if (particles[j].getTranslate()[3][1] < box.y - d.y)
					{
						particles[j].setPos(glm::vec3(particles[j].getTranslate()[3][0], 0.0f, particles[j].getTranslate()[3][2]));
						particles[j].setVel(glm::vec3(particles[j].getVel().x * 0.75, -particles[j].getVel().y * 0.9, particles[j].getVel().z * 0.75));
						if (particles[j].getVel().y <= box.y - d.y)
						{
							Fg = glm::vec3(0.0f, 0.0f, 0.0f);
							
							particles[j].setPos(glm::vec3(particles[j].getTranslate()[3][0], 0.0f, particles[j].getTranslate()[3][2]));
						}
					}

					//if particle enters cone increase vertical velocity
					//if particle is below threshold of cones coneHeight
					if (particles[j].getTranslate()[3][1] < conePos.y + coneHeight)
					{
						//printf("test" + radX);
						//find radius at height x
						radX = radBot + particles[j].getTranslate()[3][1] * ((radTop - radBot) / coneHeight);
						radPart = radBot + particles[j].getTranslate()[3][1] * ((particles[j].getTranslate()[3][1] - radBot) / coneHeight);
						

						//if particle is within the base radius
						if (particles[j].getTranslate()[3][0] < conePos.x + radBot && particles[j].getTranslate()[3][0] > conePos.x - radBot && particles[j].getTranslate()[3][2] < conePos.z + radBot && particles[j].getTranslate()[3][2] > conePos.z - radBot)
							particles[j].setVel(particles[j].getVel() + glm::vec3(0.0f, 2.0f, 0.0f));
						//If particle is within the radius of the cone at coneHeight X 
						else if (radPart < radX)
						{
							printf("rad" + radPart);
							printf("" + radX);
							particles[j].setVel(particles[j].getVel() + glm::vec3(0.0f, 2.0f, 0.0f));
						}

					}




				


			
			}
			

			// Update accumulator and time
			accumulator -= dt;
			t += dt;
		}

		//clear buffer
		app.clear();

		for each(Particle part in particles)
		{			
			// draw particles
			app.draw(part.getMesh(), Shader("resources/shaders/core.vert", "resources/shaders/core_green.frag"));
		}
		// draw groud plane
		app.draw(plane);

		app.display();

	}

	app.terminate();

	return EXIT_SUCCESS;
}