#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoord;
out vec4 color;

uniform vec3 viewPos;
// 冯氏光照材质

struct Material {
    sampler2D   diffuseTexture;
    vec3        diffuse;
    sampler2D   specularTexture;
    vec3        specular;
    sampler2D   emissionTexture;
    float       shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{    
    vec3 finalDiffuse = texture(material.diffuseTexture,TextureCoord).rgb + material.diffuse;
    // 环境光
    vec3 ambient = light.ambient * finalDiffuse;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * finalDiffuse);
 

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularColor = material.specular + texture(material.specularTexture,TextureCoord).rgb;
    vec3 specular = light.specular * (spec * specularColor);  

       // emission
    vec3 emission = texture(material.emissionTexture, TextureCoord).rgb;

    vec3 result = ambient + diffuse + specular + emission;
    color = vec4(result, 1.0);
}