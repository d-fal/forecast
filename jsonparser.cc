#include "jsonparser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <string>
#include "cities.h"
#include "db.h"
#include <string>
#include <regex>


using namespace std;
using namespace std::chrono;

JsonParser::JsonParser() : m_Mutex(),
                           m_shall_stop(false),
                           m_has_stopped(false),
                           m_fraction_done(0.0),
                           m_message()
{
}

JsonParser::~JsonParser()
{
}
void JsonParser::get_data(double *fraction_done, Glib::ustring *message) const
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    if (fraction_done)
        *fraction_done = m_fraction_done;

    if (message)
        *message = m_message;
}

void JsonParser::stop_work()
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_shall_stop = true;
}

bool JsonParser::has_stopped() const
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    return m_has_stopped;
}

// template <typename T>
void JsonParser::load_city_names(Cities *instance)
{
    std::string message;
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_has_stopped = false;
        m_fraction_done = 0.0;
        m_message = "";
    }

    ifstream ifs("city.list.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    std::string query = "";
    const Json::Value &characters = obj;
    std::string cityName;
    for (int i = 0; i < (int)characters.size(); i++)
    {
        cityName = characters[i]["name"].asString();
        std::replace(cityName.begin(),cityName.end(),'\'','`');
        message += "(" +
                   characters[i]["id"].asString() + " , '" +
                   cityName + "','" +
                   characters[i]["country"].asString() + "'),";

        if (i % 10000 == 0 && i!=0)
        {

            message = "(id, name, country) VALUES " + message.substr(0, message.size() - 1);
            instance->on_progress(message.c_str());
           
            message = "";
        }
    }

    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_shall_stop = false;
        m_has_stopped = true;
    }

    instance->notify();
}

