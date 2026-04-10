/** Project CPP includes. */ 
bool test=0;
bool start_counting=false;

#define sensoren 4
#define werte 3

#define gameduration 30

unsigned long startMillis;
const unsigned long countdownDuration = 15000;  // 15 Sekunden in Millisekunden
int gametime=15; //15s gametime

int randomdelay=0;
int delaycounter=0;
bool delaymarker=false;
uint8_t counter_interrupt=0;
int id_index = 0;
bool hit_marker=true;
uint8_t game_hit_counter=0;
bool hammer_oben=false;
int hit_merken=0;

#include "display.hpp"
#include "TLx493D_inc.hpp"
//#include "airpressure_sensor.hpp"
#include "3d_sensor.hpp"
#include "push.hpp"

//#include "game_matirx.hpp"


int merken=0;


void setup() {
  Serial.begin(115200);
  delay(6000);
  init_display();
  //init_dps_sensor();
  init_3d_sensor();
  delay(1000);
  measure_initial_values();
  init_mhb();

  kompressor.start(255);
  delay(2000); //for airpressure buildup :)

  Serial.print("init done");

  Serial.println("vor handler");

  Serial.println("nach handler");
  Serial.print("Init done!");

  test=1;
  delay(500);
}




void loop()
  {


  gametime=getRemainingSeconds();

    if(start_counting==true&&gametime>=0)
    {
      if(gametime>=0)
      { 
        showNumber(gametime);
      }
        if(gametime==0)
        {
          hit_merken=game_hit_counter;
          game_hit_counter=0;
          start_counting=false;
          gametime=15;
        }
        calculate_average(piston_now);
        if(compare_difference(piston_now, 0.1)==true)  //change this threashold of needed
        {
          hit_marker=true;
          game_hit_counter++;
        }

        if(hit_marker==true)
        {
          push(random(1,10));
          hit_marker=false;
        }
      
      }
      else
      {
        showNumber(hit_merken);
        start_game();
        merken=0;
        
      }
      return;
  }




int getRemainingSeconds() {
  long currentMillis = millis();  // Aktuelle Zeit in Millisekunden
 long elapsedMillis = currentMillis - startMillis;  // Berechne die vergangene Zeit

  if (elapsedMillis >= countdownDuration) {
    return 0;  // Countdown beendet
  } else {
    return (countdownDuration - elapsedMillis) / 1000;  // Verbleibende Sekunden
  }
}



