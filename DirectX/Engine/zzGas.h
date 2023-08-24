//#pragma once
//
//#include "zzElement.h"
//
//namespace zz
//{
//    class Gas : public Element
//    {
//    public: 
//        Gas();
//        virtual ~Gas();
//
//        virtual void UpdateUI() override;
//        virtual bool InteractElement(Element* target, Position targetPos
//            , bool isFinal, bool isFirst, Position lastPos, int depth) override;
//
//        int GetDensity() { return mDensity; }
//        virtual Element* Clone() { return nullptr; };
//
//        int mDensity;
//        int mDispersionRate;
//
//    private:
//        bool compareDensity(Gas* gasElement);
//        void swapGas(Position targetPos, Position lastPos);
//        int getAdditional(float val);
//        float getAverageVelOrGravity(float vel, float targetvel);
//        bool iterateToAdditional(int startingX, int startingY, int distance);
//
//        
//    };
//
//}
