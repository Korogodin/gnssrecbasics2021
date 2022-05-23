#include "../header/head.h"

using namespace std;

int main()
{
    frame pack;
    otpravit(&pack);
    Ephemeris *Eph = (Ephemeris*)malloc(sizeof(Ephemeris));
    razobrat(Eph, &pack);
    sohranit(Eph);
    free(Eph);
}
