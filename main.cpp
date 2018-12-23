#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <ctime>

using namespace std;
using namespace sf;

#define SCREEN_W 1366
#define SCREEN_H 768

struct WavePiece{
    CircleShape shape;
    WavePiece(Color color, Vector2f pos){
        shape.setFillColor(color);
        shape.setRadius(1);
        shape.setOrigin({shape.getRadius(), shape.getRadius()});
        shape.setPosition({SCREEN_W/2, pos.y});
    }
};

struct Wave{
    list<WavePiece> wavePieces;

    void createPieces(Vector2f pos){
        WavePiece newPiece(Color::Red, pos);
        wavePieces.push_front(newPiece);
    }

    void deletePieces(){
        if(wavePieces.back().shape.getPosition().x > SCREEN_W){
            wavePieces.pop_back();
            return;
        }
        
    }

    void movePieces(){
        for( auto &piece : wavePieces ){
            piece.shape.move({5,0});
        }
    }

    void update(Vector2f pos){
        createPieces(pos);
        deletePieces();
        movePieces();
    }
};


struct Wheel{
    RectangleShape line;
    CircleShape circle;
    float rotationRate = ((rand()%10) - 5);
    Color waveColor = Color(rand()%255, rand()%255, rand()%255);

    Wheel(float rad, Vector2f pos){
        circle.setRadius(rad);
        circle.setFillColor(Color(0,0,0,0));
        circle.setOrigin({rad, rad});
        circle.setOutlineColor(Color::Black);
        circle.setOutlineThickness(2);
        circle.setPosition(pos);

        line.setSize({rad, 2});
        line.setFillColor(Color::Red);
        line.setOrigin({0, 1});
        line.setPosition(circle.getPosition());
    }

    void update(){
        line.rotate(rotationRate);
    }
    
    void update(Vector2f newPos){
        circle.setPosition(newPos);
        line.setPosition(newPos);
        line.rotate(rotationRate);
    }


};

struct WheelList{
    vector<Wheel> wheels;

    WheelList(){
        Wheel wheel(100, { SCREEN_W/6, SCREEN_H/2}); // head wheel
        wheels.push_back(wheel);
    }

    void update(){
        list<Vector2f> allPos;
        wheels[0].update();
        for(int i=1; i<wheels.size(); i++){
            wheels[i].update(wheels[i-1].line.getTransform().transformPoint(wheels[i-1].line.getSize().x, 1) );
        }
    }

    void createWheel(int num){
        for(int i=0; i<num; i++){
            Wheel wheel(rand()%30+20, wheels[i].line.getTransform().transformPoint(wheels[i].line.getSize().x, 1) );
            wheels.push_back(wheel);
        }
    }
};

struct Pendulum{
    WheelList wheelList;
    Wave wave;
    Pendulum(){
        wheelList.createWheel(5);
    }

    void update(){
        wheelList.update();
        wave.update( wheelList.wheels.back().line.getTransform().transformPoint(wheelList.wheels.back().line.getSize().x, 1) ); // last wheels coordinates
    }
};

int main(){
    RenderWindow window(VideoMode(SCREEN_W, SCREEN_H), "Pendulum to Wave", Style::Default);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    srand(time(NULL));

    Pendulum pendulum;

    while( window.isOpen() ){
        Event e;
        while( window.pollEvent(e) ){
            switch( e.type ){
                case Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }
        pendulum.update();
        window.clear(Color::White);

        for( auto wheel : pendulum.wheelList.wheels ){
            window.draw(wheel.circle);
            window.draw(wheel.line);
        }

        for( auto wave : pendulum.wave.wavePieces ){
            window.draw(wave.shape);
        }
        window.display();
    }
}