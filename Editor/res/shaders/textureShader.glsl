#VERTEX_SHADER

#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * transform * vec4(position, 1.0);
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