#VERTEX_SHADER

#version 400

layout(std140) uniform sys_SceneProps
{
    mat4 sys_transform;
    mat4 sys_viewProjection;
};

layout(location = 0) in vec2 position;

void main()
{
    gl_Position = sys_viewProjection * vec4(position, 1.0, 1.0);
}

#FRAGMENT_SHADER

#version 400

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0);
}