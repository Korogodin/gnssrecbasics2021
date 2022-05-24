#include "../header/head.h"

double kepler(double Mk, double e) {
    double Ek = Mk;
    double Ek1;
    do {
        Ek1 = Ek;
        Ek = Mk + e * sin(Ek);
    } while (fabs(Ek1 - Ek) > 1E-8);
    return Ek;
}
