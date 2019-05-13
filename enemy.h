#pragma once

#include "model.h"
#include "common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

class Enemy: public Model
{
public:
    vec3 Pos;
    vec3 Direction;
    GLfloat speed;
    vec3 StartPos;
    GLfloat rot;
    GLfloat enemy_scale;
    Model bullet;
    vec3 Pos_bullet;
    vec3 Direction_bullet;
    GLfloat bull_scale;
    bool DEATH = false;
    
	Enemy(std::string path, vec3 pos, ShaderProgram shader, GLfloat r = 0.0): Model(path), rot(r)
    {
        Pos_bullet = vec3(20.0);
        StartPos = pos;
        Pos = StartPos;
        Direction = normalize(vec3(0.0, 0.0, 1.0));
        enemy_scale = 1.0f;
        speed = 6.0f;
        bull_scale = 0.01f;
        bullet.rad *= bull_scale;
        // cout<<"enemy "<<rad<<endl;
        // cout<<"bull_en "<<bullet.rad<<endl;
    }

    void mov(GLfloat delta)
    {
        Pos += speed * delta * Direction;
        if (DEATH) 
        {
            if (enemy_scale > 0.05) enemy_scale *= 0.9;
        }
        if (Pos.z > 0.0)
        {
            DEATH = false;
            Pos = StartPos;
            Direction = normalize(vec3(0.0, 0.0, 1.0));
            enemy_scale = 1.0f;
            speed = 6.0f;
        }
    }

    void draw(ShaderProgram shader, ShaderProgram bullet_shader, GLfloat delta)
    {
        bullet_draw(bullet_shader, delta);
        mov(delta);
        shader.StartUseShader();
		mat4 model(1.0);
        model = translate(model, Pos);
        model = scale(model, vec3(enemy_scale));
        model = rotate(model, radians(rot), vec3(0.0, 1.0, 0.0));
		mat4 view(1.0);
        view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
		mat4 projection(1.0);
		projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 150.0f);
		shader.SetUniform("model", model);
		shader.SetUniform("view", view);
		shader.SetUniform("projection", projection);
        shader.SetUniform("DEATH", DEATH);
        this->Draw(shader);
        shader.StopUseShader();
    }

    void bullet_draw(ShaderProgram bullet_shader, GLfloat delta)
    {
        if ((Pos.z >= -50.0) && !DEATH)
        {
            if ((int) Pos.z % 20 == 0)
            {
                Pos_bullet = Pos;
                Direction_bullet = normalize(-Pos);
            }
        }
        if (Pos.z <= 0.0)
        {
            Pos_bullet += 20.0f * delta * Direction_bullet;
            bullet_shader.StartUseShader();
            mat4 model(1.0);
            model = translate(model, Pos_bullet);
            model = scale(model, vec3(bull_scale));
            mat4 view(1.0);
            view = lookAt(vec3(0.0), vec3(0.0, 0.0, -1.0), vec3(0.0, 1.0, 0.0));
            mat4 projection(1.0);
            projection = perspective(radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 150.0f);
            bullet_shader.SetUniform("model", model);
            bullet_shader.SetUniform("view", view);
            bullet_shader.SetUniform("projection", projection);
            bullet.Draw(bullet_shader);
            bullet_shader.StopUseShader();
        }
    }

    void death() {
        DEATH = true;
        speed = 40.0f;
        Direction = normalize(vec3(1.0, -1.0, 0.0) - Pos);
    }
};