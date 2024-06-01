#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projectionViewMatrix;
	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
} ubo;

layout(push_constant) uniform Push {
	mat4 modelMatrix;
	mat4 normalMatrix;
} push;

void main() {
	vec3 positionWorld = push.modelMatrix * vec4(position, 1.0);

	gl_Position = ubo.projectionViewMatrix * positionWorld;

	fragNormalWorld = normalize(mat3(push.modelMatrix) * normal);
	fragPosWorld = positionWorld.xyz;

	vec3 directionLight = ubo.lightPosition - positionWorld.xyz;
	float attenuation = 1.0 / dot(directionLight, directionLight);

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w;
	vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 diffuseLight = lightColor * max(dot(normalWordSpace, normalize(directionLight)), 0);

	fragColor = (diffuseLight + ambientLight) * color;
}