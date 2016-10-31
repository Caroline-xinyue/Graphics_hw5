#version 150

// vertex shader
in vec4 vPosition;
in vec3 vNormal;
out vec4 color; // vertex shade

// light and material properties
uniform vec4 vAmbientProduct, vDiffuseProduct, vSpecularProduct;
uniform vec4 vLightPosition;
uniform float vShininess;
//uniform mat4 vModelView;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

void main() {
	mat4 vModelView = vView * vModel;
	//vertex position into eye coordinates
	vec3 pos = (vModelView * vPosition).xyz;

	vec3 L = normalize(vLightPosition.xyz - pos);
	//vec3 L = normalize(vec3(0, 0, 6) - pos);
	vec3 E = normalize(-pos);
	vec3 H = normalize(L + E);

	//vertex normal into eye coordinates
	vec3 N = normalize(vModelView * vec4(vNormal, 0.0)).xyz;

	//terms in the illumination equation
	vec4 ambient = vAmbientProduct;
	//vec4 ambient = vec4(0.0215, 0.1745, 0.0215, 1.0);

	float Kd = max(dot(L,N), 0.0);
	vec4 diffuse = Kd * vDiffuseProduct;
	//vec4 diffuse = Kd * vec4(0.07568, 0.61424, 0.07568, 1.0);

	float Ks = pow(max(dot(N, H), 0.0), vShininess);
	//float Ks = pow(max(dot(N, H), 0.0), 0.6);

	vec4 specular = Ks * vSpecularProduct;
	//vec4 specular = Ks * vec4(0.633, 0.727811, 0.633, 1.0);

	if( dot(L, N) < 0.0 )
		specular = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position = vProjection * vModelView * vPosition;
    color = ambient + diffuse + specular;
    //color = vec4(1.0, 1.0, 1.0, 1.0);
    color.a = 1.0;
}
