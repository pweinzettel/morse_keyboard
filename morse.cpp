#include "morse.hpp"

Morse::Morse() {
  this->treeptr = 0;
  this->last_key = 0;
  this->decode_error = false;
  this->cready = true;
  this->wready = true;
}

void Morse::push(unsigned char symbol) {
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
  memset(this->decoded, 0x00, sizeof(this->decoded));
}

void Morse::setWPM(unsigned int wpm) {
  this->unit = 60000/(wpm * 50); // una palabra tiene 50 unidades (PARIS = .--. .- .-. .. ...) dit = 1u, dah = 3u, intra char = 1u, inter char = 3u, word sep = 7u
  this->dit_len = unit;
  this->dah_len = (unit*3);
  this->intra_char = (unit);
  this->inter_char = (unit*3);
  this->wrd_sep = (unit*7);
}

void Morse::decode() {
  unsigned int aux;
  if (this->treeptr > sizeof this->morse_tree) {
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
}
