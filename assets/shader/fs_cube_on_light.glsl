#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse[3];
    sampler2D specular[2];
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
};

uniform int light_type;

uniform DirectionLight direction_light[8];

uniform PointLight point_light[32];

uniform SpotLight spot_light[16];

uniform int direction_light_count;

uniform int point_light_count;

uniform int spot_light_count;
  
uniform vec3 objectColor;

uniform vec3 viewPos;

vec3 ApplyDirectionLight(DirectionLight direction_light, vec3 normal, vec3 viewDir)
{
	// ambient
	vec3 ambient = direction_light.ambient * vec3(texture(material.diffuse, TexCoords));

	// diffuse 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-direction_light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = direction_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = direction_light.specular * (spec * vec3(texture(material.specular, TexCoords)));  

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}


void main()
{
	if(light_type == 1) // direction light
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
		FragColor = vec4(result, 1.0);
	}
	else if(light_type == 2) // point light
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
			
		float distance    = length(point_light.position - FragPos);
		float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));
		
		vec3 result = (ambient + diffuse + specular)*attenuation;
		FragColor = vec4(result, 1.0);
	}
	else if(light_type == 3) // flash light (spot light)
	{
		vec3 lightDir = normalize(spot_light.position - FragPos);
		
		float theta = dot(lightDir, normalize(-spot_light.direction));
		
		if(theta > spot_light.cutOff) 
		{       
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
				
			vec3 result = (ambient + diffuse + specular);
			FragColor = vec4(result, 1.0);
		}
		else  // else, use ambient light so scene isn't completely dark outside the spotlight.
			FragColor = vec4(spot_light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
	}
	

}