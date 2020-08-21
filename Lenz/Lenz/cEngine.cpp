
#include <future>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"
#include <intrin.h>
#include <limits>

Engine::Engine() {

    start = true;
    rotation = 0;
    v_rotation = 90;
    b_rotation = false;
    active_draw = -1;

    create_points(1000, 0, 1);


    glViewport(0, 0, 2000, 2000);

    glEnable(GL_DEBUG_OUTPUT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);
}

Engine::~Engine() {

}

void Engine::init_shader() {
    Shader s("shaders/shader.vs", "shaders/shader.fs");

    camera = new Camera(s.ID, glm::dvec3(2.0, -2.0, 2.0), glm::dvec3(0.0, 0.0, 0.0), 1.0);

}

void Engine::render(double* delta_time, double zoom) {
    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if (b_rotation)
    camera->updateRotation(rotation, v_rotation);

    camera->updateZoom(zoom);



    if (start) {
        std::cout << start << std::endl;

        unsigned int* ind = new unsigned int[p_arrays.at(0)->size / 3 * 2];
        int size = 0;
            for (unsigned int k = 0; k < 21*2; k++) {
                for (unsigned int i = 0; i < 21; i++) {

                    *(ind + size) = i + 0 + (k) * 21;

                    *(ind + size + 1) = i + (k + 1) * 21;


                    size+=2;
                }
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);


        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (p_arrays.at(0))->size * sizeof(double), (*p_arrays.at(0)).p_array, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_arrays.at(0)->size/3 * 2 * sizeof(unsigned int), ind, GL_STATIC_DRAW);

        glVertexAttribLPointer(0, 3, GL_DOUBLE, 3 * sizeof(double), (void*)(0));

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        active_draw = 0;
        start = false;
    }

    if (active_draw != -1) {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //glDrawArrays(GL_LINE_STRIP, 0, (*p_arrays.at(0)).size/3);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        //glDrawElements(GL_TRIANGLE_STRIP, p_arrays.at(0)->size/3 * 5, GL_UNSIGNED_INT, (void*)0);

        //for (int i = 0; i < 21 * sizeof(unsigned int) - 2; i++) {


        //TODO use geometry shader to archieve separation of diferente slices of the lens face
        for (int i = 0; i < 20; i++) {
            glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_INT, (void*)(i*sizeof(unsigned int)*42));
        }
        for (int i = 21; i < 10 * sizeof(unsigned int) + 1; i++) {
            glDrawElements(GL_TRIANGLE_STRIP, 42, GL_UNSIGNED_INT, (void*)(i * sizeof(unsigned int) * 42));
        }

        glBindVertexArray(0);
    }
}

void Engine::create_points(double resolution, double offset, double step) {
    
    
    Lenz* l = new Lenz(0.15, 0.15, 1, 0.1, 1.5);


    Lenz::points_array* a = new Lenz::points_array();
    l->createLenzPoints(a);

    p_arrays.push_back(a);

}

void Engine::processInput(GLFWwindow* window, double* delta_time)
{


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (rotation + 100 * *delta_time >= 360)
            rotation -= 360;
        rotation += 100 * *delta_time;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (rotation - 100 * *delta_time <= 0)
            rotation += 360;
        rotation -= 100 * *delta_time;
        b_rotation = true;
    }

    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        b_rotation = false;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (v_rotation + 100 * *delta_time >= 90)
            v_rotation = 90;
        else v_rotation += 100 * *delta_time;
        b_rotation = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        b_rotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (v_rotation - 100 * *delta_time <= -90)
            v_rotation = -90;
        else v_rotation -= 100 * *delta_time;
        b_rotation = true;
    }

    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        b_rotation = false;
    }
}

