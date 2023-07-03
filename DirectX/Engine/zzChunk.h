#pragma once

namespace zz
{
    class Chunk
    {
    public:
        Chunk();
        ~Chunk();
    
        void SetActive() { mActive = true; }
        void SetDeath() { mActive = false; }

        bool isActive() { return mActive; }
    private:
        bool mActive = true;
    };
}