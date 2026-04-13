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


uniform sampler2D texSampler;
uniform Material material;
uniform mat4 Vmatrix;

uniform Light directionalLight;
uniform Light spotLight;
uniform Light pointLight;


smooth in vec3 vertexPosition_v;
smooth in vec3 vertexNormal_v;
smooth in vec2 texCoord_v;


out vec4 fragColor;

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
  
  if(mat.shininess > 0.0)
    ret += mat.specular * light.specular * pow(beta, mat.shininess);

  return ret;
}

vec3 getSpotLight(Light light, Material mat, vec3 vPos, vec3 vNorm) {
    vec3 viewSpaceLightPos = vec3(Vmatrix * vec4(light.position, 1.0));
    

    vec3 L = normalize(viewSpaceLightPos - vPos);
    vec3 N = normalize(vNorm);
    vec3 V = normalize(-vPos);
    
    vec3 ret = vec3(0.0);
    ret += mat.ambient * light.ambient;

    float NdotL = dot(N, L);
    if(NdotL < 0.0) {
      return ret;
    }
    vec3 viewSpaceLightDir = normalize(vec3(Vmatrix * vec4(light.direction, 0.0)));
    float spotCoef = max(0.0, dot(-L, viewSpaceLightDir));
    
    if(spotCoef > cos(radians(light.cutOff))) 
    {
      vec3 R = reflect(-L, N);
      float RdotV = max(0.0, dot(R, V));

      ret += mat.diffuse * light.diffuse * NdotL;
      if (mat.shininess > 0.0) 
          ret += mat.specular * light.specular * pow(RdotV, mat.shininess);
      
      ret *= pow(spotCoef, light.exponent);
    }

    return ret;
}

vec3 getPointLight(Light light, Material mat, vec3 vPos, vec3 vNorm) {
    return vec3(1.0);
}


void main() {
  vec3 N = normalize(vertexNormal_v);

  vec3 globalAmbientLight = vec3(0.0);
  vec3 outputColor = material.ambient * globalAmbientLight;
  
  //outputColor += getDirectionalLight(directionalLight, material, vertexPosition_v, N);
  
  outputColor += getSpotLight(spotLight, material, vertexPosition_v, N);
  //outputColor += getPointLight(pointLight, material, vertexPosition_v, N);

  vec4 texColor = vec4(1.0);
  if (material.useTexture) 
  {
      texColor = texture(texSampler, texCoord_v);
  }

  fragColor = vec4(outputColor, 1.0) * texColor;
}