#pragma once

#include "common.h"
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

using namespace glm;

GLfloat b_vertices[] = {
    0.5f, 0.5f, -1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -1.0f, 1.0f, 1.0f, 
    -0.5f, -0.5f, -1.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
};

GLuint b_indices[] = {
    0, 1, 2,
    2, 3, 0
};

class Background {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int background_texture;

    void initialization() 
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(b_vertices), b_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(b_indices), b_indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void upload_texture()
    {
        glGenTextures(1, &background_texture);
        glBindTexture(GL_TEXTURE_2D, background_texture);
        int width, height;
        unsigned char *image;
        image = SOIL_load_image("../objects/Space.jpg", &width, &height, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

public:
    GLfloat offset;

    Background()
    {
        initialization();
        upload_texture();
        offset = 0.0f;
    }

    void draw(ShaderProgram shader, GLfloat delta)
    {
        shader.StartUseShader();
        mat4 model(1.0);
        model = scale(model, vec3((float)WIDTH / (float)HEIGHT, 1.0, 1.0));
        shader.SetUniform("model", model);
        mat4 view(1.0);
        view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
        shader.SetUniform("view", view);
        mat4 projection(1.0);
		projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 150.0f);
		shader.SetUniform("projection", projection);
        if (offset > 50.0) offset = 0.0f;
        offset += 6.0 * delta;
        shader.SetUniform("offset", offset);
        // shader.SetUniform("width", WIDTH);
        // shader.SetUniform("height", HEIGHT);

        glBindVertexArray(VAO);
        glDepthMask(GL_FALSE);
        glBindTexture(GL_TEXTURE_2D, background_texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glBindVertexArray(0);

        shader.StopUseShader();
    }

    ~Background() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};
