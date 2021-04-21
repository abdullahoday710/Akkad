#VERTEX_SHADER

#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

layout (std140) uniform shader_props {
    mat4 projection;
    vec3 text_color;
};

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}

#FRAGMENT_SHADER

#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D text;
layout (std140) uniform shader_props {
    mat4 projection;
    vec3 text_color;
};

void main()
{   
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(text_color, 1.0) * sampled;
}  