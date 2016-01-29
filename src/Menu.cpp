#include "../include/Menu.hh"
#include <cmath>
#include <iostream>

Menu::Menu(float x, float y) {
  displayx = x;
  displayy = y;

  // Makes White Box on LHS
  MakeMenu();

  // Initial the VertexArray QUAD, initialize to full intensity
  // RGBR2 control initial RGB values from 0-255
  colors = sf::VertexArray(sf::Quads, 4);
  TL_mag = 255;
  TR_mag = 255;
  BR_mag = 255;
  BL_mag = 255;
  // runs CW
  TL = sf::Color::Red;
  TR = sf::Color::Red;
  BR = sf::Color::Blue;
  BL = sf::Color::Green;
  MakeVertexColors(TL,TR,BR,BL);

  slider_line = sf::VertexArray(sf::LinesStrip,2);
  shade = sf::VertexArray(sf::Quads, 4);
  float sliderx = 5.0;
  float slidery = 15.0;
  slider.setSize( sf::Vector2f(sliderx,slidery) );
  slider.setOrigin( sliderx/2.0, slidery/2.0 );
  slider.setFillColor( sf::Color(230,230,230) );

  MakeSlider( displayx, displayy, TL_mag, 0.0, sf::Color(80,80,80), slider);
  MakeSlider( displayx, displayy, TR_mag, 150.0, sf::Color(80,80,80), slider);
  MakeSlider( displayx, displayy, BR_mag, 300.0, sf::Color(80,80,80), slider);
  MakeSlider( displayx, displayy, BL_mag, 450.0, sf::Color(80,80,80), slider);

  MakeRGB( sf::Color(150,150,150) );

  // Controls UpdateSliders
  update = false;
  update_button = true;
}

void Menu::MakeMenu() {
  sf::Vector2f size( displayx/4.0, displayy );
  menu.setSize( size );
  menu.setOrigin( size.x/2.0, size.y/2.0 );
  menu.setFillColor(sf::Color(235,235,235));
  menu.setPosition( size.x/2.0, size.y/2.0 );
  MakeBoarder(size.x,size.y,6.0,sf::Color(80,80,80));
}

void Menu::MakeVertexColors(sf::Color TL,sf::Color BL,sf::Color BR,sf::Color TR) {
  sf::FloatRect tmp = menu.getLocalBounds();
  colors[0].position = sf::Vector2f( tmp.left + tmp.width, tmp.top );
  colors[1].position = sf::Vector2f( tmp.left + tmp.width, tmp.top + displayy ); 
  colors[2].position = sf::Vector2f( displayx, displayy );
  colors[3].position = sf::Vector2f( displayx, 0.0 );
  colors[0].color = TL;
  colors[1].color = TR;
  colors[2].color = BR;
  colors[3].color = BL;
  colorsvec.push_back( colors );
}
////////////////////////////////////////////////////////
//                     SLIDERS                        //
////////////////////////////////////////////////////////
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
  // Shade the Slider ***NEEDS UPDATING*** - how do I get vertex array to move 
  // with Mouse Position???
  //ShadeRectangle(slide.getPosition(), slide.getSize(), sf::Color(80,80,80), 2.0, true);
}

void Menu::UpdateSliders(sf::Vector2f mouseP){
  // check to see if mouseP is in the menu window
  sf::FloatRect tmp = menu.getLocalBounds();
  if( mouseP.x <= tmp.left + tmp.width ) {
    if( sf::Mouse::isButtonPressed(sf::Mouse::Left) ) {
      update = true;
      sf::Vector2f position;
      for( int i=0; i<slider_line_vec.size(); i++ ){
	// check to see if mouse if close
	position = sliders[i].getPosition(); 
	sf::FloatRect bounds = slider_line_vec[i].getBounds();
	sf::Vector2f D = mouseP - position;
	float D_2 = pow(D.x,2) + pow(D.y,2);
	if( fabs(D.y)<5 && mouseP.x > bounds.left 
	    && mouseP.x < (bounds.left+bounds.width)) {
	  sliders[i].setPosition( mouseP.x,position.y );
	}
	if( D_2 <= 0.004*( pow(position.x,2)+pow(position.y,2) ) ) {
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
	TL_mag = newRGB;
	break;
      case 1 :
	TR_mag = newRGB;
	break;
      case 2 : 
	BR_mag = newRGB;
	break;
      case 3 : 
	BL_mag = newRGB;
	break;
      default : 
	break;
      }
      
      if( buttons[0].getFillColor() == sf::Color::Red ) {
	TL = sf::Color(TL_mag,0,0);
      }
      if( buttons[0].getFillColor() == sf::Color::Green ) {
	TL = sf::Color(0,TL_mag,0);
      }
      if( buttons[0].getFillColor() == sf::Color::Blue ) {
	TL = sf::Color(0,0,TL_mag);
      }
      if( buttons[1].getFillColor() == sf::Color::Red ) {
	TR = sf::Color(TR_mag,0,0);
      }
      if( buttons[1].getFillColor() == sf::Color::Green ) {
	TR = sf::Color(0,TR_mag,0);
      }
      if( buttons[1].getFillColor() == sf::Color::Blue ) {
	TR = sf::Color(0,0,TR_mag);
      }
      if( buttons[2].getFillColor() == sf::Color::Red ) {
	BR = sf::Color(BR_mag,0,0);
      }
      if( buttons[2].getFillColor() == sf::Color::Green ) {
	BR = sf::Color(0,BR_mag,0);
      }
      if( buttons[2].getFillColor() == sf::Color::Blue ) {
	BR = sf::Color(0,0,BR_mag);
      }
      if( buttons[3].getFillColor() == sf::Color::Red ) {
	BL = sf::Color(BL_mag,0,0);
      }
      if( buttons[3].getFillColor() == sf::Color::Green ) {
	BL = sf::Color(0,BL_mag,0);
      }
      if( buttons[3].getFillColor() == sf::Color::Blue ) {
	BL = sf::Color(0,0,BL_mag);
      }	
      colors[0].color = TL;
      colors[1].color = BL;
      colors[2].color = BR;
      colors[3].color = TR;
      colorsvec.push_back( colors );
    }   
  }
}
////////////////////////////////////////////////////////
//                     BUTTONS                        //
////////////////////////////////////////////////////////
void Menu::MakeRGB(sf::Color color) {
  // Has to be called AFTER MakeSlider(), therefore
  // check to see if it is empty to avoid issues

  if(slider_line_vec.size()>0){
    for( int i=0; i<slider_line_vec.size(); i++ ){
      sf::FloatRect bounds = slider_line_vec[i].getBounds();
      sf::RectangleShape RGB_bg;
      sf::Vector2f size( (bounds.left + bounds.width)/2.0, sliders[i].getLocalBounds().height + 15.0); 
      RGB_bg.setSize( size );
      RGB_bg.setOrigin( size.x/2.0, size.y/2.0 );
      RGB_bg.setFillColor( color );
      RGB_bg.setPosition( bounds.left + 0.5*bounds.width, bounds.top + RGB_bg.getSize().y + 10.0);
      RGB_bg_vec.push_back( RGB_bg );
      ShadeRectangle( RGB_bg.getPosition(), RGB_bg.getSize(), sf::Color(80,80,80), 3.0, false );

      // Now Make the RGB Interface
      float radius = RGB_bg.getSize().y/3.0;
      float thickness = 1.0;
      rgb_vec.clear();

      // Make the RGB Button
      sf::CircleShape button( radius - 4.0 );
      sf::FloatRect buttontmp = button.getLocalBounds();
      button.setOrigin( buttontmp.width/2.0, buttontmp.height/2.0 );
      // Need to initialize our buttons:
      switch(i) {
      case 0 : button.setFillColor(TL);
	break;
      case 1 : button.setFillColor(TR);
	break;
      case 2 : button.setFillColor(BR);
	break;
      case 3 : button.setFillColor(BL);
	break;
      default : 
	break;
      }

      for( int k=1; k<=3; k++ ) {
	sf::CircleShape rgb_circle( radius );
	rgb_circle.setOutlineThickness(-thickness);
	sf::FloatRect circlebounds = rgb_circle.getLocalBounds();
	rgb_circle.setOrigin( circlebounds.width/2.0, circlebounds.height/2.0 );
	rgb_circle.setPosition( RGB_bg.getPosition().x - RGB_bg.getSize().x/2.0 + k*(RGB_bg.getSize().x)/3.0 - 2*radius, RGB_bg.getPosition().y );
	rgb_circle.setFillColor(sf::Color(220,220,220));

	switch( k ) {
	case 1 : rgb_circle.setOutlineColor( sf::Color::Red );
	  break;
	case 2 : rgb_circle.setOutlineColor( sf::Color::Green );
	  break;
	case 3 : rgb_circle.setOutlineColor( sf::Color::Blue );
	  break;
	default : 
	  break;
	}
	// Check colors, and place our Button
	if( rgb_circle.getOutlineColor() == button.getFillColor() ) {
	  button.setPosition(RGB_bg.getPosition().x - RGB_bg.getSize().x/2.0 + k*(RGB_bg.getSize().x)/3.0 - 2*radius, RGB_bg.getPosition().y);
	}
	rgb_vec.push_back( rgb_circle );
      }
      rgb_map[i] = rgb_vec;
      buttons[i] = button;
    }
  }
  else {
    std::cout << "slider_line_vec is empty!" << std::cerr;
  }
}

void Menu::UpdateRGB(sf::Vector2f mouseP) {
  if( sf::Mouse::isButtonPressed(sf::Mouse::Left) ) {
    if( update_button ) {
      sf::Vector2f position;
      for( rgb_map_it = rgb_map.begin(); rgb_map_it != rgb_map.end(); rgb_map_it++ ) {
	for( rgb_vec_it = (rgb_map_it->second).begin(); 
	     rgb_vec_it != (rgb_map_it->second).end(); rgb_vec_it++ ) {
	  position = rgb_vec_it->getPosition();
	  sf::Vector2f D = mouseP - position;
	  float D_2 = pow(D.x,2) + pow(D.y,2);
	  if( D_2 <= 0.002*( pow(position.x,2)+pow(position.y,2) ) ) {
	    buttons[rgb_map_it->first].setFillColor(rgb_vec_it->getOutlineColor());
	    buttons[rgb_map_it->first].setPosition(rgb_vec_it->getPosition());
	    update_button = false;
	    // send color and row # (or TL,TR,BR,BL)
	    UpdateVertexColors(rgb_vec_it->getOutlineColor(),rgb_map_it->first);
 	  }
	}
      }
    }
  }
  else 
    update_button = true;
}

void Menu::UpdateVertexColors(sf::Color newcolor, int corner) {
  sf::FloatRect bounds = slider_line_vec[corner].getBounds();
  sf::Vector2f sliderPos = sliders[corner].getPosition();
  float RGB_value = (sliderPos.x - bounds.left)/200.0*255.0;
  int newRGB = int(RGB_value);
  if( newcolor == sf::Color::Red ) 
    newcolor = sf::Color(newRGB,0,0);
  if( newcolor == sf::Color::Green ) 
    newcolor = sf::Color(0,newRGB,0);
  if( newcolor == sf::Color::Blue ) 
    newcolor = sf::Color(0,0,newRGB);
  
  switch(corner){
  case 0 :
    TL = newcolor;
    break;
  case 1 :
    TR = newcolor;
    break;
  case 2 : 
    BR = newcolor;
    break;
  case 3 : 
    BL = newcolor;
    break;
  default : 
    break;
  }
  sf::FloatRect tmp = menu.getLocalBounds();
  colorsvec.clear();
  colors[0].position = sf::Vector2f( tmp.left + tmp.width, tmp.top );
  colors[1].position = sf::Vector2f( tmp.left + tmp.width, tmp.top + displayy ); 
  colors[2].position = sf::Vector2f( displayx, displayy );
  colors[3].position = sf::Vector2f( displayx, 0.0 );
  colors[0].color = TL;
  colors[1].color = BL;
  colors[2].color = BR;
  colors[3].color = TR;
  colorsvec.push_back( colors );
}

////////////////////////////////////////////////////////
//                        DRAW                        //
////////////////////////////////////////////////////////
void Menu::DrawSlider(sf::RenderTarget& targ) const{
  std::vector<sf::VertexArray>::const_iterator cit1;
  for( cit1=slider_line_vec.begin(); cit1!=slider_line_vec.end(); cit1++ ){
    targ.draw(*cit1);
  }
  for( cit1=shade_vec.begin(); cit1!=shade_vec.end(); cit1++ ){
    targ.draw(*cit1);
  }
  std::vector<sf::RectangleShape>::const_iterator cit;
  for( cit=sliders.begin(); cit!=sliders.end(); cit++ ){
    targ.draw(*cit);
  }
  for( cit=RGB_bg_vec.begin(); cit!=RGB_bg_vec.end(); cit++ ){
    targ.draw(*cit);
  }
  std::map<int,std::vector<sf::CircleShape> >::const_iterator cit2;
  std::vector<sf::CircleShape>::const_iterator cit3;
  for( cit2 = rgb_map.begin(); cit2 != rgb_map.end(); cit2++ ){
    for( cit3 = (cit2->second).begin(); cit3 != (cit2->second).end(); cit3++ ) {
      targ.draw( *cit3 );
    }
  }
  std::map<int,sf::CircleShape>::const_iterator cit4;
  for(cit4 = buttons.begin(); cit4 != buttons.end(); cit4++ ){
    targ.draw(cit4->second);
  }
}

void Menu::DrawBoarder(sf::RenderTarget& targ) const{
  std::vector<sf::RectangleShape>::const_iterator cit;
  for( cit=boarder.begin(); cit!=boarder.end(); cit++ ){
    targ.draw(*cit);
  }
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
  DrawBoarder( target );
  DrawSlider( target );
}
////////////////////////////////////////////////////////
//                       SHADE                        //
////////////////////////////////////////////////////////
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

void Menu::MakeBoarder(float x, float y, float d, sf::Color color) {
  // Make a "Ceiling"
  sf::Vector2f ceiling_size( x, d );
  ceiling.setSize( ceiling_size );
  ceiling.setOrigin( x/2.0, d/2.0 );
  ceiling.setFillColor( color );
  ceiling.setPosition( x/2.0, d/2.0);
  // Make a bottom
  bottom.setSize( ceiling_size );
  bottom.setOrigin( x/2.0, d/2.0 );
  bottom.setFillColor( color );
  bottom.setPosition( x/2.0, y - d/2.0);
  // Make a Left / Right side
  left.setSize( sf::Vector2f( d, y - 2.0*d) );
  left.setOrigin( d/2.0, (y - 2.0*d)/2.0 );
  left.setFillColor( color );
  left.setPosition( d/2.0, y/2.0 );
  right.setSize( sf::Vector2f( d, y - 2.0*d) );
  right.setOrigin( d/2.0, (y - 2.0*d)/2.0 );
  right.setFillColor( color );
  right.setPosition( x - d/2.0, y/2.0 );
  boarder.push_back( ceiling );
  boarder.push_back( bottom );
  boarder.push_back( left );
  boarder.push_back( right );
}
