#include "glew/include/glew.h"
#include "glfw/include/glfw3.h"
#include "setup.h"
#include "glFunctions.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

const char* vertexShader =
	"#version 330 \n"
	"layout(location = 0) in vec2 coord;"
	"layout(location = 1) in float brightness;"
	"uniform float aspect;"
	"uniform float dif;"
	"uniform float size;"
	"uniform vec2 dis;"
	"uniform vec3 color;"
	"out vec3 light;"
	"void main()"
	"{"
	"	float magnitude = abs(sqrt(coord.x*coord.x + coord.y*coord.y));"
	"	vec2 position = (coord * magnitude * dif * size) + dis;"
	"	gl_Position = vec4(position.x / aspect, position.y, 0.0, 1.0);"
	"	light = clamp((color * brightness / magnitude), 0.0, 1.0) - dif;"
	"}";

const char* fragmentShader =
	"#version 330 \n"
	"in vec3 light;"
	"out vec3 finalColor;"
	"void main()"
	"{"
	"	finalColor = light;"
	"}";


int main()
{
	if(!glfwInit())
		return -1;

	GLFWwindow* myWindow = NULL;
	setupFullscreen(myWindow);
	if(myWindow == NULL)
	{
		glfwTerminate();
		return -1;
	}

	

	GLuint simpleShader;
	createShader(simpleShader, vertexShader, fragmentShader);
	glUseProgram(simpleShader);

	const int PointNumber = 50000;
	GLfloat* points = new float[PointNumber * 3];

	srand(unsigned int(time(0)));

	for(int i = 0; i < PointNumber ; i ++)
	{
		float randFloat1 = float(rand())/float(RAND_MAX);
		float c = sqrtf(1 - randFloat1 * randFloat1);
		float randFloat2 = (float(rand())/float(RAND_MAX/c));

		int negPos = rand()%4;
		if(negPos == 1) randFloat1 *= -1;
		if(negPos == 2) randFloat2 *= -1;
		if(negPos == 3)
		{
			randFloat1 *= -1;
			randFloat2 *= -1;
		}
		
		points[i*3 + 0] = randFloat2;
		points[i*3 + 1] = randFloat1;
		points[i*3 + 2] = float(rand())/float(RAND_MAX);
	}


	GLuint tri;
	glGenVertexArrays(1, &tri);
	glBindVertexArray(tri);

	GLuint triBuffer;
	glGenBuffers(1, &triBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * PointNumber * 3, points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, 0, sizeof(GLfloat) * 3, 0);
	glVertexAttribPointer(1, 1, GL_FLOAT, 0, sizeof(GLfloat) * 3, (void*) (sizeof(GLfloat) * 2));
	delete[](points);


	struct Firework 
	{
		float difference;
		float size;
		float displacment[2];
		float colors[3];
	};


	Firework fire1;
	fire1.difference = 0.0;
	fire1.size = 2.0;
	fire1.displacment[0] = 0.0, fire1.displacment[1] = 0.0;
	fire1.colors[0] = 0.0, fire1.colors[1] = 1.0, fire1.colors[2] = 0.0;


	Firework fire2;
	fire2.difference = 0.0;
	fire2.size = 1.5;
	fire2.displacment[0] = 0.0, fire2.displacment[1] = 0.0;
	fire2.colors[0] = 0.0, fire2.colors[1] = 0.0, fire2.colors[2] = 1.0;

	Firework fire3;
	fire3.difference = 0.0;
	fire3.size = 1.0;
	fire3.displacment[0] = 0.0, fire3.displacment[1] = 0.0;
	fire3.colors[0] = 1.0, fire3.colors[1] = 0.0, fire3.colors[2] = 1.0;

	
	GLuint displacmentID = glGetUniformLocation(simpleShader, "dis");
	GLuint differenceID = glGetUniformLocation(simpleShader, "dif");
	GLuint colorID = glGetUniformLocation(simpleShader, "color");
	GLuint sizeID = glGetUniformLocation(simpleShader, "size");
	GLuint aspectID = glGetUniformLocation(simpleShader, "aspect");
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	float aspect = float(mode->width)/float(mode->height);
	glUniform1f(aspectID, aspect);

	double time = glfwGetTime();


	while (!glfwGetKey(myWindow, GLFW_KEY_ESCAPE))
	{
		double deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);


		fire1.difference += float(0.5 * deltaTime);
		fire2.difference += float(0.6 * deltaTime);
		fire3.difference += float(0.7 * deltaTime);

		if(fire1.difference > 1.2)
		{
			fire1.difference = 0;
			fire1.size = float(rand())/(float(RAND_MAX)/2.0f) + .5f;
			fire1.displacment[0] = float(rand())/float(RAND_MAX) - 0.5f;
			fire1.displacment[1] = float(rand())/float(RAND_MAX) - 0.5f;
			fire1.colors[0] = float(rand())/float(RAND_MAX);
			fire1.colors[1] = float(rand())/float(RAND_MAX);
			fire1.colors[1] = float(rand())/float(RAND_MAX);
		}

		glUniform1f(differenceID, fire1.difference);
		glUniform1f(sizeID, fire1.size);
		glUniform2f(displacmentID, fire1.displacment[0], fire1.displacment[1]);
		glUniform3f(colorID, fire1.colors[0], fire1.colors[1], fire1.colors[2]);

		glDrawArrays(GL_POINTS, 0, PointNumber);


		if(fire2.difference > 1.2)
		{
			fire2.difference = 0;
			fire2.size = float(rand())/(float(RAND_MAX)/1.5) + .5f;
			fire2.displacment[0] = float(rand())/float(RAND_MAX) - 0.5f;
			fire2.displacment[1] = float(rand())/float(RAND_MAX) - 0.5f;
			fire2.colors[0] = float(rand())/float(RAND_MAX);
			fire2.colors[1] = float(rand())/float(RAND_MAX);
			fire2.colors[1] = float(rand())/float(RAND_MAX);
		}

		glUniform1f(differenceID, fire2.difference);
		glUniform1f(sizeID, fire2.size);
		glUniform2f(displacmentID, fire2.displacment[0], fire2.displacment[1]);
		glUniform3f(colorID, fire2.colors[0], fire2.colors[1], fire2.colors[2]);

		glDrawArrays(GL_POINTS, 0, PointNumber);


		if(fire3.difference > 1.2)
		{
			fire3.difference = 0;
			fire3.size = float(rand())/float(RAND_MAX) + .5f;
			fire3.displacment[0] = float(rand())/float(RAND_MAX) - 0.5f;
			fire3.displacment[1] = float(rand())/float(RAND_MAX) - 0.5f;
			fire3.colors[0] = float(rand())/float(RAND_MAX);
			fire3.colors[1] = float(rand())/float(RAND_MAX);
			fire3.colors[1] = float(rand())/float(RAND_MAX);
		}

		glUniform1f(differenceID, fire3.difference);
		glUniform1f(sizeID, fire3.size);
		glUniform2f(displacmentID, fire3.displacment[0], fire3.displacment[1]);
		glUniform3f(colorID, fire3.colors[0], fire3.colors[1], fire3.colors[2]);

		glDrawArrays(GL_POINTS, 0, PointNumber);

		glfwSwapBuffers(myWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}