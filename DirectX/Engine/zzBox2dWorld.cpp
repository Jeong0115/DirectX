#include "zzBox2dWorld.h"
#include "zzPixelWorld.h"
#include "..\External\Triangulation\include\earcut.hpp"



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

    Box2dWorld::Box2dWorld()
    {
    }

    Box2dWorld::~Box2dWorld()
    {
    }

    void Box2dWorld::Initialize()
    {
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

        {
            cv::Mat img = cv::imread("C:\\tree_spruce_5.png", cv::IMREAD_COLOR);

            StaticElementsBody body;

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(300.0f, 300.0f);
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
                    // OpenCV는 BGR 순서를 사용
                    cv::Vec3b color = img.at<cv::Vec3b>(i, j);

                    if (color[0] == 0 && color[1] == 0 && color[2] == 0)
                    {
                        continue;
                    }
                    else
                    {
                        Element a = ROCK;
                        a.x = j - img.cols / 2;
                        a.y = i - img.rows / 2;
                        a.Color = ((uint32_t)0xFF << 24) | ((uint32_t)color[2] << 16) | ((uint32_t)color[1] << 8) | (uint32_t)color[0];
                        body.elements.push_back(a);
                    }

                }
            }

            ReconstructBody(body);
        }

    }

    void Box2dWorld::Update()
    {
        mStaticElements.clear();
              
        StaticElementsBody body = mElementsBodys[0];
       
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
            
            int a = 0;
        }
        PixelWorld::SetStaticElements(&mStaticElements);
    }
    void Box2dWorld::ReconstructBody(int index)
    {
        if (index < mElementsBodys.size())
        {
            ReconstructBody(mElementsBodys[index]);

            mBox2dWorld->DestroyBody(mElementsBodys[index].body);
            mElementsBodys.erase(mElementsBodys.begin() + index);
        }
        else
        {
            int a = 0;
        }
    }

    void Box2dWorld::ReconstructBody(StaticElementsBody& body)
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

        float width =   (float)(int)(aabb.upperBound.x - aabb.lowerBound.x + 1);
        float height = (float)(int)(aabb.upperBound.y - aabb.lowerBound.y + 1);

        float offsetX = floor(aabb.lowerBound.x + 0.5f);
        float offsetY = floor(aabb.lowerBound.y + 0.5f);
        std::vector<std::vector<Element>> elementsArray;

        elementsArray.resize((int)width, std::vector<Element>((int)height));

        float x = floor(body.body->GetPosition().x  + 0.5f);
        float y = floor(body.body->GetPosition().y  + 0.5f);

        float x1 = (x - offsetX);
        float y1 = (y - offsetY);

        std::vector<cv::Point> points;

        for (auto element : body.elements)
        {
            if (element.Name == L"Empty") 
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



        cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);

      
        int p = 0;
        for (auto& contour : contours) 
        {
            
            std::vector<cv::Point> approxCurve; // 근사화된 곡선을 저장할 공간
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

         /*   for (const auto& pt : approxCurve)
            {
                subdiv.insert(pt);
            }*/

            std::vector<cv::Vec6f> triangleList;
            
            //subdiv.getTriangleList(triangleList);
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

            // Define the vertices of the triangle
            cv::Point pt[3];



            

            for (auto& triangle : triangleList)
            {
                b2Vec2 vertices[3] = {};
                for (int i = 0; i < 3; ++i) 
                {
                    vertices[i].Set(triangle[i * 2] + floor(offsetX + 0.5f), triangle[i * 2 + 1] + floor(offsetY + 0.5f));
                    pt[i] = cv::Point(triangle[i * 2] * 3,triangle[i * 2 + 1] * 3);
                }

                cv::line(img, pt[0], pt[1], cv::Scalar(255, 0, 0), 2);
                cv::line(img, pt[1], pt[2], cv::Scalar(255, 0, 0), 2);
                cv::line(img, pt[2], pt[0], cv::Scalar(255, 0, 0), 2);

                b2PolygonShape dynamicTriangle;
                dynamicTriangle.Set(vertices, 3);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &dynamicTriangle;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.3f;

                newBody.body->CreateFixture(&fixtureDef); // Fixture를 바디에 추가
            }
            b2Vec2 a = newBody.body->GetWorldCenter();
            newBody.body->SetTransform(a, bodyAngle);


            for (b2Fixture* fixture = newBody.body->GetFixtureList(); fixture; )
            {
                b2Fixture* nextFixture = fixture->GetNext(); // Get next fixture before we destroy the current one

                b2PolygonShape* oldShape = static_cast<b2PolygonShape*>(fixture->GetShape());
                if (oldShape && oldShape->m_type == b2Shape::e_polygon)
                {
                    // Create a new shape
                    b2Vec2 newVertices[b2_maxPolygonVertices];
                    for (int i = 0; i < oldShape->m_count; ++i)
                    {
                        // Modify the vertices as you want here
                        newVertices[i] = b2Vec2(oldShape->m_vertices[i].x - a.x, oldShape->m_vertices[i].y - a.y);
                    }

                    // Create a new fixture
                    b2PolygonShape newShape;
                    newShape.Set(newVertices, oldShape->m_count);

                    // Create a new fixture
                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &newShape;
                    fixtureDef.density = 1.0f;
                    fixtureDef.friction = 0.3f;

                    // Destroy the old fixture and add the new one
                    newBody.body->DestroyFixture(fixture);
                    newBody.body->CreateFixture(&fixtureDef);
                }

                fixture = nextFixture;
            }

            std::vector<Element> bodyElements;

            float bX = floor(newBody.body->GetPosition().x + 0.5f);
            float bY = floor(newBody.body->GetPosition().y + 0.5f);


            for(auto i : insidePointsForEachContour[p])
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

        // Display the image


       // cv::waitKey(0);
       
        //std::vector<N> indices = mapbox::earcut<N>(polygon);

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
            // 테두리에 해당하는 이진 이미지를 만듭니다.
            cv::Mat contourImage = cv::Mat::zeros(image.size(), CV_8U);
            cv::drawContours(contourImage, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), cv::FILLED);

            // contourImage에서 픽셀 값이 255인 점들을 찾습니다.
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

            // 테두리 별로 내부 픽셀들을 저장합니다.
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
        /*StaticElementsBody body;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(x, y);
        bodyDef.fixedRotation = false;
        body.body = mBox2dWorld->CreateBody(&bodyDef);

        b2PolygonShape  dynamicBox;
        dynamicBox.SetAsBox(20.f, 2.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 0.5f;
        fixtureDef.friction = 0.5f;
        body.body->CreateFixture(&fixtureDef);

        for (int i = x - 20; i <= x + 20; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                Element a = ROCK;
                a.x = i - x;
                a.y = j - y;

                body.elements.push_back(a);
            }
        }

        mElementsBodys.push_back(body);*/


        {
            cv::Mat img = cv::imread("C:\\tree_spruce_5.png", cv::IMREAD_COLOR);

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
                    // OpenCV는 BGR 순서를 사용
                    cv::Vec3b color = img.at<cv::Vec3b>(i, j);

                    if (color[0] == 0 && color[1] == 0 && color[2] == 0)
                    {
                        continue;
                    }
                    else
                    {
                        Element a = ROCK;
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
}