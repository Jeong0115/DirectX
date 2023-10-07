#include "zzCamera.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzApplication.h"
#include "zzRenderer.h"

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
        , mSize(1.f)
        , mView(Matrix::Identity)
        , mProjection(Matrix::Identity)
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
    }

    void Camera::Render()
    {
    }

    bool Camera::CreateViewMatrix()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

        // View Translate Matrix
        mView = Matrix::Identity;
        mView *= Matrix::CreateTranslation(-pos);

        // View Rotation Matrix
        Vector3 up = tr->Up();
        Vector3 right = tr->Right();
        Vector3 foward = tr->Foward();

        Matrix viewRotate;
        viewRotate._11 = right.x;	viewRotate._12 = up.x;	viewRotate._13 = foward.x;
        viewRotate._21 = right.y;	viewRotate._22 = up.y;	viewRotate._23 = foward.y;
        viewRotate._31 = right.z;	viewRotate._32 = up.z;	viewRotate._33 = foward.z;
        mView *= viewRotate;

        return true;
    }

    bool Camera::CreateProjectionMatrix(eProjectionType type)
    {
        Application& application = Application::GetInst();

        RECT rect = {};
        GetClientRect(application.GetHwnd(), &rect);
        //float width = (float)(rect.right - rect.left);
        //float height = (float)(rect.bottom - rect.top);
        float width = 1600;
        float height = 900;
        

        if (type == eProjectionType::OrthoGraphic)
        {
            float OrthorGraphicRatio = mSize;
            width *= OrthorGraphicRatio;
            height *= OrthorGraphicRatio;

            mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
        }
        else
        {
            mAspectRatio = width / height;
            mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
        }

        return true;
    }

    void Camera::RegisterCameraInRenderer()
    {
        renderer::cameras.push_back(this);
    }

    void Camera::EnableDepthStencilState()
    {
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState = renderer::depthStencilStates[(UINT)eDSType::Less];
        graphics::GetDevice()->BindDepthStencilState(dsState.Get());
    }

    void Camera::DisableDepthStencilState()
    {
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState = renderer::depthStencilStates[(UINT)eDSType::None];
        graphics::GetDevice()->BindDepthStencilState(dsState.Get());
    }
}