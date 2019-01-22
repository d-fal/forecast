#ifndef CONSTANTS_H
#define CONSTANTS_H

#define UNINTIALIZED_INSTANCE 0
#define HEIGHT_OF_WEATHER_CONDITION_ICON 60 // experimental
#define WIDTH_OF_WEATHER_CONDITION_ICON 60  // experimental


#include <gtkmm.h>
#include <vector>
#include <string>
#include "commons.h"
class Constants
{
private:
  static Constants *instance;

  Constants();
  virtual ~Constants();

  std::map<std::string, GdkRGBA> map_WeatherThemes;
  Gdk::RGBA
      rgba01,
      rgba02,
      rgba03,
      rgba04,
      rgba09,
      rgba10,
      rgba11,
      rgba13,
      rgba50;
  Glib::RefPtr<Gdk::Pixbuf> image;
  std::map<std::string, Glib::RefPtr<Gdk::Pixbuf>> map_Images;
  std::vector<std::string> vec_WeatherConditions = {"01", "02", "03", "04", "09", "10", "11", "13", "50"};

  int iconsWidth;
  TemperatureSystems chosenTemperature;
  bool dynamicUpdateContinue, showAllDataAtOnce;

public:
  static Constants *getInstance();
  std::string apiKey;
  /**
     * @return std::map<std::string , GdkRGBA>
     * @code
     * int x
     * @endcode
     */
  std::map<std::string, GdkRGBA> get_map_of_weather_theme_colors();
  GdkRGBA get_map_of_weather_theme_colors(std::string key);
  Glib::RefPtr<Gdk::Pixbuf> get_image_pointer_by_keyword(std::string keyword);
  void get_url_by_city_id(std::string &url, const int &id);
  void get_standard_resource_width(int &width);
  /**
   * Dynaimc update of the main canvas
   * */
  bool get_dynamic_update_status();
  void set_dynamic_update_status(bool status);

  bool get_show_all_data_consent();
  void set_show_all_data_consent(bool status);

  TemperatureSystems get_chosen_temprature();
  void map_temperature_system(const float &temp, int &convertedTemp, const TemperatureSystems &system);
  void convert_temperature(std::string temp, int &converetdTemp,const TemperatureSystems &system);
  void convert_temperature(float &temp, int &convertedTemp,const TemperatureSystems &system);
  
  void set_temperature_system(TemperatureSystems temp);
  std::string get_selected_temperature_symbol();
};

#endif //CONSTANTS_H