# Weather Forecast Application
This is a demo gtkmm-based weather forecast application


### The weather forecast application is tested on Ubuntu and can be run effortlessly.
> This project is designed to use gtk c++ api and does't serve any commercial purposes.

* [x] All the codes are written in C++ and sometimes, the C native syntax is invoked as well.
* [x] This project connects to ```openweathermap forecast api``` and fetches the latest weather forecast for every given city.
We are using five days forecast service offered by openweathermap in which it provides 8 temperature samples for each day.
* [ ] This project is not able to get timezone of the selected city. Though, there is no exact timing details provided.
* [x] Sqlite3 ,a mere lightweight database, is the backbone of datastorage in the app.
* [x] GTK3 v22 is used.
* [ ] GTK4 is not supported.

## Dependencies
Before compiling the project, make sure the following packages have been already installed on your machine.
- gcc
- g++ supporting c++11 (>4.6)
- libgtkmm-3.0-dev
- libsqlite3-dev
- libjsoncpp-dev

## Compiling the project
Get the [source code](https://github.com/falahati1987/forecast)

``` 
git clone https://github.com/falahati1987/forecast 
cd forecast
make -f Makefile
```
Now, you can run it by ```./forecast```

## Unit Test
The main functionality of this application is tested by Google Unit Test framework. Database connection, query execution and
api connection are subjected to test.
# Snapshot
![alt text](https://github.com/falahati1987/forecast/blob/master/demo.png)
