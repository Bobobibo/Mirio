
#include <iostream>
#include "AllegroUtil.hpp"
#include <windows.h>
#include <cstdlib>

using namespace std;

const int FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;

class Figure
{
protected:
    double x_;
    double y_;
    double dx_;
    double dy_;

public:
    Figure()
    {
        Reset();
    }
    virtual ~Figure()
    {
    }

    void Reset()
    {
        x_ = rand() % SCREEN_W;
        y_ = rand() % SCREEN_H;
        dx_ = 10.0 - rand() % 21;
        dy_ = 10.0 - rand() % 21;
    }

    virtual void Draw(){}

    virtual void Move()
    {
        x_ += dx_;
        y_ += dy_;
        if ( ( x_ < 1.0 ) || ( x_ > SCREEN_W ) ||
             ( y_ < 1.0 ) || ( y_ > SCREEN_H ) )
        {
            Reset();
        }
    };
};


typedef Figure * PFigure;

class Square : public Figure
{
protected:
    double a_;
public:
    Square( double a ) :
        Figure(),
        a_( a )
    {
    }
    virtual void Draw()
    {
        double half = a_ / 2;
        al_draw_filled_rectangle( x_ - half, y_ - half,
            x_ + half, y_ + half, al_map_rgb( 255, 0, 0 ) );
    }
};

class Circle : public Figure
{
protected:
    double r_;
    unsigned char color_;
public:
    Circle( double r ) :
        Figure(),
        r_( r ),
        color_( rand() % 256 )
    {
    }
    virtual void Draw()
    {
        ++color_;
        al_draw_filled_circle( x_, y_, r_, al_map_rgb( 0, color_, 0 ) );
    }
};

const int MAX = 100;
class ScreenSaver
{
private:
    PFigure figures[MAX];
    int size_;

public:
    ScreenSaver() :
        size_( 0 )
    {
        // Set to null all pointers
        memset( figures, 0, sizeof( figures ) );
    }
    virtual ~ScreenSaver()
    {
        for( int i = 0; i < size_; ++i )
        {
            delete figures[i];
            figures[i] = 0;
        }
    }

    void Draw()
    {
        al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
        for( int i = 0; i < size_; ++i )
        {
            figures[i]->Draw();
        }
    }
    void Next()
    {
        for( int i = 0; i < size_; ++i )
        {
            figures[i]->Move();
        }
    }

    void Add( Figure *f )
    {
        if ( size_ >= MAX )
        {
            return;
        }
        figures[ size_ ] = f;
        ++size_;
    }

};

ScreenSaver ss;

void fps()
{
    ss.Next();
}
void draw()
{
    ss.Draw();
}

int fu(int argc, char **argv)
{
    srand( time(0) );
    if( !InitAllegro( SCREEN_W, SCREEN_H, FPS ) )
    {
        DestroyAllegro();
        return 1;
    }
    for( int i = 0; i < 100; ++i )
    {
        if ( ( i % 2 ) == 0 )
        {
            ss.Add( new Circle( 10.0 + rand() % 30 ) );
        }
        else
        {
            ss.Add( new Square( 10.0 + rand() % 30 ) );
        }
    }
    RunAllegro( &fps, &draw );
    DestroyAllegro();
    return 0;
}
