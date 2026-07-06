/**
 * Test_All.ino
 *
 * Kombiniertes Testprogramm fuer:
 *   [d] Display  - Countdown & Segmenttest
 *   [b] Button   - Tastendruck per Serial ausgeben
 *   [m] Motor    - Einzelne Aktoren ansteuern
 *   [x] Alles ausschalten (Idle)
 *
 * Bedienung im Serial Monitor (115200 Baud, "Kein Zeilenende" oder "NL"):
 *   d      → Wechsel zu Display-Test
 *   b      → Wechsel zu Button-Test
 *   m      → Wechsel zu Motor-Test
 *   x      → Alles ausschalten
 *   h      → Hilfemenue anzeigen
 *
 *   Im Motor-Modus zusaetzlich:
 *   1-9    → Aktor 1-9 kurz ausloesen
 *   k      → Kompressor EIN
 *   K      → Kompressor AUS
 *   0      → Alle Aktoren stoppen
 */

#include "display.hpp"
#include <tle94112-ino.hpp>
#include <tle94112-motor-ino.hpp>
#include "SPI.h"

// --- Pins ---
#define BUTTON_PIN   77
#define BUZZER_PIN   76
#define PUSH_DURATION 175

// --- Modi ---
enum Mode { IDLE, DISPLAY_TEST, BUTTON_TEST, MOTOR_TEST };
Mode currentMode = IDLE;

// --- TLE94112 ---
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

// --- Button ---
bool lastButtonState = false;

// --- Display ---
int countdownValue = 15;
unsigned long lastDisplayUpdate = 0;

// -------------------------------------------------------
void init_motors()
{
  controller.begin();

  aktor1.connect(aktor1.HIGHSIDE, controller.TLE_HB8);
  aktor2.connect(aktor2.HIGHSIDE, controller.TLE_HB12);
  aktor3.connect(aktor3.HIGHSIDE, controller.TLE_HB6);
  aktor4.connect(aktor4.HIGHSIDE, controller.TLE_HB7);
  aktor5.connect(aktor5.HIGHSIDE, controller.TLE_HB3);
  aktor6.connect(aktor6.HIGHSIDE, controller.TLE_HB4);
  aktor7.connect(aktor7.HIGHSIDE, controller.TLE_HB11);
  aktor8.connect(aktor8.HIGHSIDE, controller.TLE_HB9);
  aktor9.connect(aktor9.HIGHSIDE, controller.TLE_HB10);
  kompressor.connect(kompressor.HIGHSIDE, controller.TLE_HB5);
  kompressor.connect(kompressor.HIGHSIDE, controller.TLE_HB2);
  kompressor.connect(kompressor.HIGHSIDE, controller.TLE_HB1);

  aktor1.begin(); aktor2.begin(); aktor3.begin();
  aktor4.begin(); aktor5.begin(); aktor6.begin();
  aktor7.begin(); aktor8.begin(); aktor9.begin();
  kompressor.begin();
}

void stop_all_motors()
{
  aktor1.stop(0); aktor2.stop(0); aktor3.stop(0);
  aktor4.stop(0); aktor5.stop(0); aktor6.stop(0);
  aktor7.stop(0); aktor8.stop(0); aktor9.stop(0);
  Serial.println(">> Alle Aktoren gestoppt.");
}

void fire_aktor(int num)
{
  Serial.print(">> Aktor "); Serial.print(num);
  Serial.print(" ("); Serial.print(PUSH_DURATION); Serial.println(" ms)");
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
    default: Serial.println("!! Ungueltige Nummer."); return;
  }
  Serial.println(">> Fertig.");
}

// -------------------------------------------------------
void print_help()
{
  Serial.println("========================================");
  Serial.println(" TEST-ALL - Hilfemenue");
  Serial.println("========================================");
  Serial.println("  d      : Display-Test");
  Serial.println("  b      : Button-Test");
  Serial.println("  m      : Motor-Test");
  Serial.println("  x      : Alles ausschalten (Idle)");
  Serial.println("----------------------------------------");
  Serial.println("  [Motor-Modus]");
  Serial.println("  1-9    : Aktor ausloesen");
  Serial.println("  k / K  : Kompressor EIN / AUS");
  Serial.println("  0      : Alle Aktoren stoppen");
  Serial.println("  h      : Dieses Menue");
  Serial.println("========================================");
}

void set_mode(Mode m)
{
  // Beim Moduswechsel aufraumen
  stop_all_motors();

  // Kompressor stoppen wenn wir den Motor-Modus verlassen
  if (currentMode == MOTOR_TEST && m != MOTOR_TEST)
  {
    Serial.println(">> Kompressor AUS");
    kompressor.stop(0);
    kompressor_running = false;
  }

  clearDisplay();
  currentMode = m;
  countdownValue = 15;
  lastDisplayUpdate = millis();

  switch (m)
  {
    case IDLE:
      Serial.println(">> Modus: IDLE - Alles aus. (h fuer Hilfe)");
      break;
    case DISPLAY_TEST:
      Serial.println(">> Modus: DISPLAY-TEST");
      break;
    case BUTTON_TEST:
      Serial.println(">> Modus: BUTTON-TEST  (Button druecken!)");
      break;
    case MOTOR_TEST:
      Serial.println(">> Modus: MOTOR-TEST   (1-9 / k / K / 0)");
      Serial.println(">> Kompressor startet... (2 Sek. Aufbauzeit)");
      kompressor.start(255);
      kompressor_running = true;
      delay(2000);
      Serial.println(">> Kompressor laeuft. Bereit!");
      break;
  }
}

// -------------------------------------------------------
void handle_serial()
{
  if (!Serial.available()) return;

  char cmd = Serial.read();
  if (cmd == '\n' || cmd == '\r' || cmd == ' ') return;

  // Moduswechsel immer moeglich
  if      (cmd == 'd') { set_mode(DISPLAY_TEST); return; }
  else if (cmd == 'b') { set_mode(BUTTON_TEST);  return; }
  else if (cmd == 'm') { set_mode(MOTOR_TEST);   return; }
  else if (cmd == 'x') { set_mode(IDLE);         return; }
  else if (cmd == 'h') { print_help();            return; }

  // Motor-Befehle nur im Motor-Modus
  if (currentMode == MOTOR_TEST)
  {
    if (cmd >= '1' && cmd <= '9') { fire_aktor(cmd - '0'); }
    else if (cmd == '0')          { stop_all_motors(); }
    else if (cmd == 'k')          { Serial.println(">> Kompressor EIN"); kompressor.start(255); kompressor_running = true; }
    else if (cmd == 'K')          { Serial.println(">> Kompressor AUS"); kompressor.stop(0);    kompressor_running = false; }
    else { Serial.print("!! Unbekannt: '"); Serial.print(cmd); Serial.println("'  ->  h fuer Hilfe"); }
  }
}

// -------------------------------------------------------
void run_display_test()
{
  // Countdown nicht-blockierend mit millis()
  if (millis() - lastDisplayUpdate >= 1000)
  {
    showNumber(countdownValue);
    Serial.print("Display -> "); Serial.println(countdownValue);
    countdownValue--;
    if (countdownValue < 0) countdownValue = 15;
    lastDisplayUpdate = millis();
  }
}

void run_button_test()
{
  bool state = digitalRead(BUTTON_PIN);
  if (state == true  && lastButtonState == false) Serial.println(">> Button gedrueckt!");
  if (state == false && lastButtonState == true)  Serial.println(">> Button losgelassen.");
  lastButtonState = state;
}

// -------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  delay(2000);

  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  pinMode(BUZZER_PIN, OUTPUT);

  init_display();
  init_motors();

  Serial.println("Initialisierung abgeschlossen!");
  print_help();
  set_mode(IDLE);
}

void loop()
{
  handle_serial();

  switch (currentMode)
  {
    case IDLE:         /* nichts tun */ break;
    case DISPLAY_TEST: run_display_test(); break;
    case BUTTON_TEST:  run_button_test();  delay(20); break;
    case MOTOR_TEST:   /* alles per Serial */ break;
  }
}
