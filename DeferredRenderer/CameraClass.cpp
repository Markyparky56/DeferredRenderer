#include "CameraClass.hpp"

CameraClass::CameraClass()
    : posX(0.f)
    , posY(0.f)
    , posZ(0.f)
    , rotX(0.f)
    , rotY(0.f)
    , rotZ(0.f)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float _X, float _Y, float _Z)
{
    posX = _X;
    posY = _Y;
    posZ = _Z;
}

void CameraClass::SetRotation(float _X, float _Y, float _Z)
{
    rotX = _X;
    rotY = _Y;
    rotZ = _Z;
}

XMFLOAT3 CameraClass::GetPositon()
{
    return XMFLOAT3(posX, posY, posZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
    return XMFLOAT3(rotX, rotY, rotZ);
}

void CameraClass::Render()
{
    Render(XMFLOAT3(0.f, 1.f, 0.f));
}

void CameraClass::Render(XMFLOAT3 up)
{
    XMFLOAT3 pos, lookAt;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;

    pos = XMFLOAT3(posX, posY, posZ);
    lookAt = XMFLOAT3(0.f, 0.f, 1.f);

    pitch = rotX * 0.0174532925f;
    yaw = rotY * 0.0174532925f;
    roll = rotZ * 0.0174532925f;

    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    XMVECTOR posVec = XMLoadFloat3(&pos);
    XMVECTOR lookAtVec = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
    XMVECTOR upVec = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);

    lookAtVec = XMLoadFloat3(&pos) + lookAtVec;

    viewMatrix = XMMatrixLookAtLH(posVec, lookAtVec, upVec);
}

void CameraClass::GetViewMatrix(XMMATRIX & out)
{
    out = viewMatrix;
}

void CameraClass::RenderBaseViewMatrix()
{
    XMFLOAT3 pos, lookAt, up;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;

    up = XMFLOAT3(0.f, 1.f, 0.f);
    pos = XMFLOAT3(posX, posY, posZ);
    lookAt = XMFLOAT3(0.f, 0.f, 1.f);

    pitch = rotX * 0.0174532925f;
    yaw = rotY * 0.0174532925f;
    roll = rotZ * 0.0174532925f;

    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    XMVECTOR posVec = XMLoadFloat3(&pos);
    XMVECTOR lookAtVec = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
    XMVECTOR upVec = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);

    lookAtVec = XMLoadFloat3(&pos) + lookAtVec;

    baseViewMatrix = XMMatrixLookAtLH(posVec, lookAtVec, upVec);
}

void CameraClass::GetBaseViewMatrix(XMMATRIX & out)
{
    out = baseViewMatrix;
}
