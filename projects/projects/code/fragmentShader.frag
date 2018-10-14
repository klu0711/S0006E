#version 430
out vec4 fragColor;
in vec2 texCords2;
in vec3 position;
in vec4 Color;
in vec3 uNormal;
in vec4 LightPos;
in vec4 LightColor;
uniform sampler2D diffuser;
void main()
{
	

	
	//Ambient light
	vec3 ambientLight = vec3(0.01f,0.01f,0.01f);
	//Diffuse light
	vec3 posToLightDirVec = normalize(vec3(LightPos) - position);
	vec3 diffuseColor = vec3(LightColor);
	float diffuse = clamp(dot(posToLightDirVec, uNormal),0,1);
	vec3 finalDiffuse = diffuseColor * diffuse;
	
	fragColor = texture(diffuser, texCords2) * (vec4(ambientLight,1.0f) + vec4(finalDiffuse, 1.0f));
}