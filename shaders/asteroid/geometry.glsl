#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords;

uniform float current_time;
uniform float damage_time;
uniform bool DAMAGE;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 11.0;
    vec3 direction;
    if (DAMAGE) {
        if (current_time - damage_time <= 2.0f) 
            direction = normal * (current_time - damage_time) * magnitude;
        else
            direction = normal * 2.0f *  magnitude;
    } else {
        direction = vec3(0.0, 0.0, 0.0);
    }
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}