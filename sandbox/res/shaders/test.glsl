#VERTEX_SHADER

#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0);
    TexCoord = aTexCoord;
}


#FRAGMENT_SHADER

#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;



void main()
{
    FragColor = texture(ourTexture, TexCoord);
}