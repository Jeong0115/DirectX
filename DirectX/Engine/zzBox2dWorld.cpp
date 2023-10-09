#include "zzBox2dWorld.h"
#include "zzPixelWorld.h"
#include "..\External\Triangulation\include\earcut.hpp"

#include "zzMesh.h"
#include "zzShader.h"
#include "zzResourceManager.h"
#include "zzRenderer.h"
#include "zzCamera.h"
#include <array>


#define OBJ_DENSITY 0.5f
#define OBJ_FRICTION 0.5f
#define OBJ_RESTITUTION 0.3f
#define GROUND_FRICTION 0.5F
#define WOOD_DENSITY 0.5f
#define WOOD_FRICTION 0.5f

namespace zz
{
    using Coord = double;
    using N = uint32_t;
    using Point = std::array<Coord, 2>;

    b2World* Box2dWorld::mBox2dWorld = new b2World(b2Vec2(0.0f, 5.f));
    std::vector<b2Body*> Box2dWorld::mBodys;

    std::vector<Box2dWorld::StaticElementsBody> Box2dWorld::mElementsBodys;
    DrawBox2dWorld* Box2dWorld::mDrawBow2dBody;
    //bool Box2dWorld::StaticElementsBody::isDestroy = false;

    Box2dWorld::Box2dWorld()
    {
    }

    Box2dWorld::~Box2dWorld()
    {

    }

    void Box2dWorld::Initialize()
    {
        mDrawBow2dBody = new DrawBox2dWorld();
        mDrawBow2dBody->SetFlags(b2Draw::e_shapeBit);
        mBox2dWorld->SetDebugDraw(mDrawBow2dBody);
    }

    void Box2dWorld::Update()
    {
        mBox2dWorld->Step(0.1f, 6, 2);

        for (StaticElementsBody& body : mElementsBodys)
        {
            b2Vec2 linearVelocity = body.body->GetLinearVelocity();
            float angularVelocity = body.body->GetAngularVelocity();

            if (linearVelocity.Length() > 0 || abs(angularVelocity) > 0)
            {
                PixelWorld::DeleteStaticElement(body.elementsInfo);

                float centerX = body.body->GetPosition().x;
                float centerY = body.body->GetPosition().y;
                float angle = body.body->GetAngle();

                float sin = std::sin(angle);
                float cos = std::cos(angle);

                std::vector<StaticElementInfo> vec;

                body.elementsInfo.clear();
                for (Element& element : body.elements)
                {
                    float relativeX = element.x * cos - element.y * sin;
                    float relativeY = element.x * sin + element.y * cos;

                    int posX = (int)(floor(relativeX + 0.5f) + (int)centerX);
                    int posY = (int)(floor(relativeY + 0.5f) + (int)centerY);

                    body.elementsInfo.push_back({ element, posX, posY });
                }

                PixelWorld::MoveStaticElement(body.elementsInfo);
            }
        }
    }

    void Box2dWorld::Render()
    {
        renderer::TransformCB trCB = {};
        trCB.mView = Camera::GetGpuViewMatrix();
        trCB.mProjection = Camera::GetGpuProjectionMatrix();

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
        cb->SetBufferData(&trCB);
        cb->BindConstantBuffer(eShaderStage::VS);
        cb->BindConstantBuffer(eShaderStage::PS);

        mBox2dWorld->DebugDraw();
    }

    void Box2dWorld::InitializePresentWorld()
    {
        for (auto elementsBody : mElementsBodys)
        {
            delete elementsBody.isDestroy;
            elementsBody.isDestroy = nullptr;

            mBox2dWorld->DestroyBody(elementsBody.body);
            elementsBody.body = nullptr;
        }
        mElementsBodys.clear();
        std::vector<StaticElementsBody>().swap(mElementsBodys);
        
        for (b2Body* body : mBodys)
        {
            mBox2dWorld->DestroyBody(body);
        }
        mBodys.clear();
        std::vector<b2Body*>().swap(mBodys);
        

        delete mDrawBow2dBody;
        mDrawBow2dBody = nullptr; 

        delete mBox2dWorld;
        mBox2dWorld = nullptr;

        mBox2dWorld = new b2World(b2Vec2(0.0f, 5.f));
        mDrawBow2dBody = new DrawBox2dWorld();
        mDrawBow2dBody->SetFlags(b2Draw::e_shapeBit);
        mBox2dWorld->SetDebugDraw(mDrawBow2dBody);
    }

    void Box2dWorld::Release()
    {
        for (int i = 0; i < mElementsBodys.size(); i++)
        {
            delete mElementsBodys[i].isDestroy;
        }

        delete mDrawBow2dBody;
        delete mBox2dWorld;
    }

    void Box2dWorld::CreateBody(b2Body*& body, eBodyShape shape, Vector2 scale, Vector3 pos)
    {
        float angle = 0.f;

        switch (shape)
        {
        case eBodyShape::Box:
        {
            b2PolygonShape shape;
            shape.SetAsBox(scale.x / 2.f, scale.y / 2.f, { 0,0 }, angle);

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos.x, -pos.y);

            body = mBox2dWorld->CreateBody(&bodyDef);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            fixtureDef.density = OBJ_DENSITY;
            fixtureDef.friction = OBJ_FRICTION;
            fixtureDef.restitution = OBJ_RESTITUTION;
            fixtureDef.filter.categoryBits = 0x0002;
            fixtureDef.filter.maskBits = 0xFFFF;

            body->CreateFixture(&fixtureDef);

            break;
        }
        case eBodyShape::Circle:
        {
            b2CircleShape circleShape;
            circleShape.m_radius = scale.x / 2.f;
            circleShape.m_p.Set(0, 0);

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pos.x, -pos.y);

            body = mBox2dWorld->CreateBody(&bodyDef);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = OBJ_DENSITY;
            fixtureDef.friction = OBJ_FRICTION;
            fixtureDef.restitution = OBJ_RESTITUTION;
            fixtureDef.filter.categoryBits = 0x0002;
            fixtureDef.filter.maskBits = 0xFFFF;

            body->CreateFixture(&fixtureDef);

            break;
        }
        default: break;
        }

    }

    void Box2dWorld::DeleteBody(b2Body* body)
    {
        mBox2dWorld->DestroyBody(body);
    }

    void Box2dWorld::RenewalChunkBody(std::array<std::bitset<64>, 64>& points, int offsetX, int offsetY, std::vector<b2Body*>& bodies)
    {
        for (int i = 0; i < bodies.size(); i++)
        {
            mBox2dWorld->DestroyBody(bodies[i]);
        }
        bodies.clear();

        std::vector<std::vector<cv::Point>> contours = getContours(points, 64, 64, offsetX, offsetY);

        int cnt = 0;

        for (auto& contour : contours)
        {
            std::vector<cv::Point> approxCurve;
            double epsilon = 2; // 최대 거리 (여기서는 원래 곡선의 길이의 10%로 설정)
            cv::approxPolyDP(contour, approxCurve, epsilon, true);

            auto x_min_it = std::min_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
            auto x_max_it = std::max_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
            auto y_min_it = std::min_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });
            auto y_max_it = std::max_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });

            cv::Rect bounds(*x_min_it, cv::Point(x_max_it->x + 1, y_max_it->y + 1));

            std::vector<std::vector<Point>> polygon;
            std::vector<Point> pa;
            for (auto pp : approxCurve)
            {
                pa.push_back({ (Coord)pp.x, (Coord)pp.y });
            }
            polygon.push_back(pa);

            int size = polygon[0].size();

            if (size > 2)
            {
                b2Vec2 pointA(polygon[cnt][size - 1].front(), polygon[cnt][size - 1].back());
                b2Vec2 pointB(polygon[cnt][0].front(), polygon[cnt][0].back());

                b2Vec2 center = 0.5f * (pointA + pointB);
                float width = (pointB - pointA).Length();
                float angle = atan2(pointB.y - pointA.y, pointB.x - pointA.x);

                b2PolygonShape shape;
                shape.SetAsBox(width / 2, 0.5f, { 0,0 }, angle); // Here, 0.5f is the half-height of the rectangle.

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody; // Assuming it's a static body, like ground.
                bodyDef.position.Set(center.x, center.y);

                b2Body* body = mBox2dWorld->CreateBody(&bodyDef);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &shape;
                fixtureDef.friction = GROUND_FRICTION;
                fixtureDef.filter.categoryBits = 0x0001;
                fixtureDef.filter.maskBits = 0x0002;

                body->CreateFixture(&fixtureDef);

                bodies.push_back(body);
            }
            for (size_t i = 0; i < size - 1; ++i)
            {
                b2Vec2 pointA(polygon[cnt][i].front(), polygon[cnt][i].back());
                b2Vec2 pointB(polygon[cnt][i + 1].front(), polygon[cnt][i + 1].back());

                b2Vec2 center = 0.5f * (pointA + pointB);
                float width = (pointB - pointA).Length();
                float angle = atan2(pointB.y - pointA.y, pointB.x - pointA.x);

                b2PolygonShape shape;
                shape.SetAsBox(width / 2, 0.5f, { 0,0 }, angle); // Here, 0.5f is the half-height of the rectangle.

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody; // Assuming it's a static body, like ground.
                bodyDef.position.Set(center.x, center.y);

                b2Body* body = mBox2dWorld->CreateBody(&bodyDef);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &shape;
                fixtureDef.friction = GROUND_FRICTION;
                fixtureDef.filter.categoryBits = 0x0001;
                fixtureDef.filter.maskBits = 0x0002;

                body->CreateFixture(&fixtureDef);

                bodies.push_back(body);
            }
            //cnt++;
        }
    }

    void Box2dWorld::ReconstructBody()
    {
        std::vector<int> list;

        for (int i = 0; i < mElementsBodys.size(); i++)
        {        
            if (mElementsBodys[i].elements.size() <= 5)
            {
                PixelWorld::DeleteStaticElement(mElementsBodys[i].elementsInfo);
                list.push_back(i);
            }
            else if (*mElementsBodys[i].isDestroy)
            {
                PixelWorld::DeleteStaticElement(mElementsBodys[i].elementsInfo);
                ReconstructBody(mElementsBodys[i]);
                list.push_back(i);
            }
        }
        std::sort(list.begin(), list.end(), std::greater<int>());

        for (int i = 0; i < list.size(); i++)
        {
            mBox2dWorld->DestroyBody(mElementsBodys[list[i]].body);
            delete mElementsBodys[list[i]].isDestroy;
            mElementsBodys.erase(mElementsBodys.begin() + list[i]);
        }
    }

    void Box2dWorld::ReconstructBody(StaticElementsBody body)
    {
        bool isFirstFixture = true;
        b2AABB aabb = {};

        float bodyAngle = body.body->GetAngle();
        for (b2Fixture* fixture = body.body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            if (isFirstFixture)
            {
                aabb = fixture->GetAABB(0);
                isFirstFixture = false;
            }
            else
            {
                aabb.Combine(aabb, fixture->GetAABB(0));
            }
        }

        float width = (float)(int)(aabb.upperBound.x - aabb.lowerBound.x + 1);
        float height = (float)(int)(aabb.upperBound.y - aabb.lowerBound.y + 1);

        float offsetX = floor(aabb.lowerBound.x + 0.5f);
        float offsetY = floor(aabb.lowerBound.y + 0.5f);
        std::vector<std::vector<Element>> elementsArray;

        elementsArray.resize((int)width, std::vector<Element>((int)height));

        float x = floor(body.body->GetPosition().x + 0.5f);
        float y = floor(body.body->GetPosition().y + 0.5f);

        float x1 = (x - offsetX);
        float y1 = (y - offsetY);

        std::vector<cv::Point> points;

        // 나중에 element 멤버변수로 구별
        for (auto element : body.elements)
        {
            if (element.SolidType != eSolidType::DYNAMIC)
                continue;


            if (x1 + element.x < 0 || y1 + element.y < 0 ||
                y1 + element.y >= height || x1 + element.x >= width)
                continue;

            elementsArray[x1 + element.x][y1 + element.y] = element;
            points.push_back(cv::Point((int)(element.x + x1), (int)(element.y + y1)));
        }

        std::vector<std::vector<cv::Point>> contours = getContours(points, width, height);
        auto insidePointsForEachContour = getInsidePointsForEachContour(cv::Mat::zeros(height, width, CV_8UC1), contours);

        std::vector<cv::Vec6f> allTriangles;

        int p = 0;

        if (contours.size() >= 2)
        {
            int op = 0;
        }
        for (auto& contour : contours)
        {
            std::vector<cv::Point> approxCurve;
            double epsilon = 2.5; // 최대 거리 (여기서는 원래 곡선의 길이의 10%로 설정)
            cv::approxPolyDP(contour, approxCurve, epsilon, true);

            auto x_min_it = std::min_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
            auto x_max_it = std::max_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
            auto y_min_it = std::min_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });
            auto y_max_it = std::max_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });

            cv::Rect bounds(*x_min_it, cv::Point(x_max_it->x + 1, y_max_it->y + 1));

            std::vector<std::vector<Point>> polygon;
            std::vector<Point> pa;
            for (auto pp : approxCurve)
            {
                pa.push_back({ (Coord)pp.x, (Coord)pp.y });
            }
            polygon.push_back(pa);
            std::vector<N> indices = mapbox::earcut<N>(polygon);
            cv::Subdiv2D subdiv(bounds);

            std::vector<cv::Vec6f> triangleList;

            for (int i = 0; i < indices.size(); i += 3)
            {
                triangleList.push_back
                (
                    {
                        (float)polygon[0][indices[i]].front(),      (float)polygon[0][indices[i]].back(),
                        (float)polygon[0][indices[i + 1]].front(),  (float)polygon[0][indices[i + 1]].back(),
                        (float)polygon[0][indices[i + 2]].front(),  (float)polygon[0][indices[i + 2]].back(),
                    }
                );
            }

            StaticElementsBody newBody;

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            newBody.body = mBox2dWorld->CreateBody(&bodyDef);
            newBody.isDestroy = new bool();

            for (auto& triangle : triangleList)
            {
                b2Vec2 vertices[3] = {};
                for (int i = 0; i < 3; ++i)
                {
                    vertices[i].Set(triangle[i * 2] + floor(offsetX + 0.5f), triangle[i * 2 + 1] + floor(offsetY + 0.5f));
                }

                b2PolygonShape dynamicTriangle;
                dynamicTriangle.Set(vertices, 3);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicTriangle;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.3f;

                newBody.body->CreateFixture(&fixtureDef);
            }

            b2Vec2 a = newBody.body->GetWorldCenter();
            newBody.body->SetTransform(a, bodyAngle);

            for (b2Fixture* fixture = newBody.body->GetFixtureList(); fixture; )
            {
                b2Fixture* nextFixture = fixture->GetNext();

                b2PolygonShape* oldShape = static_cast<b2PolygonShape*>(fixture->GetShape());
                if (oldShape && oldShape->m_type == b2Shape::e_polygon)
                {
                    b2Vec2 newVertices[b2_maxPolygonVertices];
                    for (int i = 0; i < oldShape->m_count; ++i)
                    {
                        newVertices[i] = b2Vec2(oldShape->m_vertices[i].x - a.x, oldShape->m_vertices[i].y - a.y);
                    }

                    b2PolygonShape newShape;
                    newShape.Set(newVertices, oldShape->m_count);

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &newShape;
                    fixtureDef.density = 1.0f;
                    fixtureDef.friction = 0.3f;
                    fixtureDef.filter.categoryBits = 0x0002;
                    fixtureDef.filter.maskBits = 0xFFFF;

                    newBody.body->DestroyFixture(fixture);
                    newBody.body->CreateFixture(&fixtureDef);
                }

                fixture = nextFixture;
            }

            std::vector<Element> bodyElements;

            float bX = floor(newBody.body->GetPosition().x + 0.5f);
            float bY = floor(newBody.body->GetPosition().y + 0.5f);


            for (auto i : insidePointsForEachContour[p])
            {
                elementsArray[i.x][i.y].x = x - bX + elementsArray[i.x][i.y].x;
                elementsArray[i.x][i.y].y = y - bY + elementsArray[i.x][i.y].y;
                elementsArray[i.x][i.y].destoryBody = newBody.isDestroy;
                //elementsArray[i.x][i.y].
                bodyElements.push_back(elementsArray[i.x][i.y]);
            }

            float centerX = newBody.body->GetPosition().x;
            float centerY = newBody.body->GetPosition().y;
            float angle = newBody.body->GetAngle();

            float sin = std::sin(angle);
            float cos = std::cos(angle);

            newBody.elements = std::move(bodyElements);

            std::vector<StaticElementInfo> vec;

            mElementsBodys.push_back(std::move(newBody));

            for (auto& element : mElementsBodys.back().elements)
            {
                element.destoryBody = newBody.isDestroy;

                float relativeX = element.x * cos - element.y * sin;
                float relativeY = element.x * sin + element.y * cos;

                int posX = (int)(floor(relativeX + 0.5f) + (int)centerX);
                int posY = (int)(floor(relativeY + 0.5f) + (int)centerY);

                mElementsBodys.back().elementsInfo.push_back({ element, posX, posY });
            }

            PixelWorld::MoveStaticElement(mElementsBodys.back().elementsInfo);

            p++;
        }
    }

    std::vector<std::vector<cv::Point>> Box2dWorld::getContours(const std::vector<cv::Point>& points, float width, float height)
    {
        cv::Mat img = cv::Mat::zeros(height, width, CV_8UC1);

        for (const auto& pt : points)
        {
            img.at<uchar>(pt) = 255; 
        }

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        return contours;
    }
    std::vector<std::vector<cv::Point>> Box2dWorld::getContours(cv::Mat& image)
    {
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        return contours;
    }
    std::vector<std::vector<cv::Point>> Box2dWorld::getContours(const std::array<std::bitset<64>, 64>& points, float width, float height, float offsetX, float offsetY)
    {
        cv::Mat img = cv::Mat::zeros(height, width, CV_8UC1);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (points[y][x])
                {
                    img.at<uchar>(y, x) = 255;
                }
            }
        }

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (auto& contour : contours)
        {
            for (auto& point : contour)
            {
                point.x += offsetX;
                point.y += offsetY;
            }
        }

        return contours;
    }

    std::vector<std::vector<cv::Point>> Box2dWorld::getInsidePointsForEachContour(const cv::Mat& image, const std::vector<std::vector<cv::Point>>& contours)
    {
        std::vector<std::vector<cv::Point>> insidePointsForEachContour;

        for (const auto& contour : contours)
        {
            cv::Mat contourImage = cv::Mat::zeros(image.size(), CV_8U);
            cv::drawContours(contourImage, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), cv::FILLED);

            std::vector<cv::Point> insidePoints;
            for (int y = 0; y < contourImage.rows; y++)
            {
                for (int x = 0; x < contourImage.cols; x++)
                {
                    if (contourImage.at<uchar>(y, x) == 255)
                    {
                        insidePoints.push_back(cv::Point(x, y));
                    }   
                }
            }

            insidePointsForEachContour.push_back(insidePoints);
        }

        return insidePointsForEachContour;
    }

    void Box2dWorld::Draw(int x, int y)
    {
        StaticElementsBody body;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(x, y);
        bodyDef.fixedRotation = false;
        body.body = mBox2dWorld->CreateBody(&bodyDef);

        b2PolygonShape  dynamicBox;
        dynamicBox.SetAsBox(5.5f, 5.5f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.5f;
        fixtureDef.friction = 0.5f;
        fixtureDef.filter.categoryBits = 0x0002;
        fixtureDef.filter.maskBits = 0xFFFF;

        body.body->CreateFixture(&fixtureDef);
        body.isDestroy = new bool();
        *body.isDestroy = false;

        body.elements.resize(121);
        int cnt = 0;
        for (int i = -5; i <= 5; i++)
        {
            for (int j = -5; j <= 5; j++)
            {
                Element a = ROCK;
                a.x = i;
                a.y = j;

                a.destoryBody = body.isDestroy;
                body.elements[cnt] = a;

                body.elementsInfo.push_back({ body.elements[cnt], (int)a.x + x, (int)a.y + y});
                cnt++;
            }
        }
        mElementsBodys.push_back(std::move(body));
        PixelWorld::MoveStaticElement(mElementsBodys.back().elementsInfo);
    }

    void Box2dWorld::Draw2(int x, int y)
    {
        cv::Mat img = cv::imread("..\\Resources\\Texture\\Tree\\tree_spruce_5.png", cv::IMREAD_COLOR);

        StaticElementsBody body;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(x, y);
        bodyDef.fixedRotation = false;
        body.body = mBox2dWorld->CreateBody(&bodyDef);

        b2PolygonShape  dynamicBox;
        dynamicBox.SetAsBox((float)img.cols / 2, (float)img.rows / 2);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.5f;
        fixtureDef.friction = 0.5f;
        body.body->CreateFixture(&fixtureDef);

        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                cv::Vec3b color = img.at<cv::Vec3b>(i, j);

                if (color[0] == 0 && color[1] == 0 && color[2] == 0)
                {
                    continue;
                }
                else
                {
                    Element a = WOOD;
                    a.x = j - img.cols / 2;
                    a.y = i - img.rows / 2;
                    a.Color = ((uint32_t)0xFF << 24) | ((uint32_t)color[2] << 16) | ((uint32_t)color[1] << 8) | (uint32_t)color[0];
                    body.elements.push_back(a);
                    //body.elementsInfo.push_back({ body.elements.back(), (int)a.x + x, (int)a.y + y});
                }

            }
        }


        //ReconstructBody(body);
        mElementsBodys.push_back(body);
    }

    void Box2dWorld::Draw(int x, int y, cv::Mat& mask_image, cv::Mat& visual_image, Element& element)
    {
        std::vector<std::vector<cv::Point>> contours = getContours(mask_image);
        auto insidePointsForEachContour = getInsidePointsForEachContour(cv::Mat::zeros(mask_image.rows, mask_image.cols, CV_8UC1), contours);

        std::vector<cv::Vec6f> allTriangles;

        int p = 0;
        if (contours.size() >= 2)
        {
            int op = 0;
        }
        for (auto& contour : contours)
        {

            std::vector<cv::Point> approxCurve;
            double epsilon = 1.0;
            cv::approxPolyDP(contour, approxCurve, epsilon, true);

            auto x_min_it = std::min_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
            auto x_max_it = std::max_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.x < b.x; });
            auto y_min_it = std::min_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });
            auto y_max_it = std::max_element(approxCurve.begin(), approxCurve.end(), [](const cv::Point& a, const cv::Point& b) { return a.y < b.y; });

            cv::Rect bounds(*x_min_it, cv::Point(x_max_it->x + 1, y_max_it->y + 1));

            std::vector<std::vector<Point>> polygon;
            std::vector<Point> pa;
            for (auto pp : approxCurve)
            {
                pa.push_back({ (Coord)pp.x, (Coord)pp.y });
            }
            polygon.push_back(pa);
            std::vector<N> indices = mapbox::earcut<N>(polygon);
            cv::Subdiv2D subdiv(bounds);

            std::vector<cv::Vec6f> triangleList;

            for (int i = 0; i < indices.size(); i += 3)
            {
                triangleList.push_back
                (
                    {
                        (float)polygon[0][indices[i]].front(),      (float)polygon[0][indices[i]].back(),
                        (float)polygon[0][indices[i + 1]].front(),  (float)polygon[0][indices[i + 1]].back(),
                        (float)polygon[0][indices[i + 2]].front(),  (float)polygon[0][indices[i + 2]].back(),
                    }
                );
            }

            StaticElementsBody newBody;
            newBody.isDestroy = new bool();
            *newBody.isDestroy = false;
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            newBody.body = mBox2dWorld->CreateBody(&bodyDef);

            std::vector<std::pair<float, float>> triangleCenters;

            float avgX = 0.0f;
            float avgY = 0.0f;

            for (auto& triangle : triangleList)
            {
                float sumX = 0;
                float sumY = 0;

                for (int i = 0; i < 3; i++)
                {
                    sumX += triangle[i * 2] + x;
                    sumY += triangle[i * 2 + 1] + y;
                }
                triangleCenters.push_back({ sumX / 3.f, sumY / 3.f });
            }

            float totalX = 0.f;
            float totalY = 0.f;

            for (const auto& center : triangleCenters)
            {
                totalX += center.first;
                totalY += center.second;
            }

            avgX = totalX / triangleCenters.size();
            avgY = totalY / triangleCenters.size();

            newBody.body->SetTransform({ avgX, avgY }, 0);

            float bX = floor(avgX + 0.5f);
            float bY = floor(avgY + 0.5f);

            for (auto& triangle : triangleList)
            {
                b2Vec2 vertices[3] = {};

                for (int i = 0; i < 3; ++i)
                {
                    vertices[i].Set(triangle[i * 2] + x - bX, triangle[i * 2 + 1] + y - bY);
                }

                b2PolygonShape staticTriangle;
                staticTriangle.Set(vertices, 3);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &staticTriangle;
                fixtureDef.density = WOOD_DENSITY;
                fixtureDef.friction = WOOD_FRICTION;
                fixtureDef.filter.categoryBits = 0x0002;
                fixtureDef.filter.maskBits = 0xFFFF;

                newBody.body->CreateFixture(&fixtureDef);
            }
         

            std::vector<Element> bodyElements;

            int pointCnt = insidePointsForEachContour[p].size();

            bodyElements.reserve(pointCnt);
            for(int i=0; i< pointCnt; i++)
            {
                auto& point = insidePointsForEachContour[p][i];

                cv::Vec4b visual_color = visual_image.at<cv::Vec4b>(point.y, point.x);

                uint32_t converted_color =
                    (visual_color[3] << 24) |
                    (visual_color[2] << 16) |
                    (visual_color[1] << 8)  |
                    (visual_color[0]);

                Element newElement = element;
                newElement.x = point.x + x - bX;
                newElement.y = point.y + y - bY;
                newElement.Color = converted_color;
                bodyElements.push_back(newElement);

                //newBody.elementsInfo.push_back({ bodyElements[i], point.x + x, point.y + y});
            }
                
            newBody.elements = std::move(bodyElements);
            mElementsBodys.push_back(std::move(newBody));

            for (auto& elem : mElementsBodys.back().elements)
            {
                elem.destoryBody = newBody.isDestroy;
                mElementsBodys.back().elementsInfo.push_back({ elem, (int)(elem.x + bX),(int)(elem.y + bY) });
            }

            PixelWorld::MoveStaticElement(mElementsBodys.back().elementsInfo);
            p++;
        }
    }

    double Box2dWorld::perpendicularDistance(const Position& pt, const Position& lineStart, const Position& lineEnd)
    {
        double dx = lineEnd.x - lineStart.x;
        double dy = lineEnd.y - lineStart.y;

        // Normalize
        double mag = std::sqrt(dx * dx + dy * dy);
        if (mag > 0.0) {
            dx /= mag; dy /= mag;
        }
        double pvx = pt.x - lineStart.x;
        double pvy = pt.y - lineStart.y;

        // Get dot product (project pv onto normalized direction)
        double pvdot = dx * pvx + dy * pvy;

        // Scale line direction vector and subtract it from pv
        double ax = pvx - pvdot * dx;
        double ay = pvy - pvdot * dy;

        return std::sqrt(ax * ax + ay * ay);
    }
    void Box2dWorld::douglasPeucker(const std::vector<Position>& points, double epsilon, std::vector<Position>& out)
    {
        if (points.size() < 3) {
            return;
        }

        int index = -1;
        double maxDist = 0;
        for (int i = 1; i < points.size() - 1; i++)
        {
            double dist = perpendicularDistance(points[i], points[0], points.back());
            if (dist > maxDist)
            {
                index = i;
                maxDist = dist;
            }
        }

        if (maxDist > epsilon)
        {
            std::vector<Position> recResults1;
            std::vector<Position> recResults2;
            std::vector<Position> points1(points.begin(), points.begin() + index + 1);
            std::vector<Position> points2(points.begin() + index, points.end());

            douglasPeucker(points1, epsilon, recResults1);
            douglasPeucker(points2, epsilon, recResults2);

            out.assign(recResults1.begin(), recResults1.end() - 1);
            out.insert(out.end(), recResults2.begin(), recResults2.end());
            if (out.size() < 2)
            {
                throw std::runtime_error("Problem assembling output");
            }
        }
        else
        {
            out.clear();
            out.push_back(points[0]);
            out.push_back(points.back());
        }
    }


    void DrawBox2dWorld::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
    }
    void DrawBox2dWorld::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        if (vertexCount == 3)
        {
            std::vector<renderer::Vertex> vertexes = {};
            std::vector<UINT> indexes = {};

            for (int i = 0; i < 3; i++)
            {
                renderer::Vertex vertex;
                vertex.pos = math::Vector3(vertices[i].x, -vertices[i].y, 0.f);
                vertex.color = math::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
                //vertexes[i].color = color;

                vertexes.push_back(vertex);
            }
            indexes.push_back(0);
            indexes.push_back(1);
            indexes.push_back(2);

            std::shared_ptr<Shader> shader = ResourceManager::Find<Shader>(L"TriangleShader");
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
            mesh->CreateVertexBuffer(vertexes.data(), static_cast<UINT>(vertexes.size()));
            mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

            mesh->BindBuffer();
            shader->BindShaders();

            mesh->Render();
        }
        else if (vertexCount == 4)
        {
            {
                std::vector<renderer::Vertex> vertexes = {};
                std::vector<UINT> indexes = {};

                for (int i = 0; i < 3; i++)
                {
                    renderer::Vertex vertex;
                    vertex.pos = math::Vector3(vertices[i].x, -vertices[i].y, 0.f);
                    vertex.color = math::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
                    //vertexes[i].color = color;

                    vertexes.push_back(vertex);
                }
                indexes.push_back(0);
                indexes.push_back(1);
                indexes.push_back(2);

                std::shared_ptr<Shader> shader = ResourceManager::Find<Shader>(L"TriangleShader");
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
                mesh->CreateVertexBuffer(vertexes.data(), static_cast<UINT>(vertexes.size()));
                mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

                mesh->BindBuffer();
                shader->BindShaders();
                mesh->Render();
            }

            {
                std::vector<renderer::Vertex> vertexes = {};
                std::vector<UINT> indexes = {};

                for (int i = 1; i < 4; i++)
                {
                    renderer::Vertex vertex;
                    vertex.pos = math::Vector3(vertices[i].x, -vertices[i].y, 0.f);
                    vertex.color = math::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
                    //vertexes[i].color = color;

                    vertexes.push_back(vertex);
                }
                indexes.push_back(1);
                indexes.push_back(2);
                indexes.push_back(0);

                std::shared_ptr<Shader> shader = ResourceManager::Find<Shader>(L"TriangleShader");
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
                mesh->CreateVertexBuffer(vertexes.data(), static_cast<UINT>(vertexes.size()));
                mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

                mesh->BindBuffer();
                shader->BindShaders();
                mesh->Render();
            }
        }
    }
    void DrawBox2dWorld::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
    {
    }
    void DrawBox2dWorld::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
    {
    }
    void DrawBox2dWorld::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    {
    }
    void DrawBox2dWorld::DrawTransform(const b2Transform& xf)
    {
    }
    void DrawBox2dWorld::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
    {
    }
}