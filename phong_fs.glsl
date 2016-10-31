#version 150

// fragment shader
out vec4  fColor;
// per-fragment interpolated values from the vertex shader
in vec3 fN;
in vec3 fL;
in vec3 fE;

uniform vec4 vAmbientProduct, vDiffuseProduct, vSpecularProduct;
//uniform mat4 vModelView;
uniform vec4 vLightPosition;
uniform float vShininess;

void main() {
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);

	vec3 H = normalize( L + E );
	vec4 ambient = vAmbientProduct;
	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd * vDiffuseProduct;

	float Ks = pow(max(dot(N, H), 0.0), vShininess);
	vec4 specular = Ks * vSpecularProduct;

	// discard the specular highlight if the vertex is not facing light
	if( dot(L, N) < 0.0 )
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	fColor = ambient + diffuse + specular;
	//fColor = vec4(1.0, 1.0, 1.0, 1.0);
	fColor.a = 1.0;
}