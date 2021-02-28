#pragma once

#include"shader_s.h"
#include<vector>
#include<complex>
#include"camera.h"
#include "cLenz.h"

class Engine
{
public:
	


public:
	Engine();
	~Engine();

public:
	Camera* camera;
	bool start;
	double rotation;
	double v_rotation;
	bool b_rotation;
	unsigned int *VBO, *VAO, *EBO;
	int active_draw;

	std::vector<Lenz*> lenses;

public:
	void init_shader();
	void render(double*, double);

public:
	void create_points();
	void push_to_render();
	//void generate_points(Engine::points_array* ,int, int, int);
	void processInput(GLFWwindow* window, double*);
};

