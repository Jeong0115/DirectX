#include "zzWand0765.h"
#include "zzTransform.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTextBox.h"

namespace zz
{
    Wand0765::Wand0765()
    {
        mCapacity = 8;
        mManaMax = 900;
        mManaChargeSpeed = 90000;
        mCurMana = 90000;
        mCastDelay = 0.08f;
        mReChargeTime = 0.02f;
        mSpread = 0.f;
        mSpells.resize(mCapacity);
        mTip = Vector3(18.f, 7.5f, 1.0f);

        SetName(L"Wand_0765");
    }
    Wand0765::~Wand0765()
    {
    }
    void Wand0765::Initialize()
    {
        GetComponent<Transform>()->SetPosition(6.f, 2.f, 0.0f);
        GetComponent<Transform>()->SetScale(18.f, 15.f, 1.0f);

        mInitialPosition = Vector3(6.f, 2.f, 0.0f);

        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"wand_0765", L"..\\Resources\\Texture\\Wand\\wand_0765.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"Wand_0765", texture, Vector2(0.0f, 0.0f), Vector2(18.f, 15.f), 1, Vector2::Zero, 1.f);
        animator->PlayAnimation(L"Wand_0765", true);

        mInformation =
            L"\n"
            L"       Shuffle       No\n"
            L"       Spells/Cast   1";

        Wand::Initialize();

        std::wstring textBox =
            L"\n"
            L"  WAND\n"
            L"\n"
            L"  \x0D Shuffle        No\n"
            L"  \x08 Spells/Cast    1\n"
            L"  \x03 Cast delay     0.08 s\n"
            L"  \x05 Rechrg.Time    0.02 s\n"
            L"  \x0C Mana max       900\n"
            L"  \x0B Mana chg. Spd  90000\n"
            L"  \x09 Capacity       8\n"
            L"  \x07 Spread         0.0 DEG\n";


        mTextBox = new TextBox(textBox, Vector3(180.f, 100.f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(180.f, 100.f, 1.0f);
    }
    void Wand0765::Update()
    {
        Wand::Update();
    }
    void Wand0765::LateUpdate()
    {
        Wand::LateUpdate();
    }
    void Wand0765::Render()
    {
        Wand::Render();
    }
    void Wand0765::UseEquipment()
    {
        Wand::UseEquipment();
    }
}