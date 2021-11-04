#VERTEX_SHADER

#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;

out vec3 color;
layout (std140) uniform sys_SceneProps {
    mat4 sys_transform;
    mat4 sys_viewProjection;
};

layout (std140) uniform shader_props {
    vec3 props_color;
};

void main()
{
    color = aColor;
    gl_Position = sys_viewProjection * vec4(position, 1.0);
}

#FRAGMENT_SHADER

#version 400

layout (std140) uniform shader_props {
    vec3 props_color;
};

in vec3 color;
out vec3 FragColor;

void main()
{
    FragColor = color;
}