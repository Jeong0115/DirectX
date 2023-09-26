#include "zzEquipment.h"
#include "zzTextBox.h"

namespace zz
{
    Equipment::Equipment()
        : mTextBox(nullptr)
    {
    }
    Equipment::~Equipment()
    {
        delete mTextBox;
    }
    void Equipment::Initialize()
    {
    }
    void Equipment::Update()
    {
    }
    void Equipment::LateUpdate()
    {
    }
    void Equipment::Render()
    {
    }
    void Equipment::UseEquipment()
    {
    }

    float Equipment::GetEquipmentRate()
    {
        return 0.0f;
    }
}