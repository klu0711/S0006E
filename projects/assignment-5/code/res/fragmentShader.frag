#version 430
out vec4 fragColor;

in VS_OUT
{
    vec3 FragPos;
    vec2 Uv;
    vec3 TangentLightPos[128];
    vec3 TangentViewPos;
    vec3 TangentFragPos;
}fs_in;

uniform sampler2D diffuser;
uniform sampler2D normalMap;
uniform float intensity;
void main()
{

    vec3 normal = texture(normalMap, fs_in.Uv).rgb;
    
    normal = normalize(normal * 2.0 - 1);
    
    //Diffuse color
    vec3 color = texture(diffuser, fs_in.Uv).rgb;
    //fragColor = texture(normalMap, fs_in.Uv);
    //return;
    //Ambient color
    vec3 ambient = 0.1 * color;
    //diffuse
    vec3 lightDir = normalize(fs_in.TangentFragPos - fs_in.TangentLightPos[0]);
    float diff = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // Specular
    vec3 viewDir = normalize(fs_in.TangentFragPos - fs_in.TangentViewPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    
    vec3 specular = vec3(0.2) * spec;
    
    fragColor = vec4(ambient + diffuse + specular, 1.0);

}
