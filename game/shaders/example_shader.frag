#version 330 core

out vec4 FragColor;

uniform vec4 color;

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D heightTexture;
uniform sampler2D roughnessTexture;
uniform sampler2D metallicTexture;
uniform sampler2D aoTexture;

in vec2 texCoord;
in vec3 faceNormal;
in vec3 globalLight;
in vec3 globalPos;

void main (void)
{
    //float mod = dot(normalize(normal), normalize(globalLight)) / (length(normal) * length(globalLight));
    vec4 albedo = texture(albedoTexture, texCoord)*color;
    float metallic = texture(metallicTexture, texCoord).r;
    float roughness = texture(roughnessTexture, texCoord).r;
    vec3 normal = faceNormal;//texture(normalTexture, texCoord).rgb;
    //normal = normalize(normal * 2.0 - 1.0);

    FragColor = vec4(albedo.rgb * dot(globalLight, normal) / length(globalLight) / length(normalw), 1.0);
}
