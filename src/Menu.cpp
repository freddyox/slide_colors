#include "../include/Menu.hh"
#include <cmath>
#include <iostream>

Menu::Menu(float x, float y) {
  displayx = x;
  displayy = y;

  MakeMenu();

  colors = sf::VertexArray(sf::Quads, 4);

  R = 0;
  G = 100;
  B = 0;
  R2 = 100;
  MakeVertexColors();

  slider_line = sf::VertexArray(sf::LinesStrip,2);
  shade = sf::VertexArray(sf::Quads, 4);
  float sliderx = 5.0;
  float slidery = 15.0;
  slider.setSize( sf::Vector2f(sliderx,slidery) );
  slider.setOrigin( sliderx/2.0, slidery/2.0 );
  slider.setFillColor( sf::Color(230,230,230) );

  MakeSlider( displayx, displayy, R, 0.0, sf::Color::Red, slider);
  MakeSlider( displayx, displayy, G, 150.0, sf::Color::Green, slider);
  MakeSlider( displayx, displayy, B, 300.0, sf::Color::Blue, slider);
  MakeSlider( displayx, displayy, R2, 450.0, sf::Color::Red, slider);

  update = false;
}

void Menu::MakeVertexColors() {
  sf::FloatRect tmp = menu.getLocalBounds();
  colors[0].position = sf::Vector2f( tmp.left + tmp.width, tmp.top );
  colors[1].position = sf::Vector2f( tmp.left + tmp.width, tmp.top + displayy ); 
  colors[2].position = sf::Vector2f( displayx, displayy );
  colors[3].position = sf::Vector2f( displayx, 0.0 );
  colors[0].color = sf::Color(R,0,0);
  colors[1].color = sf::Color(0,G,0);
  colors[2].color = sf::Color(0,0,B);
  colors[3].color = sf::Color(R2,0,0);
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
  slider_line_vec.push_back( slider_line );

  // Make a background rectangle
  sf::Vector2f bg_size( end.x - start.x + 20.0, slide.getLocalBounds().height + 20.0 ); 
  bg.setSize( bg_size );
  bg.setOrigin( bg_size.x/2.0, bg_size.y/2.0 );
  bg.setPosition( sf::Vector2f( (start.x+end.x)/2.0, y/8.0 + offset ) );
  bg.setFillColor( sf::Color(150,150,150));
  bg_vec.push_back( bg );

  // Make "Shade"
  ShadeRectangle(bg.getPosition(),bg.getSize(),sf::Color(80,80,80),4.0,false);

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
    shade_vec.push_back( slider_line );
  }
  // Make the slider
  float initial_offset = float(input) / 255.0;
  slide.setPosition( sf::Vector2f(start.x+initial_offset*slider_mag, start.y) );
  sliders.push_back( slide );
  // Shade the Slider
  ShadeRectangle(slide.getPosition(), slide.getSize(), sf::Color(80,80,80), 2.0,true);
}

void Menu::DrawSlider(sf::RenderTarget& targ) const{
  std::vector<sf::VertexArray>::const_iterator cit1;
  std::vector<sf::RectangleShape>::const_iterator cit;
  for( cit1=slider_line_vec.begin(); cit1!=slider_line_vec.end(); cit1++ ){
    targ.draw(*cit1);
  }
  for( cit1=shade_vec.begin(); cit1!=shade_vec.end(); cit1++ ){
    targ.draw(*cit1);
  }
  for( cit=sliders.begin(); cit!=sliders.end(); cit++ ){
    targ.draw(*cit);
  }
}

void Menu::ShadeRectangle(sf::Vector2f pos,sf::Vector2f size,sf::Color col,float thickness,bool YorN){
  // TOP
  shade[0].position = sf::Vector2f( pos.x - size.x/2.0, pos.y - size.y/2.0 );
  shade[1].position = sf::Vector2f( pos.x - size.x/2.0 + thickness, pos.y - size.y/2.0 - thickness);
  shade[2].position = sf::Vector2f(  pos.x + size.x/2.0 + thickness, pos.y - size.y/2. - thickness );
  shade[3].position = sf::Vector2f( pos.x + size.x/2.0, pos.y - size.y/2.0 );
  shade[0].color =  col;
  shade[1].color =  col;
  shade[2].color =  col;
  shade[3].color =  col;
  shade_vec.push_back( shade );
  // RIGHT
  shade[0].position = sf::Vector2f(  pos.x + size.x/2.0, pos.y - size.y/2.0 );
  shade[1].position = sf::Vector2f( pos.x + size.x/2.0 + thickness, pos.y - size.y/2.0 - thickness );
  shade[2].position = sf::Vector2f( pos.x + size.x/2.0 + thickness, pos.y + size.y/2.0 - thickness);
  shade[3].position = sf::Vector2f( pos.x + size.x/2.0, pos.y + size.y/2.0 );
  shade[0].color =  col;
  shade[1].color =  col;
  shade[2].color =  col;
  shade[3].color =  col;
  shade_vec.push_back( shade );
  // Line that shows dimension in TL corner
  slider_line[0].position = sf::Vector2f( pos.x + size.x/2.0, pos.y - size.y/2.0 );
  slider_line[1].position = sf::Vector2f( pos.x + size.x/2.0 + (thickness-0.25), pos.y - size.y/2.0 - (thickness-0.25) );
  slider_line[0].color = sf::Color(50,50,50);
  slider_line[1].color = sf::Color(50,50,50);
  if(YorN) slider_shade_vec.push_back( slider_line );
  else shade_vec.push_back( slider_line );
}  

void Menu::UpdateSliders(sf::Vector2f mouseP){

  if( sf::Mouse::isButtonPressed(sf::Mouse::Left) ) {
    update = true;
    sf::Vector2f position;
    for( int i=0; i<slider_line_vec.size(); i++ ){
      // check to see if mouse if close
      position = sliders[i].getPosition(); 
      sf::FloatRect bounds = slider_line_vec[i].getBounds();
      sf::Vector2f D = mouseP - position;
      float D_2 = pow(D.x,2) + pow(D.y,2);

      if( D_2 <= 0.04*( pow(position.x,2)+pow(position.y,2) ) ) {
	if( mouseP.x <= bounds.left ) {
	  sliders[i].setPosition( bounds.left, bounds.top );
	}
	if( mouseP.x >= (bounds.left+bounds.width) ) {
	  sliders[i].setPosition( bounds.left + bounds.width, bounds.top );
	}
	if( mouseP.x > bounds.left && mouseP.x < (bounds.left+bounds.width) )  {
	  sliders[i].setPosition( mouseP.x, position.y );  
	}
      }
      
    }
  }
  else {
    if( update ) {
      SetSliderValue();
      update = false;
    }
  }
}

void Menu::SetSliderValue(){
  for( int i=0; i<slider_line_vec.size(); i++ ){
    sf::FloatRect bounds = slider_line_vec[i].getBounds();
    sf::Vector2f sliderPos = sliders[i].getPosition();
    
    float RGB_value = (sliderPos.x - bounds.left)/200.0*255.0;
    int newRGB = int(RGB_value);
    sf::FloatRect tmp = menu.getLocalBounds();
    colorsvec.clear();
    colors[0].position = sf::Vector2f( tmp.left + tmp.width, tmp.top );
    colors[1].position = sf::Vector2f( tmp.left + tmp.width, tmp.top + displayy ); 
    colors[2].position = sf::Vector2f( displayx, displayy );
    colors[3].position = sf::Vector2f( displayx, 0.0 );
    // 0 = TL
    // 1 = TR
    // 2 = BR
    // 3 = BL
    if(newRGB>=0){ 
      switch(i){
      case 0 :
	R = newRGB;
	colors[0].color = sf::Color(R,0.0,0.0);
	colors[1].color = sf::Color(0.0,G,0.0);
	colors[2].color = sf::Color(0.0,0.0,B);
	colors[3].color = sf::Color(R2,0.0,0.0);
	break;
      case 1 :
	G = newRGB;
	colors[0].color = sf::Color(R,0.0,0.0);
	colors[1].color = sf::Color(0.0,G,0.0);
	colors[2].color = sf::Color(0.0,0.0,B);
	colors[3].color = sf::Color(R2,0.0,0.0);
	break;
      case 2 : 
	B = newRGB;
	colors[0].color = sf::Color(R,0.0,0.0);
	colors[1].color = sf::Color(0.0,G,0.0);
	colors[2].color = sf::Color(0.0,0.0,B);
	colors[3].color = sf::Color(R2,0.0,0.0);
	break;
      case 3 : 
	R2 = newRGB;
	colors[0].color = sf::Color(R,0.0,0.0);
	colors[1].color = sf::Color(0.0,G,0.0);
	colors[2].color = sf::Color(0.0,0.0,B);
	colors[3].color = sf::Color(R2,0.0,0.0);
	break;
      default : 
      break;
      }
      colorsvec.push_back( colors );
    }
    
  }
}
