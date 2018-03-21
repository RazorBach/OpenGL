#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 fragPos;

uniform sampler2D diffuseTexture;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{             
	vec3 normal = vec3(0.0,0.0,1.0) ;
	normal = normalize(normal* 2 - 1.0);


    vec3 color = texture(diffuseTexture, TexCoords).rgb;
	vec3 lightColor = vec3(1.0);
	vec3 ambient = 0.15 * lightColor;

	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(lightDir, normal),0.0);
    vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = (lightDir + viewDir)/2;
	float spec = pow(max(dot(halfwayDir,normal),0.0),64);
	vec3 specular = spec * lightColor;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result * color, 1.0);
}