#VERTEX_SHADER

#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;

layout (std140) uniform sys_SceneProps {
    mat4 sys_transform;
    mat4 sys_viewProjection;
};

layout (std140) uniform shader_props {
    vec3 props_color;
};

void main()
{
    sys_transform * 1.0f;
    gl_Position = sys_viewProjection * vec4(position, 1.0);
}

#FRAGMENT_SHADER

#version 400

layout (std140) uniform shader_props {
    vec3 props_color;
};

out vec3 FragColor;

void main()
{
    FragColor = props_color;
}