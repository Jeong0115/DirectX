#include "zzComponent.h"

namespace zz
{
    Component::Component(eComponentType type)
        : mType(type)
        , mOwner(nullptr)
    {
    }

    Component::~Component()
    {
    }

    void Component::Initialize()
    {
    }

    void Component::Update()
    {
    }

    void Component::LateUpdate()
    {
    }

    void Component::Render()
    {
    }
}