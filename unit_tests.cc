#include <gtest/gtest.h>
#include "db.h"
#include "commons.h"
#include "sqlite3.h"
#include <gtkmm.h>
#include "constants.h"
#include "forecastapi.h"
/**
 * g++ unit_tests.cc db.cc constants.cc forecastapi.cc mainapp.cc canvas.cc cities.cc settings.cc jsonparser.cc treeview_withpopup.cc skeletonapplication.cc resources.o -lgtest  -lgtest_main -pthread -lsqlite3 -lcurl -ljsoncpp `pkg-config gtkmm-3.0 --cflags --libs` -o test
* */
TEST(DatabaseTest, RunQuery_1)
{
    Db *dbInstance = Db::getInstance();
    int response = dbInstance->exec("Select count(*) from Cities");
    ASSERT_TRUE(response == SQLITE_OK);
}
TEST(ConnectivityTest, ConnectServer_1)
{
    std::string url;
    int testCityId = 112931; /* valid ID */
    int errorCode;
    Constants *c = Constants::getInstance();
    ForecastAPI *api = new ForecastAPI();
    std::map<std::string, std::vector<std::string>> resultMap;
    c->get_url_by_city_id(url, testCityId);
    api->make_post_request(url.c_str(), resultMap, errorCode);

    for (auto const &it : resultMap)
    {
        ASSERT_TRUE(std::stoi(it.second[9]) == testCityId);
    }
    EXPECT_EQ(errorCode, int(CONNECTION_OK));
}
TEST(ConnectivityTest, InvalidCityId_1) /* Test if the unknown city id is being handled */
{
    std::string url;
    int testCityId = 11293;
    int errorCode;
    Constants *c = Constants::getInstance();
    ForecastAPI *api = new ForecastAPI();
    std::map<std::string, std::vector<std::string>> resultMap;
    c->get_url_by_city_id(url, testCityId);
    api->make_post_request(url.c_str(), resultMap, errorCode);
    EXPECT_EQ(errorCode, int(CONNECTION_ADDRESS_NOT_FOUND));
}
/* Disconnect internet and uncomment the following lines. 
 * this test should be successful then */
// TEST(ConnectivityTest, NoInternet_1) 
// {
//     std::string url;
//     int testCityId = 11293;
//     int errorCode;
//     Constants *c = Constants::getInstance();
//     ForecastAPI *api = new ForecastAPI();
//     std::map<std::string, std::vector<std::string>> resultMap;
//     c->get_url_by_city_id(url, testCityId);
//     api->make_post_request(url.c_str(), resultMap, errorCode);
//     EXPECT_EQ(errorCode, int(CONNECTION_FAILED));
// }

TEST(Conversion , CelciusToFahrenheitDefault)
{
    Constants *c = Constants::getInstance();
    float temp = 18.f; //In celcius
    int convertedTemp;
    c->set_temperature_system(CELCIUS);
    c->convert_temperature(temp,convertedTemp, CELCIUS);
    EXPECT_EQ(convertedTemp,18);
}
TEST(Conversion , CelciusToFahrenheitSetCelcius)
{
    Constants *c = Constants::getInstance();
    float temp = 273.f; //In celcius
    int convertedTemp;
    c->set_temperature_system(CELCIUS);
    c->convert_temperature(temp, convertedTemp, KELVIN);
    EXPECT_EQ(convertedTemp,0);
}
TEST(Conversion , CelciusToFahrenheitSetFahrenheit)
{
    Constants *c = Constants::getInstance();
    float temp = 273.f; //In celcius
    int convertedTemp;
    c->set_temperature_system(FAHRENHEIT);
    c->convert_temperature(temp, convertedTemp, KELVIN);
    EXPECT_EQ(convertedTemp,32);
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}