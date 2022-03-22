#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct Ephemeris {
	double   Crs;
	double   Dn;
	double   M0;
	double   Cuc;
	double   e;
	double   Cus;
	double   sqrtA;
	uint32_t toe;
	double   Cic;
	double   Omega0;
	double   Cis;
	double   i0;
	double   Crc;
	double   omega;
	double   OmegaDot;
	double   iDot;
	double   Tgd;
	uint32_t toc;
	double   af2;
	double   af1;
	double   af0;
	uint32_t WN;
	uint16_t IODC;
	uint8_t  URA;
	uint8_t  Health;
	uint16_t IODE2;
	uint16_t IODE3;
	bool     codeL2;
	bool     L2P;
	uint32_t slot;
};
const int32_t subFrameLength = 300;
struct SF1_3 {
	uint32_t slot;
	char sf1[subFrameLength+1];
	char sf2[subFrameLength+1];
	char sf3[subFrameLength+1];
};
void printEmp(Ephemeris* ep);
int32_t file2subFrames(SF1_3* sf, FILE* fid, uint8_t svNum);
int32_t subFrames2Eph(Ephemeris* ep, SF1_3* subframes);


int main(void)
{
	printf(" Hello, World \n");
	uint8_t svNum = 3;
	FILE* fid = fopen("in.txt", "r");
	if (fid != nullptr) {
		SF1_3 subframes;
		if (!file2subFrames(&subframes, fid, svNum)) {
			
			Ephemeris *ep = (Ephemeris*) calloc(1, sizeof(Ephemeris));
			if (!subFrames2Eph(ep, &subframes)) {
			printEmp(ep);
			
			} else {
				printf(" Cannot decode subframes\n ");
			}
			free(ep);
			fclose(fid);
		}
		else {
			printf(" Subframes not found\n ");
		}
	}	
	else {
		printf(" Cannot open in.txt ");
	}
		
	
	return 0;
}
uint32_t str2uint(char *sf, int32_t start, int32_t stop) {
	uint32_t ans = 0;
	for(int i = start; i < stop; i++) {
		bool bit = (sf[i-1] == '1');
		ans = ans | (bit << (stop - i - 1));
		
	}
	return ans;
	
	
	
	
}
int32_t subFrames2Eph(Ephemeris* ep, SF1_3* subframes) {
	ep->slot = subframes->slot;
	ep->WN = str2uint(subframes->sf1, 61, 71);
	ep->toe = str2uint(subframes->sf2, 271, 287);
	ep->toc = str2uint(subframes->sf1, 219, 219+16);
	ep->IODC = str2uint(subframes->sf1, 83, 93);
	ep->URA = str2uint(subframes->sf1, 73, 77);
	ep->Health = str2uint(subframes->sf1, 77, 83);
	ep->IODE2 = str2uint(subframes->sf2, 61, 69);
	ep->IODE3 = str2uint(subframes->sf3, 219, 219+16);
	ep->codeL2 = str2uint(subframes->sf1, 71, 73);
	ep->L2P = str2uint(subframes->sf1, 91, 92);
	return 0;
	
	
	
	
	
}
int32_t file2subFrames(SF1_3* sf, FILE* fid, uint8_t svNum){
	int32_t sth1, sth2, sth3, sth4, sth5;
	char str_0R[8];
	char str_GPSL1CA[12];
	char str_reh[8];
	char str[1000];
	uint32_t svStr;
	uint32_t slot;
	int32_t subFrameNum;
	
	uint32_t slot_SF1 = 0;
	uint32_t slot_SF2 = 0;
	uint32_t slot_SF3 = 0;
	int32_t readres = 0;
	
	while(readres != EOF)
		{
		svStr = 0;
		readres = fscanf( fid, "%d %d %d %s %s %s %u\t %u %d %d %d %s", &sth1, &sth2, &sth3, str_0R, str_GPSL1CA, str_reh, &svStr, &slot, &sth4, &sth5, &subFrameNum, str);
		if (( svStr == svNum ) && (slot >= (604800/6))) {
			if ( subFrameNum == 1 ) {
				slot_SF1 = slot;
				strncpy(sf->sf1, str, sizeof(sf->sf1));
			}
			else if (subFrameNum == 2) {
				slot_SF2 = slot;
				strncpy(sf->sf2, str, sizeof(sf->sf2));
			}
			else if (subFrameNum == 3) {
				slot_SF3 = slot;
				strncpy(sf->sf3, str, sizeof(sf->sf3));
			}
			if ((slot_SF1 + 1 == slot_SF2) && (slot_SF2 + 1 == slot_SF3)) {
				sf->slot = slot_SF1;
				return 0;
			}
		}
			
				
			
				
			
		
	}
	return 1;
}	
void printEmp(Ephemeris* ep)
{
	printf("LNAV Ephemeris (slot = %u) =  \n",  ep->slot      );
	printf("\tCrs     = %f                \n",  ep->Crs       );
	printf("\tDn      = %f \t[deg/s]      \n",  ep->Dn        );
	printf("\tM0      = %f \t[deg]        \n",  ep->M0        );
	printf("\tCuc     = %f                \n",  ep->Cuc       );
	printf("\te       = %f                \n",  ep->e         );
	printf("\tCus     = %f                \n",  ep->Cus       );
	printf("\tsqrtA   = %f                \n",  ep->sqrtA     );
	printf("\ttoe     = %u                \n",  ep->toe       );
	printf("\tCic     = %f                \n",  ep->Cic       );
	printf("\tOmega0  = %f \t[deg]        \n",  ep->Omega0    );
	printf("\tCis     = %f                \n",  ep->Cis       );
	printf("\ti0      = %f \t[deg]        \n",  ep->i0        );
	printf("\tCrc     = %f                \n",  ep->Crc       );
	printf("\tomega   = %f \t[deg]        \n",  ep->omega     );
	printf("\tOmegaDot= %f \t[deg/s]      \n",  ep->OmegaDot  );
	printf("\tiDot    = %f \t[deg/s]      \n",  ep->iDot      );
	printf("\tTgd     = %f                \n",  ep->Tgd       );
	printf("\ttoc     = %u                \n",  ep->toc       );
	printf("\taf2     = %f                \n",  ep->af2       );
	printf("\taf1     = %f                \n",  ep->af1       );
	printf("\taf0     = %f                \n",  ep->af0       );
	printf("\tWN      = %u                \n",  ep->WN        );
	printf("\tIODC    = %u                \n",  ep->IODC      );
	printf("\tURA     = %u                \n",  ep->URA       );
	printf("\tHealth  = %u                \n",  ep->Health    );
	printf("\tIODE2   = %u                \n",  ep->IODE2     );
	printf("\tIODE3   = %u                \n",  ep->IODE3     );
	printf("\tcodeL2  = %u                \n",  ep->codeL2    );
	printf("\tL2P     = %u                \n",  ep->L2P       );
}	                                 
