#include "zzCamera.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzApplication.h"
#include "zzRenderer.h"
#include "zzScene.h"
#include "zzSceneManger.h"
#include "zzMeshRenderer.h"

namespace zz
{
    Matrix Camera::View = Matrix::Identity;
    Matrix Camera::Projection = Matrix::Identity;

    Camera::Camera()
        : Component(eComponentType::Camera)
        , mType(eProjectionType::OrthoGraphic)
        , mAspectRatio(1.0f)
        , mNear(1.0f)
        , mFar(1000.0f)
        , mSize(1920.f)
    {
    }

    Camera::~Camera()
    {
    }
    void Camera::Initialize()
    {
    }

    void Camera::Update()
    {
    }

    void Camera::LateUpdate()
    {
        CreateViewMatrix();
        CreateProjectionMatrix(mType);
    }
    void Camera::Render()
    {
    }
    bool Camera::CreateViewMatrix()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

        // View Translate Matrix
        View = Matrix::Identity;
        View *= Matrix::CreateTranslation(-pos);

        // View Rotation Matrix
        Vector3 up = tr->Up();
        Vector3 right = tr->Right();
        Vector3 foward = tr->Foward();

        Matrix viewRotate;
        viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
        viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
        viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;
        View *= viewRotate;

        return true;
    }

    //int a = 0;
    //float width = 640 * 0.7;
    //float height = 360 * 0.7;

    bool Camera::CreateProjectionMatrix(eProjectionType type)
    {
        Application& application = Application::GetInst();

        RECT rect = {};
        GetClientRect(application.GetHwnd(), &rect);
        //float width = (float)(rect.right - rect.left);
        //float height = (float)(rect.bottom - rect.top);
        float width = 1600;
        float height = 900;
        mAspectRatio = width / height;

        if (type == eProjectionType::OrthoGraphic)
        {
            float OrthorGraphicRatio = 0.275f;
            width *= OrthorGraphicRatio;
            height *= OrthorGraphicRatio;

            Projection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
        }
        else
        {
            Projection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
        }
 

        return true;
    }

    void Camera::RegisterCameraInRenderer()
    {
        renderer::cameras.push_back(this);
    }

    void Camera::TurnLayerMask(eLayerType type, bool enable)
    {
        mLayerMask.set((UINT)type, enable);
    }

    void Camera::SortGameObjects()
    {
        //mOpaqueGameObjects.clear();
        //mCutOutGameObjects.clear();
        //mTransparentGameObjects.clear();

        //Scene* scene = SceneManager::GetActiveScene();
        //for (size_t i = 0; i < (UINT)eLayerType::End; i++)
        //{
        //    if (mLayerMask[i] == true)
        //    {
        //        Layer& layer = scene->GetLayer((eLayerType)i);
        //        const std::vector<GameObject*> gameObjs
        //            = layer.GetGameObjects();
        //        // layer에 있는 게임오브젝트를 들고온다.

        //        for (GameObject* obj : gameObjs)
        //        {
        //            //렌더러 컴포넌트가 없다면?
        //            MeshRenderer* mr
        //                = obj->GetComponent<MeshRenderer>();
        //            if (mr == nullptr)
        //                continue;

        //            std::shared_ptr<Material> mt = mr->GetMaterial();
        //            eRenderingMode mode = mt->GetRenderingMode();
        //            switch (mode)
        //            {
        //            case ya::graphics::eRenderingMode::Opaque:
        //                mOpaqueGameObjects.push_back(obj);
        //                break;
        //            case ya::graphics::eRenderingMode::CutOut:
        //                mCutOutGameObjects.push_back(obj);
        //                break;
        //            case ya::graphics::eRenderingMode::Transparent:
        //                mTransparentGameObjects.push_back(obj);
        //                break;
        //            default:
        //                break;
        //            }
        //        }
        //    }
        //}
    }
    void Camera::RenderOpaque()
    {
        for (GameObject* gameObj : mOpaqueGameObjects)
        {
            if (gameObj == nullptr)
                continue;

            gameObj->Render();
        }
    }
    void Camera::RenderCutOut()
    {
        for (GameObject* gameObj : mCutOutGameObjects)
        {
            if (gameObj == nullptr)
                continue;

            gameObj->Render();
        }
    }
    void Camera::RenderTransparent()
    {
        for (GameObject* gameObj : mTransparentGameObjects)
        {
            if (gameObj == nullptr)
                continue;

            gameObj->Render();
        }
    }
}