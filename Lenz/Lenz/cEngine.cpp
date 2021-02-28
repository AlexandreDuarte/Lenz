
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cEngine.h"

Engine::Engine() {

    start = true;
    rotation = 0;
    v_rotation = 90;
    b_rotation = false;
    active_draw = -1;

    create_points();


    glViewport(0, 0, 2000, 2000);

    glEnable(GL_DEBUG_OUTPUT);

    glEnable(GL_DEPTH_TEST);
    /*glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

    glEnable(GL_MULTISAMPLE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Engine::~Engine() {
    delete[] VAO;
    delete[] VBO;
    delete[] EBO;
    delete camera;
    for (auto p : lenses)
    {
        delete p;
    }
    lenses.clear();
}

void Engine::init_shader() {
    Shader s("shaders/shader.vs", "shaders/shader.fs", "shaders/shader.gs");

    camera = new Camera(s.ID, glm::dvec3(2.0, -2.0, 2.0), glm::dvec3(0.0, 0.0, 0.0), 1.0);

}

void Engine::push_to_render() {
    for (int render_id = 0; render_id < lenses.size(); render_id++) {
        unsigned int* l_VAO = (VAO + render_id);
        unsigned int* l_VBO = (VBO + render_id);
        unsigned int* l_EBO = (EBO + render_id);


        glGenVertexArrays(1, l_VAO);
        glGenBuffers(1, l_VBO);
        glGenBuffers(1, l_EBO);


        glBindVertexArray(*l_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, *l_VBO);
        glBufferData(GL_ARRAY_BUFFER, (lenses.at(render_id))->output->size * sizeof(double), (lenses.at(render_id))->output->p_array, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *l_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (lenses.at(render_id))->output->size * sizeof(unsigned int), (lenses.at(render_id))->output->ind, GL_STATIC_DRAW);

        glVertexAttribLPointer(0, 3, GL_DOUBLE, 4 * sizeof(double), (void*)(0));
        glEnableVertexAttribArray(0);


        glVertexAttribLPointer(1, 1, GL_DOUBLE, 4 * sizeof(double), (void*)(3 * sizeof(double)));
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }
}

void Engine::render(double* delta_time, double zoom) {
    glBindVertexArray(0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if (b_rotation)
    camera->updateRotation(rotation, v_rotation);

    camera->updateZoom(zoom);


    for (int render_id = 0; render_id < lenses.size(); render_id++) {

        
        unsigned int* l_VAO = (VAO + render_id);
        unsigned int* l_VBO = (VBO + render_id);
        unsigned int* l_EBO = (EBO + render_id);

        glBindVertexArray(*l_VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *l_EBO);

        glDrawElements(GL_TRIANGLE_STRIP, 42*40, GL_UNSIGNED_INT, (void*)(0));

        /*for (int r = 0; r < 20; r++) {
            glDrawElements(GL_TRIANGLE_STRIP, 21, GL_UNSIGNED_INT, (void*)(42 * 4 * (2 * r)));
        }


        for (int r = 0; r < 20; r++) {
            glDrawElements(GL_TRIANGLE_STRIP, 21, GL_UNSIGNED_INT, (void*)(42 * 4 * (2 * r) + 21 * 4));
        }


        for (int r = 0; r < 20; r++) {
            glDrawElements(GL_TRIANGLE_STRIP, 21, GL_UNSIGNED_INT, (void*)(42*4*(2*r + 1)));
        }

        for (int r = 0; r < 20; r++) {
            glDrawElements(GL_TRIANGLE_STRIP, 21, GL_UNSIGNED_INT, (void*)(42 * 4 * (2 * r + 1) + 21 * 4));
        }*/
        glBindVertexArray(0);
    }

}

void Engine::create_points() {


    Lenz* l3 = new Biconvex();

    l3->initializeLenz(-0.3, 0.2, 0.15, 1, 0.03, 1.5);

    l3->createLenzPoints();

    lenses.push_back(l3);


    Lenz* l2 = new ConcaveConvex();

    l2->initializeLenz(-0.15, -0.2, -0.1, 1, 0.05, 1.5);

    l2->createLenzPoints();

    lenses.push_back(l2);
    
    Lenz* l = new ConcaveConvex();

    l->initializeLenz(0.25, 0.35, 0.15, 1, 0.02, 1.5);

    l->createLenzPoints();

    lenses.push_back(l);


    Lenz* l1 = new Biconvex();

    l1->initializeLenz(0, 0.35, 0.15, 1, 0.1, 1.5);

    l1->createLenzPoints();

    lenses.push_back(l1);



    VAO = new unsigned int[lenses.size()];
    VBO = new unsigned int[lenses.size()];
    EBO = new unsigned int[lenses.size()];

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

