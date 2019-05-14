#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int id;
uniform int health;
uniform int score;
uniform sampler2D tex;

void main()
{
    vec2 coords = TexCoords;

    if (id == 1)
    {
        coords.x *= 0.1;
        coords.x += 0.1 * ((score / 100) % 10);
    }
    else if (id == 2)
    {
        coords.x *= 0.1;
        coords.x += 0.1 * ((score / 10) % 10);
    }
    else if (id == 3)
    {
        coords.x *= 0.1;
        coords.x += 0.1 * (score % 10);
    }
    else if (id == 4)
    {
        coords.x *= 0.1;
        coords.x += 0.1 * ((health / 10) % 10);
    }
    else if (id == 5)
    {
        coords.x *= 0.1;
        coords.x += 0.1 * (health % 10);
    }

    FragColor = texture(tex, coords);
}