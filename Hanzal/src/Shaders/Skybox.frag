#version 140


const vec3  FOG_COLOR   = vec3(0.7, 0.7, 0.7);


uniform samplerCube skyboxSampler;
uniform bool FOG_ENABLED = false;


in vec3 texCoord_v;

out vec4 color_f;


void main() {
    vec4 skyColor = texture(skyboxSampler, texCoord_v);

    if (FOG_ENABLED) 
    {
        vec3 viewDir = normalize(texCoord_v);

        float fogFactor = smoothstep(0.0, 0.2, viewDir.z);
        
        vec3 blendedColor = mix(FOG_COLOR, skyColor.rgb, fogFactor);
        
        color_f = vec4(blendedColor, skyColor.a);
    } else 
    {
        color_f = skyColor;
    }
}