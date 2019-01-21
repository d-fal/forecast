#ifndef GTKMM_EXAMPLE_CANVAS_H
#define GTKMM_EXAMPLE_CANVAS_H

#define TRIANGLE_EDGE_LENGTH 10
#include <gtkmm.h>
#include <vector>
#include "constants.h"
#include <string>
#include <iostream>

struct Point
{
  int x;
  int y;
};
class Canvas : public Gtk::DrawingArea
{
public:
  Canvas(std::map<std::string, std::vector<std::string>> &map_, int id, const std::string& name);
  Canvas();
  virtual ~Canvas();
  void set_input_param(std::map<std::string, std::vector<std::string>> &map_, int id, const std::string& name);
  void set_animation_param(const int &offset);
  void force_redraw();
  bool check_is_available();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
  void draw_rectangle(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
  void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, std::string message,
                 int rectangle_width, int rectangle_height, int font_size = 0);
  void draw_text(const Cairo::RefPtr<Cairo::Context> &cr, int message,
                 int rectangle_width, int rectangle_height, int font_size = 0);

  void set_base_colors();
  void draw_grid(const Cairo::RefPtr<Cairo::Context> &, int, int);
  Point rotate(Point, double);
  void draw_equilateral_triangle(const Cairo::RefPtr<Cairo::Context> &cr,
                                 int x_center,
                                 int y_center,
                                 double edgeLen,
                                 double rotation);

private:
  void get_contrast_color(const GdkRGBA &rgbMap, GdkRGBA &rgbaInstance);
  double m_fix;
  double forecastWatchWindow;
  std::map<int, std::string> map_ActualWeatherInWindow;

  std::map<std::string, GdkRGBA> map_WeatherThemes;

  int animation_offset;
  int animation_position;
  int city_id;
  std::map<std::string, std::vector<std::string>> map_result;
  
  std::string cityName;
  bool isBusy;
};

#endif // GTKMM_EXAMPLE_Canvas_H