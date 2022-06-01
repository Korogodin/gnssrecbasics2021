#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <windows.h>
#include <ctime>

#define Semi_circles 180
#define pi 3.1415326535898
#define SF1       pow(2,-5)
#define SF2       pow(2,-43)
#define SF3       pow(2,-31)
#define SF4       pow(2,-29)
#define SF5       pow(2,-33)
#define SF6       pow(2,-19)
#define SF7       pow(2,4)
#define SF8       pow(2,-55)

using namespace std;
struct Ephemeris 
{
    float    Crs;
    float    Dn;
    float    M0;
    float    Cuc;
    float    e;
    float    Cus;
    float    sqrtA;
    uint32_t toe;
    float    Cic;
    double   Omega0;
    float    Cis;
    float    i0;
    float    Crc;
    float    omega;
    float    OmegaDot;
    float    iDot;
    int16_t  Tgd;
    uint32_t toc;
    float    af2;
    float    af1;
    float    af0;
    uint16_t WN;
    uint16_t IODC;
    uint32_t URA;
    uint32_t Health;
    uint16_t IODE2;
    uint16_t IODE3;
    bool     codeL2;
    bool     L2P;
    uint32_t slot;
};

struct WorkingSub 
{
    string sf1;
    string sf2;
    string sf3;
	uint32_t slot;
};

struct Coordinates 
{
    double x;
    double y;
    double z;
};

void Extraction(WorkingSub *Subframes);
void sf2eph(Ephemeris* ep, WorkingSub *data);
int32_t  str2uint(string sf, int32_t Begin, int End);
void printEPH(Ephemeris* ep);
void saveEPH(Ephemeris* ep);
void Coordinate_calculation(Ephemeris* ep,uint32_t t, Coordinates *Position);


int main(void)
{
	uint32_t begin_time = clock();
    WorkingSub data;
    Extraction(&data);
    Ephemeris *ep = (Ephemeris*) calloc(1, sizeof(Ephemeris));
    sf2eph(ep,&data);
    printEPH(ep);
    saveEPH(ep);
	
	uint32_t start = 86419;
	uint32_t stop = 172818;
	
    Coordinates Position;
	
    double** Massive_coordinates = new double*[3];
	
    for (int i = 0; i < 3; i++) 
	{
        Massive_coordinates[i] = new double[stop - start];
    }

    double** Massive_coordinates_matlab = new double*[3];
	
    for (int i = 0; i < 3; i++) 
	{
        Massive_coordinates_matlab[i] = new double[stop - start];
    }
    
    for (int t = start; t < stop; t++) 
	{
        Coordinate_calculation(ep, t, &Position);
		Massive_coordinates[0][t-start] = Position.x;
        Massive_coordinates[1][t-start] = Position.y;
        Massive_coordinates[2][t-start] = Position.z;
    }

    ifstream file("coordinates.txt");
    double sec;
    if (!file.is_open())
        cout << "Can't open" << endl;
    else {
        for (int t = 0; t < stop - start; t++) 
		{
            file >> sec >> Massive_coordinates_matlab[0][t] >> Massive_coordinates_matlab[1][t] >> Massive_coordinates_matlab[2][t];
        }
        file.close();
    }

    double max_delta = 0;
    for (int i = 0; i < 3; i++) 
	{
        for (int k = 0; k < stop - start; k++) 
		{
            if (abs(Massive_coordinates[i][k] - Massive_coordinates_matlab[i][k]) > max_delta) 
			{
                max_delta = abs(Massive_coordinates[i][k] - Massive_coordinates_matlab[i][k]);
            }
        }
    }
	
	delete[] *Massive_coordinates;
    delete[] Massive_coordinates;
    delete[] *Massive_coordinates_matlab;
    delete[] Massive_coordinates_matlab;
    
    uint32_t end_time = clock();
    uint32_t calculation_time = end_time - begin_time;
	
    cout << "max_delta = " << max_delta << "(m)" << endl;
	cout << "calculation time = " << (double)calculation_time/1000.0 << "(sec)" <<endl;
    free(ep);
}

void Coordinate_calculation(Ephemeris* ep,uint32_t t, Coordinates *Position)
{
	double Omega_e_dot = 7.2921151467e-5;
    double mu = 3.986005e14;
	double A = pow(ep->sqrtA,2);
	double n_0 = sqrt(mu / pow(A, 3));
	int32_t t_k = t - ep->toe;
	
    if (t_k > 302400) 
	{
        t_k = t_k - 604800;
    }
    else if (t_k < -302400) 
	{
        t_k = t_k + 604800;
    }

    double n = n_0 + ep->Dn;
	
    double M_k = ep->M0 + n * t_k;
	double E_0 = M_k;
	double E_k = 0;
	int k = 0;
    
    while (abs(E_0 - E_k) > 1e-8)
	{
        E_k = E_0;
		
		E_0 = E_0 + (M_k - E_0 + ep->e * sin(E_0))/(1 - ep->e * cos(E_0));
		
        k = k + 1;
    }

    double nu_k = atan2((sqrt(1 - pow(ep->e,2)) * sin(E_k)/(1 - ep->e * cos(E_k))), ((cos(E_k) - ep->e)/(1 - ep->e * cos(E_k))));
    double Phi_k = nu_k + ep->omega;

    double delta_u_k = ep->Cus * sin(2 * Phi_k) + ep->Cuc * cos(2 * Phi_k);
    double delta_r_k = ep->Crs * sin(2 * Phi_k) + ep->Crc * cos(2 * Phi_k);
    double delta_i_k = ep->Cis * sin(2 * Phi_k) + ep->Cic * cos(2 * Phi_k);
    double u_k = Phi_k + delta_u_k;
    double r_k = pow(ep->sqrtA,2) * (1 - ep->e * cos(E_k)) + delta_r_k;
    double i_k = ep->i0 + delta_i_k + ep->iDot * t_k ;

    double x_k_sh = r_k * cos(u_k);
    double y_k_sh = r_k * sin(u_k);
	
    double Omega_k = ep->Omega0 + (ep->OmegaDot - Omega_e_dot) * t_k - Omega_e_dot * ep->toe;

    double x_k = x_k_sh * cos(Omega_k) - y_k_sh * cos(i_k) * sin(Omega_k);
    double y_k = x_k_sh * sin(Omega_k) + y_k_sh * cos(i_k) * cos(Omega_k);
    double z_k = y_k_sh * sin(i_k);

	Position->x = x_k;
    Position->y = y_k;
    Position->z = z_k;
}

void Extraction(WorkingSub *Subframes)
{
    string path = "in.txt";
    ifstream fin;
    fin.open(path);

    if(fin.is_open()) {
		
        while (!fin.eof()) {
            
            uint32_t slot;
			uint32_t slot_SF1 = 0;
            uint32_t slot_SF2 = 0;
            uint32_t slot_SF3 = 0;
            uint32_t subFrameNum;
			int svStr;
			int svNum = 9;
            string strSF;
			string useless;
            
            fin >> useless >> useless >> useless >> useless >> useless >> useless >> svStr >> slot >> useless >> useless >> subFrameNum >> strSF;

            if (svStr == svNum and slot>= 604800/6){

                if (subFrameNum == 1)
                {
                    slot_SF1 = slot;
                    Subframes->sf1 = strSF;
                }
                else if (subFrameNum == 2)
                {
                    slot_SF2 = slot;
                    Subframes->sf2 = strSF;

                }
                else if (subFrameNum == 3)
                {
                    slot_SF3 = slot;
                    Subframes->sf3 = strSF;
                }

                if (slot_SF1 + 1 == slot_SF2 and slot_SF2 + 1 == slot_SF3) {
                    Subframes->slot = slot_SF1;
                    return;
                }
            }
        }
    }
    else
    {
        cout << "Can't open" << endl;
    }
    fin.close();


}

void saveEPH(Ephemeris* ep)
{
    ofstream fout;
    string path = "out.txt";
    fout.open(path);
    if(fout.is_open()) {
        fout << endl << "LNAV Ephemeris (slot = " << ep->slot << ") =" << endl;
        fout << "\t\t Crs\t\t\t= " << ep->Crs << endl;
        fout << "\t\t Dn\t\t\t= " << ep->Dn << endl;
        fout << "\t\t M0\t\t\t= " << ep->M0 << "\t\t[deg]" << endl;
        fout << "\t\t Cuc\t\t\t= " << ep->Cuc << endl;
        fout << "\t\t e\t\t\t= " << ep->e << endl;
        fout << "\t\t Cus\t\t\t= " << ep->Cus << endl;
        fout << "\t\t sqrtA\t\t= " << ep->sqrtA << endl;
        fout << "\t\t toe\t\t\t= " << ep->toe << endl;
        fout << "\t\t Cic\t\t\t= " << ep->Cic << endl;
        fout << "\t\t Omega0\t\t= " << ep->Omega0 << "\t\t[deg]" << endl;
        fout << "\t\t Cis\t\t\t= " << ep->Cis << endl;
        fout << "\t\t i0\t\t\t= " << ep->i0 << "\t\t[deg]" << endl;
        fout << "\t\t Crc\t\t\t= " << ep->Crc << endl;
        fout << "\t\t omega\t\t= " << ep->omega << "\t\t[deg]" << endl;
        fout << "\t\t omegaDot\t\t= " << ep->OmegaDot << "\t[deg/s]" << endl;
        fout << "\t\t iDot\t\t\t= " << ep->iDot << "\t[deg/s]" << endl;
        fout << "\t\t Tgd\t\t\t= " << ep->Tgd << "\t\t\t[sec]" << endl;
        fout << "\t\t toc\t\t\t= " << ep->toc << endl;
        fout << "\t\t af2\t\t\t= " << ep->af2 << endl;
        fout << "\t\t af1\t\t\t= " << ep->af1 << endl;
        fout << "\t\t af0\t\t\t= " << ep->af0 << endl;
        fout << "\t\t WN\t\t\t= " << ep->WN << endl;
        fout << "\t\t IODC\t\t\t= " << ep->IODC << endl;
        fout << "\t\t URA\t\t\t= " << ep->URA << endl;
        fout << "\t\t Health\t\t= " << ep->Health << endl;
        fout << "\t\t IODE2\t\t= " << ep->IODE2 << endl;
        fout << "\t\t IODE3\t\t= " << ep->IODE3 << endl;
        fout << "\t\t codeL2\t\t= " << ep->codeL2 << endl;
        fout << "\t\t L2P\t\t\t= " << ep->L2P << endl;
    }
    else
    {
        cout << "Cant open" << endl;
    }
    fout.close();
    
}

void printEPH(Ephemeris* ep)
{
    cout << endl << "LNAV Ephemeris (slot = " << ep->slot << ") =" << endl;
    cout << "\t\t Crs	= " << ep->Crs <<  endl;
    cout << "\t\t Dn	= " << ep->Dn <<  endl;
    cout << "\t\t M0	= " << ep->M0 << "\t\t[deg]" << endl;
    cout << "\t\t Cuc	= " << ep->Cuc <<  endl;
    cout << "\t\t e	= " << ep->e <<  endl;
    cout << "\t\t Cus	= " << ep->Cus <<  endl;
    cout << "\t\t sqrtA	= " << ep->sqrtA <<  endl;
    cout << "\t\t toe	= " << ep->toe <<  endl;
    cout << "\t\t Cic	= " << ep->Cic <<  endl;
    cout << "\t\t Omega0	= " << ep->Omega0 << "\t\t[deg]" << endl;
    cout << "\t\t Cis	= " << ep->Cis <<  endl;
    cout << "\t\t i0	= " << ep->i0 << "\t\t[deg]" << endl;
    cout << "\t\t Crc	= " << ep->Crc <<  endl;
    cout << "\t\t omega	= " << ep->omega << "\t\t[deg]" << endl;
    cout << "\t\t omegaDot = " << ep->OmegaDot << "\t[deg/s]" << endl;
    cout << "\t\t iDot	= " << ep->iDot << "\t\t[deg/s]" << endl;
    cout << "\t\t Tgd	= " << ep->Tgd << "\t\t\t[sec]" <<  endl;
    cout << "\t\t toc	= " << ep->toc <<  endl;
    cout << "\t\t af2	= " << ep->af2 <<  endl;
    cout << "\t\t af1	= " << ep->af1 <<  endl;
    cout << "\t\t af0	= " << ep->af0 <<  endl;
    cout << "\t\t WN	= " << ep->WN <<  endl;
    cout << "\t\t IODC	= " << ep->IODC <<  endl;
    cout << "\t\t URA	= " << ep->URA <<  endl;
    cout << "\t\t Health	= " << ep->Health <<  endl;
    cout << "\t\t IODE2	= " << ep->IODE2 <<  endl;
    cout << "\t\t IODE3	= " << ep->IODE3 <<  endl;
    cout << "\t\t codeL2	= " << ep->codeL2 <<  endl;
    cout << "\t\t L2P	= " << ep->L2P <<  endl;

}

int32_t str2uint(string sf, int32_t Begin, int End) 
{
    int32_t  ans = 0;
    for (int i = Begin; i < End; i++) {
        bool bit = (sf[i - 1] == '1');
		ans = ans | bit;
        if (i < End-1){
            ans = ans<<1;
        }
    }
    return ans;
}

int32_t compl2int(uint32_t ans, int Lenght)
{
    int32_t Result = 0;
	int32_t Mask = 0;
    if (Lenght < 32){
        if (bool((1<<Lenght-1) & ans)){
            for (int i = 0; i < 32 - Lenght + 1; i++) {
				Mask |= 0x80000000 >> i;
			}
			ans |= Mask;
            Result = ~(ans-1);	
            return -Result;
        }
    }
    if (Lenght == 32){
        if (bool((1<<31) & ans)){
            Result = ~(ans-1);
            return -Result;
        }
    }
    return ans;
}

uint32_t splitstr2uint(string sf, uint16_t Begin, int End, uint16_t Contin, int End_of_Contin)
{
    uint32_t ans = 0;
    for (int i = Begin; i < End; i++) {
        bool bit = (sf[i - 1] == '1');
		ans = ans | bit;
		ans = ans<<1;
    }
    for (int i = Contin; i < End_of_Contin; i++) {
        bool bit = (sf[i - 1] == '1');
		ans = ans | bit;
        if (i < End_of_Contin-1){
            ans = ans<<1;	
        }
    }
    return ans;
}

void sf2eph(Ephemeris* ep, WorkingSub *data)
{	
	double deg2rad = pi / Semi_circles;
	
    ep->slot = data->slot;

    ep->Crs = compl2int(str2uint(data->sf2,69,85),16)*SF1;

    ep->Dn = compl2int(str2uint(data->sf2,91,107),16)*SF2*Semi_circles*deg2rad;

    ep->M0 = compl2int(splitstr2uint(data->sf2,107, 115, 121, 145),32)*SF3*Semi_circles*deg2rad;

    ep->Cuc = compl2int(str2uint(data->sf2,151,167),16)*SF4;

    ep->e = splitstr2uint(data->sf2,167, 175, 181, 205)*SF5;

    ep->Cus = compl2int(str2uint(data->sf2,211,227),16)*SF4;

    ep->sqrtA = splitstr2uint(data->sf2,227, 235, 241, 265)*SF6;

    ep->toe = str2uint(data->sf2,271,287)*SF7;

    ep->Cic = compl2int(str2uint(data->sf3,61,77),16)*SF4;

    ep->Omega0 = compl2int(splitstr2uint(data->sf3,77, 85, 91, 115),32)*SF3*Semi_circles*deg2rad;

    ep->Cis = compl2int(str2uint(data->sf3,121,137),16)*SF4;

    ep->i0 = compl2int(splitstr2uint(data->sf3,137, 145, 151, 175),32)*SF3*Semi_circles*deg2rad;

    ep->Crc = compl2int(str2uint(data->sf3,181,197),16)*SF1;

    ep->omega = compl2int(splitstr2uint(data->sf3,197, 205, 211, 235),32)*SF3*Semi_circles*deg2rad;

    ep->OmegaDot = compl2int(str2uint(data->sf3,241,265),24)*SF2*Semi_circles*deg2rad;

    ep->iDot = compl2int(str2uint(data->sf3,279,293),14)*SF2*Semi_circles*deg2rad;

    ep->Tgd = compl2int(str2uint(data->sf1,197,205),8)*SF3;

    ep->toc = compl2int(str2uint(data->sf1,219,235),16)*SF7;

    ep->af2 = compl2int(str2uint(data->sf1,241,249),8)*SF8;

    ep->af1 = compl2int(str2uint(data->sf1,249,265),16)*SF2;

    ep->af0 = compl2int(str2uint(data->sf1,271,293),22)*SF3;

    ep->WN = str2uint(data->sf1,61,71);

    ep->IODC = splitstr2uint(data->sf1,83, 85, 211, 219);

    ep->URA = str2uint(data->sf1,73,75);

    ep->Health = ep->IODE2 = str2uint(data->sf1,73,79);

    ep->IODE2 = str2uint(data->sf2,61,69);

    ep->IODE3 = str2uint(data->sf3,271,279);

    ep->codeL2 = bool (data->sf1[91]);

    ep->L2P = bool (data->sf1[90]);
}