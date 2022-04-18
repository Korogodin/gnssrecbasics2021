// Made on Earth by Murathon.


#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <bitset>

#define P2_5        pow(2,-5)
#define P2_55       pow(2,-55)
#define P2_43       pow(2,-43)
#define P2_31       pow(2,-31)
#define P2_4        pow(2,4)
#define P2_19       pow(2,-19)
#define P2_29       pow(2,-29)
#define P2_33       pow(2,-33)
#define SemiCirc    180


using namespace std;

struct Ephemeris {
    double    Crs;
    double    Dn;
    double    M0;
    double    Cuc;
    double   e;
    double   Cus;
    double   sqrtA;
    uint32_t toe;
    double   Cic;
    double   Omega0;
    double   Cis;
    double    i0;
    double   Crc;
    double   omega;
    double   OmegaDot;
    double   iDot;
    int16_t  Tgd;
    uint32_t toc;
    double   af2;
    double   af1;
    double   af0;
    uint16_t WN;
    uint16_t IODC;
    uint32_t  URA;
    uint32_t  Health;
    uint16_t IODE2;
    uint16_t IODE3;
    bool     codeL2;
    bool     L2P;
    uint32_t slot;
};

struct nssrSF {
    uint32_t slot;
    string sf1;
    string sf2;
    string sf3;
};

void parsStr(nssrSF *Rdata);
uint32_t pickSplitParam(string sf, uint16_t FS, int HMF, uint16_t FtS, int HMS);
void printEPH(Ephemeris* EPH);
void saveEPH(Ephemeris* EPH);
void decodeSF(Ephemeris* EPH, nssrSF *data);
int64_t  pick32(string sf, int32_t FrmN, int HmR);


int main(void)
{
    nssrSF data;
    parsStr(&data);
    cout << data.sf1 << endl << data.sf2 << endl << data.sf3 << endl;
    Ephemeris *EPH = (Ephemeris*) calloc(1, sizeof(Ephemeris));
    decodeSF(EPH,&data);
    printEPH(EPH);
    saveEPH(EPH);
    free(EPH);


}

void parsStr(nssrSF *Rdata)
{

    SetConsoleOutputCP(CP_UTF8);
    string path = "in.txt";

    ifstream fin;
    fin.open(path);

    if(fin.is_open()) {

        cout << "Файл открыт." << endl;
        while (!fin.eof()) {
            int N = 3;
            string rubber;
            int nmbr_stlt;
            uint32_t slot;
            uint32_t subFrameNum;
            string str;

            uint32_t slot_SF1;
            uint32_t slot_SF2;
            uint32_t slot_SF3;

            int mass[N];
            for(int i=0;i<N;++i)
            {
                fin >> mass[i];
            }
            fin >> rubber >> rubber >> rubber;
            fin >> nmbr_stlt >> slot >> rubber >> rubber >> subFrameNum;
            fin >> str;

            if (nmbr_stlt == 4 and slot>= 604800/6){

                if (subFrameNum == 1)
                {
                    slot_SF1 = slot;
                    Rdata->sf1 = str;
                }
                else if (subFrameNum == 2)
                {
                    slot_SF2 = slot;
                    Rdata->sf2 = str;

                }
                else if (subFrameNum == 3)
                {
                    slot_SF3 = slot;
                    Rdata->sf3 = str;
                }

                if (slot_SF1 + 1 == slot_SF2 and slot_SF2 + 1 == slot_SF3) {
                    Rdata->slot = slot_SF1;
                    return;
                }

            }
             //
        }
    }
    else
    {
        cout << "Ошибка открытия файла!!!" << endl;
    }
    fin.close();


}
void saveEPH(Ephemeris* EPH)
{
    ofstream fout;
    string path = "out.txt";
    fout.open(path);
    if(fout.is_open()) {

        cout << "Выгружаю" << endl;
        fout << endl << "LNAV Ephemeris (slot = " << EPH->slot << ") =" << endl;
        fout << "\t\t Crs	= " << EPH->Crs << endl;
        fout << "\t\t Dn	    = " << EPH->Dn << endl;
        fout << "\t\t M0	    = " << EPH->M0 << "\t\t[deg]" << endl;
        fout << "\t\t Cuc	= " << EPH->Cuc << endl;
        fout << "\t\t e      = " << EPH->e << endl;
        fout << "\t\t Cus	= " << EPH->Cus << endl;
        fout << "\t\t sqrtA	= " << EPH->sqrtA << endl;
        fout << "\t\t toe	= " << EPH->toe << endl;
        fout << "\t\t Cic	= " << EPH->Cic << endl;
        fout << "\t\t Omega0	= " << EPH->Omega0 << "\t\t[deg]" << endl;
        fout << "\t\t Cis	= " << EPH->Cis << endl;
        fout << "\t\t i0	    = " << EPH->i0 << "\t\t[deg]" << endl;
        fout << "\t\t Crc	= " << EPH->Crc << endl;
        fout << "\t\t omega	= " << EPH->omega << "\t\t[deg]" << endl;
        fout << "\t\t omeDot	= " << EPH->OmegaDot << "\t\t[deg/s]" << endl;
        fout << "\t\t iDot	= " << EPH->iDot << "\t\t[deg/s]" << endl;
        fout << "\t\t Tgd	= " << EPH->Tgd << "\t\t\t[sec]" << endl;
        fout << "\t\t toc	= " << EPH->toc << endl;
        fout << "\t\t af2	= " << EPH->af2 << endl;
        fout << "\t\t af1	= " << EPH->af1 << endl;
        fout << "\t\t af0	= " << EPH->af0 << endl;
        fout << "\t\t WN	    = " << EPH->WN << endl;
        fout << "\t\t IODC	= " << EPH->IODC << endl;
        fout << "\t\t URA	= " << EPH->URA << endl;
        fout << "\t\t Health	= " << EPH->Health << endl;
        fout << "\t\t IODE2	= " << EPH->IODE2 << endl;
        fout << "\t\t IODE3	= " << EPH->IODE3 << endl;
        fout << "\t\t codeL2	= " << EPH->codeL2 << endl;
        fout << "\t\t L2P	= " << EPH->L2P << endl;
    }
    else
    {
        cout << "Ошибка открытия файла!!!" << endl;
    }
    fout.close();
    cout << "Готово!";
}

void printEPH(Ephemeris* EPH)
{

    cout << endl << "LNAV Ephemeris (slot = " << EPH->slot << ") =" << endl;
    cout << "\t\t Crs	= " << EPH->Crs <<  endl;
    cout << "\t\t Dn	= " << EPH->Dn <<  endl;
    cout << "\t\t M0	= " << EPH->M0 << "\t\t[deg]" << endl;
    cout << "\t\t Cuc	= " << EPH->Cuc <<  endl;
    cout << "\t\t e	= " << EPH->e <<  endl;
    cout << "\t\t Cus	= " << EPH->Cus <<  endl;
    cout << "\t\t sqrtA	= " << EPH->sqrtA <<  endl;
    cout << "\t\t toe	= " << EPH->toe <<  endl;
    cout << "\t\t Cic	= " << EPH->Cic <<  endl;
    cout << "\t\t Omega0	= " << EPH->Omega0 << "\t\t[deg]" << endl;
    cout << "\t\t Cis	= " << EPH->Cis <<  endl;
    cout << "\t\t i0	= " << EPH->i0 << "\t\t[deg]" << endl;
    cout << "\t\t Crc	= " << EPH->Crc <<  endl;
    cout << "\t\t omega	= " << EPH->omega << "\t\t[deg]" << endl;
    cout << "\t\t omeDot	= " << EPH->OmegaDot << "\t\t[deg/s]" << endl;
    cout << "\t\t iDot	= " << EPH->iDot << "\t\t[deg/s]" << endl;
    cout << "\t\t Tgd	= " << EPH->Tgd << "\t\t\t[sec]" <<  endl;
    cout << "\t\t toc	= " << EPH->toc <<  endl;
    cout << "\t\t af2	= " << EPH->af2 <<  endl;
    cout << "\t\t af1	= " << EPH->af1 <<  endl;
    cout << "\t\t af0	= " << EPH->af0 <<  endl;
    cout << "\t\t WN	= " << EPH->WN <<  endl;
    cout << "\t\t IODC	= " << EPH->IODC <<  endl;
    cout << "\t\t URA	= " << EPH->URA <<  endl;
    cout << "\t\t Health	= " << EPH->Health <<  endl;
    cout << "\t\t IODE2	= " << EPH->IODE2 <<  endl;
    cout << "\t\t IODE3	= " << EPH->IODE3 <<  endl;
    cout << "\t\t codeL2	= " << EPH->codeL2 <<  endl;
    cout << "\t\t L2P	= " << EPH->L2P <<  endl;

}
int64_t pick32(string sf, int32_t FrmN, int HmR) {
    int64_t  ans = 0;
    int64_t Rans = 0;
    for (int i = FrmN; i < FrmN+HmR; i++) {
        ans = (ans | ((sf[i - 1] == '1') ? 1 : 0));
        cout << sf[i-1];
        if (i < FrmN+HmR-1){
            ans = ans<<1;
        }

    }
    return ans;


}

int64_t compl2int(uint64_t ans, int HmZ){
    int64_t Rans = 0;
    if (HmZ == 8){
        if (bool((1<<7) & ans)){
            ans |= 0xFFFFFFFFFFFFFF00;
            Rans = ~(ans-1);
            /*cout<< endl << bitset<64>(Rans).to_string() << endl;*/
            return -Rans;
        }

    }
    if (HmZ == 14){
        if (bool((1<<13) & ans)){
            ans |= 0xFFFFFFFFFFFFC000;
            Rans = ~(ans-1);
            return -Rans;
        }

    }
    if (HmZ == 16){
        if (bool((1<<15) & ans)){
            ans |= 0xFFFFFFFFFFFF0000;
            Rans = ~(ans-1);
            return -Rans;
        }

    }
    if (HmZ == 22){
        if (bool((1<<21) & ans)){
            ans |= 0xFFFFFFFFFFC00000;
            Rans = ~(ans-1);
            return -Rans;
        }

    }
    if (HmZ == 24){
        if (bool((1<<23) & ans)){
            ans |= 0xFFFFFFFFFF000000;
            Rans = ~(ans-1);
            return -Rans;
        }

    }
    if (HmZ == 32){
        if (bool((1<<31) & ans)){
            ans |= 0xFFFFFFFF00000000;
            Rans = ~(ans-1);
            return -Rans;
        }

    }
    return ans;
}
uint32_t pickSplitParam(string sf, uint16_t FS, int HMF, uint16_t FtS, int HMS) {
    uint32_t ans = 0;

    for (int i = FS; i < FS+HMF; i++) {
        ans = (ans | ((sf[i-1] == '1')? 1 : 0)) << 1;
    }
    for (int i = FtS; i < FtS+HMS; i++) {
        ans = ans | ((sf[i-1] == '1')? 1 : 0);
        if (i < FtS+HMS-1){
            ans = ans<<1;
        }
    }

    return ans;

}

void decodeSF(Ephemeris* EPH, nssrSF *data){

    EPH->slot = data->slot;

    EPH->Crs = compl2int(pick32(data->sf2,69,16),16)*P2_5;

    EPH->Dn = compl2int(pick32(data->sf2,91,16),16)*P2_43;

    EPH->M0 = compl2int(pickSplitParam(data->sf2,107, 8, 121, 24),32)*P2_31*SemiCirc;

    EPH->Cuc = compl2int(pick32(data->sf2,151,16),16)*P2_29;

    EPH->e = pickSplitParam(data->sf2,167, 8, 181, 24) * P2_33;

    EPH->Cus = compl2int(pick32(data->sf2,211,16),16)*P2_29;

    EPH->sqrtA = pickSplitParam(data->sf2,227, 8, 241, 24) * P2_19;

    EPH->toe = pick32(data->sf2,271,16)*pow(2,4);

    EPH->Cic = compl2int(pick32(data->sf3,61,16),16)*P2_29;

    EPH->Omega0 = compl2int(pickSplitParam(data->sf3,77, 8, 91, 24),32)*P2_31*SemiCirc;

    EPH->Cis = compl2int(pick32(data->sf3,121,16),16)*P2_29;

    EPH->i0 = compl2int(pickSplitParam(data->sf3,137, 8, 151, 24),32)*P2_31*SemiCirc;

    EPH->Crc = compl2int(pick32(data->sf3,181,16),16)*P2_5;

    EPH->omega = compl2int(pickSplitParam(data->sf3,197, 8, 211, 24),32)*P2_31*SemiCirc;

    EPH->OmegaDot = compl2int(pick32(data->sf3,241,24),24)*P2_43*SemiCirc;

    EPH->iDot = compl2int(pick32(data->sf3,279,14),14)*P2_43*SemiCirc;

    EPH->Tgd = compl2int(pick32(data->sf1,197,8),8)*P2_31;

    EPH->toc = compl2int(pick32(data->sf1,219,16),16)*P2_4;

    EPH->af2 = compl2int(pick32(data->sf1,241,8),8)*P2_55;

    EPH->af1 = compl2int(pick32(data->sf1,249,16),16)*P2_43;

    EPH->af0 = compl2int(pick32(data->sf1,271,22),22)*P2_31;

    EPH->WN = pick32(data->sf1,61,10);

    EPH->IODC = pickSplitParam(data->sf1,83, 2, 211, 8);

    EPH->URA = pick32(data->sf1,73,4);

    EPH->Health = EPH->IODE2 = pick32(data->sf1,73,6);

    EPH->IODE2 = pick32(data->sf2,61,8);

    EPH->IODE3 = pick32(data->sf3,271,8);

    EPH->codeL2 = pick32(data->sf1,71,2);

    EPH->L2P = data->sf1[90];






}

