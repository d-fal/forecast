#include "forecastapi.h"
#include <curl/multi.h>
#include <string.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include "mainapp.h"

using namespace std;
ForecastAPI::ForecastAPI()
{
}
size_t ForecastAPI::write_response(void *ptr, size_t size, size_t nmemb, std::string *data)
{
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}
void ForecastAPI::make_post_request(MainApp *instance, const char *url)
{
    int errorCode;
    std::map<std::string, std::vector<std::string>> map_result;
    make_post_request(url, map_result, errorCode);
    instance->notify_update_map(map_result);
}

void ForecastAPI::make_post_request(const char *url,
                                    std::map<std::string, std::vector<std::string>> &map_result,
                                    int &errorCode)
{
    std::string data;
    auto curl = curl_easy_init();
    std::vector<std::string> _vec;

    if (curl)
    {
        try
        {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
            curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
            curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

            std::string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            curl = NULL;

            Json::Value root;
            Json::Reader reader;

            bool parsingSuccessful = reader.parse(data, root);
            if (!parsingSuccessful)
            {
                cout << "Error parsing the string" << endl;
            }
            const Json::Value convertedCitiesArray = root["list"];
            errorCode = std::stoi(root["cod"].asString());
            for (int index = 0; index < (int)convertedCitiesArray.size(); ++index)
            {
                try
                {
                    _vec.push_back(convertedCitiesArray[index]["dt"].asString());                        // Index: 0
                    _vec.push_back(convertedCitiesArray[index]["main"]["temp"].asString());              // Index: 1
                    _vec.push_back(convertedCitiesArray[index]["main"]["temp_min"].asString());          // Index: 2
                    _vec.push_back(convertedCitiesArray[index]["main"]["temp_max"].asString());          // Index: 3
                    _vec.push_back(convertedCitiesArray[index]["weather"][0]["main"].asString());        // Index: 4
                    _vec.push_back(convertedCitiesArray[index]["weather"][0]["description"].asString()); // Index: 5
                    _vec.push_back(convertedCitiesArray[index]["weather"][0]["icon"].asString());        // Index: 6
                    _vec.push_back(convertedCitiesArray[index]["main"]["humidity"].asString());          // Index: 7
                    _vec.push_back(convertedCitiesArray[index]["clouds"]["all"].asString());             // Index: 8
                    _vec.push_back(root["city"]["id"].asString());                                       // Index: 9

                    map_result.insert(
                        std::pair<std::string, std::vector<std::string>>(convertedCitiesArray[index]["dt"].asString(), _vec));

                    _vec.clear();
                }
                catch (const std::exception &e)
                {
                    cout << "error: " << e.what();
                }
            }
        }
        catch (...)
        {
            errorCode = int(CONNECTION_FAILED);
        }
    }
}
