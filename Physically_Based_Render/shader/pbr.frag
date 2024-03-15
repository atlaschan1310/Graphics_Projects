#version 410 core

const float PI = 3.14159265359;

// You can use Epsilon ot avoid division by 0
const float Epsilon = 0.00001;

const int NumLights = 4;
const float gamma     = 2.2;
const float pureWhite = 1.0;
const float ambientFactor = 0.1;

// Constant normal incidence Fresnel factor for all dielectrics.
const vec3 Fdielectric = vec3(0.04);

struct AnalyticalLight {
    vec3 position;
    vec3 radiance;
};

uniform AnalyticalLight lights[NumLights];

in vec3 worldPos;
in vec2 texCoord;


layout(location=0) out vec4 color;
uniform vec3 camPos;
in mat3 tbnMatrix;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D metalnessTexture;
uniform sampler2D roughnessTexture;

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
    return F0 + (vec3(1.0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

/* TODO: Add here the functions for the different terms of the BRDF (simmilar to the fresnelSchlick function above)
 
*/
float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
    
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return nom / denom;
}
  
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
    
    return ggx1 * ggx2;
}
// END TODO
void main()
{
    vec3 albedo = texture(diffuseTexture, texCoord).rgb;
    float metalness = texture(metalnessTexture, texCoord).r;
    float roughness = texture(roughnessTexture, texCoord).r;

    vec3 viewDirection = normalize(camPos - worldPos);

    vec3 Normal = 2.0*texture(normalTexture, texCoord).rgb - 1.0;
    vec3 N = normalize(tbnMatrix * Normal);

    vec3 F0 = mix(Fdielectric, albedo, metalness);

    // Add a small ambient term, for self occlusion assume self occlusion is proportional to roughness
    vec3 directLighting = albedo * ambientFactor * (1.0 - roughness);

    vec3 Lh;
    vec3 F;
    for (int i=0; i<NumLights; ++i)
    {
        vec3 Li = normalize(lights[i].position);
        vec3 Lradiance = lights[i].radiance;

        // Half-vector between Li and viewDirection.
        Lh = normalize(Li + viewDirection);

        // Calculate Fresnel term for direct lighting.
        F  = fresnelSchlick(F0, max(0.0, dot(Lh, viewDirection)));

        /* TODO: Implement the normal distribution function (GGX NDF), the joint masking shadowing function and combine
            them with the provided Fresnel to the BRDF for the specular term. Also add the BRDF for the diffuse term (pay
            attention to energy conservation. Eventually add everyhting to the directLighting
        */
        vec3 L = normalize(lights[i].position - worldPos);
        
        float NDF = DistributionGGX(N, Lh, roughness);
        float G = GeometrySmith(N, viewDirection, L, roughness);
        
        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, viewDirection), 0.0) * max(dot(N, L), 0.0) + Epsilon;
        vec3 specular = numerator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metalness;
        
        directLighting += (kD * albedo / PI + specular) * Lradiance * max(dot(N, L), 0.0);

        // END TODO
    }

    // Tone Correction
    float luminance = dot(directLighting, vec3(0.2126, 0.7152, 0.0722));
    float mappedLuminance = (luminance * (1.0 + luminance/(pureWhite*pureWhite))) / (1.0 + luminance);

    // Scale color by ratio of average luminances.
    vec3 mappedColor = (mappedLuminance / luminance) * directLighting;

    // Gamma correction.
    color = vec4(pow(mappedColor, vec3(1.0/gamma)), 1.0);
}
