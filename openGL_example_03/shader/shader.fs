#version 460 core

in vec3     vertexColor;
in vec2     TexCoord;

out vec4    fragColor;

uniform sampler2D   tex1;
uniform sampler2D   tex2;

void    main()
{
    fragColor = mix(texture(tex2, TexCoord), texture(tex1, TexCoord), 0.4);
}