//Display

#define segmentData 51
#define segmentClock 52
#define segmentLatch 50

#define dpa  1<<0
#define dpb  1<<6
#define dpc  1<<5
#define dpd  1<<4
#define dpe  1<<3
#define dpf  1<<1
#define dpg  1<<2
#define dpdp 1<<7

void init_display()
{
  Serial.println("Init Display begin");

  pinMode(segmentClock, OUTPUT);
  pinMode(segmentData, OUTPUT);
  pinMode(segmentLatch, OUTPUT);

  digitalWrite(segmentClock, LOW);
  digitalWrite(segmentData, LOW);
  digitalWrite(segmentLatch, LOW);

  Serial.println("Init Display finished");
}

void postNumber(byte number, boolean decimal)
{

  byte segments;

  switch (number)
  {
    case 1: segments = dpb | dpc; break;
    case 2: segments = dpa | dpb | dpd | dpe | dpg; break;
    case 3: segments = dpa | dpb | dpc | dpd | dpg; break;
    case 4: segments = dpf | dpg | dpb | dpc; break;
    case 5: segments = dpa | dpf | dpg | dpc | dpd; break;
    case 6: segments = dpa | dpf | dpg | dpe | dpc | dpd; break;
    case 7: segments = dpa | dpb | dpc; break;
    case 8: segments = dpa | dpb | dpc | dpd | dpe | dpf | dpg; break;
    case 9: segments = dpa | dpb | dpc | dpd | dpf | dpg; break;
    case 0: segments = dpa | dpb | dpc | dpd | dpe | dpf; break;
    case ' ': segments = 0; break;
    case 'c': segments = dpg | dpe | dpd; break;
    case '-': segments = dpg; break;
  }

  if (decimal) segments |= dpdp;

  //Clock these bits out to the drivers
  for (byte x = 0 ; x < 8 ; x++)
  {
    digitalWrite(segmentClock, LOW);
    digitalWrite(segmentData, segments & 1 << (7 - x));
    digitalWrite(segmentClock, HIGH); //Data transfers to the register on the rising edge of SRCK
  }
}



void showNumber(float value) //vielleicht kann dies auch ein INT sein. Ich muss dies mal testen
{
  int number = abs(value); //Remove negative signs and any decimals

  for (byte x = 0 ; x < 2 ; x++)
  {
    int remainder = number % 10;

    postNumber(remainder, false);

    number /= 10;
  }

  //Latch the current segment data
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH); //Register moves storage register on the rising edge of RCK
}


void clearDisplay() {
  postNumber(' ', false); // Alle Segmente ausschalten
  postNumber(' ', false); 
  digitalWrite(segmentLatch, LOW);
  digitalWrite(segmentLatch, HIGH); // Register moves storage register on the rising edge of RCK
}
//Given a number, or '-', shifts it out to the display
