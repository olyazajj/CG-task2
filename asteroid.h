#pragma once

#include "model.h"
#include "common.h"

using namespace glm;

class Asteroid: public Model
{
public:
    vec3 Pos;
    vec3 Direction;
    vec3 StartPos;
    GLfloat speed;
    GLfloat ast_scale;
    bool DAMAGE = false;
    GLfloat damage_time;

    Asteroid(std::string path, vec3 pos):Model(path)
    {
        StartPos = pos;
        Pos = StartPos;
        ast_scale = 0.5f;
        Direction = normalize(vec3(0.0, 0.0, 1.0));
        speed = 6.0f;
        rad *= ast_scale;
        // cout<<"ast "<<rad<<endl;
    }

    void mov(GLfloat delta)
    {
        Pos += speed * delta * Direction;
        if (DAMAGE) 
        {
            if (ast_scale > 0.025) ast_scale *= 0.99;
        }
        if (Pos.z > 0.0)
        {
            DAMAGE = false;
            Pos = StartPos;
            Direction = normalize(vec3(0.0, 0.0, 1.0));
            ast_scale = 0.5f;
            speed = 6.0f;
        }
    }

    void draw(ShaderProgram shader, GLfloat time, GLfloat delta)
    {
        mov(delta);
        shader.StartUseShader();
		mat4 model(1.0);
        model = translate(model, Pos);
        model = scale(model, vec3(ast_scale));
		mat4 view(1.0);
        view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
		mat4 projection(1.0);
		projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 150.0f);
		shader.SetUniform("model", model);
		shader.SetUniform("view", view);
		shader.SetUniform("projection", projection);
        shader.SetUniform("DAMAGE", DAMAGE);
        shader.SetUniform("current_time", time);
        shader.SetUniform("damage_time", damage_time);
        this->Draw(shader);
        shader.StopUseShader();
    }

    void damage(GLfloat time)
    {
        DAMAGE = true;
        damage_time = time;
    }
};