#VERTEX_SHADER

#version 330 core 

layout (location = 0) in vec3 position;

uniform mat4 transform;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * transform * vec4(position, 1.0);
}


#FRAGMENT_SHADER

#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0f);
}