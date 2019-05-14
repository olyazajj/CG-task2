#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float offset;
uniform vec3 Pos;
uniform sampler2D tex;

const int count = 14;
const float eps = 0.001;
const int max_steps = 30;
const float max_dist = 150.0;

vec3 Positions[count] = vec3[count](
vec3(-2.0, -1.0, -20.6),
vec3(-2.0, 0.0, -35.6),
vec3(-2.0, 1.0, -28.9),
vec3(-1.0, -1.0, -28.7),
vec3(-1.0, 0.0, -24.7),
vec3(-1.0, 1.0, -12.6),
vec3(0.0, -1.0, -42.2),
vec3(0.0, 1.0, -47.3),
vec3(1.0, -1.0, -23.2),
vec3(1.0, 0.0, -21.7),
vec3(1.0, 1.0, -47.2),
vec3(2.0, -1.0, -10.6),
vec3(2.0, 0.0, -26.1),
vec3(2.0, 1.0, -49.2)
);

float scene(vec3 point) 
{
    float min_dist = max_dist;
    float dist;
    for (int i = 0; i < count; ++i) 
    {
        dist = length(Positions[i] + vec3(0.0, 0.0, offset) - point) - 0.01;
        if (min_dist > dist) 
        {
            min_dist = dist;
        }
    }
    return min_dist;
}

float RayMarching(vec3 pos, vec3 dir) 
{
    float cur = eps;
    for (int i = 0; i < max_steps; i++) 
    {
        float left = scene(pos + cur * dir);
        if (left < eps) 
        {
            return cur;
        }
        cur += left;
        if (cur >= max_dist) 
        {
            return max_dist;
        }
    }
    return max_dist;
}

void main() 
{
    float dist = RayMarching(vec3(0.0, 0.0, 0.0), normalize(vec3(TexCoords.x - 0.5, TexCoords.y - 0.5, -1 / tan(45.0f / 2))));
    if (dist > max_dist - eps) 
    {
        FragColor = texture(tex, TexCoords);
    } 
    else 
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}