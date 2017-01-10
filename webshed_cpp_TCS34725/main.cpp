//main.cpp
#include "tcs34725_functions.h"
#include "tcs34725.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "timestuff.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

uint16_t red = 0,green = 0,blue = 0,clear = 0, lux = 0;
uint32_t rgb = 0;

std::string query = "INSERT INTO `minutedata`(`DateTime`, `Red`, `Green`, `Blue`, `Clear`, `Lux`) VALUES ('";


int main()
{
  //start by setting gain and integration time to max
  // read a set of values, if the values are too high [to be defined]
  // then we reduce gain and try again.

    cout<<"Trying to init the chip"<<endl;

    if (!init(TCS34725_INTEGRATIONTIME_700MS,TCS34725_GAIN_60X))
        cout<<"Failed to init the chip"<<endl;
    else
        cout<<"That seemed to go ok"<<endl;

    cout<<"Reading sensor"<<endl;

    enable();

    delay_ms(700);

    // get initial values
    tie(red,green,blue,clear) = get_raw_data();
    lux = calculate_lux(red, green, blue);
    cout << "Lux with gain 60X = " << lux << endl;
    // check if it's too bright, if so reduce gain and try again

while (lux > 500)
{
    switch (lux)
    {
      //want to be in range 0 to 500ish
      case 0 ... 500:
      break;
      case 501 ... 5999:
      {
        cout << "Still too bright, reduced gain to 1X" << endl;
        set_gain(TCS34725_GAIN_1X);
        enable();
        delay_ms(700);
        tie(red,green,blue,clear) = get_raw_data();
        lux = calculate_lux(red, green, blue);
        cout << "Lux with gain 1X = " << lux << endl;
        if (lux > 500)
        {
          cout << "Capping lux at 500" << endl;
          lux = 500; //cap lux at 500, could change integration time to lower, but needs testing. This may do.

          // this bit here should work, but doesn't
          // set_gain(TCS34725_GAIN_1X);
          // set_integration_time(TCS34725_INTEGRATIONTIME_50MS);
          // enable();
          // delay_ms(700);
          // tie(red,green,blue,clear) = get_raw_data();
          // lux = calculate_lux(red, green, blue);
          // cout << "Lux with gain 1X and integration time 50ms = " << lux << endl;
        }
      }
      break;
      case 6000 ... 65535:
      {
        cout << "First results were too bright. Gain now 4x" << endl;
        set_gain(TCS34725_GAIN_4X);
        enable();
        delay_ms(700);
        tie(red,green,blue,clear) = get_raw_data();
        lux = calculate_lux(red, green, blue);
        cout << "Lux with gain 4X = " << lux << endl;
      }
      break;
    }
}


    // values should be ok here

    std::stringstream SQL_QUERY;
    SQL_QUERY << query;
    SQL_QUERY << currentDateTime() << "','";
    SQL_QUERY << red << "','";
    SQL_QUERY << green << "','";
    SQL_QUERY << blue << "','";
    SQL_QUERY << clear << "','";
    SQL_QUERY << lux << "')";

    cout << SQL_QUERY.str() <<endl;

    rgb = (red * 65536) + (green * 256) + blue;
    cout<<"RGB Colour: " << std::hex << rgb << endl;

    try {
      sql::Driver *driver;
      sql::Connection *con;
      sql::Statement *stmt;
      sql::ResultSet *res;

      /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "sky", "colour");
    /* Connect to the MySQL test database */
    con->setSchema("skycolour");
    stmt = con->createStatement();
    res = stmt->executeQuery(SQL_QUERY.str());
    if (res) cout << "Result of SQL is " << res << endl;
}
catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}


return 0;
}
