#version 330 core

out vec4 FragColor;

#define MAX_POINT_LIGHTS 50
#define MAX_DIRECT_LIGHTS 10
#define MAX_SPOT_LIGHTS 20
#define MAX_LIGHTS 80

const vec3 Fdielectric = vec3(0.04);
const float PI = 3.141592;
const float Epsilon = 0.00001;

uniform vec4 color;
uniform vec3 cameraPos;

struct PointLight {
    vec3 pos;
    vec3 radiance;
};

struct DirectLight {
    vec3 direction;
    vec3 radiance;
};

struct SpotLight {
    vec3 pos;
    vec3 direction;
    vec3 radiance;
    float angle;
};

struct Light {
    vec3 direction;
    vec3 radiance;
};

uniform int point_lights_count;
uniform int direct_lights_count;
uniform int spot_lights_count;

uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform DirectLight direct_lights[MAX_DIRECT_LIGHTS];
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];

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
in mat3 tangentBasis;

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

Light lights[MAX_LIGHTS];

void main (void)
{
    int i, j;
    vec3 debugColor;
    j = 0;
    for (i = 0; i < point_lights_count; i++) {
        vec3 direction = globalPos - point_lights[i].pos;
        float d = length(direction);
        float F = 1.0 / (1.0 + d);
        vec3 radiance = point_lights[i].radiance * F;
        if (length(radiance) > 0.1) {
            lights[j].direction = normalize(direction);
            lights[j].radiance = radiance;
            j++;
        }
    }
    for (i = 0; i < direct_lights_count; i++) {
        lights[j].radiance = direct_lights[i].radiance;
        lights[j].direction = direct_lights[i].direction;
        j++;
    }
    for (i = 0; i < spot_lights_count; i++) {
//        vec3 direction = globalPos - point_lights[i].pos;
//        float d = length(direction);
//        float F = 1.0 / (1.0 + d);
//        vec3 spot_to_point_vec = normalize(direction);
//        vec3 spot_direction = normalize(spot_lights[i].direction);
//        float lightAngle = dot(spot_to_point_vec, spot_direction);
//        if (lightAngle > 0.2) {
            lights[j].radiance = spot_lights[i].radiance;
            lights[j].direction = spot_lights[i].direction;
            j++;
        //}
    }
    int lights_count = direct_lights_count + point_lights_count + spot_lights_count;
    vec3 albedo = (texture(albedoTexture, texCoord)*color).rgb;
    float metallic = texture(metallicTexture, texCoord).r;
    float roughness = texture(roughnessTexture, texCoord).r;
    vec3 normal = texture(normalTexture, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(tangentBasis * normal);

    vec3 Lo = normalize(cameraPos - globalPos);
    float cosLo = max(0.0, dot(normal, Lo));
    vec3 Lr = 2.0 * cosLo * normal - Lo;
    vec3 F0 = mix(Fdielectric, albedo, metallic);

    vec3 directLighting = vec3(0);
    for (i = 0; i < lights_count; i++) {
        vec3 Li = -normalize(lights[i].direction);
        vec3 Lradiance = lights[i].radiance;
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
    }
    // Ambient lighting (IBL).
    vec3 ambientLighting = vec3(1)*0.1;

    FragColor = vec4(directLighting + ambientLighting, 1.0); //vec4(debugColor, 1.0);//
}
