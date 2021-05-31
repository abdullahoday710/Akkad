#VERTEX_SHADER

#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

layout (std140) uniform sys_SceneProps {
    mat4 sys_transform;
    mat4 sys_viewProjection;
};

layout (std140) uniform shader_props {
    vec3 tint_color;
    uint has_tint_color;
};

void main()
{
    sys_transform * 1.0f;
    TexCoord = aTexCoord;
    gl_Position = sys_viewProjection * vec4(position, 1.0);
}

#FRAGMENT_SHADER

#version 400
uniform sampler2D rect_texture;
in vec2 TexCoord;
out vec4 FragColor;

layout (std140) uniform shader_props {
    vec3 tint_color;
    uint has_tint_color;
};

void main()
{
    if(has_tint_color == 1)
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(rect_texture, TexCoord).r);
        FragColor = vec4(tint_color, 1.0) * sampled;
    }

    else
    {
        FragColor = texture(rect_texture, TexCoord);
    }

}