#version 330

#define PI 3.14159265358979

#pragma include "util.glsl"
#pragma include "noise4d.glsl"

in vec3 inPosition;
in vec3 inVelocity;
in float inAge;
in float inLifetime;

out vec3 outPosition;
out vec3 outVelocity;
out float outAge;
out float outLifetime;

uniform float time;
uniform float timestep;
uniform float scale;

void main() {
    vec3 pos = inPosition;
    vec3 vel = inVelocity;
    float age = inAge;
    float lifetime = inLifetime;
    
    if(age >= lifetime){
        lifetime = 100.0 + 50.0 * abs(random(pos.xy));
        age = 0;
        vel.xyz = normalize(vel);
        pos = vec3(0.0);
    }else{
        vel.x += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 0.1352 * time)) * timestep;
        vel.y += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 1.2814 * time)) * timestep;
        vel.z += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 2.5564 * time)) * timestep;
        
        age++;
    }
    
    outPosition = pos + vel;
    outVelocity = vel;
    outAge = age;
    outLifetime = lifetime;
}