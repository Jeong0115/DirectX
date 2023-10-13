#pragma once

#include "zzEngine.h"
#include "zzElement.h"
#include "zzPixelChunk.h"
#include "zzPixelChunkMap.h"
#include "zzThreadPool.h"
#include "zzBox2dWorld.h"

namespace zz
{
    struct pair_hash 
    {
        template<typename T1, typename T2>
        size_t operator() (const std::pair<T1, T2>& pair) const 
        {
            return (std::hash<T1>()(pair.first) * 0x1f1f1f1f) ^ std::hash<T2>()(pair.second);
        }
    };
    class PixelWorld
    {
    public:
        struct MaterialImage
        {
            cv::Mat image;

            int width;
            int height;

            int indexX;
            int indexY;

            int imageBits;
        };
        //struct move
        //{
        //    float x;
        //    float y;
        //    Element& element;
        //    float prevX;
        //    float prevY;

        //};

        enum class RotateOption
        {
            Right90,
            Right180,
            Right270,
        };
        PixelWorld();
        ~PixelWorld();

        static void Initialize();
        static void Update();
        static void Release();
        
        static PixelChunk* GetChunk(int x, int y);
        static PixelChunkMap* GetChunkMapDirect(std::pair<int, int> location);
        static std::pair<int, int> GetChunkMapLocation(int x, int y);

        static Element& GetElement(int x, int y) { 
            if (GetChunk(x, y) == nullptr)
                return ROCK;
            else
                return GetChunk(x, y)->GetElement(x, y); 
        }

        static void InsertElement(int x, int y, const Element& element);
        static void InsertElementIfEmpty(int x, int y, const Element& element);
        static void DeleteElement(int x, int y);
        static void MoveStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements);
        static void DeleteStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements);
        
        static void CreateNewWorld();
        static void CreateNextWorld();

        static void InsertElementFromImage(int y, int x, const cv::Mat& image, Element& element);
        static void InsertElementFromImage(int y, int x, const cv::Mat& image, const cv::Mat& lineImage, Element& element, int type);
        static uint32_t Vec3bToColor(const cv::Vec3b& vec3b);
        static uint32_t Vec4bToColor(const cv::Vec4b& vec4b);
        static void RoatateImage(RotateOption option, cv::Mat& image);

        static void DrawRandomSceneImage(const cv::Mat& material_image, const cv::Mat& visual_image, int x, int y);
        static void InsertElementFromWangColor(uint32_t wangColor, int x, int y);

        static void LoadRandomScene_01(int x, int y);
        static void LoadRandomScene_02(int x, int y);
        static void LoadRandomScene_03(int x, int y);

        static void KeepAlive(int x, int y) { if (PixelChunk* chunk = GetChunk(x, y)) { chunk->KeepAlive(x, y); }}

        static bool InBounds(int x, int y);
        static bool IsEmpty(int x, int y) { return InBounds(x, y) && GetChunk(x, y)->IsEmpty(x, y);}

        static void RenewalChunkBody(int x, int y);

        static void DrawPixels();
        static void SetImage(int x, int y, std::shared_ptr<class Texture> texture, std::shared_ptr<class Texture> texture_visual);

        static std::vector<uint32_t>& GetPixelColor() { return mPixelColor; }
        static std::uint32_t& GetPixelColor(size_t index) { return mPixelColor[index]; }

        static std::vector<PixelChunkMap*> mChunkMaps;
        static ThreadPool threadPool;
        static void Temp();
        static uint16_t FrameCount;

    private:
        static PixelChunkMap* CreateChunkMap(std::pair<int, int> location);
        static uint32_t getMaterialColor(const std::wstring& material_name);
        static void loadMaterialImage();
        static void createVegetation(int x, int y);

        static std::unordered_map<std::pair<int, int>, PixelChunkMap*, pair_hash> mChunkMapLookUp;
        static std::unordered_map<std::wstring, MaterialImage*> mMaterialImages;

        static std::map<char, Element> mElementMap;
        static Element mSelectElement;

        static std::vector<uint32_t> mPixelColor;
        static class PixelGridColor* mImage;    

        static const UINT mChunkMapWidth;
        static const UINT mChunkMapHeight;

        static float mTime;
        //static std::vector<std::vector<Box2dWorld::StaticElementInfo>>& mStaticElements;
    };
}
