#include "LightClass.hpp"

LightClass::LightClass()
{
}

LightClass::~LightClass()
{
}

void LightClass::SetAmbientColour(float red, float green, float blue, float alpha)
{
    ambientColour = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDiffuseColour(float red, float green, float blue, float alpha)
{
    diffuseColour = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularColour(float red, float green, float blue, float alpha)
{
    specularColour = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDirection(float x, float y, float z)
{
    direction = XMFLOAT3(x, y, z);
}

void LightClass::SetSpecularPower(float p)
{
    specularPower = p;
}

XMFLOAT4 LightClass::GetAmbientColour()
{
    return ambientColour;
}

XMFLOAT4 LightClass::GetDiffuseColour()
{
    return diffuseColour;
}

XMFLOAT4 LightClass::GetSpecularColour()
{
    return specularColour;
}

XMFLOAT3 LightClass::GetDirection()
{
    return direction;
}

float LightClass::GetSpecularPower()
{
    return specularPower;
}
