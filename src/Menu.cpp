#include "../include/Menu.hh"
#include <cmath>
#include <iostream>

Menu::Menu(float x, float y) {
  displayx = x;
  displayy = y;

  MakeMenu();

  colors = sf::VertexArray(sf::Quads, 4);
  MakeColors();
  R = 100;
  G = 255;
  B = 100;

  slider_line = sf::VertexArray(sf::LinesStrip,2);
  shade = sf::VertexArray(sf::Quads, 4);
  float sliderx = 5.0;
  float slidery = 15.0;
  slider.setSize( sf::Vector2f(sliderx,slidery) );
  slider.setOrigin( sliderx/2.0, slidery/2.0 );
  slider.setFillColor( sf::Color::White );

  MakeSlider( displayx, displayy, R, 0.0, sf::Color::White, slider);
  MakeSlider( displayx, displayy, G, 150.0, sf::Color::Green, slider);
  MakeSlider( displayx, displayy, G, 300.0, sf::Color::Blue, slider);
  MakeSlider( displayx, displayy, R, 450.0, sf::Color::Red, slider);
}

void Menu::MakeColors() {
  sf::FloatRect tmp = menu.getLocalBounds();
  colors[0].position = sf::Vector2f( tmp.left + tmp.width, tmp.top );
  colors[1].position = sf::Vector2f( tmp.left + tmp.width, tmp.top + displayy ); 
  colors[2].position = sf::Vector2f( displayx, displayy );
  colors[3].position = sf::Vector2f( displayx, 0.0 );
  colors[0].color = sf::Color::Blue;
  colors[1].color = sf::Color::Red;
  colors[2].color = sf::Color::Green;
  colors[3].color = sf::Color::Red;
  colorsvec.push_back( colors );
}

void Menu::MakeMenu() {
  sf::Vector2f size( displayx/4.0, displayy );
  menu.setSize( size );
  menu.setOrigin( size.x/2.0, size.y/2.0 );
  menu.setFillColor(sf::Color::White);
  menu.setPosition( size.x/2.0, size.y/2.0 );
}

void Menu::draw( sf::RenderTarget& target, sf::RenderStates ) const {
  target.draw( menu );
  std::vector<sf::VertexArray>::const_iterator cit;
  for(cit=colorsvec.begin(); cit!=colorsvec.end(); cit++){
    target.draw(*cit);
  }
  std::vector<sf::RectangleShape>::const_iterator rit;
  for(rit=bg_vec.begin(); rit!=bg_vec.end(); rit++){
    target.draw(*rit);
  }
  DrawSlider( target );
}

void Menu::MakeSlider(float x, float y, int input, float offset, sf::Color color, sf::RectangleShape slide ) {
  sf::FloatRect tmp = menu.getLocalBounds();
  float centerX = tmp.left+tmp.width;
  sf::Vector2f start( 50.0, y/8.0 + offset);
  sf::Vector2f end( centerX-50.0 , y/8.0 + offset);
  slider_line[0].position = start;
  slider_line[1].position = end;
  slider_line[0].color = color;
  slider_line[1].color = color;
  slider_vec.push_back( slider_line );

  // Make a background rectangle
  sf::Vector2f bg_size( end.x - start.x + 20.0, slide.getLocalBounds().height + 20.0 ); 
  bg.setSize( bg_size );
  bg.setOrigin( bg_size.x/2.0, bg_size.y/2.0 );
  bg.setPosition( sf::Vector2f( (start.x+end.x)/2.0, y/8.0 + offset ) );
  bg.setFillColor( sf::Color::Black);
  bg_vec.push_back( bg );

  // Make "Shade"
  sf::Vector2f bg_pos = bg.getPosition();
  sf::Color shadeColor = sf::Color(100,100,100);
  shade[0].position = sf::Vector2f( bg_pos.x - bg_size.x/2.0, bg_pos.y - bg_size.y/2.0 );
  shade[1].position = sf::Vector2f( bg_pos.x - bg_size.x/2.0 + 4.0, bg_pos.y - bg_size.y/2.0 - 4.0);
  shade[2].position = sf::Vector2f(  bg_pos.x + bg_size.x/2.0 + 4.0, bg_pos.y - bg_size.y/2. - 4.0 );
  shade[3].position = sf::Vector2f( bg_pos.x + bg_size.x/2.0, bg_pos.y - bg_size.y/2.0 );
  shade[0].color =  shadeColor;
  shade[1].color =  shadeColor;
  shade[2].color =  shadeColor;
  shade[3].color =  shadeColor;
  shade_vec.push_back( shade );
  shade[0].position = sf::Vector2f(  bg_pos.x + bg_size.x/2.0, bg_pos.y - bg_size.y/2.0 );
  shade[1].position = sf::Vector2f( bg_pos.x + bg_size.x/2.0 + 4.0, bg_pos.y - bg_size.y/2.0 - 4.0 );
  shade[2].position = sf::Vector2f( bg_pos.x + bg_size.x/2.0 + 4.0, bg_pos.y + bg_size.y/2.0 - 4.0);
  shade[3].position = sf::Vector2f( bg_pos.x + bg_size.x/2.0, bg_pos.y + bg_size.y/2.0 );
  shade[0].color =  shadeColor;
  shade[1].color =  shadeColor;
  shade[2].color =  shadeColor;
  shade[3].color =  shadeColor;
  shade_vec.push_back( shade );
  slider_line[0].position = sf::Vector2f( bg_pos.x + bg_size.x/2.0, bg_pos.y - bg_size.y/2.0 );
  slider_line[1].position = sf::Vector2f( bg_pos.x + bg_size.x/2.0 + 4.0, bg_pos.y - bg_size.y/2.0 - 4.0 );
  slider_line[0].color = sf::Color::Black;
  slider_line[1].color = sf::Color::Black;
  shade_vec.push_back( slider_line );

  // Making the little "increments"
  sf::Vector2f slider_line_dis = end - start;
  int slider_mag = sqrt( pow(slider_line_dis.x,2) + pow(slider_line_dis.y,2) );
  int increment = slider_mag / 20;
  int slider_index = 1;

  for( int i=0; i<=increment; i++ ){
    sf::Vector2f start_inc( start.x + i*2*increment, start.y - 3.0 );
    sf::Vector2f end_inc( start.x + i*2*increment, start.y + 3.0 );
    slider_line[0].position = start_inc;
    slider_line[1].position = end_inc;
    slider_line[0].color = color;
    slider_line[1].color = color;
    slider_vec.push_back( slider_line );
  }
 // Make the slider
  float initial_offset = float(input) / 255.0;
  std::cout << initial_offset << " " << input << " " << slider_mag << std::endl;
  slide.setPosition( sf::Vector2f(start.x+initial_offset*slider_mag, start.y) );
  sliders.push_back( slide );
}

void Menu::DrawSlider(sf::RenderTarget& targ) const{
  std::vector<sf::VertexArray>::const_iterator cit1;
  std::vector<sf::RectangleShape>::const_iterator cit;
  for( cit1=slider_vec.begin(); cit1!=slider_vec.end(); cit1++ ){
    targ.draw(*cit1);
  }
  for( cit=sliders.begin(); cit!=sliders.end(); cit++ ){
    targ.draw(*cit);
  }
  for( cit1=shade_vec.begin(); cit1!=shade_vec.end(); cit1++ ){
    targ.draw(*cit1);
  }
}
