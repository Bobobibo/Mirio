#include <iostream>
#include "AllegroBase.hpp"
#include <windows.h>
#include <cstdlib>

using namespace std;



class Figure
{
protected:
    double dx=1.0;
    double dy=1.0;
    double d;
    double x = rand() % SCREEN_W;
    double y = rand() % SCREEN_H;
    double r = rand() % 256;
    double g = rand() % 256;
    double b = rand() % 256;

    double s;

public:
    Figure(){
        Reset();
    }
    virtual ~Figure()
    {
    }
    void Reset()
    {
        d=1;
    }
    void randomColor() {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    }
    double randomSize(){
        s= rand() % 1920;
        return s;
    }
    void movef() {
        dx = d;
        //dy=dy*d;

        d = 1;
        x += dx;
        //y+=dy;

        /*if ((x >= SCREEN_W - 1.0))
        {
            x -= dx;
            Reset();

            dx=-1.0  ;
            //randomSize();
            //randomColor();
        }else if(x  <=1.0 ){
            x -= dx;
            Reset();

            dx=1.0  ;
            //randomSize();
            //randomColor();

        }*/






    }
    double getX() const { return x; }
    double getY() const { return y; }
    double getDx() const { return dx; }
    double getDy() const { return dy; }
    double getSize() const { return s; }
    //double getD() const { return d; }
    void setDx(double dx1) { dx = dx1; }
    void setDy(double dy1) { dy = dy1; }
    void setX(double x1) { x = x1; }
    void setY(double y1) { y = y1; }
    //double getSize() const{return s;}

    virtual void draw()=0;


};


typedef Figure * PFigure;


class Rectangle : public Figure
{
protected:
    double sy_=randomSize();
    double sx_= randomSize();


public:

    Rectangle() :
            Figure()
    {
    }
    //double getSize() const override { return sx_; }
    virtual void draw()
    {
        al_draw_filled_rectangle( 0, 0, SCREEN_W , SCREEN_H,
                                  al_map_rgb( 4, 156, 216) );

    }

};
class Land : public Rectangle
{
protected:


public:

    Land() ://double s
            Rectangle()
    {
    }
    //double getSize() const override { return s_; }
    virtual void draw()
    {

        al_draw_filled_rectangle( 0, SCREEN_H-200, SCREEN_W , SCREEN_H,
                                  al_map_rgb( 67, 176, 71) );
    }
};

class Goomba : public Rectangle
{
protected:
    double s_ = randomSize();

public:

    Goomba() ://double s
            Rectangle()
    {
    }
    //double getSize() const override { return s_; }
    virtual void draw()
    {
        double size=s_;
        x = rand() % SCREEN_W;
        y =  SCREEN_H-215;
        al_draw_filled_rectangle( x, y, x +15 , y +15,
                                  al_map_rgb( 150, 75, 0) );
    }
};
const int MAXF =3;

class ScreenSaver
{
private:
    PFigure figures[MAXF];
    int size_;

public:
    ScreenSaver() :
            size_( 0 )
    {
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
            figures[i]->draw();
        }
    }
    void Next()
    {

        for( int i = 0; i < size_; ++i )
        {
            figures[i]->movef();
        }//Collision();


    }
    void Add( Figure *f )
    {
        if ( size_ >= MAXF )
        {
            return;
        }
        figures[ size_ ] = f;
        ++size_;
    }
public: void Collision() {
        for (int i = 0; i < size_; ++i) {
            for (int j = i + 1; j < size_; ++j) {
                double x1 = figures[i]->getX();
                double y1 = figures[i]->getY();
                double sizex1 = figures[i]->getSize();
                double sizey1 = figures[i]->getSize();

                double x2 = figures[j]->getX();
                double y2 = figures[j]->getY();
                double sizex2 = figures[j]->getSize();
                double sizey2 = figures[j]->getSize();

                double left1 = x1;
                double right1 = x1 + sizex1;
                double top1 = y1;
                double bottom1 = y1 + sizey1;

                double left2 = x2;
                double right2 = x2 + sizex2;
                double top2 = y2;
                double bottom2 = y2 + sizey2;
                //double d1 = figures[j]->getD();


                if (left1 < right2 && right1 > left2 && top1 < bottom2 && bottom1 > top2) {

                    double dx = 2;
                    double dy = 2;

                    figures[i]->setX(figures[i]->getX() + dx);//to bounce off
                    figures[i]->setY(figures[i]->getY() + dy);
                    figures[j]->setX(figures[j]->getX() - dx);
                    figures[j]->setY(figures[j]->getY() - dy);

                    figures[i]->setDx(-figures[i]->getDx());//to change directions
                    figures[i]->setDy(-figures[i]->getDy());
                    figures[j]->setDx(-figures[j]->getDx());
                    figures[j]->setDy(-figures[j]->getDy());
                }
            }
        }
    }
    void DrawBackground(){
        al_draw_filled_rectangle( 0, 0, SCREEN_W , SCREEN_H,
                                  al_map_rgb( 4, 156, 216) );
    };

};
class ControlledSquare : public Goomba{
protected:
    double sy_ = 40;
    double sx_ = 20;
    double x=0;
    double y=SCREEN_H-240;
public:

    ControlledSquare():
            Goomba(){

    }
    virtual void draw()
    {

        double sizex=sx_;
        double sizey=sy_;
        al_draw_filled_rectangle( x, y, x +sizex , y+sizey,
                                  al_map_rgb( 229, 37, 33) );
    }

    void CheckWin(double x, double y){
        if((x>=SCREEN_W)){
            cout<<"YOU WON"<<endl;
        }
    }

    void MoveBy(double dx_, double dy_){
        dx=dx_;
        dy=dy_;
        x+=dx;
        y+=dy;
        if((x<=0.0)||(x+s_>= SCREEN_W)){
            dx*=0;
        }if((y<=0.0)||(y+s_>= SCREEN_H)) {
            dy *= 0;
        }
        CheckWin(x,y);

    }



};
//ScreenSaver ss;
class AllegroApp:public AllegroBase
{
private:
    ControlledSquare humanSquare_;
    ScreenSaver ss;
    //Figure.randomsize();

public:
    AllegroApp():
            AllegroBase(),
            ss()
    {

        for( int i = 0; i < 100; ++i )
        {
            if(i==0){
                ss.Add((new class Rectangle ));
            }else if(i==1){
                ss.Add(new Land);
            }
            else{
            ss.Add( new Goomba(  ) );}
        }



    }
    virtual void Fps()
    {

        ss.Next();

        double dx_ =0, dy_=0;
        //if (IsPressed((ALLEGRO_KEY_UP))){dy_= -1;}
        //if(IsPressed((ALLEGRO_KEY_DOWN))){dy_= +1;}
        if(IsPressed((ALLEGRO_KEY_LEFT))){dx_= -1;}
        if(IsPressed((ALLEGRO_KEY_RIGHT))){dx_= +1;}
        if(IsPressed((ALLEGRO_KEY_LSHIFT))){
            dx_*=10;
            dy_*=10;
        }
        humanSquare_.MoveBy(dx_,dy_);

        //ss.Collision();
    }
    virtual void Draw()
    {

        ss.Draw();
        humanSquare_.draw();

    }




};





int main(int argc, char **argv)
{
    srand( time(0) );

    AllegroApp app;
    if( !app.Init( SCREEN_W, SCREEN_H, FPS ) )
    {
        return 1;
    }
    app.Run();
    return 0;
};