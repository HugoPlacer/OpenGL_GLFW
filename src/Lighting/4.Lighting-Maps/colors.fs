#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;

uniform vec3 viewPos;
uniform float u_time;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emision;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //vec3 emision = vec3(texture(material.emision, TexCoords));
    vec3 emision = texture(material.emision,TexCoords + vec2(0.0,u_time)).rgb * floor(vec3(1.f) - texture(material.specular,TexCoords ).rgb);

    FragColor = vec4(ambient + diffuse + specular + emision, 1.0);
}