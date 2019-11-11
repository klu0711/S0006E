#pragma once
#include "Vector4D.h"


class LightingNode
{
private:
    vec4 position;
    vec4 color;
	float intensity;

public:
	LightingNode(vec4 pos, vec4 col, float intens);
	LightingNode();
	~LightingNode();
	float getIntensity();
    vec4 getPosition();
    vec4 getColor();
	void setIntensity(float intens);
	void setColor(vec4 color);
	void setPosition(vec4 pos);
};