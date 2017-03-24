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
    //Render(XMFLOAT3(0.f, 1.f, 0.f));
    XMVECTOR up, position, lookAt;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;

    // Set the up vector
    up = XMVectorSet(0.f, 1.f, 0.f, 1.f);

    // Set the position of the camera in the world
    position = XMVectorSet(posX, posY, posZ, 1.f);

    // Set where the camera is looking by default
    lookAt = XMVectorSet(0.f, 0.f, 1.f, 1.f);

    // Set the yaw, pitch and roll rotations in radians
    pitch = rotX * 0.0174532925f;
    yaw = rotY * 0.0174532925f;
    roll = rotZ * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch and roll values
    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    // Transform the lookAt and up vector by the rotation matrix, rotating the camera at the origin
    lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
    up = XMVector3TransformCoord(up, rotationMatrix);

    // Translate the rotation camera to the location of the viewer
    lookAt = position + lookAt;

    // Create the view matrix
    viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

//void CameraClass::Render(XMFLOAT3 up)
//{
//    XMFLOAT3 pos, lookAt;
//    float yaw, pitch, roll;
//    XMMATRIX rotationMatrix;
//
//    pos = XMFLOAT3(posX, posY, posZ);
//    lookAt = XMFLOAT3(0.f, 0.f, 1.f);
//
//    pitch = rotX * 0.0174532925f;
//    yaw = rotY * 0.0174532925f;
//    roll = rotZ * 0.0174532925f;
//
//    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
//
//    XMVECTOR posVec = XMLoadFloat3(&pos);
//    XMVECTOR lookAtVec = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
//    XMVECTOR upVec = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);
//
//    lookAtVec = XMLoadFloat3(&pos) + lookAtVec;
//
//    viewMatrix = XMMatrixLookAtLH(posVec, lookAtVec, upVec);
//}

void CameraClass::GetViewMatrix(XMMATRIX & out)
{
    out = viewMatrix;
}

void CameraClass::RenderBaseViewMatrix()
{
    XMVECTOR up, position, lookAt;

    up = XMVectorSet(0.f, 1.f, 0.f, 1.f);
    position = XMVectorSet(0.f, 0.f, -10.f, 1.f);
    lookAt = XMVectorSet(0.f, 0.f, 0.f, 1.f);
    baseViewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void CameraClass::GetBaseViewMatrix(XMMATRIX & out)
{
    out = baseViewMatrix;
}
