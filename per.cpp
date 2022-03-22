#include <stdio.h>
#include <stdint.h >
#include <stdlib.h >
#include <string.h >
struct Epheneris
{
double 	Crs;
double 	Dn;
double 	MB;
double 	Cuc;
double	 e;
double 	Cus;
double   		sqrtA;
uint32_t 	 	toe;
double 			Cic;
double 			Omega0;
double 			Cis;
double 			io;
double 			Crc;
double 			omega;
double 			omeDot ;
double 			iDot ;
double 			Tgd ;
unit32_t  		toc;
double 			af2 ;
double 			af1 ;
double 			af0 ;
unit32_t  		WN;
unit16_t		IODC;
uint8_t			URA;
uint8_t			Health;
uint16_t		IODE2;
uint16_t		IODE3;
bool			codeL2;
bool			L2P;
unit32_t  		slot;
}; 

 const int32_t subFrameLenght = 300;
struct SF1_3{ 
	unit32_t slot;
	char sf1[subFrameLenght+1];
	char sf2[subFrameLenght+1];
	char sf3[subFrameLenght+1];
};


void printEph(Epheneris* ep);
int32_t file2subFrames(SF1_3* sf, FILE* fid, uint8_t svNum);
int32_t subFrames2Eph(Epheneris* ep,SF1_3* subFrames);

int main(void) 
{
	printf(" HeLLo world \n");
	uint8_t	svNum = 12;
	FILE* fid = fopen("in.txt", "r");
		if (!fid != nullptr){
			SF1_3 subFrames;
			if (fid && !file2subFrames(sf , fid , svNum))
			{
			printf("SF1: %s\n", subFrames.sf1);
			printf("SF2:%s\n", subFrames.sf2);
			printf("SF3:%s\n", subFrames.sf3);
			Ephemeris *ep= (Ephemeris*) calloc(1, sizeof(Ephemeris));
				if (!subFrames2Eph(ep, &subFrames))
				{
				printEph(ep);
				}else{
				 printf("Cannot decode subframes\n")
				}
			free(ep);
			}
			else
			{
			printf("Subframes not found\n");
			}
		}
		else
			{
			printf("cannot open\n");
			}
			
	return 0;
}


uint32_t str2uint(char* sf, int32_t start, int32_t stop){
	uint32_t ans =0;
	//70
	for (int i= start; i< stop; i++){
	bool bit = (sf[i-1] == '1');
	ans = ans | (bit << (stop - i - 1 ));
	}
	return ans;	
}
int32_t subFrames2Eph(Epheneris* ep,SF1_3* subFrames) {
ep->slot = subFrames-> slot;
ep->WN = str2uint(subFrames->sf1, 61, 71);
return 0;
}

int32_t file2subFrames(SF1_3* sf, FILE* fid, uint8_t svNum){
  int32_t sth1, sth2 , sth3;
  char str_OR[8];
  char str_GpsL1CA[12];
  char str_reh[8];   
  char str[1000];
  uint32_t svStr;
  uint32_t slot;
  uint32_t subFrameNum;
  
   uint32_t slot_SF1 = 0;
   uint32_t slot_SF2 = 0;
   uint32_t slot_SF3 = 0;
   
   int32_t readres = 0;
 
  
  while (readres != EOF)
  {
  printf("String %d\n", i);
  svStr = 0;
  readres = fscanf(fid,"%d %d %d %s %s %s %u\t %u %d %d %d %s", &sth1, &sth2, &sth3, sth_OR, sth_Gpsl1CA, sth_reh, svStr,
		slot,&sth4,&sth5,&subFrameNum, str );
 if (((svStr == svNum) && (slot>= 604800/6))){
	if (subFrameNum == 1)
		{
		slot_SF1= slot;
		strncpy(sf-> sf1, str);
	} else if (subFrameNum == 2)
				{
				slot_SF2 = slot;
				strncpy(sf-> sf2, str);
				
	}else if (subFrameNum == 3)
				{
				slot_SF3 = slot;
				strncpy(sf-> sf3, str);
				}
	if ((slot_SF1 + 1 == slot_SF2) && (slot_SF2 + 1 == slot_SF3)){
	sf->slot = slot_SF1;
		return 0;			
	}

	
 
  printf("sth1 =%d\n",sth1);
  printf("str =%s\n",str);
   }
	return 1;
}

	void printEph(Epheneris* ep);
		{
		printf(" 	LNAV Ephemeris (slot = %u) = \n", 	ep->slot);
		printf("	\t Crs	= %f 				\n",	ep->Crs	);
		printf(" 	\t Dn	= %f \t[deg/s]		\n",	ep->Dn	);
		printf(" 	\t MB	= %f \t[deg]		\n",	ep->MB	);
		printf(" 	\t Cuc	= %f				\n",	ep->Cuc	);
		printf("	\t e	= %f				\n",	ep->e	);
		printf(" 	\t Cus	= %f				\n",	ep->Cus	);
		printf("   	\t sqrtA	= %f			\n",	ep->sqrtA );
		printf("	\t toe	= %f				\n",	ep->toe	);	
		printf(" 	\t Cic	= %f				\n",	ep->Cic	;
		printf(" 	\t Omega0	= %f \t [deg]	\n",	ep->Omega0	);
		printf("	\t Cis	= %f				\n",	ep->Cis	);	
		printf(" 	\t io	= %f \t[deg]		\n",	ep->io	);
		printf("	\t Crc	= %f				\n",	ep->Crc	);
		printf("	\t omega	= %f \t[deg]	\n",	ep->omega	);
		printf(" 	\t omeDot 	= %f \t[deg/s]	\n",	ep->omeDot 	);
		printf(" 	\t iDot 	= %f \t[deg/s]	\n",	ep->iDot 	);
		printf(" 	\t Tgd	= %f \t[deg/s]		\n",	ep->Tgd	 );
		printf(" 	\t toc	= %f				\n",	ep->toc	);
		printf("	\t af2 	= %f				\n",	ep->af2 	);
		printf(" 	\t af1	= %f				\n",	ep->af1	 );
		printf(" 	\t af0 	= %f				\n",	ep->af0 	);
		printf("  	\t WN	= %f				\n",	ep->WN	);
		printf("	\t IODC	= %f				\n",	ep->IODC	);
		printf("	\t URA	= %f				\n",	ep->URA	);
		printf("	\t Health	= %f			\n",	ep->Health	);
		printf("	\t IODE2	= %f			\n",	ep->IODE2	);
		printf("	\t IODE3	= %f			\n",	ep->IODE3	);
		printf("	\t codeL2	= %f			\n",	ep->codeL2	);
		printf("	\t L2P	= %f				\n",	ep->L2P	);
		}