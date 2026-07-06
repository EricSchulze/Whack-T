/**
 * Test_Display.ino
 * 
 * Testprogramm fuer das 2-stellige 7-Segment-Display.
 * 
 * Pins:
 *   Data  → Pin 51
 *   Clock → Pin 52
 *   Latch → Pin 50
 * 
 * Ablauf:
 *   1. Alle Segmente kurz aufblinken (Display-Test)
 *   2. Countdown von 15 bis 0
 *   3. Display blinkt 3x
 *   4. Von vorne
 */

#include "display.hpp"

void setup()
{
  Serial.begin(115200);
  delay(5000);
  init_display();
  Serial.println("Display Test gestartet.");
}

void loop()
{
  // --- Schritt 1: Display-Test (alle Segmente an) ---
  Serial.println("Display-Test: Alle Segmente an (88)");
  for (int i = 0; i < 3; i++)
  {
    showNumber(88);
    delay(400);
    clearDisplay();
    delay(200);
  }
  delay(500);

  // --- Schritt 2: Countdown 15 -> 0 ---
  Serial.println("Countdown laeuft...");
  for (int i = 15; i >= 0; i--)
  {
    showNumber(i);
    Serial.print("  -> ");
    Serial.println(i);
    delay(1000);
  }

  // --- Schritt 3: Blinken am Ende ---
  Serial.println("Countdown fertig! Blinken...");
  for (int i = 0; i < 5; i++)
  {
    showNumber(0);
    delay(200);
    clearDisplay();
    delay(200);
  }

  delay(1000);
}
