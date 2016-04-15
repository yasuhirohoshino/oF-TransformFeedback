#version 330
#pragma include "util.glsl"

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;

uniform mat4 normalMatrix;

uniform vec3 lightPos;
uniform vec4 startColor;
uniform vec4 endColor;

in vec4 position;
in vec3 normal;
in vec2 texcoord;
in vec4 color;

in vec3 inPosition;
in vec3 inVelocity;
in float inAge;
in float inLifetime;

out vec4 positionVarying;
out vec3 normalVarying;

out vec4 colorVarying;
out vec3 v_normalVarying;
out vec4 v_positionVarying;
out vec3 v_lightPos;

void main() {
    float elapsedTime = clamp(float(inAge) / float(max(inLifetime, 1.0)), 0.0, 1.0);
    
    mat4 lookAt = makeLookAt( inVelocity.xyz, vec3(0.0), vec3(0,1,0) );
    vec4 size = vec4(vec3(sin(PI * elapsedTime)), 1.0);
    positionVarying = vec4(vec3(lookAt * (position * size)) + inPosition, 1.0);
    v_positionVarying = modelViewMatrix * positionVarying;
    
    normalVarying = normalize(mat3(lookAt) * normal);
    v_normalVarying = normalize(vec3(mat3(normalMatrix) * normalVarying));
    
    vec3 color = map(vec3(elapsedTime), vec3(0.0), vec3(1.0), startColor.rgb, endColor.rgb);
    colorVarying = vec4(color, 1.0);
    
    v_lightPos = vec3(modelViewMatrix * vec4(lightPos, 1.0));
    
    gl_Position = modelViewProjectionMatrix * positionVarying;
}