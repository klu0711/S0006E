#include "LightingNode.h"

LightingNode::LightingNode(vec4 pos, vec4 col, float intens)
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

vec4 LightingNode::getPosition()
{
	return position;
}

vec4 LightingNode::getColor()
{
	return color;
}

void LightingNode::setIntensity(float intens)
{
	intensity = intens;
}

void LightingNode::setColor(vec4 color)
{
	this->color = color;
}

void LightingNode::setPosition(vec4 pos)
{
	position = pos;
}
