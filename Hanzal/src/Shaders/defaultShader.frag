#version 140

struct Material {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  float shininess;
  bool  useTexture;
};

struct Light {
  vec3  position;
  
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  
  //SpotLight
  vec3 direction;
  float cutOff;
  float exponent;

  //PointLight params
  float radius;
  bool quadratic;
};

uniform sampler2D texSampler;;
uniform Material material;
uniform mat4 Vmatrix;

uniform Light directionalLight;
uniform Light spotLight;
uniform Light pointLight;


smooth in vec3 vertexPosition_v;
smooth in vec3 vertexNormal_v;
smooth in vec2 texCoord_v;


out vec4 fragColor;

// hardcoded lights
Light sun;

void setupLights() {
  // SUN
  sun.position = vec3(Vmatrix * vec4(0.0, 0.0, 1.0,0.0));
  sun.ambient  = vec3(0.2);
  sun.diffuse  = vec3(1.0);
  sun.specular = vec3(0.0);
}

vec3 getDirectionalLight(Light light, Material mat, vec3 vPos, vec3 vNorm) {
  vec3 viewSpacePos = vec3(Vmatrix * vec4(directionalLight.position, 0.0));
  
  vec3 L = normalize(viewSpacePos);
  vec3 N = normalize(vNorm);
  vec3 V = normalize(-vPos);
  vec3 R = reflect(-L, N);

  float alpha = max(dot(N, L), 0.0);
  float beta = max(dot(R, V), 0.0);

  vec3 ret = vec3(0.0);
  ret += mat.ambient * light.ambient;
  ret += mat.diffuse * light.diffuse * alpha;
  ret += mat.specular * light.specular * pow(beta, mat.shininess);

  return ret;
}

vec3 getSpotLight(Light light, Material mat, vec3 vPos, vec3 vNorm) {
    vec3 L = normalize(light.position - vPos);
    vec3 N = normalize(vNorm); // Znovu normalizujeme
    vec3 V = normalize(-vPos);
    vec3 R = reflect(-L, N);
    vec3 D = normalize(light.direction);

    float spotEffect = dot(D, -L);
    vec3 ret = vec3(0.0);

    if(spotEffect >= light.cutOff) {
        float spotIntensity = pow(spotEffect, light.exponent);
        
        float NdotL = max(dot(N, L), 0.0);
        float RdotV = max(dot(R, V), 0.0);

        ret += mat.ambient * light.ambient;
        ret += spotIntensity * mat.diffuse * light.diffuse * NdotL;
        ret += spotIntensity * mat.specular * light.specular * pow(RdotV, mat.shininess);
    }
    return ret;
}

vec3 getPointLight(Light light, Material mat, vec3 vPos, vec3 vNorm) {
    return vec3(1.0);
}


void main() {
  setupLights();
  vec3 N = normalize(vertexNormal_v);

  vec3 globalAmbientLight = vec3(0.1);
  vec3 outputColor = material.ambient * globalAmbientLight;
  
  outputColor += getDirectionalLight(sun, material, vertexPosition_v, N);
  
  //outputColor += getSpotLight(sun, material, vertexPosition_v, N);
  //outputColor += getPointLight(sun, material, vertexPosition_v, N);

  vec4 texColor = vec4(1.0);
  if (material.useTexture) 
  {
      texColor = texture(texSampler, texCoord_v);
  }

  fragColor = vec4(outputColor, 1.0) * texColor;
}