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
    vec3 color_tint;
    float tintColorIntensity;
};

void main()
{
    gl_Position = sys_viewProjection * sys_transform * vec4(position, 1.0);
    TexCoord = aTexCoord;
}

#FRAGMENT_SHADER

#version 400

layout (std140) uniform shader_props {
    vec3 color_tint;
    float tintColorIntensity;
};

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    if (color_tint.x == 0 && color_tint.y == 0 && color_tint.z == 0)
    {
        FragColor = texture(Texture, TexCoord);
    }
    else
    {
        FragColor = mix(texture(Texture, TexCoord), vec4(color_tint, 1.0), tintColorIntensity);
    }
}