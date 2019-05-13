#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main()
{    
    // FragColor = vec4(0.5, 0.5, 1.0, 1.0);
    FragColor = texture(tex, TexCoords);
}