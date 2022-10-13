#version 460 core

in vec3     Normal;
in vec2     TexCoord;
in vec3     Position;

out vec4    fragColor;

struct Light {
    vec3    position;
    vec3    direction;
    vec2    cutoff;
    vec3    attenuation;
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
};

struct Material {
    sampler2D   diffuse;
    sampler2D   specular;
    float       shininess;
};

uniform vec3        viewPos;
uniform Light       light;
uniform Material    material;

void    main()
{
    fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}