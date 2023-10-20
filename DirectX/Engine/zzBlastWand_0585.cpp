#include "zzBlastWand_0585.h"
#include "zzTransform.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTextBox.h"

namespace zz
{
    BlastWand_0585::BlastWand_0585()
    {
        mCapacity = 3;
        mManaMax = 113;
        mManaChargeSpeed = 30;
        mCurMana = 113;
        mCastDelay = 0.2f;
        mReChargeTime = 0.2f;
        mSpread = 2.f;
        mSpells.resize(mCapacity);
        mTip = Vector3(15.f, 3.f, 1.0f);

        SetName(L"BlastWand_0585");
    }

    BlastWand_0585::~BlastWand_0585()
    {
    }
    void BlastWand_0585::Initialize()
    {
        GetComponent<Transform>()->SetPosition(6.f, 2.f, 0.0f);
        GetComponent<Transform>()->SetScale(15.f, 7.f, 1.0f);
            
        mInitialPosition = Vector3(6.f, 2.f, 0.0f);

        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"BlastWand_0585", L"..\\Resources\\Texture\\Wand\\BlastWand_0585.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"BlastWand_0585", texture, Vector2(0.0f, 0.0f), Vector2(15.0f, 7.0f), 1, Vector2::Zero, 1.f);
        animator->PlayAnimation(L"BlastWand_0585", true);

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
            L"  \x03 Cast delay     0.2 s\n"
            L"  \x05 Rechrg.Time    0.2 s\n"
            L"  \x0C Mana max       113\n"
            L"  \x0B Mana chg. Spd  30\n"
            L"  \x09 Capacity       3\n"
            L"  \x07 Spread         2.0 DEG\n";


        mTextBox = new TextBox(textBox, Vector3(180.f, 100.f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(180.f, 100.f, 1.0f);
    }

    void BlastWand_0585::Update()
    {
        Wand::Update();
    }

    void BlastWand_0585::LateUpdate()
    {
        Wand::LateUpdate();
    }

    void BlastWand_0585::Render()
    {
        Wand::Render();
    }
    void BlastWand_0585::UseEquipment()
    {
        Wand::UseEquipment();
    }
}