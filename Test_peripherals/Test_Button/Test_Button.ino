/**
 * Test_Button.ino
 * 
 * Testprogramm fuer den Button.
 * Pin: 77
 * 
 * Ausgabe im Serial Monitor (115200 Baud):
 *   - "Button gedrueckt!"  wenn der Button betaetigt wird
 *   - "Button losgelassen" wenn er wieder losgelassen wird
 */

#define BUTTON_PIN 77

bool lastState = false;

void setup()
{
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  Serial.println("Button Test gestartet. Druecke den Button!");
}

void loop()
{
  bool currentState = digitalRead(BUTTON_PIN);

  if (currentState == true && lastState == false)
  {
    Serial.println("Button gedrueckt!");
  }
  else if (currentState == false && lastState == true)
  {
    Serial.println("Button losgelassen.");
  }

  lastState = currentState;
  delay(20);  // Entprellung
}
