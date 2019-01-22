## Unit test
In order to compile the tests, copy and paste the following compilation script in the terminal.

```g++ unit_tests.cc db.cc constants.cc forecastapi.cc mainapp.cc canvas.cc cities.cc settings.cc jsonparser.cc treeview_withpopup.cc skeletonapplication.cc resources.o -lgtest  -lgtest_main -pthread -lsqlite3 -lcurl -ljsoncpp `pkg-config gtkmm-3.0 --cflags --libs` -o test```

A ```test``` executable will be created and the base functionalities of the app, such as interoperability capabalities would be tested. 
