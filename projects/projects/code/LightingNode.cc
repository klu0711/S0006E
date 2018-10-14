#include "LightingNode.h"

LightingNode::LightingNode(Vector4D pos, Vector4D col, float intens)
{
	position = pos;
	color = col;
	intensity = intens;
}

LightingNode::LightingNode()
{
}

LightingNode::~LightingNode()
{
}

float LightingNode::getIntensity()
{
	return intensity;
}

Vector4D LightingNode::getPosition()
{
	return position;
}

Vector4D LightingNode::getColor()
{
	return color;
}

void LightingNode::setIntensity(float intens)
{
	intensity = intens;
}

void LightingNode::setColor(Vector4D color)
{
	this->color = color;
}

void LightingNode::setPosition(Vector4D pos)
{
	position = pos;
}
