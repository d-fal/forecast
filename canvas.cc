#include "canvas.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <iterator>
#include <jsoncpp/json/json.h>
#include <stdexcept>
#include "constants.h"

using namespace std;
Canvas::Canvas()
{
  set_size_request(CANVAS_WIDTH, CANVAS_HEIGHT);
}
Canvas::Canvas(std::map<std::string, std::vector<std::string>> &map_, int id, const std::string &name)
{
  set_size_request(CANVAS_WIDTH, CANVAS_HEIGHT);
  city_id = id;
  map_result = map_;
  cityName = name;
}
void Canvas::set_input_param(std::map<std::string, std::vector<std::string>> &map_, int id, const std::string &name)
{
  city_id = id;
  map_result = map_;
  cityName = name;
}
Canvas::~Canvas()
{
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  draw_grid(cr, width, height);

  return true;
}
/**
 * @param const Cairo::RefPtr<Cairo::Context> &cr 
 * the cairo pointer
 * @param int width 
 * (Self representative)
 * @param int height
 * (Self representative)
 * 
 * */
void Canvas::draw_grid(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
  int pos_x = 0;

  time_t rawtime;
  struct tm *ptm;
  int pos_center = height >> 1;
  int lastPrintedDay = -1;
  int numberOfDaysToCover = 5;
  int incrementBy = width / numberOfDaysToCover;
  int standardImageWidth;
  GdkRGBA rgbMap, rgbContrast, rgbRed, rgbBlue;
  int tempMax, tempMin, temp, _tempMax = INT_MIN, _tempMin = INT_MAX, _temp = 0;

  int pos_temp = pos_center;
  int pos_tempMin = pos_center + 30;
  int pos_tempMax = pos_center - 30;
  int pos_weatherCondition = int(height - 20);
  int pos_weatherConditionDescription = int(height - 10);
  int pos_triangleUp = pos_center - 15;
  int pos_triangleDown = pos_center + 15;
  int pos_date = pos_center - 60;
  int pos_humidity = pos_center + 50;
  int pos_cityName = pos_center - 100;
  int pos_cloudiness = pos_center + 70;
  char dateString[50];
  int samplesCount = 1;
  rgbRed = {.red = .9,
            .green = 0,
            .blue = 0};

  rgbBlue = {.red = .1,
             .green = 0,
             .blue = .9};
  Constants::getInstance()->get_standard_resource_width(standardImageWidth);
  for (auto const &it : map_result)
  {
    try
    {

      rawtime = (time_t)std::stoi(it.second[0]);
      ptm = localtime(&rawtime);
      Constants::getInstance()->convert_temperature(it.second[3], tempMax);
      Constants::getInstance()->convert_temperature(it.second[2], tempMin);
      Constants::getInstance()->convert_temperature(it.second[1], temp);
      _temp += temp;
      _tempMax = tempMax > _tempMax ? tempMax : _tempMax;
      _tempMin = tempMin < _tempMin ? tempMin : _tempMin;
      samplesCount++;
      if (ptm->tm_mday == lastPrintedDay)
        continue;

      cr->rectangle(pos_x, 0, incrementBy - 1, height);
      rgbMap = Constants::getInstance()->get_map_of_weather_theme_colors(it.second[6]);
      get_contrast_color(rgbMap, rgbContrast);
      cr->set_source_rgba(rgbMap.red, rgbMap.green, rgbMap.blue, .3);
      cr->fill();
      /** Write temprature
      * Contrast text color */

      cr->set_source_rgba(rgbContrast.red, rgbContrast.green, rgbContrast.blue, 1);
      draw_text(cr, it.second[4], pos_x + incrementBy / 2, pos_weatherCondition, 14);
      draw_text(cr, it.second[5], pos_x + incrementBy / 2, pos_weatherConditionDescription, 8);

      draw_text(cr, std::to_string(_temp / samplesCount) + Constants::getInstance()->get_selected_temperature_symbol(),
                pos_x + incrementBy / 2, pos_temp, 16);
      draw_text(cr, _tempMax, pos_x + incrementBy / 2, pos_tempMax, 10);
      draw_text(cr, _tempMin, pos_x + incrementBy / 2, pos_tempMin, 10);

      strftime(dateString, 50, "%a, %d %b", ptm);
      draw_text(cr, dateString, pos_x + incrementBy / 2, pos_date, 12);

      draw_text(cr, "Humidity: " + it.second[7] + "%", pos_x + incrementBy / 2, pos_humidity, 12);
      draw_text(cr, "Cloudiness: " + it.second[8] + "%", pos_x + incrementBy / 2, pos_cloudiness, 10);
      draw_text(cr, cityName, width / 2, pos_cityName, 20);

      cr->set_source_rgba(rgbRed.red, rgbRed.green, rgbRed.blue, .9);
      draw_equilateral_triangle(cr, pos_x + incrementBy / 2, pos_triangleUp, 10, 45);

      cr->set_source_rgba(rgbBlue.red, rgbBlue.green, rgbBlue.blue, .9);
      draw_equilateral_triangle(cr, pos_x + incrementBy / 2, pos_triangleDown, 10, 0);

      Gdk::Cairo::set_source_pixbuf(cr, Constants::getInstance()->get_image_pointer_by_keyword(it.second[6]),
                                    pos_x + incrementBy / 2 - standardImageWidth / 2, 5 + animation_offset);
      cr->paint();

      pos_x += incrementBy;
      if (lastPrintedDay != -1)
      {
        _tempMax = INT_MIN;
        _tempMin = INT_MAX;
        _temp = 0;
        samplesCount = 1;
      }
      lastPrintedDay = (int)ptm->tm_mday;
    }
    catch (std::overflow_error e)
    {
      std::cerr << "Error" << e.what();
    }
    catch (const std::out_of_range &e)
    {
      // index out of range
    }
  }
}

void Canvas::set_animation_param(const int &offset)
{
  animation_offset = offset;
  force_redraw();
}

// force the redraw of the image
void Canvas::force_redraw()
{
  auto win = get_window();
  if (win)
  {
    Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
    win->invalidate_rect(r, false);
  }
}
void Canvas::draw_text(const Cairo::RefPtr<Cairo::Context> &cr, int message, int x, int y, int font_size)
{
  draw_text(cr, std::to_string(message), x, y, font_size);
}
void Canvas::draw_text(const Cairo::RefPtr<Cairo::Context> &cr, std::string message, int x, int y, int font_size)
{
  Pango::FontDescription font;

  font.set_family("Monospace");
  font.set_weight(Pango::WEIGHT_BOLD);

  if (font_size != 0)
    font.set_absolute_size(font_size * Pango::SCALE);

  auto layout = create_pango_layout(Glib::ustring(message));

  layout->set_font_description(font);

  int text_width;
  int text_height;
  layout->get_pixel_size(text_width, text_height);
  cr->move_to((x - text_width / 2), (y - text_height / 2));
  layout->show_in_cairo_context(cr);
}

void Canvas::draw_equilateral_triangle(const Cairo::RefPtr<Cairo::Context> &cr,
                                       int x_center,
                                       int y_center,
                                       double edgeLen,
                                       double rotation)
{
  Point A, B, C;
  A.x = 0;
  A.y = edgeLen * (1 / (sqrt(3)));
  B.x = edgeLen * (0.5);
  B.y = -edgeLen * (1 / (2 * sqrt(3)));
  C.x = -B.x;
  C.y = B.y;
  A = rotate(A, rotation);
  B = rotate(B, rotation);
  C = rotate(C, rotation);
  cr->move_to(A.x + x_center, A.y + y_center);
  cr->line_to(B.x + x_center, B.y + y_center);
  cr->line_to(C.x + x_center, C.y + y_center);
  cr->line_to(A.x + x_center, A.y + y_center);
  cr->close_path();
  cr->stroke_preserve();
  cr->fill();
}
Point Canvas::rotate(Point p, double deg)
{
  Point p_Prime;
  p_Prime.x = p.x * cos(deg) - p.y * sin(deg);
  p_Prime.y = p.x * sin(deg) + p.y * cos(deg);
  return p_Prime;
}


void Canvas::get_contrast_color(const GdkRGBA &rgbMap, GdkRGBA &rgbaInstance)
{
  rgbaInstance.red = 1.f - rgbMap.red;
  rgbaInstance.blue = 1.f - rgbMap.blue;
  rgbaInstance.green = 0;
}
