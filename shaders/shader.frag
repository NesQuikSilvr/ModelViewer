#version 330 core

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 fragNormal;
in vec3 fragPosition;

// uniforms used for lighting
uniform vec3 AmbientColor = vec3(0.2);
uniform vec3 LightDirectionA = normalize(vec3(10, 5, -5));
uniform vec3 LightColorA = vec3(0.0, 0.75, 1.0);
uniform vec3 LightDirectionB = normalize(vec3(-10, 5, 5));
uniform vec3 LightColorB = vec3(1.0, 0.75, 0.0);
uniform vec3 DiffuseColor;
uniform vec3 viewPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
	float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 reflectDir = reflect(-LightDirectionA, fragNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specularA = specularStrength * spec * LightColorA;

    viewDir = normalize(viewPos - fragPosition);
    reflectDir = reflect(-LightDirectionB, fragNormal);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specularB = specularStrength * spec * LightColorA;

	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = specularA + AmbientColor + LightColorA * max(0, dot(LightDirectionA, fragNormal)) + specularB + AmbientColor + LightColorB * max(0, dot(LightDirectionB, fragNormal));

	// Diffuse reflectance
	vec3 reflectance = irradiance * DiffuseColor;

	// Gamma correction
	fragColor = vec4(sqrt(reflectance), 1);
}