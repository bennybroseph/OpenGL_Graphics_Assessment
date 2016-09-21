// classic Phong equation
#version 410

in vec4 fragPosition;
in vec4 fragColour;
in vec4 fragNormal;

out vec4 outColour;

uniform vec3 LightDirection;

uniform vec3 LightAmbient;
uniform vec3 LightDiffuse;
uniform vec3 LightSpecular;

uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;

uniform float SpecularPower;

uniform vec3 CameraPosition;

vec3 Ambient(vec3 Ka, vec3 Ia);
vec3 Diffuse(vec3 Kd, vec3 Id, vec3 Lm, vec3 N);
vec3 Specular(vec3 Ks, vec3 Is, vec3 Lm, vec3 N, vec3 cameraPosition, vec3 position);

void main()
{
	// Do not normalize 'fragPosition', just convert it to a vec3
	vec3 position = fragPosition.xyz;

	vec3 colour = fragColour.xyz;
	vec3 normal = normalize(fragNormal.xyz);

	// Normalize the light's direction direction
	vec3 lightDirection = normalize(LightDirection);

	vec3 lightAmbient = LightAmbient;
	vec3 lightDiffuse = LightDiffuse;
	vec3 lightSpecular = LightSpecular;

	// Normalize material values
	vec3 materialAmbient = MaterialAmbient;
	vec3 materialDiffuse = MaterialDiffuse;
	vec3 materialSpecular = MaterialSpecular;

	// Do not normalize 'CameraPosition', just convert it to a vec3
	vec3 cameraPosition = CameraPosition.xyz;

	// Calculate Ambient
	vec3 ambient = Ambient(materialAmbient, lightAmbient);
	// Calculate Diffuse
	vec3 diffuse = Diffuse(materialDiffuse, lightDiffuse, lightDirection, normal);
	// Calculate Specular
	vec3 specular = Specular(materialSpecular, lightSpecular, lightDirection, normal, cameraPosition, position);

	outColour = vec4(colour * (ambient + diffuse + specular), 1.f);
}

/*
	Calculates the ambient light given values

	@param Ka	The material's normalized ambient colour
	@param Ia	The light's normalized ambient colour
*/
vec3 Ambient(vec3 Ka, vec3 Ia)
{
	return Ka * Ia;
}
/*
	Calculates the diffuse light given values

	@param Kd	The material's normalized diffuse colour
	@param Id	The light's normalized diffuse colour
	@param Lm	The light's normalized direction vector
	@param N	The fragment's normalized normal vector
*/
vec3 Diffuse(vec3 Kd, vec3 Id, vec3 Lm, vec3 N)
{
	float diffuseTerm = dot(Lm, N);
	diffuseTerm = max(0.f, diffuseTerm);

	return Kd * Id * diffuseTerm;
}
/*
	Calculates the specular light given values

	@param Ks				The material's normalized specular colour
	@param Is				The light's normalized specular colour
	@param Lm				The light's normalized direction vector
	@param N				The fragment's normalized normal vector
	@param cameraPosition	The position of the camera used when drawing the fragment
	@param position			The position of the fragment in world space

	@return A vec3 representing the specular lighting
*/
vec3 Specular(vec3 Ks, vec3 Is, vec3 Lm, vec3 N, vec3 cameraPosition, vec3 position)
{
	vec3 Rm = 2 * dot(Lm, N) * N - Lm;
	vec3 V = normalize(cameraPosition - position);

	float specularTerm = pow(max(0.f, dot(Rm, V)), SpecularPower);

	return Ks * Is * specularTerm;
}