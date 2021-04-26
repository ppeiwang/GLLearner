#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
uniform Material material;

struct DirectionLight{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
    vec3 position;
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
	float outerCutOff;
};

uniform DirectionLight direction_lights[8];

uniform PointLight point_lights[32];

uniform SpotLight spot_lights[16];

uniform int direction_lights_count;

uniform int point_lights_count;

uniform int spot_lights_count;

uniform vec3 viewPos;

vec3 AddDirectionLight(DirectionLight direction_light)
{
	// ambient
	vec3 ambient = direction_light.ambient * vec3(texture(material.diffuse, TexCoords));

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-direction_light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = direction_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = direction_light.specular * (spec * vec3(texture(material.specular, TexCoords)));  

	vec3 result = (ambient + diffuse + specular);
	
	return result;
}

vec3 AddPointLight(PointLight point_light)
{
	// ambient
	vec3 ambient = point_light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(point_light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = point_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 
	
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = point_light.specular * (spec * vec3(texture(material.specular, TexCoords)));  
		
	float distance = length(point_light.position - FragPos);
	float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));
	
	vec3 result = (ambient + diffuse + specular)*attenuation;

	return result;
}

vec3 AddSpotLight(SpotLight spot_light)
{
	vec3 lightDir = normalize(spot_light.position - FragPos);

	// ambient
	vec3 ambient = spot_light.ambient * vec3(texture(material.diffuse, TexCoords));
	// diffuse 
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = spot_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spot_light.specular * (spec * vec3(texture(material.specular, TexCoords)));  

	    // spotlight intensity
    float theta = dot(lightDir, normalize(-spot_light.direction)); 
    float epsilon = spot_light.cutOff - spot_light.outerCutOff;
    float intensity = clamp((theta - spot_light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 result = (ambient + diffuse + specular) * intensity;
	return result;
}

void main()
{
	vec3 color = vec3(0.0, 0.0, 0.0);
	for(int i = 0; i < direction_lights_count; i++)
	{
		color += AddDirectionLight(direction_lights[i]);
	}

	for(int i = 0; i < point_lights_count; i++)
	{
		color += AddPointLight(point_lights[i]);
	}

	for(int i = 0; i < spot_lights_count; i++)
	{
		color += AddSpotLight(spot_lights[i]);
	}

	FragColor = vec4(color, 0.0);
}