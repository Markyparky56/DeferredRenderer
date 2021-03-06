#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class CameraClass
{
public:
    void* operator new(size_t i)
    {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }

    CameraClass();
    ~CameraClass();

    void SetPosition(float _X, float _Y, float _Z);
    void SetRotation(float _X, float _Y, float _Z);

    XMFLOAT3 GetPositon();
    XMFLOAT3 GetRotation();

    void Render();
    void Render(XMFLOAT3 up);
    void GetViewMatrix(XMMATRIX &out);

    void RenderBaseViewMatrix();
    void GetBaseViewMatrix(XMMATRIX &out);

private:
    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    XMMATRIX viewMatrix, baseViewMatrix;

};
