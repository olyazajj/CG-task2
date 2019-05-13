#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform bool DEATH;
uniform sampler2D texture_diffuse1;

void main()
{    
    if (DEATH) 
    {
        FragColor = vec4(1.0, 1.0, 1.0, 0.7);
    } 
    else
    {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
}