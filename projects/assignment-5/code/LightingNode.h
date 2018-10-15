#pragma once
#include "Vector4D.h"


class LightingNode
{
private:
	Vector4D position;
	Vector4D color;
	float intensity;

public:
	LightingNode(Vector4D pos, Vector4D col, float intens);
	LightingNode();
	~LightingNode();
	float getIntensity();
	Vector4D getPosition();
	Vector4D getColor();
	void setIntensity(float intens);
	void setColor(Vector4D color);
	void setPosition(Vector4D pos);
};