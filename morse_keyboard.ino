/*
 * Teclado morse multifuncion para practicar
 * Pedro Weinzettel - https://lu9abm.com/
 * Link del proyecto: https://github.com/pweinzettel/morse_keyboard
 */

// Librerias a utilizar
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>
#include <Keyboard_es_ES.h>
#include <EEPROM.h>

// Incluir archivo de configuracion
#include "morse_keyboard.hpp"

// Incluir decodificador morse
#include "morse.hpp"
Morse morse;

LiquidCrystal_I2C lcd(LCD_ADDR,LCD_COL,LCD_ROW);

// Variables que deberian tomarse desde la eeprom <-> config
bool iambic = true; // Al iniciar, detectar si el manipuador conectado es iambico o straight //TODO
bool vband = false; // modo vband o modo teclado
int spk_freq = SPK_FREQ; // freq a la que suena el parlante, pasar a config EEPROM //TODO
int wpm = 20; // word per minute, pasar a config EEPROM //TODO

// Variables auxiliares
int lcdrow = 0; // fila en la que estoy posicionado en el LCD
int lcdcol = 0; // columna en la que estoy posicionado en el LCD

bool KS = false;
bool KI = false;
bool KB = false;
bool KC = false;

void DIT_PIN_CHANGE() {
  morse.dit_pressed = true; // dejo guardado que se presiono dit
}
void DAH_PIN_CHANGE() {
  morse.dah_pressed = true; // dejo guardado que se presiono dah
}

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor((LCD_COL/2)-(((welcome_1.length()*10)/2)+5)/10,0);
  lcd.print(welcome_1);
  lcd.setCursor((LCD_COL/2)-(((welcome_2.length()*10)/2)+5)/10,1);
  lcd.print(welcome_2);
  lcd.setCursor(0,0);
  delay(1000);

  Keyboard.begin(KeyboardLayout_es_ES);

  pinMode(DIT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DIT_PIN), DIT_PIN_CHANGE, FALLING);
  pinMode(DAH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DAH_PIN), DAH_PIN_CHANGE, FALLING);

  lcd.clear();

#ifdef SPK_READ
  pinMode(SPK_READ, INPUT_PULLUP);
#endif
#ifdef WPM_DEF
  pinMode(WPM_DEF, INPUT_PULLUP);
#endif

  pinMode(SPK_OUT, OUTPUT);
  pinMode(PULSE_LED, OUTPUT);
  pinMode(CHAR_LED, OUTPUT);
  pinMode(WORD_LED, OUTPUT);

#ifdef KEY_SEND
  pinMode(KEY_SEND, INPUT_PULLUP);
#endif
#ifdef KEY_INTRO
  pinMode(KEY_INTRO, INPUT_PULLUP);
#endif
#ifdef KEY_BACKSP
  pinMode(KEY_BACKSP, INPUT_PULLUP);
#endif
#ifdef KEY_CLEAR
  pinMode(KEY_CLEAR, INPUT_PULLUP);
#endif

  read_opts();
}

void read_opts() {
#ifdef SPK_READ
  spk_freq = map(analogRead(SPK_READ),0,1023,31,1200);
#endif

  wpm = EEPROM.read(EEPROM_WPM);
  morse.setWPM(wpm);
  print_wpm();
}

void print_wpm() {
  lcd.setCursor(0,3);
  lcd.print(wpm);
  lcd.print("WPM ");
}

void clear_all() {
    lcd.clear();
    morse.clear();
    lcdrow = 0;
    lcdcol = 0;
    print_wpm();
}

void decode_char(){
  morse.decode();
  if (morse.decode_error) {
    //return;
  }
  for (int i = 0; i < sizeof(morse.decoded) / sizeof(morse.decoded[0]); i++) {
    if (morse.decoded[i] != 0x00) lcd_print(morse.decoded[i]);
  }
}

void lcd_print(char prnt) {
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
  lcd.print(prnt);
  if (!vband) Keyboard.print(prnt);
}

void print_space() {
  lcd_print(' ');
}

void key_press() {
  // apagar led de control de letra / palabra
  digitalWrite(CHAR_LED,LOW);
  digitalWrite(WORD_LED,LOW);
}

void dit(){
  key_press(); // todo lo que necesito hacer cada vez que se inicia un key press

  tone(SPK_OUT, spk_freq); // iniciar tono
  digitalWrite(PULSE_LED,HIGH); // iniciar pulso
  if (vband) Keyboard.press(KEY_LEFT_CTRL); // simular por teclaro la presion de ctrl izquierdo
  delay(morse.dit_len); // esperar el tiempo del punto
  if (vband) Keyboard.releaseAll(); // soltar todas las teclas
  noTone(SPK_OUT); // frenat tono
  digitalWrite(PULSE_LED,LOW); // frenar pulso

  delay(morse.intra_char); // separacion intra char

  morse.push(DIT);
}

void dah(){
  key_press(); // todo lo que necesito hacer cada vez que se inicia un key press

  tone(SPK_OUT, spk_freq); // iniciar tono
  digitalWrite(PULSE_LED,HIGH); // iniciar pulso
  if (vband) Keyboard.press(KEY_RIGHT_CTRL); // simular por teclaro la presion de ctrl derecho
  delay(morse.dah_len); // esperar el tiempo de raya
  if (vband) Keyboard.releaseAll(); // soltar todas las teclas
  noTone(SPK_OUT); // frenar tono
  digitalWrite(PULSE_LED,LOW); // frenar pulso

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
  EEPROM.write(EEPROM_WPM, wpm);
}

void check_keys() {
#ifdef KEY_SEND
  if (digitalRead(KEY_SEND) == LOW) {
    if (KS) return;
    key_send();
    KS = true;
  } else { KS = false; }
#endif
#ifdef KEY_INTRO
  if (digitalRead(KEY_INTRO) == LOW) {
    if (KI) return;
    key_intro();
    KI = true;
  } else { KI = false; }
#endif
#ifdef KEY_BACKSP
  if (digitalRead(KEY_BACKSP) == LOW) {
    if (KB) {
      set_wpm();
      return;
    }
    key_backsp();
    KB = true;
  } else { KB = false; }
#endif
#ifdef KEY_CLEAR
  if (digitalRead(KEY_CLEAR) == LOW) {
    if (KC) return;
    key_clear();
    KC = true;
  } else { KC = false; }
#endif
}

void loop() {
  //read_opts();
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
    if (!morse.wready && !morse.decode_error) print_space();
    morse.wready = true;
  }
  delay(1);
}
