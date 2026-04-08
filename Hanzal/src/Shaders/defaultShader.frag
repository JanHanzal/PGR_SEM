#version 140

struct Material {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  float shininess;
  bool  useTexture;
};

struct Light {
  vec3  ambient;
  vec3  diffuse;
  vec3  specular;
  vec3  position;
};

uniform sampler2D texSampler;;
uniform Material material;
uniform mat4 Vmatrix;

smooth in vec3 vertexPosition_v;
smooth in vec3 vertexNormal_v;
smooth in vec2 texCoord_v;

uniform Light light;

out vec4 fragColor;

// hardcoded lights
Light sun;
float sunSpeed = 0.5f;

void setupLights() {
  // SUN
  sun.ambient  = vec3(1.0);
  sun.diffuse  = vec3(1.0);
  sun.specular = vec3(0.0);
  sun.position = vec3(Vmatrix * vec4(0.0, 0.0, 1.0,0.0));
}

vec3 directionalLight(Light light, Material mat, vec3 vPos, vec3 vNorm) {
  vec3 L = normalize(light.position);
  vec3 N = normalize(vNorm);
  vec3 V = normalize(-vPos);
  vec3 R = reflect(-L, N);

  float NdotL = max(dot(N, L), 0.0);
  float RdotV = max(dot(R, V), 0.0);

  vec3 ret = vec3(0.0);
  ret += mat.ambient * light.ambient;
  ret += mat.diffuse * light.diffuse * NdotL;
  ret += mat.specular * light.specular * pow(RdotV, mat.shininess);

  return ret;
}


void main() {
  setupLights();

  // POZOR: Extrémnì dùležitý krok - normalizace interpolované normály
  vec3 N = normalize(vertexNormal_v);

  // Inicializace základním ambientním osvìtlením
  vec3 globalAmbientLight = vec3(0.0f);
  vec3 outputColor = material.ambient * globalAmbientLight;

  // Akumulace svìtel
  outputColor += directionalLight(sun, material, vertexPosition_v, N);

  // Aplikace textury, pokud se má použít
  vec4 texColor = vec4(1.0);
  if (material.useTexture) {
      texColor = texture(texSampler, texCoord_v);
  }

  // Finální zapsání barvy pixelu
  fragColor = vec4(outputColor, 1.0) * texColor;
}   