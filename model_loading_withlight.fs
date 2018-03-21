#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct PointLight{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic; 
};

uniform PointLight pointLight[2];
uniform vec3 viewPos;
vec3 calculatePointLight(PointLight light);

in vec3 FragPos;
in vec3 Normal;

void main()
{    
	vec3 result;
	for(int i = 0; i != 2; ++i)
		result += calculatePointLight(pointLight[i]);

    //FragColor = vec4(result,1.0f);
	FragColor = texture(texture_diffuse1 , TexCoords);
}

vec3 calculatePointLight(PointLight light){

	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(lightDir,Normal), 0.0);
	
	vec3 reflectDir = normalize(reflect(-lightDir,Normal));
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),32.0f);

	float distance = length(light.position - FragPos);
	float attenuation = 1.0f/(light.constant + light.linear * distance + light.quadratic * distance *distance);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1,TexCoords));
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
