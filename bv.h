// bv.h — Bit Vector interface
// Author: Prof. Darrell Long

# ifndef _BVector
# define _BVector
# include <stdint.h>
# include <stdbool.h>
# include "code.h"

typedef struct bitV {
	uint8_t *v;
	uint32_t l;
    uint32_t pos;
} bitV;

bitV *newVec(uint32_t);

void delVec(bitV *);

void one(bitV *);

void setBit(bitV *, uint32_t);

void clrBit(bitV *, uint32_t);

uint8_t valBit(bitV *, uint32_t);

uint32_t lenVec(bitV *);

void append(bitV* bitVec, code c, uint32_t *array);
# endif
