// Configuraciones

// Mensaje de bienvenida
const String welcome_1 = "MORSE";
const String welcome_2 = "LU9ABM";

// LCD a utilizar, especificar address del modulo serie/paralelo i2c y tamaño del mismo
#define LCD_ADDR 0x27
#define LCD_COL 16
#define LCD_ROW 2

// LEDs a utilizar
#define CHAR_LED 7
#define WORD_LED 8
#define PULSE_LED 9

// Que tecla va a que pin de la placa
#define KEY_CLEAR 4
#define KEY_BACKSP 14
#define KEY_INTRO 15
#define KEY_SEND 16

// Anti-rebote de teclas en ms //TODO
#define DEBOUNCE_DEALY 500
#define DEBOUNCE_EVERY 100

// Pines donde se conectara el manipulador (deben ser interrupt)
#define DIT_PIN 1
#define DAH_PIN 0

// Salida de audio
#define SPK_OUT 10

// notdef = default = SPK_FREQREQ
//#define SPK_READ A1
#define SPK_FREQ 580


// notdef = default = WPM_DEF
//#define WPM_READ A0
#define WPM_DEF 20
