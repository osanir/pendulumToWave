#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>

using namespace std;
using namespace sf;

#define SCREEN_W 1366
#define SCREEN_H 768


struct Wheel{
    RectangleShape line;
    CircleShape circle;
    float rotationRate = ((rand()%10) - 5);
    Color waveColor = Color(rand()%255, rand()%255, rand()%255);

    void update(){
        line.rotate(rotationRate);

    }
    
    void update(Vector2f newPos){
        circle.setPosition(newPos);
        line.setPosition(newPos);
        line.rotate(rotationRate);

    }

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

};

struct WheelList{
    vector<Wheel> wheels;

    WheelList(){
        Wheel wheel(100, { SCREEN_W/6, SCREEN_H/2}); // head wheel
        wheels.push_back(wheel);
    }

    void update(){
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

int main(){
    RenderWindow window(VideoMode(SCREEN_W, SCREEN_H), "Pendulum to Wave", Style::Default);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    srand(time(NULL));

    WheelList pendulum;
    pendulum.createWheel(5);

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

        for( auto wheel : pendulum.wheels ){
            window.draw(wheel.circle);
            window.draw(wheel.line);
        }
        window.display();
    }
}