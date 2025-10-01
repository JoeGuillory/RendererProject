#version 430 core


layout (location = 3) uniform sampler2D albedo;
layout (location = 4) uniform vec3 ambientLightColor;
layout (location = 5) uniform vec3 lightDir;
layout (location = 6) uniform vec3 lightColor;
in vec4 vPos;
in vec2 vUV;
in vec3 vNormal;
out vec4 outColor;
void main() 
{
	float d = max(0,dot(vNormal, -lightDir)); 
	vec3 diffuse = d * lightColor; 
	vec4 base = texture(albedo,vUV); 
	outColor.xyz = base.xyz * (ambientLightColor + diffuse); 
	outColor.a = base.a;
}