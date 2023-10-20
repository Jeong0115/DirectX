#include "zzObjectPoolManager.h"
#include "zzGameObject.h"
#include "zzSmoke_Orange.h"
#include "zzBouncingBurst.h"
#include "zzSparkBolt.h"
#include "zzMagicArrow.h"
#include "zzLuminous_drill.h"
#include "zzBomb.h"
#include "zzMegalaser.h"


namespace zz
{
    std::map<std::string, std::stack<GameObject*>> ObjectPoolManager::mObjectPool = {};

    ObjectPoolManager::ObjectPoolManager()
    {
    }

    ObjectPoolManager::~ObjectPoolManager()
    {
    }

    void ObjectPoolManager::Initialize()
    {
        std::stack<GameObject*> smoke;
        for (int i = 0; i < 48; i++)
        {
            Smoke_Orange* object = new Smoke_Orange();
         //   SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::Object);
            smoke.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(Smoke_Orange).name(), smoke));
        
        std::stack<GameObject*> bouncing;
        for (int i = 0; i < 302; i++)
        {
            BouncingBurst* object = new BouncingBurst();
          //  SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::PlayerAttack);
            bouncing.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(BouncingBurst).name(), bouncing));
        
        std::stack<GameObject*> spark;
        for (int i = 0; i < 150; i++)
        {
            SparkBolt* object = new SparkBolt();
            //SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::PlayerAttack);
            spark.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(SparkBolt).name(), spark));
        
        std::stack<GameObject*> arrow;
        for (int i = 0; i < 150; i++)
        {
            MagicArrow* object = new MagicArrow();
           // SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::PlayerAttack);
            arrow.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(MagicArrow).name(), arrow));
        
        std::stack<GameObject*> megalaser;
        for (int i = 0; i < 6; i++)
        {
            Megalaser* object = new Megalaser();
          //  SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::PlayerAttack);
            megalaser.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(Megalaser).name(), megalaser));
        
        std::stack<GameObject*> bomb;
        for (int i = 0; i < 3; i++)
        {
            Bomb* object = new Bomb();
          //  SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::PlayerAttack);
            bomb.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(Bomb).name(), bomb));
        
        std::stack<GameObject*> drill;
        for (int i = 0; i < 20; i++)
        {
            Luminous_drill* object = new Luminous_drill();
         //   SceneManager::GetActiveScene()->AddGameObject(object, eLayerType::PlayerAttack);
            drill.push(object);
            object->SetActive(false);
        }
        mObjectPool.insert(std::make_pair(typeid(Luminous_drill).name(), drill));
    }

    void ObjectPoolManager::Release()
    {
        for (auto& iter : mObjectPool)
        {
            while (!iter.second.empty())
            {
                delete iter.second.top();
                iter.second.pop();
            }
        }
        mObjectPool = {};
        mObjectPool.clear();
    }
}