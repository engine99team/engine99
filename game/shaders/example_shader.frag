#version 330 core

out vec4 FragColor;

const vec3 Fdielectric = vec3(0.04);
const float PI = 3.141592;
const float Epsilon = 0.00001;

uniform vec4 color;
uniform vec3 cameraPos;

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

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha   = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
    return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}


void main (void)
{
    //float mod = dot(normalize(normal), normalize(globalLight)) / (length(normal) * length(globalLight));
    vec3 albedo = (texture(albedoTexture, texCoord)*color).rgb;
    float metallic = texture(metallicTexture, texCoord).r;
    float roughness = texture(roughnessTexture, texCoord).r;
    vec3 normal = faceNormal;//texture(normalTexture, texCoord).rgb;
    //normal = normalize(normal * 2.0 - 1.0);

    vec3 Lo = normalize(cameraPos - globalPos);
    float cosLo = max(0.0, dot(normal, Lo));
    vec3 Lr = 2.0 * cosLo * normal - Lo;
    vec3 F0 = mix(Fdielectric, albedo, metallic);

    vec3 directLighting = vec3(0);
    // foreach Light
    vec3 Li = -normalize(globalLight);
    vec3 Lradiance = vec3(1, 1, 1);
    vec3 Lh = normalize(Li + Lo);
    float cosLi = max(0.0, dot(normal, Li));
    float cosLh = max(0.0, dot(normal, Lh));

    vec3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
    float D = ndfGGX(cosLh, roughness);
    float G = gaSchlickGGX(cosLi, cosLo, roughness);

    vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metallic);
    vec3 diffuseBRDF = kd * albedo;
    vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
    /////
    // Ambient lighting (IBL).
    vec3 ambientLighting = vec3(1)*0.1;

    FragColor = vec4(directLighting + ambientLighting, 1.0);
}
