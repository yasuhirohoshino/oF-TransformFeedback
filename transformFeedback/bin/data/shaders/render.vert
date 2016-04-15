#version 330
#pragma include "util.glsl"

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec4 globalColor;

uniform vec4 startColor;
uniform vec4 endColor;

in vec3 inPosition;
in vec3 inVelocity;
in float inAge;
in float inLifetime;

out vec4 positionVarying;

out vec4 colorVarying;

void main() {
    float elapsedTime = clamp(float(inAge) / float(max(inLifetime, 1.0)), 0.0, 1.0);
    
    positionVarying = vec4(inPosition, 1.0);
    
    vec3 color = map(vec3(elapsedTime), vec3(0.0), vec3(1.0), startColor.rgb, endColor.rgb);
    colorVarying = vec4(color, 1.0);
    
    gl_Position = modelViewProjectionMatrix * positionVarying;
    gl_PointSize = 6 * clamp((sin(PI * (float(inAge) / float(inLifetime + 1.0)))), 0.0, 1.0);
}