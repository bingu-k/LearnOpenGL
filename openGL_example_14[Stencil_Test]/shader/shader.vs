#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

uniform mat4    transform;
uniform mat4    modelTransform;

out vec3    Normal;
out vec2    TexCoord;
out vec3    Position;

void    main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    Normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    TexCoord = aTexCoord;
    Position = (modelTransform * vec4(aPos, 1.0)).xyz;
}