#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;

struct Light {
    int type; // 0 = directional, 1 = point, 2 = spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float range;
};

uniform vec3 viewPos;
uniform Light lights[2]; // Exactly 2 lights
uniform int numLights;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform bool useNormalMap;
uniform bool usePBRMaps;

void main() {
    vec3 albedo = texture(albedoMap, TexCoord).rgb;
    float metallic = usePBRMaps ? texture(metallicMap, TexCoord).r : 0.5;
    float roughness = usePBRMaps ? texture(roughnessMap, TexCoord).r : 0.5;
    float ao = usePBRMaps ? texture(aoMap, TexCoord).r : 1.0;
    
    vec3 N = normalize(Normal);
    if (useNormalMap) {
        vec3 normalMapValue = texture(normalMap, TexCoord).rgb * 2.0 - 1.0;
        mat3 TBN = mat3(normalize(Tangent), normalize(Bitangent), N);
        N = normalize(TBN * normalMapValue);
    }
    
    vec3 V = normalize(viewPos - FragPos);
    
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < numLights && i < 2; i++) {
        vec3 L = vec3(0.0);
        vec3 lightColor = lights[i].color * lights[i].intensity;
        
        if (lights[i].type == 0) { // Directional
            L = normalize(-lights[i].direction);
        } else if (lights[i].type == 1) { // Point
            vec3 lightDir = lights[i].position - FragPos;
            float distance = length(lightDir);
            L = normalize(lightDir);
            
            // Attenuation
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
            lightColor *= attenuation;
        }
        
        vec3 H = normalize(V + L);
        float NdotL = max(dot(N, L), 0.0);
        
        // Simple PBR approximation
        vec3 F0 = mix(vec3(0.04), albedo, metallic);
        vec3 F = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, V), 0.0), 5.0);
        
        float NDF = 1.0 / (3.14159 * pow(max(dot(N, H), 0.0) * (roughness * roughness) + 0.0001, 2.0));
        float G = 1.0 / (max(dot(N, V), 0.0) * (roughness + 1.0) + roughness);
        
        vec3 specular = (NDF * G * F) / (4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001);
        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - metallic);
        
        Lo += (kD * albedo / 3.14159 + specular) * lightColor * NdotL;
    }
    
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
    
    // HDR tone mapping
    color = color / (color + vec3(1.0));
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
}

