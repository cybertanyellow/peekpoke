#ifndef PEEKPOKE_H
#define PEEKPOKE_H

unsigned peek(unsigned addr);
void poke(unsigned addr, unsigned value);
unsigned short peek16(unsigned addr);
void poke16(unsigned addr, unsigned short value);
unsigned char peek8(unsigned addr);
void poke8(unsigned addr, unsigned char value);

#endif
