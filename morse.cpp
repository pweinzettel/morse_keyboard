#include "morse.hpp"

Morse::Morse() {
  this->key_mode = key_mode;
  this->treeptr = 0;
  this->last_key = 0;
  this->decode_error = false;
  this->cready = true;
  this->wready = true;
  memset(this->straight_pulse, 0x00, sizeof(this->straight_pulse));
  memset(this->straight_space, 0x00, sizeof(this->straight_space));
}

void Morse::push(byte symbol) {
  this->last_key = millis();
  this->cready = false;
  this->wready = false;
  this->decode_error = false;

  switch (symbol) {
    case DIT:
      this->treeptr = (2*this->treeptr)+1;
      this->dit_pressed = false;
    break;
    case DAH:
      this->treeptr = (2*this->treeptr)+2;
      this->dah_pressed = false;
    break;
  }
}

void Morse::clear() {
  this->treeptr = 0;
  memset(this->straight_pulse, 0x00, sizeof(this->straight_pulse));
}

void Morse::setKeyMode(byte *key_mode) {
  this->key_mode = key_mode;
}

void Morse::setWPM(unsigned int wpm) {
  this->wpm = wpm;
  this->unit = 60000/(wpm * 50); // una palabra tiene 50 unidades (PARIS = .--. .- .-. .. ...) dit = 1u, dah = 3u, intra char = 1u, inter char = 3u, word sep = 7u
  this->dit_len = unit;
  this->dah_len = (unit*3);
  this->intra_char = (unit);
  this->inter_char = (unit*3);
  this->wrd_sep = (unit*7);
}

void Morse::straight_start() {
  this->straight_ini = millis();
  if (this->straight_s_ini != 0) this->straight_s_push(millis() - this->straight_s_ini);
  this->last_key = millis()         ;
  this->cready = false;
  this->wready = false;
  this->decode_error = false;
  this->straight_pressed = true;
}

void Morse::straight_stop() {
  this->straight_s_ini = millis();
  this->straight_push(millis() - this->straight_ini);
  this->last_key = millis();
  this->cready = false;
  this->wready = false;
  this->decode_error = false;
  this->straight_pressed = false;
}

void Morse::straight_push(int *t) {
  if (straight_p_indx >= STACK) straight_p_indx = 0;
  this->straight_pulse[straight_p_indx++] = t;
}

void Morse::straight_s_push(int *t) {
  if (straight_s_indx >= STACK) straight_s_indx = 0;
  this->straight_space[straight_s_indx++] = t;
}

void Morse::unit_len() {
  for (byte i = 0; i < (sizeof(this->straight_pulse) / sizeof(this->straight_pulse[0])); i++) {
    if (straight_pulse[i] != 0) {
      for (byte j = 0; j < (sizeof(this->straight_pulse) / sizeof(this->straight_pulse[0])); j++) {
        if (straight_pulse[j] != 0) {
          if (straight_pulse[i] > (straight_pulse[j]*2) && straight_pulse[i] < (straight_pulse[j]*4)) {
            this->dit_len = (this->dit_len+straight_pulse[j])/2;
          }
          if (straight_pulse[i] < (straight_pulse[j]/2) && straight_pulse[i] > (straight_pulse[j]/4)) {
            this->dah_len = (this->dah_len+straight_pulse[j])/2;
          }
        }
      }
    }
  }
  if (this->dit_len == 0) return;
  this->unit = this->dit_len;
  this->dah_len = (unit*3);
  this->intra_char = (unit);
  this->inter_char = (unit*3);
  this->wrd_sep = (unit*7);
  this->wpm = (60000/50/this->unit);
}

void Morse::decode(byte *decoded) {
  memset(decoded, 0x00, sizeof(decoded));
  switch (this->key_mode) {
    case IAMBIC:
      unsigned int aux;
      if (this->treeptr > sizeof(this->morse_tree)/sizeof(this->morse_tree[0])) {
        aux = 0; // Evito pasarme de index - 0 es CER (char err)
      } else {
        aux = this->treeptr;
      }
      this->clear();
      switch (this->morse_tree[aux]) {
        case 0x80:
          decoded[0] = 'A';
          decoded[1] = 'R';
        break;
        case 0x81:
          decoded[0] = 'A';
          decoded[1] = 'S';
        break;
        case 0x82:
          decoded[0] = 'H';
          decoded[1] = 'H';
        break;
        case 0x83:
          decoded[0] = 'K';
          decoded[1] = 'N';
        break;
        case 0x84:
          decoded[0] = 'S';
          decoded[1] = 'K';
        break;
        case 0x85:
          decoded[0] = 'A';
          decoded[1] = 'A';
        break;
        case 0x86:
          decoded[0] = 'C';
          decoded[1] = 'H';
        break;

        default:
          decoded[0] = this->morse_tree[aux];
          if (decoded[0] == CER) {
            this->decode_error = true;
          }
        break;
      }
    break;

    case STRAIGHT:
      if (this->straight_s_ini == 0) return;
      this->unit_len();
      if (this->unit == 0) return;
#ifdef SERIAL_DEBUG
      Serial.print("(");
      Serial.print(this->wpm);
      Serial.print(",");
      Serial.print(millis());
      Serial.print(",");
      Serial.print(this->last_key);
      Serial.print(",");
      Serial.print(this->wrd_sep);
      Serial.print(")");
#endif //SERIAL_DEBUG
      for (byte i = 0; i < (sizeof(this->straight_pulse) / sizeof(this->straight_pulse[0])); i++) {
#ifdef SERIAL_DEBUG
        Serial.print(",");
        Serial.print(this->straight_pulse[i]);
#endif //SERIAL_DEBUG
        if (straight_pulse[i] != 0) {
          if (straight_pulse[i] > this->dit_len/2 && straight_pulse[i] < this->dit_len*2) {
            //this->push(DIT);
          }
          if (straight_pulse[i] > this->dah_len/2 && straight_pulse[i] > this->dah_len*2) {
            //this->push(DAH);
          }
        }
      }
#ifdef SERIAL_DEBUG
      Serial.println();
#endif //SERIAL_DEBUG
    break;
  }
}
