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
        mCurMana = 10;
        mCastDelay = 0.f;
        mReChargeTime = 2.f;
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
        GetComponent<Transform>()->SetPosition(6.f, -2.f, 0.0f);
        GetComponent<Transform>()->SetScale(15.f, 7.f, 1.0f);
            
        mInitialPosition = Vector3(6.f, -2.f, 0.0f);

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
            L" WAND\n"
            L"\n"
            L" Shuffle        No\n"
            L" Spells/Cast    1\n"
            L" Cast delay     0 s\n"
            L" Rechrg. Time   2 s\n"
            L" Mana max       113\n"
            L" Mana chg. Spd  30\n"
            L" Capacity       3\n"
            L" Spread         2\n";


        mTextBox = new TextBox(textBox, Vector3(140.f * 1.2f, 120.f * 1.2f, 1.0f));
        mTextBox->Initialize();
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
    }
}