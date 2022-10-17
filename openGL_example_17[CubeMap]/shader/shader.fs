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
    vec3    texColor = texture2D(material.diffuse, TexCoord).rgb;
    vec3    ambient = texColor * light.ambient;

    float   dist = length(light.position - Position);
    vec3    distPoly = vec3(1.0, dist, dist*dist);
    float   attenuation = 1.0 / dot(distPoly, light.attenuation);
    vec3    lightDir = (light.position - Position) / dist;

    vec3    result = ambient;

    float   theta = dot(lightDir, normalize(-light.direction));
    float   intensity = clamp((theta - light.cutoff[1]) / (light.cutoff[0] - light.cutoff[1]),
                                0.0, 1.0);
    
    if (intensity > 0.0)
    {
        vec3    pixelNorm = normalize(Normal);
        float   diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3    diffuse = diff * texColor * light.diffuse;

        vec3    specColor = texture2D(material.specular, TexCoord).rgb;
        vec3    viewDir = normalize(viewPos - Position);
        vec3    reflectDir = reflect(-lightDir, pixelNorm);
        float   spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3    specular = spec * specColor * light.specular;

        result = (diffuse + specular) * intensity;
    }
    result *= attenuation;
    fragColor = vec4(result, 1.0);
}