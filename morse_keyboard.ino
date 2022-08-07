/*
 * Teclado morse multifuncion - Pedro Weinzettel - https://lu9abm.com
 * 
 * IMPORTANTE!
 * -- Todas las configuraciones se realizan desde el archivo morse_keyboard.h
 * 
 * Para mas informacion: https:
 * 
 */

// Incluir archivo de configuracion
#include "morse_keyboard.h"
// Librerias...
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keyboard.h>

/*
 * Inicio del proyecto:
 * El soft deberia poder
 * 1. Permitir conectar manipulador imabico o straight
 * 2. Que la salida sea por pulso (simulando tecla) o como teclado, decodificando los pulsos
 * 3. Tener la opcion de conectar parlante o conectar uno buzzer interno
 * 
 * Otros:
 * Salida transistorada para poder conectar a equipo?
 * 
 * Opciones:
 * 1. Seleccionar keyer: Iambic - Straight
 * 2. Seleccionar PPM para iambic
 * 3. Seleccionar modo: Keyboard - Pulse
 * 4. Seleccionar freq de buzzer?
 * 5. Guardar config en eeprom
 */

LiquidCrystal_I2C lcd(LCD_ADDR,LCD_COL,LCD_ROW);


bool iambic = true; // es iambic?
int spk_freq = SPK_FREQ;
int wpm = WPM_DEF; // word per minute
int unit = 0; // unidad de tiempo
int dit_len = 0; // dit en ms
int dah_len = 0; // dah en ms
int intra_char = 0; // char sep en ms
int inter_char = 0; // char sep en ms
int wrd_sep = 0; // word sep en ms
int ccnt = 0; // cuantos caracteres tengo en morse?
char morse[7];
int lcdrow = 0;
int lcdcol = 0;
bool vband = false;

// para los interrupt
volatile bool dit_pressed = false;
volatile bool dah_pressed = false;

bool cready = true; // char ready?
bool wready = true; // word ready?
bool decode_error = true;

unsigned long last_key = 0; // time control para leds

int treeptr = 0;
const char morse_tree[] = {
  CER, 'E', 'T', 'I', 'A', 'N', 'M', 'S', // 0 - 7
  'U', 'R', 'W', 'D', 'K', 'G', 'O', 'H', // 8 - 15
  'V', 'F', 'U', 'L', 0xFB,'P', 'J', 'B', // 16 - 23
  'X', 'C', 'Y', 'Z', 'Q', CER, 0xFA,'5', // 24 - 31
  '4', CER, '3', CER, CER, CER, '2', CER, // 32 - 39
  CER, 0xFC,CER, CER, CER, CER, '1', '6', // 40 - 47
  '=', '/', CER, CER, CER, '(', CER, '7', // 48 - 55
  CER, CER, 0xEE,'8', CER, '9', '0', CER, // 56 - 63
  CER, CER, CER, CER, CER, CER, CER, CER, // 64 - 71
  CER, CER, CER, '?', '_', CER, CER, CER, // 72 - 79
  CER, '"', CER, CER, '.', CER, CER, CER, // 80 - 87
  CER, '@', CER, CER, CER, CER, CER, CER, // 88 - 95
  '-', CER, CER, CER, CER, CER, CER, CER, // 96 - 103
  CER, ';', '!', CER, ')', CER, CER, CER, // 104 - 111
  CER, CER, ',', CER, CER, CER, CER, ':', // 112 - 119
  CER, CER, CER, CER, CER, CER, CER, CER  // 120 - 128
};

// dit = (2*treeptr)+1
// dah = (2*treeptr)+2

void setup() {
  lcd.init();

  lcd.backlight();
  lcd.setCursor((LCD_COL/2)-3,0);
  lcd.print("MORSE");
  lcd.setCursor((LCD_COL/2)-3,1);
  lcd.print("LU9ABM");
  lcd.setCursor(0,0);

  //Serial.begin(9600);
  delay(1000);
  Keyboard.begin();

  lcd.clear();
  //pinMode(IS_IAMBIC, INPUT_PULLUP);

  pinMode(DIT_PIN, INPUT_PULLUP); // usar los pin como entrada con pullup
  attachInterrupt(digitalPinToInterrupt(DIT_PIN), DIT_PIN_CHANGE, FALLING); // cuando detecte caida, ejecutar funcion
  pinMode(DAH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DAH_PIN), DAH_PIN_CHANGE, FALLING);

#ifdef SPK_READ
  pinMode(SPK_READ, INPUT_PULLUP);
#endif

#ifdef WPM_DEF
  pinMode(WPM_DEF, INPUT_PULLUP);
#endif
  pinMode(SPK_OUT, OUTPUT);
  pinMode(PULSE_OUT, OUTPUT);
  pinMode(CHAR_LED, OUTPUT);
  pinMode(WORD_LED, OUTPUT);

  read_opts();
}

void DIT_PIN_CHANGE() {
  dit_pressed = true; // dejo guardado que se presiono dit
}
void DAH_PIN_CHANGE() {
  dah_pressed = true; // dejo guardado que se presiono dah
}

void read_opts() {
#ifdef SPK_READ
  spk_freq = map(analogRead(SPK_READ),0,1023,31,1200);
#endif

#ifdef WPM_READ
  wpm = map(analogRead(WPM_READ),1,1030,5,50);
#endif

  lcd.setCursor(0,3);
  lcd.print(wpm);
  lcd.print("WPM ");

  unit = 60000/(wpm * 50); // una palabra tiene 50 unidades (PARIS = .--. .- .-. .. ...) dit = 1u, dah = 3u, intra char = 1u, inter chat =3u, word sep = 7
  dit_len = unit;
  dah_len = (unit*3);
  intra_char = (unit);
  inter_char = (unit*3);
  wrd_sep = (unit*7);
}

void decode_char(){
  if (decode_error) {
    decode_error = false;
    lcd.clear();
    ccnt = 0; // reseteo la cuenta de chars en morse...
    treeptr = 0;
    cready = true;
    wready = true;
    last_key = 0;
    lcdrow = 0;
    lcdcol = 0;
    return;
  }

  if (treeptr > sizeof morse_tree) treeptr = 0;
  switch (morse_tree[treeptr]) {
    case char(0xFA):
      lcd_print('C');
      lcd_print('H');
    break;
    case char(0xFB):
      lcd_print('A');
      lcd_print('A');
    break;
    case char(0xFC):
      lcd_print('A');
      lcd_print('R');
    break;
    default:
      lcd_print(morse_tree[treeptr]); // vamos imprimiendo de a uno...
    break;
  }
  ccnt = 0; // reseteo la cuenta de chars en morse...
  treeptr = 0;
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
  //lcd_print(char(126));
}

void key_press() {
  // apagar led de control de letra / palabra
  digitalWrite(CHAR_LED,LOW);
  digitalWrite(WORD_LED,LOW);
}

void dit(){
  key_press(); // todo lo que necesito hacer cada vez que se inicia un key press

  tone(SPK_OUT, spk_freq); // iniciar tono
  digitalWrite(PULSE_OUT,HIGH); // iniciar pulso
  if (vband) Keyboard.press(KEY_LEFT_CTRL); // simular por teclaro la presion de ctrl izquierdo
  delay(dit_len); // esperar el tiempo del punto
  if (vband) Keyboard.releaseAll(); // soltar todas las teclas
  noTone(SPK_OUT); // frenat tono
  digitalWrite(PULSE_OUT,LOW); // frenar pulso

  last_key = millis(); // guardar timestamp desde ultimo pulso
  delay(intra_char); // separacion intra char

  cready = false; // estoy escribiendo... no hay char ready
  wready = false; // estoy escribiendo... no hay word ready
  if (ccnt > sizeof morse) {
    ccnt = 0; // no cagarla...
    decode_error = true;
  }
  morse[ccnt++] = '.';
  treeptr = (2*treeptr) + 1;
  dit_pressed = false;
}

void dah(){
  key_press(); // todo lo que necesito hacer cada vez que se inicia un key press

  tone(SPK_OUT, spk_freq); // iniciar tono
  digitalWrite(PULSE_OUT,HIGH); // iniciar pulso
  Keyboard.press(KEY_RIGHT_CTRL); // simular por teclaro la presion de ctrl derecho
  delay(dah_len); // esperar el tiempo de raya
  Keyboard.releaseAll(); // soltar todas las teclas
  noTone(SPK_OUT); // frenar tono
  digitalWrite(PULSE_OUT,LOW); // frenar pulso

  last_key = millis(); // guardar timestamp desde ultimo pulso
  delay(intra_char); // separacion intra char

  cready = false; // estoy escribiendo... no hay char ready
  wready = false; // estoy escribiendo... no hay word ready
  if (ccnt > sizeof morse) {
    ccnt = 0; // no cagarla...
    decode_error = true;
  }
  morse[ccnt++] = '-';
  treeptr = (2*treeptr) + 2;
  dah_pressed = false;
}

void loop() {
  if (iambic) {
    while (1) {
      read_opts();
      if (digitalRead(DIT_PIN) == LOW || dit_pressed == true) dit(); // si presiono un dit o lo presione previamente (guardado por interrupt), ir a funcion de dit o dah respectivamente
      if (digitalRead(DAH_PIN) == LOW || dah_pressed == true) dah();

      if (millis()-last_key > inter_char) { // si el tiempo que paso desde el ultimo release de pulso es mayor al espacio interchar...
        digitalWrite(CHAR_LED,HIGH);        // prendo el led de limite de char
        if (!cready) decode_char();         // decodificar char
        cready = true;                      // termine de escribir un char! char ready!
      }
      if (millis()-last_key > wrd_sep) {    // si el tiempo que paso desde el ultimo release de pulso es mayor al espacio de palabra...
        digitalWrite(WORD_LED,HIGH);        // prendo el led de limite de palabra
        if (!wready) print_space();         // agrego un espacio al string
        wready = true;                      // termine de escribir una palabra! word ready!
      }
    }
  } else {
    while (1) {
      if (digitalRead(DIT_PIN) == LOW) {
        tone(SPK_OUT, spk_freq); // start tone
        Keyboard.press(KEY_RIGHT_CTRL); // simular por teclaro la presion de ctrl derecho
      } else {
        noTone(SPK_OUT); // stop sound
        Keyboard.releaseAll(); // soltar todas las teclas
      }
    }
  }
}
