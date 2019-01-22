#ifndef GTKMM_EXAMPLE_CANVAS_H
#define GTKMM_EXAMPLE_CANVAS_H

#define CANVAS_WIDTH 600
#define CANVAS_HEIGHT 400
#define TRIANGLE_EDGE_LENGTH 10
#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include <gdkmm/rgba.h>

#include <vector>
#include "constants.h"
#include <string>
#include <iostream>
/**
 * This struct serves drawing purposes for Cairo context
 * */ 
struct Point
{
  int x;
  int y;
};
class Canvas : public Gtk::DrawingArea
{
public:
  /**
   * Constructor. There are two ways to instantiate the Canvas. One can be injected the drawing information exactly when the class 
   * is being instantiated. The other way is to use @set_input_param method and instantiate empty context.
   * */
  Canvas(std::map<std::string, std::vector<std::string>> &map_, int id, const std::string& name);
  /**
   * Empty canvas, with defined size
   * */
  Canvas();
  /**
   * Destructor
   * */
  virtual ~Canvas();
  /**
   * This method injects the temperature information to the canvas
   * Other way to do so, is passing the information map in the constructor.
   * */
  void set_input_param(std::map<std::string, std::vector<std::string>> &map_, int id, const std::string& name);
  /**
   * This feature sets the animation parameter (The horizontal displacement of the icons)
   * */
  void set_animation_param(const int &offset);
  /**
   * Redrawing the painted canvas
   * */
  void force_redraw();
  

protected:
  /* Override default signal handler */
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
  /* Drawing a rectangle in the canvas */
  void draw_rectangle(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
  /**
   * Drawing text in the defined position
   * @var const Cairo::RefPtr<Cairo::Context> &cr reference to the passed cairo context
   * @var std::string message, the text is deemed to be printed
   * @var int x , @var int y the coordinate this text will be put in.
   * @var int font_size , the size of the printed text
   *  
   * */
  void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, std::string message,
                 int x, int y, int font_size = 0);
                   /**
   * Drawing text in the defined position
   * @var const Cairo::RefPtr<Cairo::Context> &cr reference to the passed cairo context
   * @var int message, the text is deemed to be printed
   * @var int x , @var int y the coordinate this text will be put in.
   * @var int font_size , the size of the printed text
   *  
   * */
  void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, int message,
                 int x, int y, int font_size = 0);

  /**
   * Drawing the main grid of the canvas
   * All the elements will be placed in this grid
   * */
  void draw_grid(const Cairo::RefPtr<Cairo::Context> &, int width, int height);
  /**
   * Computing the rotated coordinates of a given point
   * */
  Point rotate(Point, double);
  /**
   * Drawing a triangle in the given place on the canvas
   * @var int x_center: x coord of the triangle
   * @var int y_center: y coord of the triangle
   * @var double edgeLen: the length of the edge
   * @var double rotation: rotation degree of the triangle 
   * */
  void draw_equilateral_triangle(const Cairo::RefPtr<Cairo::Context> &cr,
                                 int xCenter,
                                 int yCenter,
                                 double edgeLen,
                                 double rotation);

private:
/**
 * Computing the contrasted version of every GdkRGBA
 * */
  void get_contrast_color(const GdkRGBA &rgbMap, GdkRGBA &rgbaInstance);
  
  std::map<int, std::string> map_ActualWeatherInWindow;
  int animation_offset;
  int city_id;
  std::map<std::string, std::vector<std::string>> map_result;
  
  std::string cityName;
  GdkRGBA rgbRed, rgbBlue;
};

#endif // GTKMM_EXAMPLE_Canvas_H