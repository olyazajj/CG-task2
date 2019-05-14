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
    0.1f, 0.1f, -0.1f, 1.0f, 0.0f,
    0.1f, -0.1f, -0.1f, 1.0f, 1.0f,
    -0.1f, -0.1f, -0.1f, 0.0f, 1.0f,
    -0.1f, 0.1f, -0.1f, 0.0f, 0.0f
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
    unsigned int health_texture;
    unsigned int score_texture;
    unsigned int digits_texture;
    vec2 aim_size;
    vec2 health_size;
    vec2 score_size;
    vec2 digits_size;

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

    void upload_texture(unsigned int & tex, const char *path, vec2 & size)
    {
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        int width, height;
        unsigned char *image;
        image = SOIL_load_image(path, &width, &height, nullptr, SOIL_LOAD_RGBA);
        size = vec2(width, height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void aim_draw(ShaderProgram shader, GLfloat x, GLfloat y)
    {
        mat4 model(1.0);
        mat4 view(1.0);
        mat4 projection(1.0);
        model = translate(model, vec3(x, y, 0.0f));
        model = scale(model, vec3(0.05f, 0.05f, 1.0f));
        view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
        projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 150.0f);
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("id", 0);
        glBindTexture(GL_TEXTURE_2D, aim_texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void words_draw(ShaderProgram shader)
    {
        mat4 model(1.0);
        mat4 view(1.0);
        mat4 projection(1.0);
        model = translate(model, vec3(-0.7, -0.7, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(health_size.x / health_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("id", 0);
        glBindTexture(GL_TEXTURE_2D, health_texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = mat4(1.0);
        view = mat4(1.0);
        projection = mat4(1.0);
        model = translate(model, vec3(0.7, -0.7, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(score_size.x / score_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("id", 0);
        glBindTexture(GL_TEXTURE_2D, score_texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void digits_draw(ShaderProgram shader)
    {
        mat4 model(1.0);
        mat4 view(1.0);
        mat4 projection(1.0);
        glBindTexture(GL_TEXTURE_2D, digits_texture);

        model = mat4(1.0);
        model = translate(model, vec3(0.65, -0.8, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(0.1 * digits_size.x / digits_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("health", health);
        shader.SetUniform("score", score);
        shader.SetUniform("id", 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = mat4(1.0);
        model = translate(model, vec3(0.7, -0.8, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(0.1 * digits_size.x / digits_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("health", health);
        shader.SetUniform("score", score);
        shader.SetUniform("id", 2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = mat4(1.0);
        model = translate(model, vec3(0.75, -0.8, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(0.1 * digits_size.x / digits_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("health", health);
        shader.SetUniform("score", score);
        shader.SetUniform("id", 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = mat4(1.0);
        model = translate(model, vec3(-0.725, -0.8, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(0.1 * digits_size.x / digits_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("health", health);
        shader.SetUniform("score", score);
        shader.SetUniform("id", 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        model = mat4(1.0);
        model = translate(model, vec3(-0.675, -0.8, 0.0));
        model = scale(model, vec3(0.3, 0.3, 1.0));
        model = scale(model, vec3(0.1 * digits_size.x / digits_size.y, 1.0, 1.0f));
        shader.SetUniform("model", model);
        shader.SetUniform("view", view);
        shader.SetUniform("projection", projection);
        shader.SetUniform("health", health);
        shader.SetUniform("score", score);
        shader.SetUniform("id", 5);
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
        upload_texture(aim_texture, "../objects/Aim.png", aim_size);
        upload_texture(health_texture, "../objects/Health.png", health_size);
        upload_texture(score_texture, "../objects/Score.png", score_size);
        upload_texture(digits_texture, "../objects/Digits.png", digits_size);
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
        glBindVertexArray(VAO);
        words_draw(shader);
        digits_draw(shader);
        aim_draw(shader, x, y);
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

    bool wound()
    {
        health -= 10;
        if (health <= 0)
        {
            return true;
        }
        return false;
    }

    void hit()
    {
        score += 10;
        Pos_bullet = vec3(0.0f);
        Direction = vec3(0.0f);
    }
};