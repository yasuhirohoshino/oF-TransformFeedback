#define PI 3.14159265358979

//
// Description : Array and textureless GLSL 2D/3D/4D simplex
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//
vec4 mod289(vec4 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

float mod289(float x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
    return mod289(((x*34.0)+1.0)*x);
}

float permute(float x) {
    return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

float taylorInvSqrt(float r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

// excerpt from https://github.com/andreasmuller/NoiseWorkshop
mat4 makeLookAt(vec3 eye, vec3 center, vec3 up)
{
    mat4 M;
    
    vec3 zaxis = normalize(eye - center);
    vec3 xaxis = normalize( cross(up, zaxis) );
    vec3 yaxis = cross(zaxis,xaxis);
    
    M[0] = vec4(xaxis,0);
    M[1] = vec4(yaxis,0);
    M[2] = vec4(zaxis,0);
    M[3] = vec4(eye,1);
    
    return M;
}

// Random function
float random(vec2 n)
{
    return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

float random(float nx, float ny)
{
    return random(vec2(nx, ny));
}

//Map
float map(float value, float inputMin, float inputMax, float outputMin, float outputMax){
    return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

vec2 map(vec2 value, vec2 inputMin, vec2 inputMax, vec2 outputMin, vec2 outputMax){
    float x = map(value.x, inputMin.x, inputMax.x, outputMin.x, outputMax.x);
    float y = map(value.y, inputMin.y, inputMax.y, outputMin.y, outputMax.y);
    return vec2(x, y);
}

vec3 map(vec3 value, vec3 inputMin, vec3 inputMax, vec3 outputMin, vec3 outputMax){
    float x = map(value.x, inputMin.x, inputMax.x, outputMin.x, outputMax.x);
    float y = map(value.y, inputMin.y, inputMax.y, outputMin.y, outputMax.y);
    float z = map(value.z, inputMin.z, inputMax.z, outputMin.z, outputMax.z);
    return vec3(x, y, z);
}

vec4 map(vec4 value, vec4 inputMin, vec4 inputMax, vec4 outputMin, vec4 outputMax){
    float x = map(value.x, inputMin.x, inputMax.x, outputMin.x, outputMax.x);
    float y = map(value.y, inputMin.y, inputMax.y, outputMin.y, outputMax.y);
    float z = map(value.z, inputMin.z, inputMax.z, outputMin.z, outputMax.z);
    float w = map(value.w, inputMin.w, inputMax.w, outputMin.w, outputMax.w);
    return vec4(x, y, z, w);
}