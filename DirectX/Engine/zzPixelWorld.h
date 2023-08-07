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
        struct move
        {
            float x;
            float y;
            Element& element;
            float prevX;
            float prevY;

        };
        PixelWorld();
        ~PixelWorld();

        static void Initialize();
        static void Update();
        static void Release();
        
        static PixelChunk* GetChunk(int x, int y);
        static PixelChunkMap* GetChunkMapDirect(std::pair<int, int> location);
        static std::pair<int, int> GetChunkMapLocation(int x, int y);
        static void RemoveEmptyChunks();

        static Element& GetElement(int x, int y) { return GetChunk(x, y)->GetElement(x, y); }
        static void SwapElement(int x, int y, const Element& element);
        static void SwapElement(int x, int y, int xto, int yto);
        static void InsertElement(int x, int y, const Element& element);
        static void MoveStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements);
        static void DeleteStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements, int index);
        static void SetStaticElements(std::vector<std::vector<Box2dWorld::StaticElementInfo>>* staticElements) { mStaticElements = staticElements; }

        static void KeepAlive(int x, int y) { if (PixelChunk* chunk = GetChunk(x, y)) { chunk->KeepAlive(x, y); }}

        static bool InBounds(int x, int y);
        static bool IsEmpty(int x, int y) { return InBounds(x, y) && GetChunk(x, y)->IsEmpty(x, y);}

        static void DrawPixels();
        static void SetImage(int x, int y, std::shared_ptr<class Texture> texture, std::shared_ptr<class Texture> texture_visual);

        static std::vector<uint8_t>& GetPixelColor() { return mPixelColor; }
        static std::vector<PixelChunkMap*> mChunkMaps;
        static ThreadPool threadPool;
    private:
        static PixelChunkMap* CreateChunkMap(std::pair<int, int> location);

        
        static std::unordered_map<std::pair<int, int>, PixelChunkMap*, pair_hash> mChunkMapLookUp;

        static std::map<char, Element> mElementMap;
        static Element mSelectElement;

        static std::vector<uint8_t> mPixelColor;
        static class PixelGridColor* mImage;    

        static const UINT mChunkMapWidth;
        static const UINT mChunkMapHeight;
        static std::vector<std::vector<Box2dWorld::StaticElementInfo>>* mStaticElements;
    };
}
