#ifndef MENU_HH
#define MENU_HH

#include <SFML/Graphics.hpp>

class Menu : public sf::Drawable, public sf::Transformable {
private:
  float displayx, displayy; // x,y coordinates of Window
  sf::RectangleShape menu;

  // The Color Window:
  sf::VertexArray colors;
  std::vector<sf::VertexArray> colorsvec;

  // The Slider Menu
  sf::RectangleShape slider,bg,rgb_bg;
  std::vector<sf::RectangleShape> sliders, bg_vec, RGB_bg_vec;

  sf::VertexArray slider_line, shade;
  std::vector<sf::VertexArray> slider_line_vec,shade_vec, slider_shade_vec;
  
  std::vector<sf::RectangleShape>::iterator slide_it;
  std::vector<sf::VertexArray>::iterator sit;

  // controls RGB circles
  std::map<int,std::vector<sf::CircleShape> > rgb_map;
  std::map<int,std::vector<sf::CircleShape> >::iterator rgb_map_it;
  std::vector<sf::CircleShape> rgb_vec;
  std::vector<sf::CircleShape>::iterator rgb_vec_it;

  // controls the "button" feature
  std::map<int,sf::CircleShape> buttons;
  std::map<int,sf::CircleShape>::iterator bit;

  // Initializations for top left (TL), top right (TR), etc...
  sf::Color TL,TR,BR,BL;
  int TL_mag,TR_mag,BR_mag,BL_mag;
  bool update; // controls UpdateSliders, we only want an update once
               // the mouse is released
  bool update_button;

  // Boarder
  sf::RectangleShape ceiling, left, right, bottom;
  std::vector<sf::RectangleShape> boarder;


public:
  Menu(float,float);
  ~Menu() {};
  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void DrawSlider(sf::RenderTarget&) const;
  void DrawBoarder(sf::RenderTarget&) const;
  
  void MakeMenu();
  void MakeVertexColors(sf::Color,sf::Color,sf::Color,sf::Color);
  void UpdateVertexColors(sf::Color,int);

  void MakeSlider(float,float,int,float,sf::Color, sf::RectangleShape);
  void UpdateSliders(sf::Vector2f);
  void SetSliderValue();
  void ResetSlider();
  void MakeRGB(sf::Color);
  void UpdateRGB(sf::Vector2f);
  void ShadeRectangle(sf::Vector2f,sf::Vector2f,sf::Color,float,bool);
  void MakeBoarder(float,float,float,sf::Color);
};
#endif
