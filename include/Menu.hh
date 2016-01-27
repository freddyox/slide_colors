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
  sf::RectangleShape slider,bg;
  std::vector<sf::RectangleShape> sliders, bg_vec;

  sf::VertexArray slider_line, shade;
  std::vector<sf::VertexArray> slider_line_vec,shade_vec, slider_shade_vec;
  
  std::vector<sf::RectangleShape>::iterator slide_it;
  std::vector<sf::VertexArray>::iterator sit;

  int R,G,B,R2;
  bool update;

public:
  Menu(float,float);
  ~Menu() {};
  void draw(sf::RenderTarget&, sf::RenderStates) const;

  void MakeMenu();
  void MakeVertexColors();

  void MakeSlider(float,float,int,float,sf::Color, sf::RectangleShape);
  void DrawSlider(sf::RenderTarget&) const;
  void UpdateSliders(sf::Vector2f);
  void SetSliderValue();

  void ShadeRectangle(sf::Vector2f,sf::Vector2f,sf::Color,float,bool);
};
#endif
