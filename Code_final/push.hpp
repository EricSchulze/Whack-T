#include <tle94112-ino.hpp>
#include <tle94112-motor-ino.hpp>
#include "SPI.h"

#define duration 100
Tle94112Ino controller = Tle94112Ino();

Tle94112Motor aktor1(controller);
Tle94112Motor aktor2(controller);
Tle94112Motor aktor3(controller);
Tle94112Motor aktor4(controller);
Tle94112Motor aktor5(controller);
Tle94112Motor aktor6(controller);
Tle94112Motor aktor7(controller);
Tle94112Motor aktor8(controller);
Tle94112Motor aktor9(controller);
Tle94112Motor kompressor(controller);

void init_mhb()
{
  controller.begin();
  
  aktor1.connect(aktor1.HIGHSIDE,controller.TLE_HB7);  //11
  aktor2.connect(aktor2.HIGHSIDE,controller.TLE_HB11); //8
  aktor3.connect(aktor3.HIGHSIDE,controller.TLE_HB10); //10
  aktor4.connect(aktor4.HIGHSIDE,controller.TLE_HB9);  //7
  aktor5.connect(aktor5.HIGHSIDE,controller.TLE_HB12);//9
  aktor6.connect(aktor6.HIGHSIDE,controller.TLE_HB8); //12
  aktor7.connect(aktor7.HIGHSIDE,controller.TLE_HB3);  
  aktor8.connect(aktor8.HIGHSIDE,controller.TLE_HB4);
  aktor9.connect(aktor9.HIGHSIDE,controller.TLE_HB6);
  kompressor.connect(kompressor.HIGHSIDE,controller.TLE_HB5);  
  kompressor.connect(kompressor.HIGHSIDE,controller.TLE_HB2);
  kompressor.connect(kompressor.HIGHSIDE,controller.TLE_HB1);

  aktor1.begin();
  aktor2.begin();
  aktor3.begin();
  aktor4.begin();
  aktor5.begin();
  aktor6.begin();
  aktor7.begin();
  aktor8.begin();
  aktor9.begin();
  kompressor.begin();
}

void push(int i)
{
  switch (i) 
    {
  //  hit_marker=0;
    case 1:    // your hand is on the sensor
       aktor1.start(255);
       delay(duration);
       aktor1.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    case 2:    // your hand is close to the sensor
       aktor2.start(255);
       delay(duration);
       aktor2.stop(0);  
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up);
      break;
    case 3:    // your hand is a few inches from the sensor
       aktor3.start(255);
       delay(duration);
       aktor3.stop(0);  
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up);
      break;
    case 4:    // your hand is nowhere near the sensor
       aktor4.start(255);
       delay(duration);
       aktor4.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    case 5:    // your hand is on the sensor
       aktor5.start(255);
       delay(duration);
       aktor5.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    case 6:    // your hand is close to the sensor
       aktor6.start(255);
       delay(duration);
       aktor6.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    case 7:    // your hand is a few inches from the sensor
       aktor7.start(255);
       delay(duration);
       aktor7.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    case 8:    // your hand is nowhere near the sensor
       aktor8.start(255);
       delay(duration);
       aktor8.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    case 9:    // your hand is nowhere near the sensor
       aktor9.start(255);
       delay(duration);
       aktor9.stop(0); 
       calculate_average(piston_up);
       print_sensor_magnitudes(piston_up); 
      break;
    }
  
  //to see the value, if the piston is at the top :)

}

int random_generator()
{
  //if trust M works this code will be added here
  return random(1,9);
}
