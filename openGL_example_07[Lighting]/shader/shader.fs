#version 460 core

in vec3     Normal;
in vec2     TexCoord;
in vec3     Position;

out vec4    fragColor;

struct Light {
    vec3    position;
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
};

struct Material {
    vec3    ambient;
    vec3    diffuse;
    vec3    specular;
    float   shininess;
};

uniform vec3        viewPos;
uniform Light       light;
uniform Material    material;

void    main()
{
    vec3    ambient = material.ambient * light.ambient;

    vec3    lightDir = normalize(light.position - Position);
    vec3    pixelNorm = normalize(Normal);
    float   diff = max(dot(pixelNorm, lightDir), 0.0);
    vec3    diffuse = diff * material.diffuse * light.diffuse;

    vec3    viewDir = normalize(viewPos - Position);
    vec3    reflectDir = reflect(-lightDir, pixelNorm);
    float   spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3    specular = spec * material.specular * light.specular;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}