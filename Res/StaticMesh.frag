/**
* @file StaticMesh.frag
*/
#version 430

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in vec3 inNormal;

out vec4 fragColor;

uniform sampler2D texColor;

/**
* スプライト用フラグメントシェーダー
*/
void main()
{
	//暫定でひとつの平行光源を置く
	vec3 vLight = normalize(vec3(1,-2,-1));
	float power = max(dot(normalize(inNormal),-vLight),0.0) + 0.2;

	fragColor = texture(texColor,inTexCoord);
	fragColor.rgb *= power;
}