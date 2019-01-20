#ifndef FORECASTAPI_H
#define FORECASTAPI_H
#include <string>
#include <iostream>


class MainApp;

class ForecastAPI
{
  public:
    ForecastAPI();
    void make_post_request(MainApp *instance, const char* url);
    static size_t write_response(void *ptr, size_t size, size_t nmemb, std::string *data);
    

  private:
 
    std::string response;
};

#endif // FORECASTAPI_H
