#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 emissiveColor;
uniform float emissiveIntensity;
uniform sampler2D emissiveMap;
uniform bool useTexture;

void main() {
    vec3 color = useTexture ? texture(emissiveMap, TexCoord).rgb : emissiveColor;
    color *= emissiveIntensity;
    
    FragColor = vec4(color, 1.0);
}

