//    ************************************************************
//    *                    Color  Simulation                     *
//    *                       Jan  2016                          *
//    ************************************************************
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "../include/Menu.hh"

const float gDisplayx = 1200;
const float gDisplayy = 600;
int main() {
  
  sf::RenderWindow window(sf::VideoMode(gDisplayx,gDisplayy), "Colors Simulation");
  window.setFramerateLimit(60);

  //////////////////////////////////////////////////////
  //                   Initialize                     //
  //////////////////////////////////////////////////////
  sf::Clock clock;
  double t = 0.0;
  double dt = 1.0/60.0;

  Menu menu( gDisplayx, gDisplayy );

  while( window.isOpen() ) {
    sf::Event event;
    while( window.pollEvent(event) ) {
      if( event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ) {
	window.close();
      }
    }
    // UPDATING
    window.clear( sf::Color(0,0,0) );

    // DRAWINGS
    window.draw( menu );

    window.display();   
    t+=dt;
  }
  return 0;
}
