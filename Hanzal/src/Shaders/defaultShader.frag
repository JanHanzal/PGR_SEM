#version 140

#define DIRECTIONAL 0
#define SPOT 1
#define POINT 2


const int   FOG_TYPE    = 1;                  
const vec3  FOG_COLOR   = vec3(0.7, 0.7, 0.7); 
const float FOG_START   = 50.0;                
const float FOG_END     = 250.0;               
const float FOG_DENSITY = 0.05;                

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


smooth in vec3 vertexPosition_v;
smooth in vec3 vertexNormal_v;
smooth in vec2 texCoord_v;

uniform bool isObjectSun;

uniform sampler2D texSampler;
uniform Material material;
uniform mat4 Vmatrix;

uniform Light directionalLight;
uniform Light spotLight;
uniform Light pointLight;

uniform int activeLightning;
uniform bool FOG_ENABLED = false;

out vec4 fragColor;



vec3 getDirectionalLight(Material mat, vec3 vPos, vec3 vNorm) {
  vec3 viewSpacePos = vec3(Vmatrix * vec4(directionalLight.position, 0.0));
  
  vec3 L = normalize(viewSpacePos);
  vec3 N = normalize(vNorm);
  vec3 V = normalize(-vPos);
  vec3 R = reflect(-L, N);

  float alpha = max(dot(N, L), 0.0);
  float beta = max(dot(R, V), 0.0);

  vec3 ret = vec3(0.0);
  ret += mat.ambient * directionalLight.ambient;
  ret += mat.diffuse * directionalLight.diffuse * alpha;
  
  if(mat.shininess > 0.0)
    ret += mat.specular * directionalLight.specular * pow(beta, mat.shininess);

  return ret;
}

vec3 getSpotLight(Material mat, vec3 vPos, vec3 vNorm) {
    vec3 viewSpaceLightPos = vec3(Vmatrix * vec4(spotLight.position, 1.0));
    

    vec3 L = normalize(viewSpaceLightPos - vPos);
    vec3 N = normalize(vNorm);
    vec3 V = normalize(-vPos);
    
    vec3 ret = vec3(0.0);
    ret += mat.ambient * spotLight.ambient;

    float NdotL = dot(N, L);
    if(NdotL < 0.0) {
      return ret;
    }
    vec3 viewSpaceLightDir = normalize(vec3(Vmatrix * vec4(spotLight.direction, 0.0)));
    float spotCoef = max(0.0, dot(-L, viewSpaceLightDir));
    
    if(spotCoef > cos(radians(spotLight.cutOff))) 
    {
      vec3 R = reflect(-L, N);
      float RdotV = max(0.0, dot(R, V));

      ret += mat.diffuse * spotLight.diffuse * NdotL;
      if (mat.shininess > 0.0) 
          ret += mat.specular * spotLight.specular * pow(RdotV, mat.shininess);
      
      ret *= pow(spotCoef, spotLight.exponent);
    }

    return ret;
}

vec3 getPointLight(Material mat, vec3 vPos, vec3 vNorm) {
    vec3 ret = vec3(0.0);
    
    vec3 viewSpacePos = vec3(Vmatrix * vec4(pointLight.position, 1.0));
    
    vec3 L = normalize(viewSpacePos - vPos);
    vec3 N = normalize(vNorm);
    vec3 V = normalize(-vPos);
    vec3 R = reflect(-L, vNorm);

    float NdotL = max(0.0, dot(N, L));
    float VdotR = max(0.0, dot(V, R));


    float kc = 1.0; 

    float kl = 2.0 / pointLight.radius;
    float kq = 1.0 / (pointLight.radius * pointLight.radius);

    if(!pointLight.quadratic) {
        kq = 0.0;
    }

    float d = length(viewSpacePos - vPos);

    float attenuationFactor = 0.0;
    if (d <= pointLight.radius) 
        attenuationFactor = 1.0 / (kc + kl * d + kq * (d * d));
    
    ret += mat.ambient * pointLight.ambient;

    if(NdotL > 0.0) 
    {
        ret += mat.diffuse * pointLight.diffuse * NdotL;
        
        if(mat.shininess > 0.0) 
            ret += mat.specular * pointLight.specular * pow(VdotR, mat.shininess);
    }

    ret *= attenuationFactor;

    return ret;
}


void main() {
  vec3 N = normalize(vertexNormal_v);

  vec3 globalAmbientLight = vec3(0.0);
  vec3 outputColor = material.ambient * globalAmbientLight;
  
  if(!isObjectSun || activeLightning != POINT)
  {
    switch(activeLightning)
    {
      case DIRECTIONAL:
          outputColor += getDirectionalLight(material, vertexPosition_v, N);
          break;
      case SPOT:
          outputColor += getSpotLight(material, vertexPosition_v, N);
          break;
      case POINT:
          outputColor += getPointLight(material, vertexPosition_v, N);
          break;
      default:
          outputColor += getDirectionalLight(material, vertexPosition_v, N);
          break;
    }
  }
  else
  {
    outputColor = vec3(1.0);
  }

  vec4 texColor = vec4(1.0);
  if (material.useTexture) 
  {
      texColor = texture(texSampler, texCoord_v);
  }
  vec3 baseColor = outputColor * texColor.rgb;
  if(FOG_ENABLED)
  {
 
      float dist = length(vertexPosition_v);
      float fogFactor = 1.0; 

    if (FOG_TYPE == 0) 
          fogFactor = (FOG_END - dist) / (FOG_END - FOG_START);
    else if (FOG_TYPE == 1) 
          fogFactor = exp(-FOG_DENSITY * dist);
    else if (FOG_TYPE == 2) 
          fogFactor = exp(-pow(FOG_DENSITY * dist, 2.0));
      
      
      fogFactor = clamp(fogFactor, 0.0, 1.0);
      baseColor = mix(FOG_COLOR, baseColor, fogFactor);
  }
  fragColor = vec4(baseColor, texColor.a);
}