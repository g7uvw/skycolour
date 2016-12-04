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


// void delay_ms(unsigned int delay_ms)
// {
//   struct timespec tim, tim2;
//   tim.tv_sec = 0;
//   tim.tv_nsec = delay_ms * 1000000; //10 ms
//   nanosleep(&tim , &tim2);
// }

// // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
// const std::string currentDateTime() {
//     time_t     now = time(0);
//     struct tm  tstruct;
//     char       buf[80];
//     tstruct = *localtime(&now);
//     strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//
//     return buf;
// }

uint16_t red = 0,green = 0,blue = 0,clear = 0, lux = 0;

std::string query = "INSERT INTO `minutedata`(`DateTime`, `Red`, `Green`, `Blue`, `Clear`, `Lux`) VALUES ('";




using namespace std;

int main()
{

    cout<<"Trying to init the chip"<<endl;

    if (!init(TCS34725_INTEGRATIONTIME_700MS,TCS34725_GAIN_60X))
    //if (!init(TCS34725_INTEGRATIONTIME_2_4MS,TCS34725_GAIN_4X))
        cout<<"Failed to init the chip"<<endl;
    else
        cout<<"That seemed to go ok"<<endl;

    cout<<"Reading sensor"<<endl;

    enable();


    delay_ms(700);


    uint32_t rgb = 0;
    tie(red,green,blue,clear) = get_raw_data();

    lux = calculate_lux(red, green, blue);

    std::stringstream SQL_QUERY;
    SQL_QUERY << query;
    SQL_QUERY << currentDateTime() << "','";
    SQL_QUERY << red << "','";
    SQL_QUERY << green << "','";
    SQL_QUERY << blue << "','";
    SQL_QUERY << clear << "','";
    SQL_QUERY << lux << "')";

    cout << SQL_QUERY.str() <<endl;

    rgb = (red/32 * 65536) + (green/32 * 256) + blue/32;
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
