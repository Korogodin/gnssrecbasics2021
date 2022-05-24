#include "../header/head.h"

void Ocoord(double t, double* coord, Ephemeris Ef)
{   double pi = 3.1415326535898;
    double c = pi/180;
    double mu = 3.986004418E+14;
    double we = 7.2921150E-5;
    Ef.Dn = Ef.Dn*c;
    Ef.M0 = Ef.M0*c;
    Ef.Omega0 = Ef.Omega0*c;
    Ef.i0 = Ef.i0*c;
    Ef.omega = Ef.omega*c;
    Ef.OmegaDot = Ef.OmegaDot*c;
    Ef.iDot = Ef.iDot*c;
    double tk = t - Ef.toe;
    if (tk > 302400)
        tk = tk - 604800;
    else if (tk < -302400)
        tk = tk + 604800;
    Ef.sqrtA = pow(Ef.sqrtA, 2);
    double Mk = Ef.M0 + (sqrt(mu) / pow(sqrt(Ef.sqrtA), 3) + Ef.Dn) * tk;
    double Ek = kepler(Mk, Ef.e);
    double Vk = atan2(sqrt(1 - pow(Ef.e, 2)) * sin(Ek), ((cos(Ek) - Ef.e)/(1-Ef.e*cos(Ek))));
    double Uk = Ef.omega + Vk + Ef.Cus * sin(2 * (Ef.omega + Vk)) + Ef.Cuc * cos(2 * (Ef.omega + Vk));
    double rk = Ef.sqrtA * (1 - Ef.e * cos(Ek)) + Ef.Crc * sin(2 * (Ef.omega + Vk)) + Ef.Crc* cos(2 * (Ef.omega + Vk));
    double ik = Ef.i0 + Ef.iDot * tk + Ef.Cic * cos(2 * (Ef.omega + Vk)) + Ef.Cis * sin(2 * (Ef.omega + Vk));
    double lambk = Ef.Omega0 + (Ef.OmegaDot - we) * tk - we * Ef.toe;
    double xk = rk * cos(Uk);
    double yk = rk * sin(Uk);
    coord[0] = xk * cos(lambk) - yk * cos(ik) * sin(lambk);
    coord[1] = xk * sin(lambk) + yk * cos(ik) * cos(lambk);
    coord[2] = yk * sin(ik);
}
