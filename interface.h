#pragma once

#include "common.h"
#include "model.h"
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

using namespace glm;

GLfloat i_vertices[] = {
    0.005f, 0.005f, -0.1f, 1.0f, 0.0f,
    0.005f, -0.005f, -0.1f, 1.0f, 1.0f, 
    -0.005f, -0.005f, -0.1f, 0.0f, 1.0f,
    -0.005f, 0.005f, -0.1f, 0.0f, 0.0f,
};

GLint i_indices [] = {
    0, 1, 2,
    2, 3, 0
};

class Interface {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int aim_texture;

    void initialization() 
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(i_vertices), i_vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i_indices), i_indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    void upload_texture(unsigned int & tex, const char *path)
    {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        int width, height;
        unsigned char *image;
        image = SOIL_load_image(path, &width, &height, nullptr, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void aim_draw()
    {
        glBindTexture(GL_TEXTURE_2D, aim_texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


public:
    Model bullet;
    vec3 Pos_bullet = vec3(0.0f);
    vec3 Direction = vec3(0.0f);
    GLfloat bull_scale;
    GLint health = 50;
    GLint score = 0;

    Interface()
    {
        initialization();
        upload_texture(aim_texture, "../objects/aim.png");
        bull_scale = 0.01f;
        bullet.rad *= bull_scale;
        // cout<<"bull_gm "<<bullet.rad<<endl;
    }

    ~Interface() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw(ShaderProgram shader, ShaderProgram shader_bullet, GLfloat x, GLfloat y, GLfloat delta)
    {
        bullet_draw(shader_bullet, delta);

        shader.StartUseShader();
        mat4 model(1.0);
        model = translate(model, vec3(x, y, 0.0f));
        shader.SetUniform("model", model);

        mat4 view(1.0);
        view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
        shader.SetUniform("view", view);

        mat4 projection(1.0);
		projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 150.0f);
		shader.SetUniform("projection", projection);

        glBindVertexArray(VAO);
        aim_draw();
        glBindVertexArray(0);
        shader.StopUseShader();
    }

    void shot(vec3 dir)
    {
        Pos_bullet = vec3(0.0f);
        Direction = normalize(dir);
    }

    void bullet_draw(ShaderProgram bullet_shader, GLfloat delta)
    {
        Pos_bullet += 100.0f * delta * Direction;
        bullet_shader.StartUseShader();
        mat4 model(1.0);
        model = translate(model, Pos_bullet);
        model = scale(model, vec3(bull_scale));
        mat4 view(1.0);
        view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
        mat4 projection(1.0);
        projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 200.0f);
        bullet_shader.SetUniform("model", model);
        bullet_shader.SetUniform("view", view);
        bullet_shader.SetUniform("projection", projection);
        bullet.Draw(bullet_shader);
        bullet_shader.StopUseShader();
    }


};