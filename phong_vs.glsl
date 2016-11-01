#version 150

// vertex shader
in vec4 vPosition;
in vec3 vNormal;

// output values to fragment shader
out vec3 fN;
out vec3 fE;
out vec3 fL;

//uniform mat4 vModelView;
uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;
//uniform vec4 vLightPosition;

void main() {
	vec4 vAmbientProduct = vec4(0.0215, 0.1745, 0.0215, 1.0);
	vec4 vDiffuseProduct = vec4(0.07568, 0.61424, 0.07568, 1.0);
	vec4 vSpecularProduct = vec4(0.633, 0.727811, 0.633, 1.0);
	float vShininess = 0.6;
	vec4 vLightPosition = vec4(0.0, 0.0, 50.0, 1.0);
	mat4 vModelView = vView * vModel;
	vLightPosition = vView * vLightPosition;
	vec3 pos = (vModelView * vPosition).xyz;
	fN = (vModelView * vec4(vNormal, 0.0)).xyz;
	fE = -pos.xyz;

	//fN = vNormal;
	//fE = vPosition.xyz;
	//fL = vLightPosition.xyz - pos.xyz;
	//fL = (vView * vLightPosition).xyz;
	fL = (vView * vLightPosition).xyz - pos.xyz;

	if( vLightPosition.w != 0.0 ) {
		fL = (vView * vLightPosition).xyz - pos.xyz;
	}

    gl_Position = vProjection * vModelView * vPosition;
}

/*
void main() {
mat4 vModelView = vView * vModel;
vec3 pos = (vModelView * vPosition).xyz;
fN = (vModelView * vec4(vNormal, 0.0)).xyz;
fE = -pos.xyz;
//fL = (vView * vLightPosition).xyz;

//fN = vNormal;
//fE = vPosition.xyz;
fL = vLightPosition.xyz - pos.xyz;

//if( vLightPosition.w != 0.0 ) {
//	fL = vLightPosition.xyz - vPosition.xyz;
//}

    gl_Position = vProjection * vModelView * vPosition;
}
*/
