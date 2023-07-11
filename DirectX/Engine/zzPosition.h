#pragma once

namespace zz
{
    struct Position
    {
        int x;
        int y;

        Position()
            : x(0)
            , y(0)
        {}
        
        Position(int x, int y)
            : x(x)
            , y(y)
        {}


        Position operator+ (Position other)
        {
            return Position(x + other.x, y + other.y);
        }

        Position operator+= (Position other)
        {
            return Position(x += other.x, y += other.y);
        }

        Position operator-= (Position other)
        {
            return Position(x -= other.x, y -= other.y);
        }

        Position operator- (Position other)
        {
            return Position(x - other.x, y - other.y);
        }

        bool operator== (Position other)
        {
            if (x == other.x && y == other.y)
                return true;
            else
                return false;
        }

        Position operator* (int i)
        {
            return Position(x * i, y * i);
        }

        Position operator- (int i)
        {
            return Position(x - i, y - i);
        }

        Position operator+ (int i)
        {
            return Position(x + i, y + i);
        }

        //Position operator* (int i)
        //{
        //    return Position(x * i, y * i);
        //}

        Position operator/ (int i)
        {
            return Position(x / i, y / i);
        }

        Position operator/ (Position other)
        {
            return Position(x / other.x, y / other.y);
        }

        void operator*=(const Position& other)
        {
            x *= other.x;
            y *= other.y;
        }

        void operator*=(const int& value)
        {
            x *= value;
            y *= value;
        }
    };
}