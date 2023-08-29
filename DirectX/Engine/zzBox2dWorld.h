#pragma once

#include "zzEngine.h"
#include "zzElement.h"
#include "../External/Box2d/include/box2d.h"
#include <opencv2/opencv.hpp>

namespace zz
{
    class Box2dWorld
    {
        struct StaticElementsBody
        {
            b2Body* body = nullptr;
            std::vector<Element> elements;
        };

    public:       
        struct StaticElementInfo
        {
            Element& element;
            int x;
            int y;
        };

        struct Position 
        {
            double x, y;
        };

        Box2dWorld();
        ~Box2dWorld();

        static void Initialize();
        static void Update();
        static void ReconstructBody(StaticElementsBody body);
        static void ReconstructBody(int index);
        static void Draw(int x, int y);
        static void Draw2(int x, int y);

        static double perpendicularDistance(const Position& pt, const Position& lineStart, const Position& lineEnd);
        static void douglasPeucker(const std::vector<Position>& points, double epsilon, std::vector<Position>& out);

        static std::vector<std::vector<cv::Point>> getContours(const std::vector<cv::Point>& points, float width, float height);
        static std::vector<std::vector<cv::Point>> getInsidePointsForEachContour(const cv::Mat& image, const std::vector<std::vector<cv::Point>>& contours);

        static std::vector<std::vector<StaticElementInfo>>& GetTemp() { return mStaticElements; }
    private:
        static b2World* mBox2dWorld;
        static std::vector<b2Body*> mBodys;
        static std::vector<b2BodyDef*> mBodyDefs;
        static std::vector<b2FixtureDef*> mFixtureDefs;
        static std::vector<b2Shape*> mShapes;

        static std::vector<StaticElementsBody> mElementsBodys;
        static std::vector<std::vector<StaticElementInfo>> mStaticElements;
    };
}
