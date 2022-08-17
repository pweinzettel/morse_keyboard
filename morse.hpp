#ifndef MORSE_h
#define MORSE_h

#define DIT '.'
#define DAH '-'

#include <Arduino.h>

class Morse {
  public:
  // Func
    Morse();
    void push(unsigned char symbol);
    void decode();
    void setWPM(unsigned int wpm);
    void clear();

  // Vars
    unsigned short decoded[2];
    unsigned int unit;
    unsigned int dit_len;
    unsigned int dah_len;
    unsigned int intra_char;
    unsigned int inter_char;
    unsigned int wrd_sep;
    unsigned long last_key;
    bool cready;
    bool wready;
    bool decode_error;
    volatile bool dit_pressed;
    volatile bool dah_pressed;

  private:
  // Func

  // Vars
    int treeptr;
    int symcnt;
    unsigned char morse[8];

//    const unsigned char CER = (char)255; // Char Error
    const unsigned char CER = '#'; // Char Error

    const unsigned char morse_tree[256] = {
      CER, 'E', 'T', 'I', 'A', 'N', 'M', 'S',  // 0 - 7
      'U', 'R', 'W', 'D', 'K', 'G', 'O', 'H',  // 8 - 15
      'V', 'F', CER, 'L', 0x85,'P', 'J', 'B',  // 16 - 23
      'X', 'C', 'Y', 'Z', 'Q', CER, 0x86,'5',  // 24 - 31
      '4', CER, '3', CER, CER, CER, '2', 0x81, // 32 - 39
//    CER, 0x80,CER, CER, CER, CER, '1', '6',  // 40 - 47
      CER, '+' ,CER, CER, CER, CER, '1', '6',  // 40 - 47
//    '=', '/', CER, CER, CER, 0x83,CER, '7',  // 48 - 55
      '=', '/', CER, CER, CER, '(', CER, '7',  // 48 - 55
      CER, CER, 'Ñ', '8', CER, '9', '0', CER,  // 56 - 63
      CER, CER, CER, CER, 0x84,CER, CER, CER,  // 64 - 71
      CER, CER, CER, '?', '_', CER, CER, CER,  // 72 - 79
      CER, '"', CER, CER, '.', CER, CER, CER,  // 80 - 87
      CER, '@', CER, CER, CER, '\'',CER, CER,  // 88 - 95
      '-', CER, CER, CER, CER, CER, CER, CER,  // 96 - 103
      CER, ';', '!', CER, ')', CER, CER, CER,  // 104 - 111
      CER, CER, ',', CER, CER, CER, CER, ':',  // 112 - 119
      CER, CER, CER, CER, CER, CER, CER, CER,  // 120 - 128
      CER, CER, CER, CER, CER, CER, CER, CER,  // 129 - 136
      CER, CER, CER, CER, CER, CER, CER, CER,  // 137 - 144
      CER, CER, CER, CER, CER, CER, CER, CER,  // 145 - 152
      CER, CER, CER, CER, CER, CER, CER, CER,  // 153 - 160
      CER, CER, CER, CER, CER, CER, CER, CER,  // 161 - 168
      CER, CER, CER, CER, CER, CER, CER, CER,  // 169 - 176
      CER, CER, CER, CER, CER, CER, CER, CER,  // 177 - 184
      CER, CER, CER, CER, CER, CER, CER, CER,  // 185 - 192
      CER, CER, CER, CER, CER, CER, CER, CER,  // 193 - 200
      CER, CER, CER, CER, CER, CER, CER, CER,  // 201 - 208
      CER, CER, CER, CER, CER, CER, CER, CER,  // 209 - 216
      CER, CER, CER, CER, CER, CER, CER, CER,  // 225 - 224
      CER, CER, CER, CER, CER, CER, CER, CER,  // 233 - 232
      CER, CER, CER, CER, CER, CER, CER, CER,  // 241 - 240
      CER, CER, CER, CER, CER, CER, CER, CER,  // 241 - 248
      CER, CER, CER, CER, CER, CER, CER, 0x82  // 249 - 256
    };
};

#endif
