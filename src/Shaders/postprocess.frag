#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D sceneTexture;
uniform bool alarmActive;
uniform float time;

void main() {
    vec4 sceneColor = texture(sceneTexture, TexCoord);
    
    if (alarmActive) {
        // Red pulsing overlay
        float pulse = sin(time * 3.0) * 0.5 + 0.5;
        vec3 alarmColor = vec3(1.0, 0.0, 0.0) * pulse * 0.3;
        sceneColor.rgb = mix(sceneColor.rgb, sceneColor.rgb + alarmColor, 0.5);
    }
    
    FragColor = sceneColor;
}

