#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class LightClass
{
public:
    LightClass();
    ~LightClass();

    void SetAmbientColour(float red, float green, float blue, float alpha);
    void SetDiffuseColour(float red, float green, float blue, float alpha);
    void SetSpecularColour(float red, float green, float blue, float alpha);
    void SetDirection(float x, float y, float z);
    void SetSpecularPower(float p);

    XMFLOAT4 GetAmbientColour();
    XMFLOAT4 GetDiffuseColour();
    XMFLOAT4 GetSpecularColour();
    XMFLOAT3 GetDirection();
    float GetSpecularPower();

private:
    XMFLOAT4 ambientColour;
    XMFLOAT4 diffuseColour;
    XMFLOAT4 specularColour;
    XMFLOAT3 direction;
    float specularPower;
};
