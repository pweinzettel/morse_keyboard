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

#define EEPROM_WPM 0

#define SHIFT 0x80
#define ALT_GR 0xc0
#define ISO_KEY 0x64
#define ISO_REPLACEMENT 0x32

const byte enie[8] = {
 B01101,
 B10010,
 B00000,
 B10001,
 B11001,
 B10101,
 B10011,
 B10001
};

extern const uint8_t KeyboardLayout_custom[129] PROGMEM =
{
	0x00,          // NUL
	0x00,          // SOH
	0x00,          // STX
	0x00,          // ETX
	0x00,          // EOT
	0x00,          // ENQ
	0x00,          // ACK
	0x00,          // BEL
	0x2a,          // BS  Backspace
	0x2b,          // TAB Tab
	0x28,          // LF  Enter
	0x00,          // VT
	0x00,          // FF
	0x00,          // CR
	0x00,          // SO
	0x00,          // SI
	0x00,          // DEL
	0x00,          // DC1
	0x00,          // DC2
	0x00,          // DC3
	0x00,          // DC4
	0x00,          // NAK
	0x00,          // SYN
	0x00,          // ETB
	0x00,          // CAN
	0x00,          // EM
	0x00,          // SUB
	0x00,          // ESC
	0x00,          // FS
	0x00,          // GS
	0x00,          // RS
	0x00,          // US
	0x2c,          // ' '
	0x1e|SHIFT,    // !
	0x1f|SHIFT,    // "
	0x20|SHIFT,    // #
	0x21|SHIFT,    // $
	0x22|SHIFT,    // %
	0x23|SHIFT,    // &
	0x2d,          // '
	0x25|SHIFT,    // (
	0x26|SHIFT,    // )
	0x30|SHIFT,    // *
	0x30,          // +
	0x36,          // ,
	0x38,          // -
	0x37,          // .
	0x24|SHIFT,    // /
	0x27,          // 0
	0x1e,          // 1
	0x1f,          // 2
	0x20,          // 3
	0x21,          // 4
	0x22,          // 5
	0x23,          // 6
	0x24,          // 7
	0x25,          // 8
	0x26,          // 9
	0x37|SHIFT,    // :
	0x36|SHIFT,    // ;
	0x32,          // <
	0x27|SHIFT,    // =
	0x32|SHIFT,    // >
	0x2d|SHIFT,    // ?
	0x14|ALT_GR,   // @
	0x04|SHIFT,    // A
	0x05|SHIFT,    // B
	0x06|SHIFT,    // C
	0x07|SHIFT,    // D
	0x08|SHIFT,    // E
	0x09|SHIFT,    // F
	0x0a|SHIFT,    // G
	0x0b|SHIFT,    // H
	0x0c|SHIFT,    // I
	0x0d|SHIFT,    // J
	0x0e|SHIFT,    // K
	0x0f|SHIFT,    // L
	0x10|SHIFT,    // M
	0x11|SHIFT,    // N
	0x12|SHIFT,    // O
	0x13|SHIFT,    // P
	0x14|SHIFT,    // Q
	0x15|SHIFT,    // R
	0x16|SHIFT,    // S
	0x17|SHIFT,    // T
	0x18|SHIFT,    // U
	0x19|SHIFT,    // V
	0x1a|SHIFT,    // W
	0x1b|SHIFT,    // X
	0x1c|SHIFT,    // Y
	0x1d|SHIFT,    // Z
	0x2f|ALT_GR,   // [
	0x35|ALT_GR,   // bslash
	0x30|ALT_GR,   // ]
	0x00,          // ^  not supported (requires dead key + space)
	0x38|SHIFT,    // _
	0x00,          // `  not supported (requires dead key + space)
	0x04,          // a
	0x05,          // b
	0x06,          // c
	0x07,          // d
	0x08,          // e
	0x09,          // f
	0x0a,          // g
	0x0b,          // h
	0x0c,          // i
	0x0d,          // j
	0x0e,          // k
	0x0f,          // l
	0x10,          // m
	0x11,          // n
	0x12,          // o
	0x13,          // p
	0x14,          // q
	0x15,          // r
	0x16,          // s
	0x17,          // t
	0x18,          // u
	0x19,          // v
	0x1a,          // w
	0x1b,          // x
	0x1c,          // y
	0x1d,          // z
	0x34|ALT_GR,   // {
	0x1e|ALT_GR,   // |
	0x31|ALT_GR,   // }
	0x00,          // ~  not supported (requires dead key + space)
  0x33|SHIFT,    // Ñ
};
