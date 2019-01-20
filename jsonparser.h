#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <gtkmm.h>
#include <jsoncpp/json/json.h>
#include <thread>
#include <mutex>

class Cities;


class JsonParser
{

public:
  JsonParser();
  ~JsonParser();
  
  // template <typename T>
  void load_city_names(Cities *instance);
  // template <class T>
  

  void get_data(double *fraction_done, Glib::ustring *message) const;
  void stop_work();
  bool has_stopped() const;


private:
  // Synchronizes access to member data.
  mutable std::mutex m_Mutex;

  // Data used by both GUI thread and worker thread.
  bool m_shall_stop;
  bool m_has_stopped;
  double m_fraction_done;
  Glib::ustring m_message;
 
};

#endif // JSON_PARSER_H