#pragma once
#include "framework/framework.h" 
struct Pulse {
	bool active;
	float width;
	vec3 color;
	vec3 center;
	float radius;
};
extern Pulse pulse;