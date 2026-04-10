#include <Dps3xx.h>

// Dps3xx Object
Dps3xx Dps3xxPressureSensor = Dps3xx();
//Dps3xx Dps3xxPressureSensor2 = Dps3xx();
float difference_dps_sensor = 0;
bool game_begin = false;
uint8_t oversampling = 7;
const float threshold = 30.0; // Threshold in centimeters
const int moving_average_size = 5;
float pressure_readings[moving_average_size] = {0};
int pressure_index = 0;
float initial_pressure = 0;

float calc_difference();
float hight();
float measure_initial_pressure();
float calculate_height();
void update_initial_pressure(float new_pressure);
float calculate_moving_average(const float readings[], int size);

void init_dps_sensor()
{
  int ret=0;
  float pressure=0.0;
  do
  {
  Dps3xxPressureSensor.begin(Wire1);
  ret = Dps3xxPressureSensor.measurePressureOnce(pressure, oversampling);
  delay(50);
  }while(ret!=0);
//  Dps3xxPressureSensor2.begin(Wire1);
//  delay(1000);
  Serial.println("DPS Sensor Init complete!");
  initial_pressure = measure_initial_pressure();
}

bool hammer_lift()
{
  bool logic = false;
  float hoehe = calculate_height();
  if (game_begin == false)
  {
    if (hoehe >= 30)
    {
      Serial.print("Hammer oben");
      Serial.println(hoehe);
      game_begin = true;
      logic = true;
    }
    else
    {
      Serial.println(hoehe);
    }
  }
  else
  {
    game_begin = false;
  }
  return logic;
}

float measure_initial_pressure()
{
  float pressure = 0;
  int ret = 0;

  for (int i = 0; i < moving_average_size; ++i)
  {
    do
    {
      ret = Dps3xxPressureSensor.measurePressureOnce(pressure, oversampling);
      Serial.println(ret);
      delay(500);
    } while (ret != 0);

    update_initial_pressure(pressure);
  }

  return initial_pressure;
}

void update_initial_pressure(float new_pressure)
{
  pressure_readings[pressure_index] = new_pressure;
  pressure_index = (pressure_index + 1) % moving_average_size;
  initial_pressure = calculate_moving_average(pressure_readings, moving_average_size);
}

float calculate_moving_average(const float readings[], int size)
{
  float sum = 0;
  for (int i = 0; i < size; ++i)
  {
    sum += readings[i];
  }
  return sum / size;
}

float calculate_height()
{
  float current_pressure;
  int ret;
  const float rho = 1.225; // kg/m³
  const float g = 9.81;    // m/s²

  do
  {
    
      ret = Dps3xxPressureSensor.measurePressureOnce(current_pressure, oversampling);
      Serial.println(ret);
      delay(100);
    while(ret!=0)
    {
      Dps3xxPressureSensor.begin(Wire1);
      ret = Dps3xxPressureSensor.measurePressureOnce(current_pressure, oversampling);
       Serial.println(ret);
    }
    
    if (pressure_index >= moving_average_size)
    {
      float height_difference = (initial_pressure - current_pressure) / (rho * g) * 100; // in centimeters

      if (height_difference < -50 || height_difference > 100)
      {
        Serial.println("Invalid pressure reading, re-measuring...");
        continue;
      }
    
    } 
    break;
  } while (true);

  update_initial_pressure(current_pressure);

  float height_difference = (initial_pressure - current_pressure) / (rho * g) * 100; // in centimeters
  return height_difference;
}
