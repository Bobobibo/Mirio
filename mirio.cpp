#include <iostream>
#include "AllegroBase.hpp"
#include <windows.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;



class Figure
{
protected:
    double dx=1.0;
    double dy=1.0;
    double d;
    double x = rand() % SCREEN_W;
    double y = rand() % SCREEN_H;

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
    double randomSize(){
        s= rand() % 1920;
        return s;
    }

    virtual void movef() {
        dx = -1;
        d = -1;
        x += dx;

    }
    double getX() const { return x; }
    double getY() const { return y; }


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

    Land() :
            Rectangle()
    {
    }

    virtual void draw()
    {

        al_draw_filled_rectangle( 0, SCREEN_H-200, SCREEN_W , SCREEN_H,
                                  al_map_rgb( 67, 176, 71) );
    }
};
class Platform : public Rectangle {
public:
    Platform() : Rectangle() {
        y = SCREEN_H - 300;
        sx_ = 200;
        sy_ = 20;
        x = 300;
    }

    virtual void draw() override {
        al_draw_filled_rectangle(x, y, x + sx_, y + sy_, al_map_rgb(203, 79, 15));
    }

    virtual  void movef() override {
        //not moving
    }
};
class Platform2 : public Rectangle {
public:
    Platform2() : Rectangle() {
        y = SCREEN_H - 300;
        sx_ = 200;
        sy_ = 20;
        x = 1000;
    }

    virtual void draw() override {
        al_draw_filled_rectangle(x, y, x + sx_, y + sy_, al_map_rgb(203, 79, 15));
    }

    virtual  void movef() override {
        //not moving
    }
};

class Cloud : public Rectangle {
public:
    Cloud() : Rectangle() {
        y = 300;
        sx_ = rand() % 100;

        x = rand() % SCREEN_W;
    }

    virtual void draw() override {
        al_draw_filled_circle(x, y, sx_, al_map_rgb(255, 255, 253));
    }

    virtual  void movef() override {
        //not moving
    }
};
class Flag : public Rectangle {
public:
    Flag() : Rectangle() {
        y = SCREEN_H;
        sx_ = 15;

        x = SCREEN_W-200;
    }

    virtual void draw() override {
        al_draw_filled_rectangle(x, 0, x + sx_, SCREEN_H-200, al_map_rgb(0, 171, 3));
    }

    virtual  void movef() override {
        //not moving
    }
};

class Goomba : public Rectangle {
protected:
    double s_ = randomSize();
    double speed = 1.0;

public:
    Goomba() : Rectangle() {
        y = SCREEN_H - 215;
        dx = -1;
    }

    virtual void draw() override {
        double size = s_;
        al_draw_filled_rectangle(x, y, x + 15, y + 15, al_map_rgb(150, 75, 0));
    }

    virtual void movef() {
        x += dx * speed;
        if (x <= 0) {
            dx = -dx;
        }
    }
};

const int MAXF = 40; // 1 Rectangle + 1 Land+ 2 platform + 7 clouds + 13 Goombas

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

    void Remove(int index) {
        if (index < 0 || index >= size_) {
            return;
        }
        delete figures[index];
        for (int i = index; i < size_ - 1; ++i) {
            figures[i] = figures[i + 1];
        }
        figures[size_ - 1] = nullptr;
        --size_;
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
    const double gravity = 0.4 ;

public:
    ControlledSquare() : Goomba() {}

    virtual void draw() override {
        double sizex = sx_;
        double sizey = sy_;
        al_draw_filled_rectangle(x, y, x + sizex, y + sizey, al_map_rgb(229, 37, 33));
    }

    void CheckWin(double x) {
        if (x >= SCREEN_W-200) {
            cout << "YOU WON (crossed the flag) :D" << endl;
            sleep(5);
            exit(0);
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

        if ((y <= SCREEN_H - sy_ - 300 && x>=300 && x<=300+200 )||(y <= SCREEN_H - sy_ - 300 && x>=1000 && x<=1000+200 )){
            y = SCREEN_H - sy_ - 300;
            vy = 0;
            onGround = true;
        }else if((y == SCREEN_H - sy_ - 300 && (x>1000 || x<1000+200 ))||(y == SCREEN_H - sy_ - 300 && (x>1000 || x<1000+200 ))){
            onGround=false;}



        if ((x <= 0.0) || (x + sx_ >= SCREEN_W)) {
            dx *= 0;
        }
        if ((y <= 0.0) || (y + sy_ >= SCREEN_H)) {
            dy *= 0;
         }
        CheckWin(x);

    }

    void Jump() {
        if (onGround) {
            vy = -10;
            onGround = false;
        }
    }

    bool CheckCollision(const Goomba &goomba) {
        return (x < goomba.getX() + 15 && x + sx_ > goomba.getX() &&
                y < goomba.getY() + 15 && y + sy_ > goomba.getY());
    }

    bool CheckJumpOn(const Goomba &goomba) {
        return (y + sy_ >= goomba.getY() && vy > 0 &&
                x + sx_ > goomba.getX() && x <  goomba.getX() + 15);
    }
    void Bounce() {
        vy = -5;
    }

};

class AllegroApp : public AllegroBase {
private:
    ControlledSquare humanSquare_;
    ScreenSaver ss;
    int goombaCount_;

public:

    AllegroApp() : AllegroBase(), ss() {
        ss.Add(new class Rectangle());
        ss.Add(new Land());
        ss.Add(new Platform());
        ss.Add(new Platform2());
        ss.Add(new Flag());
        for (int i = 4; i < 9; ++i) {
            ss.Add(new Cloud());
        }
        for (int i = 11; i < 24 ; ++i) {
            ss.Add(new Goomba());


        }
        goombaCount_=13;//delta from goombas creation ( beginning i and second i)
        cout<< " Goomba count:"<<goombaCount_<<endl;

    }

    virtual void Fps() override {
        ss.Next();

        double dx_ = 0, dy_ = 0;
        if (IsPressed(ALLEGRO_KEY_LEFT)) { dx_ = -1; }
        if (IsPressed(ALLEGRO_KEY_RIGHT)) { dx_ = 1; }
        if (IsPressed(ALLEGRO_KEY_LSHIFT)) {
            dx_ *= 3;
            dy_ *= 3;
        }
        if (IsPressed(ALLEGRO_KEY_SPACE)) {
            humanSquare_.Jump();
        }
        humanSquare_.MoveBy(dx_, dy_);



        // this is collision with goombas
        for (int i = MAXF - 1; i >= 2; --i) {
            if (Goomba *goomba = dynamic_cast<Goomba *>(ss.getFigure(i))) {
                if (humanSquare_.CheckJumpOn(*goomba)) {
                    ss.Remove(i); // Remove Goomba
                   goombaCount_--;
                    cout<< " Goomba count:"<<goombaCount_<<endl;

                    humanSquare_.Bounce(); // Bounce
                } else if (humanSquare_.CheckCollision(*goomba)) {
                    cout << "YOU LOST :(" << endl;
                    exit(0); //lose
                }
                if (goombaCount_ == 0) {
                    cout << "YOU WON (destroyed the goombas) :)" << endl;
                    sleep(5);
                    exit(0); //victory
                }
            }
        }
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
    app.Run();     return 0;
};