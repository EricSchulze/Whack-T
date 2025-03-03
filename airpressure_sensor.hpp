#include <Dps3xx.h>

// Dps3xx Object
Dps3xx Dps3xxPressureSensor = Dps3xx();
Dps3xx Dps3xxPressureSensor2 = Dps3xx();
float difference_dps_sensor=0;
bool game_begin=false;
uint8_t oversampling = 7;
float calc_difference();
float hight();
void init_dps_sensor()
{
  Dps3xxPressureSensor.begin(Wire1, 0x76);
  Dps3xxPressureSensor2.begin(Wire1);
  delay(1000);
  Serial.println("DPS Sensor Init complete!");
  difference_dps_sensor=calc_difference();
  Serial.println("Difference: ");
  Serial.print(difference_dps_sensor);
}

bool hammer_lift()
{
  bool logic=false;
  float hoehe=hight();
  if(game_begin==false)
  {
    if(hoehe>=30)
    {
    Serial.print("Hammer oben");
    game_begin=true;
    logic=true;
    }
    else
    {
    Serial.print("Hammer unten");
    }
  }
  else
  {
    game_begin=false;
  }
  return logic;
}

float calc_difference()
{//vielleicht hilft ein Filter
  float pressure_init_1=0;
  float pressure_init_1_filter=0;
  float pressure_init_2=0;
  float pressure_init_2_filter=0;
  int ret=0;
  float difference=0;
  for(int i=0; i<10;i++)
  {
    do
    {
      ret = Dps3xxPressureSensor.measurePressureOnce(pressure_init_1, oversampling);

      delay(50);
    }
    while(ret!=0);
    pressure_init_1_filter=pressure_init_1_filter+pressure_init_1;
  }

  pressure_init_1_filter=pressure_init_1_filter/10;
  for(int i=0; i<10;i++)
  {
    do
    {
      ret = Dps3xxPressureSensor2.measurePressureOnce(pressure_init_2, oversampling);

      delay(50);
    }
    while(ret!=0);
    pressure_init_2_filter=pressure_init_2_filter+pressure_init_2;

  }
  pressure_init_2_filter=pressure_init_2_filter/10;
  difference=pressure_init_2_filter-pressure_init_1_filter;

  return difference;
}

float hight()
{
  float pressure1;
  float pressure2;
  float heightDifference;
  
  int16_t ret;
  const float rho = 1.225; // kg/m³ (Luftdichte bei Standardbedingungen)
  const float g = 9.81; // m/s² (Erdbeschleunigung)

  Serial.println();

 do
  {
    ret = Dps3xxPressureSensor.measurePressureOnce(pressure1, oversampling);
    Serial.print(ret);
  }
  while(ret!=0);

 do
  {
    ret = Dps3xxPressureSensor2.measurePressureOnce(pressure2, oversampling);
    Serial.print(ret);
  }
  while(ret!=0);

  heightDifference =abs((pressure1 - pressure2) / (rho * g) * 100)-abs(justus/ (rho * g) * 100); // in Zentimetern
  return heightDifference;
}
