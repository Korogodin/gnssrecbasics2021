#include "../header/head.h"

void otpravit(frame *Rpack)
{
    std::string path = "in.txt";
    std::ifstream fin;
    fin.open(path);
    while (!fin.eof()) {
        int N = 3;
        std::string empty;
        int nmbr_stlt;
        uint32_t pack;
        uint32_t frameFrameNum;
        std::string str;
        uint32_t pack_Cnst1;
        uint32_t pack_Cnst2;
        uint32_t pack_Cnst3;
        int mass[N];
        for(int i=0;i<N;++i)
        {
            fin >> mass[i];
        }
        fin >> empty >> empty >> empty;//
        fin >> nmbr_stlt >> pack >> empty >> empty >> frameFrameNum;

        fin >> str;
        if (nmbr_stlt == 17 and pack>= 604800/6){
            if (frameFrameNum == 1)
            {
                pack_Cnst1 = pack;
                Rpack->Cnst1 = str;
            }
            else if (frameFrameNum == 2)
            {
                pack_Cnst2 = pack;
                Rpack->Cnst2 = str;
            }
            else if (frameFrameNum == 3)
            {
                pack_Cnst3 = pack;
                Rpack->Cnst3 = str;
            }
            if (pack_Cnst1 + 1 == pack_Cnst2 and pack_Cnst2 + 1 == pack_Cnst3) {
                Rpack->pack_slot = pack_Cnst1;
                return;
            }
        }
    }
    fin.close();
}

