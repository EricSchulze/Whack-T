#include "TLx493D_inc.hpp"

#define sensoren 4
#define werte 3

double piston_now[sensoren][werte],piston_up[sensoren][werte], sensor_values_1_time[sensoren][werte], senor_value_polar[sensoren][werte] = {0.0, 0.0, 0.0,
                                                                                                                0.0, 0.0, 0.0,
                                                                                                                0.0, 0.0, 0.0,
                                                                                                                0.0, 0.0, 0.0};
double initial_sensor_values[sensoren][werte] = {0.0, 0.0, 0.0,
                                                 0.0, 0.0, 0.0,
                                                 0.0, 0.0, 0.0,
                                                 0.0, 0.0, 0.0};

using namespace ifx::tlx493d;

TLx493D_P3I8 sensor1(SPI2);
TLx493D_P3I8 sensor2(SPI2);
TLx493D_P3I8 sensor3(SPI2);
TLx493D_P3I8 sensor4(SPI2);

const uint8_t CHIP_SELECT_PIN_1 = 88;
const uint8_t CHIP_SELECT_PIN_2 = 91;
const uint8_t CHIP_SELECT_PIN_3 = 89;
const uint8_t CHIP_SELECT_PIN_4 = 92;



double val[4][3] = {0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0};

void init_3d_sensor()
{
  SPI2.begin();
  delay(3000);
  Serial.print("Init Sensor 1 begin");
  sensor1.setSelectPin(CHIP_SELECT_PIN_1, OUTPUT, INPUT, LOW, HIGH, 0, 0,0,5);
  sensor1.begin(true, true);
  Serial.print("Init Sensor 1 finished");
  
  delay(1000);
  Serial.print("Init Sensor 2 begin");
  sensor2.setSelectPin(CHIP_SELECT_PIN_2, OUTPUT, INPUT, LOW, HIGH, 0, 0,0,5);
  sensor2.begin(true, true);
  Serial.print("Init Sensor 2 finished");

  delay(1000);
  Serial.print("Init Sensor 3 begin");
  sensor3.setSelectPin(CHIP_SELECT_PIN_3, OUTPUT, INPUT, LOW, HIGH, 0, 0,0,5);
  sensor3.begin(true, true);
  Serial.print("Init Sensor 3 finished");


  delay(1000); 
  Serial.print("Init Sensor 4 begin");
  sensor4.setSelectPin(CHIP_SELECT_PIN_4, OUTPUT, INPUT, LOW, HIGH, 0, 0,0,5);
  sensor4.begin(true, true);
  Serial.print("Init Sensor 4 finished");


  sensor1.setSensitivity(TLx493D_EXTRA_SHORT_RANGE_e);
  sensor2.setSensitivity(TLx493D_EXTRA_SHORT_RANGE_e);
  sensor3.setSensitivity(TLx493D_EXTRA_SHORT_RANGE_e);
  sensor4.setSensitivity(TLx493D_EXTRA_SHORT_RANGE_e);
  
  Serial.print("setup sensors done.\n");
}

void convertToSpherical(double x, double y, double z, double &r, double &theta, double &phi) {
  // Calculate the radius
  r = sqrt(x * x + y * y + z * z);

  // Calculate the azimuth angle (theta)
  theta = atan2(y, x) * 180.0 / PI; // Convert to degrees

  // Calculate the polar angle (phi)
  phi = acos(z / r) * 180.0 / PI; // Convert to degrees
}

void sensor_value(double (&sensor_values)[sensoren][werte])
{
  sensor1.getMagneticField(&sensor_values[0][0], &sensor_values[0][1], &sensor_values[0][2]);
  sensor2.getMagneticField(&sensor_values[1][0], &sensor_values[1][1], &sensor_values[1][2]);
  sensor3.getMagneticField(&sensor_values[2][0], &sensor_values[2][1], &sensor_values[2][2]);
  sensor4.getMagneticField(&sensor_values[3][0], &sensor_values[3][1], &sensor_values[3][2]);
}

void calculate_average(double(&sensor_average)[sensoren][werte]) {
  double sum_vals[sensoren][werte] = {0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0,
                                      0.0, 0.0, 0.0};
  for (int i = 0; i < 10; ++i) {
    sensor_value(sensor_values_1_time);
    for (int j = 0; j < sensoren; j++) {
      for (int k = 0; k < werte; k++) {
        sum_vals[j][k] += sensor_values_1_time[j][k];
      }
    }
  }
  

  for (int j = 0; j < sensoren; j++) {
    for (int k = 0; k < werte; k++) {
      sensor_average[j][k] = sum_vals[j][k] / 10.0;
    }
  }
}

double converttobetrag(double x, double y, double z) {
  return sqrt(x * x + y * y + z * z);
}
double sensor1_magnitude, sensor2_magnitude, sensor3_magnitude, sensor4_magnitude, sensor1_theta, sensor2_theta, sensor3_theta, sensor4_theta,sensor1_phi,sensor2_phi,sensor3_phi,sensor4_phi;
void print_sensor_magnitudes(double (&sensor_values)[sensoren][werte]) {
  for (int i = 0; i < sensoren; i++) {
    double diff[werte];
    for (int j = 0; j < werte; j++) {
      diff[j] = sensor_values[i][j] - initial_sensor_values[i][j]; //initial_sensor_values is needed xD
    }
    double r,theta,phi;
    convertToSpherical(diff[0], diff[1], diff[2], r, theta, phi);
    piston_up[i][0] = r;
    piston_up[i][1] = theta;
    piston_up[i][2] = phi;
  }
}

void measure_initial_values() {
  calculate_average(initial_sensor_values);
}

bool compare(double piston_stuff[sensoren][werte])
{
  int hit_counter = 0;
  bool hit_detected = false;
  for (int i = 0; i < sensoren; i++) {
    double diff[werte];
    for (int j = 0; j < werte; j++) {
      diff[j] = piston_stuff[i][j]; //- initial_sensor_values[i][j];
    }
    double r,theta,phi;
    convertToSpherical(diff[0], diff[1], diff[2], r, theta, phi);
    piston_stuff[i][0] = r;
    piston_stuff[i][1] = theta;
    piston_stuff[i][2] = phi;
  }
  for (int i = 0; i < sensoren; i++) {
    double lower_bound = piston_up[i][0] * 0.875;
    double upper_bound = piston_up[i][0] * 1.125;
;
    if (piston_stuff[i][0] > upper_bound) {

      
      hit_counter++;
    } else if (piston_stuff[i][0] < lower_bound) {

      hit_counter++;
    } else {
       
    }
      
  }
  if (hit_counter>0)
  {
    hit_detected=true;
  }
  else
  {
    hit_detected=false;
  }
 return hit_detected;

}

bool compare_difference(double piston_stuff[sensoren][werte], double threshold)
{
  int hit_counter = 0;
  bool hit_detected = false;
  //int zahlen=0;
  for (int i = 0; i < sensoren; i++) {
    double diff[werte];
    for (int j = 0; j < werte; j++) {
      diff[j] = piston_stuff[i][j] - initial_sensor_values[i][j]; //ok initial_sensor_values[i][j] is needed xD
    }
    double r, theta, phi;
    //Serial.println("vorconverttosperical");
    convertToSpherical(diff[0], diff[1], diff[2], r, theta, phi);
    //Serial.println("nachconverttosperical");
    piston_stuff[i][0] = r;
    piston_stuff[i][1] = theta;
    piston_stuff[i][2] = phi;
    //zahlen++;
  }
 // Serial.println(zahlen);
  for (int i = 0; i < sensoren; i++) {
    double r_diff = abs(piston_stuff[i][0] - piston_up[i][0]);
    //Serial.print("r_diff: ");
    //Serial.println(r_diff);
    if (r_diff > threshold) {
      hit_counter++;
    }
  }
  if (hit_counter > 0) {
    hit_detected = true;
  } else {
    hit_detected = false;
  }
  return hit_detected;
}

void start_game()
{
if(start_counting==false)
    {
 //   Serial.println("Loop_false");
    /*if (Serial.available() > 0) 
    {
      String input = Serial.readStringUntil('\n');
      int zahl = input.toInt();*/
      //(hammer_lift()||
      if(digitalRead(buttonPin))  //if hammer is lifted or button is pressed
      {
        //start countdown
        for(int i=5; i>=0; i--)
        {
           showNumber(i);
           delay(1000);
           
        }
        analogWrite(buzzer_pin, 127);
        for(int i=0; i<3; i++)
        {
          showNumber(88);
          delay(100);
         clearDisplay();
          delay(100);
        }
        analogWrite(buzzer_pin, 0);
        startMillis = millis();
        gametime=15;
        game_hit_counter=0;
        start_counting=true;

        for (int i = 0; i < sensoren; i++) {
          for (int j = 0; j < werte; j++) {
            piston_up[i][j] = 0.0;
          }
        }
      }
    //}   
}
}
