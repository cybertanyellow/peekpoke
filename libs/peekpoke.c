#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>

#include "peekpoke.h"

static unsigned peekX(unsigned addr, size_t data_size)
{
    int fd;
    void *ptr;
    unsigned page_addr, page_offset, result;
    unsigned page_size = sysconf(_SC_PAGESIZE);

    fd = open("/dev/mem", O_RDONLY);
    if (fd < 1) {
        perror("peekpoke: unable to open /dev/mem");
        exit(EXIT_FAILURE);
    }

    page_addr = (addr & ~(page_size - 1));
    page_offset = addr - page_addr;

    ptr = mmap(NULL, page_size, PROT_READ, MAP_SHARED, fd, (addr & ~(page_size - 1)));
    if (ptr == (void*) -1) {
        perror("peekpoke: unable to map memory");
        exit(EXIT_FAILURE);
    }

	if (data_size == 1) {
		uint8_t ret8 = *((uint8_t *)(ptr + page_offset));
		result = (unsigned)ret8;
	} else if (data_size == 2) {
		uint16_t ret16 = *((uint16_t *)(ptr + page_offset));
		result = (unsigned)ret16;
	} else /*if (data_size == 4) */{
		uint32_t ret32 = *((uint32_t *)(ptr + page_offset));
		result = (unsigned)ret32;
	}

    if (munmap(ptr, page_size) < 0) {
        perror("peekpoke: unable to unmap memory");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        perror("peekpoke: unable to close /dev/mem");
        exit(EXIT_FAILURE);
    }

    return result;
}

void pokeX(unsigned addr, unsigned value, size_t data_size)
{
    int fd;
    void *ptr;
    unsigned page_addr, page_offset;
    unsigned page_size = sysconf(_SC_PAGESIZE);

    fd = open("/dev/mem", O_RDWR);
    if (fd < 1) {
        perror("peekpoke: unable to open /dev/mem");
        exit(EXIT_FAILURE);
    }

    page_addr = (addr & ~(page_size - 1));
    page_offset = addr - page_addr;

    ptr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (addr & ~(page_size - 1)));
    if (ptr == (void*) -1) {
        perror("peekpoke: unable to map memory");
        exit(EXIT_FAILURE);
    }

	if (data_size == 1) {
		*((uint8_t *)(ptr + page_offset)) = (uint8_t)value;
	} else if (data_size == 2) {
		*((uint16_t *)(ptr + page_offset)) = (uint16_t)value;
	} else {
		*((uint32_t *)(ptr + page_offset)) = (uint32_t)value;
	}

    if (munmap(ptr, page_size) < 0) {
        perror("peekpoke: unable to unmap memory");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        perror("peekpoke: unable to close /dev/mem");
        exit(EXIT_FAILURE);
    }
}

unsigned peek(unsigned addr)
{
	return peekX(addr, sizeof(unsigned));
}

void poke(unsigned addr, unsigned value)
{
	return pokeX(addr, value, sizeof(value));
}

unsigned short peek16(unsigned addr)
{
	return peekX(addr, sizeof(unsigned short));
}

void poke16(unsigned addr, unsigned short value)
{
	return pokeX(addr, value, sizeof(value));
}

unsigned char peek8(unsigned addr)
{
	return peekX(addr, sizeof(unsigned char));
}

void poke8(unsigned addr, unsigned char value)
{
	return pokeX(addr, value, sizeof(value));
}
