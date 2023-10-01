#include "zzBox2dWorld.h"
#include "zzPixelWorld.h"
#include "..\External\Triangulation\include\earcut.hpp"

#include "zzMesh.h"
#include "zzShader.h"
#include "zzResourceManager.h"
#include "zzRenderer.h"
#include "zzCamera.h"

#include <array>
namespace zz
{
    using Coord = double;
    using N = uint32_t;
    using Point = std::array<Coord, 2>;

    b2World* Box2dWorld::mBox2dWorld = new b2World(b2Vec2(0.0f, 5.f));
    std::vector<b2Body*> Box2dWorld::mBodys;
    std::vector<b2BodyDef*> Box2dWorld::mBodyDefs;
    std::vector<b2FixtureDef*> Box2dWorld::mFixtureDefs;
    std::vector<b2Shape*> Box2dWorld::mShapes;

    std::vector<Box2dWorld::StaticElementsBody> Box2dWorld::mElementsBodys;
    std::vector<std::vector<Box2dWorld::StaticElementInfo>> Box2dWorld::mStaticElements;
    DrawBox2dWorld* Box2dWorld::mDrawBow2dBody;

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

        StaticElementsBody body;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(107.0f, 377.0f);
        bodyDef.fixedRotation = false;
        body.body = mBox2dWorld->CreateBody(&bodyDef);

        b2PolygonShape  dynamicBox;
        dynamicBox.SetAsBox(20.f, 20.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.5f;
        fixtureDef.friction = 0.5f;
        body.body->CreateFixture(&fixtureDef);

        for (int i = 87; i <= 127; i++)
        {
            for (int j = 357; j <= 397; j++)
            {
                Element a = ROCK;
                a.x = i - 107.0f;
                a.y = j - 377.0f;

                body.elements.push_back(a);
            }
        }

        mElementsBodys.push_back(body);

        {
            StaticElementsBody body;

            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.position.Set(210.0f, 500.0f);
            bodyDef.fixedRotation = false;
            body.body = mBox2dWorld->CreateBody(&bodyDef);

            b2PolygonShape  dynamicBox;
            dynamicBox.SetAsBox(200.f, 2.f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 0.5f;
            fixtureDef.friction = 0.5f;
            body.body->CreateFixture(&fixtureDef);

            for (int i = -200; i <= 200; i++)
            {
                for (int j = -2; j <= 2; j++)
                {
                    Element a = ROCK;
                    a.x = (float)i;
                    a.y = (float)j;

                    body.elements.push_back(a);
                }
            }

            mElementsBodys.push_back(body);
        }
    }

    void Box2dWorld::Update()
    {
        mStaticElements.clear();

        mBox2dWorld->Step(0.1f, 6, 2);

        for (StaticElementsBody& body : mElementsBodys)
        {
            float centerX = body.body->GetPosition().x;
            float centerY = body.body->GetPosition().y;
            float angle = body.body->GetAngle();

            float sin = std::sin(angle);
            float cos = std::cos(angle);

            std::vector<StaticElementInfo> vec;

            for (Element& element : body.elements)
            {
                float relativeX = element.x * cos - element.y * sin;
                float relativeY = element.x * sin + element.y * cos;

                int posX = (int)(floor(relativeX + 0.5f) + (int)centerX);
                int posY = (int)(floor(relativeY + 0.5f) + (int)centerY);

                vec.push_back({ element, posX, posY });
            }

            mStaticElements.push_back(vec);
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

    void Box2dWorld::Release()
    {
        delete mDrawBow2dBody;
        delete mBox2dWorld;
    }

    void Box2dWorld::ReconstructBody(std::vector<int>& a)
    {

        for (int i = 0; i < a.size(); i++)
        {
            ReconstructBody(mElementsBodys[a[i]]);
        }
        
        std::sort(a.begin(), a.end(), std::greater<int>());

        for (int i = 0; i < a.size(); i++)
        {
            mBox2dWorld->DestroyBody(mElementsBodys[a[i]].body);
            mElementsBodys.erase(mElementsBodys.begin() + a[i]);
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

        for (auto element : body.elements)
        {
            if (element.Type != eElementType::SOLID)
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
            double epsilon = 3; // 최대 거리 (여기서는 원래 곡선의 길이의 10%로 설정)
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
                bodyElements.push_back(elementsArray[i.x][i.y]);
            }

            newBody.elements = bodyElements;
            mElementsBodys.push_back(newBody);
            p++;
            //allTriangles.insert(allTriangles.end(), triangleList.begin(), triangleList.end());
        }


       // cv::waitKey(0);

        //std::vector<N> indices = mapbox::earcut<N>(polygon);
        int c = 0;
    }

    std::vector<std::vector<cv::Point>> Box2dWorld::getContours(const std::vector<cv::Point>& points, float width, float height)
    {
        // Assuming all points are within [0, 1000]
        cv::Mat img = cv::Mat::zeros(height, width, CV_8UC1);

        for (const auto& pt : points)
        {
            img.at<uchar>(pt) = 255; // Set each point to white
        }

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

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
        body.body->CreateFixture(&fixtureDef);

        for (int i = -5; i <= 5; i++)
        {
            for (int j = -5; j <= 5; j++)
            {
                Element a = ROCK;
                a.x = i;
                a.y = j;

                body.elements.push_back(a);
            }
        }
        mElementsBodys.push_back(body);
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
                }

            }
        }

        //ReconstructBody(body);
        mElementsBodys.push_back(body);
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