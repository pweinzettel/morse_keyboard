/*
 * Teclado morse multifuncion para practicar
 * Pedro Weinzettel - https://lu9abm.com/
 * Link del proyecto: https://github.com/pweinzettel/morse_keyboard
 */

//#define SERIAL_DEBUG

// Librerias a utilizar
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>
#include <EEPROM.h>

// Incluir archivo de configuracion
#include "morse_keyboard.hpp"

// Incluir decodificador morse
#include "morse.hpp"
Morse morse;

LiquidCrystal_I2C lcd(LCD_ADDR,LCD_COL,LCD_ROW);

// Variables que deberian tomarse desde la eeprom <-> config
int spk_freq = SPK_FREQ; // freq a la que suena el parlante, pasar a config EEPROM //TODO
int wpm = 20; // word per minute, pasar a config EEPROM //TODO
byte key_mode = IAMBIC; // default
int old_wpm = -1;

// Variables auxiliares
int lcdrow = 0; // fila en la que estoy posicionado en el LCD
int lcdcol = 0; // columna en la que estoy posicionado en el LCD
byte decoded[2];

bool KS = false;
bool KI = false;
bool KB = false;
bool KC = false;

void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#endif //SERIAL_DEBUG
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, enie);

  Keyboard.begin(KeyboardLayout_custom);

  lcd_print_center(welcome_1,0);
  lcd_print_center(welcome_2,3);
  delay(1000);
  lcd.clear();
  lcd_print_center("Morse mode",0);

  pinMode(DIT_PIN, INPUT_PULLUP);
  pinMode(DAH_PIN, INPUT_PULLUP);

  if (!digitalRead(DAH_PIN)) key_mode = STRAIGHT;
  switch (key_mode) {
    case IAMBIC:
      attachInterrupt(digitalPinToInterrupt(DIT_PIN), DIT_PIN_CHANGE, FALLING);
      attachInterrupt(digitalPinToInterrupt(DAH_PIN), DAH_PIN_CHANGE, FALLING);
      lcd_print_center("Iambic",3);
    break;
    case STRAIGHT:
      pinMode(DIT_PIN, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(DIT_PIN), STRAIGHT_PIN_CHANGE, CHANGE);
      lcd_print_center("Straight",3);
    break;
  }
  delay(1000);
  lcd.setCursor(0,0);
  lcd.clear();

  pinMode(SPK_OUT, OUTPUT);
  pinMode(PULSE_LED, OUTPUT);
  pinMode(CHAR_LED, OUTPUT);
  pinMode(WORD_LED, OUTPUT);

  pinMode(KEY_SEND, INPUT_PULLUP);
  pinMode(KEY_INTRO, INPUT_PULLUP);
  pinMode(KEY_BACKSP, INPUT_PULLUP);
  pinMode(KEY_CLEAR, INPUT_PULLUP);

  if (key_mode == IAMBIC) read_opts();
  morse.setKeyMode(key_mode);
}
void DIT_PIN_CHANGE() {
  morse.dit_pressed = true; // dejo guardado que se presiono dit
}
void DAH_PIN_CHANGE() {
  morse.dah_pressed = true; // dejo guardado que se presiono dah
}
void STRAIGHT_PIN_CHANGE() {
  if (!digitalRead(DIT_PIN)) {
    key_press();
    morse.straight_start();
  } else {
    morse.straight_stop();
    key_release();
  }
}
void lcd_print_center(const String &msg, int row) {
  lcd.setCursor((LCD_COL/2)-(((msg.length()*10)/2)+5)/10,row);
  lcd.print(msg);
}
void read_opts() {
  wpm = EEPROM.read(0);
  morse.setWPM(wpm);
  print_wpm();
}
void print_wpm() {
  if (old_wpm != morse.wpm) {
    lcd.setCursor(0,3);
    lcd.print("        ");
    lcd.setCursor(0,3);
           lcd.print(morse.wpm);
    lcd.print("WPM ");
    old_wpm = morse.wpm;
  }
}
void clear_all() {
    lcd.clear();
    morse.clear();
    lcdrow = 0;
    lcdcol = 0;
    print_wpm();
}
void decode_char(){
  morse.decode(decoded);
  for (int i = 0; i < sizeof(decoded) / sizeof(decoded[0]); i++) {
    if (decoded[i] != 0x00) lcd_print(decoded[i]);
  }
}
void lcd_print(short prnt) {
  int lcd_prnt;
  int kbr_prnt;
  switch (prnt) {
    case 145:
      lcd_prnt = 1; // Ñ
      kbr_prnt = 127; // Ñ
    break;

    default:
      lcd_prnt = prnt;
      kbr_prnt = prnt;
    break;
  }

  if (lcdcol >= LCD_COL) {
    lcdcol = 0;
    lcdrow++;
    if (lcdrow >= LCD_ROW-1) {
      lcdrow = 0;
    }
    lcd.setCursor(lcdcol,lcdrow);
    lcd.print("                    ");
  }
  if (lcdcol == 0 && prnt == ' ') {
  } else {
    lcd.setCursor(lcdcol,lcdrow);
    lcdcol++;
  }
  //lcd.print(int(prnt),DEC);
  lcd.write(lcd_prnt);
  Keyboard.write(kbr_prnt);
}
void key_press() {
  // apagar led de control de letra / palabra
  digitalWrite(CHAR_LED,LOW);
  digitalWrite(WORD_LED,LOW);
  tone(SPK_OUT, spk_freq); // iniciar tono
  digitalWrite(PULSE_LED,HIGH); // iniciar pulso
}
void key_release() {
  noTone(SPK_OUT); // frenat tono
  digitalWrite(PULSE_LED,LOW); // frenar pulso
}
void dit(){
  key_press(); // todo lo que necesito hacer cada vez que se inicia un key press
  delay(morse.dit_len); // esperar el tiempo del punto
  key_release();
  delay(morse.intra_char); // separacion intra char
  morse.push(DIT);
}
void dah(){
  key_press(); // todo lo que necesito hacer cada vez que se inicia un key press
  delay(morse.dah_len); // esperar el tiempo de raya
  key_release();
  delay(morse.intra_char); // separacion intra char
  morse.push(DAH);
}
void key_send() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}
void key_intro() {
  Keyboard.write(KEY_RETURN);
  clear_all();
}
void key_backsp() {
  Keyboard.write(KEY_BACKSPACE);
}
void key_clear() {
  clear_all();
  Keyboard.write(KEY_HOME);
  Keyboard.write(KEY_HOME);
  delay(20);
  Keyboard.press(KEY_LEFT_SHIFT);
  delay(20);
  Keyboard.write(KEY_END);
  delay(20);
  Keyboard.write(KEY_DELETE);
  delay(20);
  Keyboard.releaseAll();
}
void set_wpm() {
  while (digitalRead(KEY_BACKSP) == LOW) {
    if (digitalRead(KEY_INTRO) == LOW) {
      if (KI) return;
      wpm = wpm-5;
      if (wpm <= 9) wpm=10;
      morse.setWPM(wpm);
      print_wpm();
      KI = true;
    } else { KI = false; }
    if (digitalRead(KEY_SEND) == LOW) {
      if (KS) return;
      wpm = wpm+5;
      if (wpm >= 100) wpm=100;
      morse.setWPM(wpm);
      print_wpm();
      KS = true;
    } else { KS = false; }
    delay(10);
  }
  EEPROM.write(0, wpm);
}
void check_keys() {
  if (digitalRead(KEY_CLEAR) == LOW) {
    if (KC) return;
    key_clear();
    KC = true;
  } else { KC = false; }

  if (digitalRead(KEY_BACKSP) == LOW) {
    if (KB) {
      set_wpm();
      return;
    }
    key_backsp();
    KB = true;
  } else { KB = false; }

  if (digitalRead(KEY_INTRO) == LOW) {
    if (KI) return;
    key_intro();
    KI = true;
  } else { KI = false; }

  if (digitalRead(KEY_SEND) == LOW) {
    if (KS) return;
    key_send();
    KS = true;
  } else { KS = false; }
}
void loop() {
  switch (morse.key_mode) {
    case IAMBIC:
      while(1) {
        if (digitalRead(DIT_PIN) == LOW || morse.dit_pressed == true) dit(); // si presiono un dit o lo presione previamente (guardado por interrupt), ir a funcion de dit o dah respectivamente
        if (digitalRead(DAH_PIN) == LOW || morse.dah_pressed == true) dah();

        check_keys();

        if (millis()-morse.last_key > morse.inter_char) {
          digitalWrite(CHAR_LED,HIGH);
          if (!morse.cready) decode_char();
          morse.cready = true;
        }
        if (millis()-morse.last_key > morse.wrd_sep) {
          digitalWrite(WORD_LED,HIGH);
          if (!morse.wready && !morse.decode_error) lcd_print(' ');
          morse.wready = true;
        }
        delay(1);
      }
    break;
    case STRAIGHT:
      while (1) {
        morse.decode(decoded);
        print_wpm();
        if (millis()-morse.last_key > morse.inter_char && !morse.straight_pressed) {
          digitalWrite(CHAR_LED,HIGH);
          if (!morse.cready) decode_char();
          morse.cready = true;
        }
        if (millis()-morse.last_key > morse.wrd_sep && !morse.straight_pressed) {
          digitalWrite(WORD_LED,HIGH);
          if (!morse.wready && !morse.decode_error) lcd_print(' ');
          morse.wready = true;
        }
        delay(1);
      }
    break;
  }
}
