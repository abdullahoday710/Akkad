#VERTEX_SHADER

#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord;

//out vec2 TexCoord;

//uniform mat4 transform;
//uniform mat4 viewProjection;

void main()
{
   // gl_Position = viewProjection * transform * vec4(position, 1.0);
    //TexCoord = aTexCoord;
    gl_Position = vec4(position, 1.0);
}


#FRAGMENT_SHADER

#version 330 core
//in vec2 TexCoord;
out vec4 FragColor;

layout (std140) uniform buffy
{
                     // base alignment  // aligned offset
    float value;     // 4               // 0 
    vec3 vector;     // 16              // 16  (offset must be multiple of 16 so 4->16)
    mat4 matrix;     // 16              // 32  (column 0)
                     // 16              // 48  (column 1)
                     // 16              // 64  (column 2)
                     // 16              // 80  (column 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
}; 

//uniform sampler2D ourTexture;



void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(value, 0.0, 0.0, 1.0);

}