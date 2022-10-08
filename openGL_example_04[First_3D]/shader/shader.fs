#version 460 core

in vec2     TexCoord;

out vec4    fragColor;

uniform sampler2D   tex1;
uniform sampler2D   tex2;

void    main()
{
    fragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), 0.2);
}