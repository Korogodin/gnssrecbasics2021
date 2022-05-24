#include "../header/head.h"

void sohranit(Ephemeris* Eph)
{
    std::ofstream fout;
    std::string path = "out.txt";
    fout.open(path);
    fout << std::endl << "LNAV Ephemeris (slot = " << Eph->pack << ") = " << std::endl;
    fout << "\t Crs = " << Eph->Crs << std::endl;
    fout << "\t Dn = " << Eph->Dn << "\t\t[deg/s]" << std::endl;
    fout << "\t M0 = " << Eph->M0 << "\t\t\t[deg]" << std::endl;
    fout << "\t Cuc = " << Eph->Cuc << std::endl;
    fout << "\t e = " << Eph->e << std::endl;
    fout << "\t Cus = " << Eph->Cus << std::endl;
    fout << "\t sqrtA = " << Eph->sqrtA << std::endl;
    fout << "\t toe = " << Eph->toe << std::endl;
    fout << "\t Cic = " << Eph->Cic << std::endl;
    fout << "\t Omega0 = " << Eph->Omega0 << "\t\t[deg]" << std::endl;
    fout << "\t Cis = " << Eph->Cis << std::endl;
    fout << "\t i0 = " << Eph->i0 << "\t\t\t[deg]" << std::endl;
    fout << "\t Crc = " << Eph->Crc << std::endl;
    fout << "\t omega = " << Eph->omega << "\t\t[deg]" << std::endl;
    fout << "\t OmegaDot = " << Eph->OmegaDot << "\t[deg/s]" << std::endl;
    fout << "\t iDot = " << Eph->iDot << "\t\t[deg/s]" << std::endl;
    fout << "\t Tgd = " << Eph->Tgd << std::endl;
    fout << "\t toc = " << Eph->toc << std::endl;
    fout << "\t af2 = " << Eph->af2 << std::endl;
    fout << "\t af1 = " << Eph->af1 << std::endl;
    fout << "\t af0 = " << Eph->af0 << std::endl;
    fout << "\t WN = " << Eph->WN << std::endl;
    fout << "\t IODC = " << Eph->IODC << std::endl;
    fout << "\t URA = " << Eph->URA << std::endl;
    fout << "\t Health = " << Eph->Health << std::endl;
    fout << "\t IODE2 = " << Eph->IODE2 << std::endl;
    fout << "\t IODE3 = " << Eph->IODE3 << std::endl;
    fout << "\t codeL2 = " << Eph->codeL2 << std::endl;
    fout << "\t L2P = " << Eph->L2P << std::endl;
    fout.close();
}
