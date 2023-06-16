#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzConstantBuffer.h"

namespace zz
{
    using namespace zz::graphics;

    Transform::Transform()
        : Component(eComponentType::Transform)
        , mPosition(Vector3::Zero)
        , mRotation(Vector3::Zero)
        , mScale(Vector3::One)
    {
    }

    Transform::~Transform()
    {
    }

    void Transform::Initialize()
    {
    }

    void Transform::Update()
    {
    }

    void Transform::LateUpdate()
    {
    }

    void Transform::Render()
    {
    }

    void Transform::BindConstantBuffer()
    {
        ConstantBuffer* buffer = renderer::constantBuffer;
        Vector4 position(mPosition.x, mPosition.y, mPosition.z, 1.0f);
        buffer->SetBufferData(&position);
        buffer->BindConstantBuffer(eShaderStage::VS);
    }
}