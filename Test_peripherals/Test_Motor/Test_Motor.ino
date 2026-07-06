/**
 * Test_Motor.ino
 * 
 * Testprogramm zum manuellen Ansteuern einzelner Motoren/Aktoren.
 * 
 * Bedienung über den Serial Monitor (115200 Baud):
 * 
 *   1 - 9   → Aktor 1-9 kurz ansteuern (push nach oben)
 *   k       → Kompressor EIN (dauerhaft)
 *   K       → Kompressor AUS
 *   0       → Alle Aktoren stoppen
 *   h       → Hilfemenü anzeigen
 * 
 * Pins:
 *   Aktor1  → TLE_HB8
 *   Aktor2  → TLE_HB12
 *   Aktor3  → TLE_HB6
 *   Aktor4  → TLE_HB7
 *   Aktor5  → TLE_HB3
 *   Aktor6  → TLE_HB4
 *   Aktor7  → TLE_HB11
 *   Aktor8  → TLE_HB9
 *   Aktor9  → TLE_HB10
 *   Kompressor → TLE_HB1, TLE_HB2, TLE_HB5
 */

#include <tle94112-ino.hpp>
#include <tle94112-motor-ino.hpp>
#include "SPI.h"

// --- Konfiguration ---
#define PUSH_DURATION  175   // ms wie lange der Aktor angesteuert wird
#define BAUD_RATE      115200

// --- TLE94112 Controller & Motoren ---
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

bool kompressor_running = false;

// -------------------------------------------------------
void init_hardware()
{
  controller.begin();

  aktor1.connect(aktor1.HIGHSIDE, controller.TLE_HB8);   // Aktor 1
  aktor2.connect(aktor2.HIGHSIDE, controller.TLE_HB12);  // Aktor 2
  aktor3.connect(aktor3.HIGHSIDE, controller.TLE_HB6);   // Aktor 3
  aktor4.connect(aktor4.HIGHSIDE, controller.TLE_HB7);   // Aktor 4
  aktor5.connect(aktor5.HIGHSIDE, controller.TLE_HB3);   // Aktor 5
  aktor6.connect(aktor6.HIGHSIDE, controller.TLE_HB4);   // Aktor 6
  aktor7.connect(aktor7.HIGHSIDE, controller.TLE_HB11);  // Aktor 7
  aktor8.connect(aktor8.HIGHSIDE, controller.TLE_HB9);   // Aktor 8
  aktor9.connect(aktor9.HIGHSIDE, controller.TLE_HB10);  // Aktor 9

  kompressor.connect(kompressor.HIGHSIDE, controller.TLE_HB5);
  kompressor.connect(kompressor.HIGHSIDE, controller.TLE_HB2);
  kompressor.connect(kompressor.HIGHSIDE, controller.TLE_HB1);

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

// -------------------------------------------------------
void stop_all()
{
  aktor1.stop(0);
  aktor2.stop(0);
  aktor3.stop(0);
  aktor4.stop(0);
  aktor5.stop(0);
  aktor6.stop(0);
  aktor7.stop(0);
  aktor8.stop(0);
  aktor9.stop(0);
  Serial.println(">> Alle Aktoren gestoppt.");
}

// -------------------------------------------------------
void fire_aktor(int num)
{
  Serial.print(">> Aktor ");
  Serial.print(num);
  Serial.print(" wird angesteuert (");
  Serial.print(PUSH_DURATION);
  Serial.println(" ms) ...");

  switch (num)
  {
    case 1: aktor1.start(255); delay(PUSH_DURATION); aktor1.stop(0); break;
    case 2: aktor2.start(255); delay(PUSH_DURATION); aktor2.stop(0); break;
    case 3: aktor3.start(255); delay(PUSH_DURATION); aktor3.stop(0); break;
    case 4: aktor4.start(255); delay(PUSH_DURATION); aktor4.stop(0); break;
    case 5: aktor5.start(255); delay(PUSH_DURATION); aktor5.stop(0); break;
    case 6: aktor6.start(255); delay(PUSH_DURATION); aktor6.stop(0); break;
    case 7: aktor7.start(255); delay(PUSH_DURATION); aktor7.stop(0); break;
    case 8: aktor8.start(255); delay(PUSH_DURATION); aktor8.stop(0); break;
    case 9: aktor9.start(255); delay(PUSH_DURATION); aktor9.stop(0); break;
    default:
      Serial.println("!! Ungültige Nummer.");
      return;
  }

  Serial.println(">> Fertig.");
}

// -------------------------------------------------------
void print_help()
{
  Serial.println("========================================");
  Serial.println(" MOTOR TEST - Hilfemenü");
  Serial.println("========================================");
  Serial.println("  1-9  : Aktor X kurz auslösen");
  Serial.println("  k    : Kompressor EIN");
  Serial.println("  K    : Kompressor AUS");
  Serial.println("  0    : Alle Aktoren stoppen");
  Serial.println("  h    : Dieses Menü anzeigen");
  Serial.println("========================================");
}

// -------------------------------------------------------
void setup()
{
  Serial.begin(BAUD_RATE);
  delay(2000);

  Serial.println("Initialisierung laeuft...");
  init_hardware();
  Serial.println("Initialisierung abgeschlossen!");

  // Kompressor kurz vorheizen
  Serial.println("Kompressor wird gestartet (2 Sek. Aufbauzeit)...");
  kompressor.start(255);
  kompressor_running = true;
  delay(2000);
  Serial.println("Kompressor laeuft.");

  print_help();
}

// -------------------------------------------------------
void loop()
{
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();

    // Leerzeichen / Zeilenumbrüche ignorieren
    if (cmd == '\n' || cmd == '\r' || cmd == ' ')
      return;

    if (cmd >= '1' && cmd <= '9')
    {
      int num = cmd - '0';  // char → int
      fire_aktor(num);
    }
    else if (cmd == '0')
    {
      stop_all();
    }
    else if (cmd == 'k')
    {
      Serial.println(">> Kompressor EIN");
      kompressor.start(255);
      kompressor_running = true;
    }
    else if (cmd == 'K')
    {
      Serial.println(">> Kompressor AUS");
      kompressor.stop(0);
      kompressor_running = false;
    }
    else if (cmd == 'h')
    {
      print_help();
    }
    else
    {
      Serial.print("!! Unbekannter Befehl: '");
      Serial.print(cmd);
      Serial.println("'  ->  'h' fuer Hilfe eingeben.");
    }
  }
}
