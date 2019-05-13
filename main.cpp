//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "model.h"
#include "background.h"
#include "enemy.h"
#include "asteroid.h"
#include "interface.h"

//External dependencies
#include <GLFW/glfw3.h>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool END = false;
bool SHOT = false;
GLfloat resol = (float)WIDTH / (float)HEIGHT;

GLfloat lastX =  WIDTH  / 2.0;
GLfloat lastY =  HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
GLfloat Yaw = -90.0f;
GLfloat Pitch = 0.0f;
vec2 mouse_pos =  vec2(0.0f, 0.0f);

int initGL();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

  GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);   
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "../shaders/enemy/vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/enemy/fragment.glsl";
	ShaderProgram enemy_shader(shaders); GL_CHECK_ERRORS;
	shaders[GL_VERTEX_SHADER]   = "../shaders/bullet/vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/bullet/fragment.glsl";
	ShaderProgram bullet_shader(shaders); GL_CHECK_ERRORS;
	shaders[GL_VERTEX_SHADER]   = "../shaders/interface/vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/interface/fragment.glsl";
	ShaderProgram int_shader(shaders); GL_CHECK_ERRORS;
	shaders[GL_VERTEX_SHADER]   = "../shaders/background/vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/background/fragment.glsl";
	ShaderProgram back_shader(shaders); GL_CHECK_ERRORS;
	shaders[GL_VERTEX_SHADER]   = "../shaders/asteroid/vertex.glsl";
	shaders[GL_GEOMETRY_SHADER] = "../shaders/asteroid/geometry.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/asteroid/fragment.glsl";
	ShaderProgram asteroid_shader(shaders); GL_CHECK_ERRORS;

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1); // force 60 frames per second
	Background background;
	Interface interface;
	vector<Enemy> ships;
	ships.push_back(Enemy(std::string("../objects/Aircraft/Aircraft.obj"), glm::vec3(5.0, -1.0, -150.0), bullet_shader, 180.0));
	ships.push_back(Enemy(std::string("../objects/dron/Drone_obj.obj"), glm::vec3(0.0, 0.0, -180.0), bullet_shader));
  ships.push_back(Enemy(std::string("../objects/ship/ship.obj"), glm::vec3(-5.0, 5.0, -170.0), bullet_shader));
	vector<Asteroid> asteroids;
	asteroids.push_back(Asteroid(std::string("../objects/rock/rock_by_dommk.obj"), glm::vec3(-10.0, 10.0, -120.0)));
	asteroids.push_back(Asteroid(std::string("../objects/rock/rock_by_dommk.obj"), glm::vec3(10.0, 10.0, -100.0)));
	asteroids.push_back(Asteroid(std::string("../objects/rock/rock_by_dommk.obj"), glm::vec3(10.0, -10.0, -150.0)));
	asteroids.push_back(Asteroid(std::string("../objects/rock/rock_by_dommk.obj"), glm::vec3(-10.0, -10.0, -200.0)));
	asteroids.push_back(Asteroid(std::string("../objects/rock2/1/Rock 01.obj"), glm::vec3(5.0, 5.0, -170.0)));
	asteroids.push_back(Asteroid(std::string("../objects/rock2/1/Rock 01.obj"), glm::vec3(5.0, -5.0, -120.0)));
	asteroids.push_back(Asteroid(std::string("../objects/rock2/1/Rock 01.obj"), glm::vec3(-5.0, -5.0, -130.0)));
	GL_CHECK_ERRORS

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL_CHECK_ERRORS

	//цикл обработки сообщений и отрисовки сцены каждый кадр
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// вычисление delta
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// очищаем экран каждый кадр
		//
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;

		// отрисовка кораблей и астероидов
		//	
		background.draw(back_shader, deltaTime);
		for(auto T = ships.begin(); T != ships.end(); T++) 
		{
			T->draw(enemy_shader, bullet_shader, deltaTime);
			if (!T->DEATH && (length(T->Pos - interface.Pos_bullet) <= (T->rad + interface.bullet.rad))) 
			{
				T->death();
			}
		} 
		for(auto T = asteroids.begin(); T != asteroids.end(); T++)
		{
			T->draw(asteroid_shader, currentFrame, deltaTime);
			if (!T->DAMAGE && (length(T->Pos - interface.Pos_bullet) <= (T->rad + interface.bullet.rad)))
			{
				T->damage(currentFrame);
			}
		} 

		if (SHOT) 
		{
			SHOT = false;
			interface.shot(Front);
		}
		interface.draw(int_shader, bullet_shader, 0.1 * Front.x / -Front.z, 0.1 * Front.y / -Front.z, deltaTime);
		glfwSwapBuffers(window); 

		if (END) break;
	}

	glfwTerminate();
	return 0;
}

int initGL()
{
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;

		// mouse_pos = vec2(xoffset/(0.5 * (float)WIDTH), yoffset/(0.5 * (float)HEIGHT));

		GLfloat mouse_sen = 0.05f;

		xoffset *= mouse_sen;
		yoffset *= mouse_sen;

    Yaw += xoffset;
    Pitch += yoffset;

		if (Pitch > 22.5f)
				Pitch = 22.5f;
		if (Pitch < -22.5f)
				Pitch = -22.5f;
		
		GLfloat cn = -90.0f;
		GLfloat of = (45.0f / 2) * resol;

		if (Yaw < cn - of)
				Yaw = cn - of;
		if (Yaw > cn + of)
				Yaw = cn + of;

		vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		// mouse_pos += vec2(xoffset/((float)WIDTH / (float)HEIGHT), yoffset/(0.5 * HEIGHT));
		// std::cout << Front.x << " " << Front.y << " " << Front.z << std::endl;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) SHOT = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	END = true;
}