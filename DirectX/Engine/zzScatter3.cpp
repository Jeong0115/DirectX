#include "zzScatter3.h"
#include "zzTransform.h"
#include "zzTripleCast.h"
#include "zzTextBox.h"

namespace zz
{
    Scatter3::Scatter3()
    {
        SetName(L"scatter_3");

        mSpell = new TripleCast();
    }
    Scatter3::~Scatter3()
    {
    }
    void Scatter3::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  TRIPLE SCATTER SPELL\n"
            L"\n"
            L" Simultaneously casts 3 spells with low accuracy\n"
            L"\n"
            L"  \x01 Type         Multicast\n"
            L"  \x04 Mana darin   1\n"
            L"\n"
            L"  \x07 Spread       +20 DEG\n";


        mTextBox = new TextBox(textBox, Vector3(250.f * 1.2f, 75.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(250.f, 75.f, 1.0f);
    }
    void Scatter3::Update()
    {
        SpellUI::Update();
    }
    void Scatter3::LateUpdate()
    {
        SpellUI::LateUpdate();
    }
    void Scatter3::Render()
    {
        SpellUI::Render();
    }
}