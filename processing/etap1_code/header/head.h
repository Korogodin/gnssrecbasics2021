#ifndef HEAD_H
#define HEAD_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>

#define Cnst_2E5 pow(2,-5)
#define Cnst_2E55 pow(2,-55)
#define Cnst_2E43 pow(2,-43)
#define Cnst_2E31 pow(2,-31)
#define Cnst_2E4 pow(2,4)
#define Cnst_2E19 pow(2,-19)
#define Cnst_2E29 pow(2,-29)
#define Cnst_2E33 pow(2,-33)
#define SC 180

struct Ephemeris {
    float Crs = 0;
    float Dn = 0;
    float M0 = 0;
    float Cuc = 0;
    float e = 0;
    float Cus = 0;
    float sqrtA = 0;
    uint32_t toe = 0;
    float Cic = 0;
    float Omega0 = 0;
    float Cis = 0;
    float i0 = 0;
    float Crc = 0;
    float omega = 0;
    float OmegaDot = 0;
    float iDot = 0;
    int16_t Tgd = 0;
    uint32_t toc = 0;
    float af2 = 0;
    float af1 = 0;
    float af0 = 0;
    uint16_t WN = 0;
    uint16_t IODC = 0;
    uint32_t URA = 0;
    uint32_t Health = 0;
    uint16_t IODE2 = 0;
    uint16_t IODE3 = 0;
    bool codeL2;
    bool L2P;
    uint32_t pack = 0;
};

struct frame {
    uint32_t pack_slot = 0;
    std::string Cnst1;
    std::string Cnst2;
    std::string Cnst3;
};

void otpravit(frame *Rpack);
uint32_t gruppirovka(std::string Cnst, uint16_t start1, int dlit1, uint16_t start2, int dlit2);
void sohranit(Ephemeris* Eph);
void razobrat(Ephemeris* Eph, frame *pack);
int32_t data(std::string Cnst, int32_t start, int dlit);

#endif // HEAD_H
