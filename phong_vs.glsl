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
uniform vec4 vLightPosition;

void main() {
	mat4 vModelView = vView * vModel;
	vec3 pos = (vModelView * vPosition).xyz;
	fN = (vModelView * vec4(vNormal, 0.0)).xyz;
	fE = -pos.xyz;
	//fL = (vView * vLightPosition).xyz;

	//fN = vNormal;
	//fE = vPosition.xyz;
	fL = vLightPosition.xyz;

	if( vLightPosition.w != 0.0 ) {
		fL = vLightPosition.xyz - vPosition.xyz;
	}

    gl_Position = vProjection * vModelView * vPosition;
}
