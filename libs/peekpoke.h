#ifndef PEEKPOKE_H
#define PEEKPOKE_H

unsigned peek(unsigned addr);
void poke(unsigned addr, unsigned value);
unsigned short peek16(unsigned addr);
void poke16(unsigned addr, unsigned short value);

#endif
