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
        dx = 1;
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

class Goomba : public Rectangle {
protected:
    double s_ = randomSize();
    double speed = 1.0;

public:
    Goomba() : Rectangle() {
        y = SCREEN_H - 215;
    }

    virtual void draw() override {
        double size = s_;
        al_draw_filled_rectangle(x, y, x + 15, y + 15, al_map_rgb(150, 75, 0));
    }

    virtual void movef(){
        x += dx * speed;
        if (x >= SCREEN_W - 15 || x <= 0) {
            dx = -dx;
        }
    }
};

const int MAXF = 7; // 1 Rectangle + 1 Land + 5 Goombas

class ScreenSaver {
private:
    int size_;

public:
    ScreenSaver() : size_(0) {
        memset(figures, 0, sizeof(figures));
    }

    virtual ~ScreenSaver() {
        for (int i = 0; i < size_; ++i) {
            delete figures[i];
            figures[i] = nullptr;
        }
    }

    void Draw() {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        for (int i = 0; i < size_; ++i) {
            figures[i]->draw();
        }
    }

    void Next() {
        for (int i = 0; i < size_; ++i) {
            figures[i]->movef();
        }
    }

    void Add(Figure *f) {
        if (size_ >= MAXF) {
            return;
        }
        figures[size_] = f;
        ++size_;
    }

    Figure* getFigure(int index) {
        if (index >= 0 && index < size_) {
            return figures[index];
        }
        return nullptr;
    }

    PFigure figures[MAXF];
};
class ControlledSquare : public Goomba {
protected:
    double sy_ = 40;
    double sx_ = 20;
    double x = 0;
    double y = SCREEN_H - 240;
    double vy = 0;
    bool onGround = true;
    const double gravity = 0.5;

public:
    ControlledSquare() : Goomba() {}

    virtual void draw() override {
        double sizex = sx_;
        double sizey = sy_;
        al_draw_filled_rectangle(x, y, x + sizex, y + sizey, al_map_rgb(229, 37, 33));
    }

    void CheckWin(double x, double y) {
        if (x >= SCREEN_W) {
            cout << "YOU WON" << endl;
        }
    }

    void MoveBy(double dx_, double dy_) {
        dx = dx_;
        if (onGround) {
            dy = dy_;
        }
        x += dx;
        y += dy;


        if (!onGround) {
            vy += gravity;
            y += vy;
        }


        if (y >= SCREEN_H - sy_ - 200) {
            y = SCREEN_H - sy_ - 200;
            vy = 0;
            onGround = true;
        }

        if ((x <= 0.0) || (x + sx_ >= SCREEN_W)) {
            dx *= 0;
        }
        if ((y <= 0.0) || (y + sy_ >= SCREEN_H)) {
            dy *= 0;
        }
        CheckWin(x, y);
    }

    void Jump() {
        if (onGround) {
            vy = -10;
            onGround = false;
        }
    }

    bool CheckCollision(const Goomba& goomba) const {
        double gx = goomba.getX();
        double gy = goomba.getY();
        double gsize = 15;

        return (x < gx + gsize && x + sx_ > gx && y < gy + gsize && y + sy_ > gy);
    }
};

class AllegroApp : public AllegroBase {
private:
    ControlledSquare humanSquare_;
    ScreenSaver ss;

public:
    AllegroApp() : AllegroBase(), ss() {
        ss.Add(new class Rectangle());
        ss.Add(new Land());
        for (int i = 0; i < 5; ++i) {
            ss.Add(new Goomba());
        }
    }

    virtual void Fps() override {
        ss.Next();

        double dx_ = 0, dy_ = 0;
        if (IsPressed(ALLEGRO_KEY_LEFT)) { dx_ = -1; }
        if (IsPressed(ALLEGRO_KEY_RIGHT)) { dx_ = 1; }
        if (IsPressed(ALLEGRO_KEY_LSHIFT)) {
            dx_ *= 10;
            dy_ *= 10;
        }
        if (IsPressed(ALLEGRO_KEY_SPACE)) {
            humanSquare_.Jump();
        }
        humanSquare_.MoveBy(dx_, dy_);

        for (int i = 0; i < 3; ++i) {
            if (Goomba* goomba = dynamic_cast<Goomba*>(ss.figures[i])) {
                if (humanSquare_.CheckCollision(*goomba)) {
                    exit(0);
                }
            }
        }


        // ss.Collision();
    }

    virtual void Draw() override {
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