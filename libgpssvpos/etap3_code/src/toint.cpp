#include "../header/head.h"

int32_t compl2int(uint32_t retrn, int dlit_frame){

    int32_t Rretrn = 0;
    if (dlit_frame == 8){
        if (bool((1<<7) & retrn)){
            retrn |= 0xFFFFFF00;
            Rretrn = ~(retrn-1);
            return -Rretrn;
        }
    }
    if (dlit_frame == 14){
        if (bool((1<<13) & retrn)){
            retrn |= 0xFFFFC000;
            Rretrn = ~(retrn-1);
            return -Rretrn;
        }
    }
    if (dlit_frame == 16){
        if (bool((1<<15) & retrn)){
            retrn |= 0xFFFF0000;
            Rretrn = ~(retrn-1);
            return -Rretrn;
        }
    }
    if (dlit_frame == 22){
        if (bool((1<<21) & retrn)){
            retrn |= 0xFFC00000;
            Rretrn = ~(retrn-1);
            return -Rretrn;
        }
    }
    if (dlit_frame == 24){
        if (bool((1<<23) & retrn)){
            retrn |= 0xFF000000;
            Rretrn = ~(retrn-1);
            return -Rretrn;
        }
    }
    if (dlit_frame == 32){
        if (bool((1<<31) & retrn)){
            retrn |= 0x00000000;
            Rretrn = ~(retrn-1);
            return -Rretrn;
        }
    }
    return retrn;
}

uint32_t gruppirovka(std::string Cnst, uint16_t start1, int dlit1, uint16_t start2, int dlit2) {
    uint32_t retrn = 0;
    for (int i = start1; i < start1+dlit1; i++) {
        retrn = (retrn | ((Cnst[i-1] == '1')? 1 : 0)) << 1;
    }
    for (int i = start2; i < start2+dlit2; i++) {
        retrn = retrn | ((Cnst[i-1] == '1')? 1 : 0);
        if (i < start2+dlit2-1){
            retrn = retrn<<1;
        }
    }
    return retrn;
}

void razobrat(Ephemeris* Eph, frame *pack){
    Eph->pack = pack->pack_slot;
    Eph->Crs = compl2int(data(pack->Cnst2,69,16),16)*Cnst_2E5;
    Eph->Dn = compl2int(data(pack->Cnst2,91,16),16)*Cnst_2E43*SC;
    Eph->M0 = compl2int(gruppirovka(pack->Cnst2,107, 8, 121, 24),32)*Cnst_2E31*SC;
    Eph->Cuc = compl2int(data(pack->Cnst2,151,16),16)*Cnst_2E29;
    Eph->e = gruppirovka(pack->Cnst2,167, 8, 181, 24) * Cnst_2E33;
    Eph->Cus = compl2int(data(pack->Cnst2,211,16),16)*Cnst_2E29;
    Eph->sqrtA = gruppirovka(pack->Cnst2,227, 8, 241, 24) * Cnst_2E19;
    Eph->toe = data(pack->Cnst2,271,16)*pow(2,4);
    Eph->Cic = compl2int(data(pack->Cnst3,61,16),16)*Cnst_2E29;
    Eph->Omega0 = compl2int(gruppirovka(pack->Cnst3,77, 8, 91, 24),32)*Cnst_2E31*SC;
    Eph->Cis = compl2int(data(pack->Cnst3,121,16),16)*Cnst_2E29;
    Eph->i0 = compl2int(gruppirovka(pack->Cnst3,137, 8, 151, 24),32)*Cnst_2E31*SC;
    Eph->Crc = compl2int(data(pack->Cnst3,181,16),16)*Cnst_2E5;
    Eph->omega = compl2int(gruppirovka(pack->Cnst3,197, 8, 211, 24),32)*Cnst_2E31*SC;
    Eph->OmegaDot = compl2int(data(pack->Cnst3,241,24),24)*Cnst_2E43*SC;
    Eph->iDot = compl2int(data(pack->Cnst3,279,14),14)*Cnst_2E43*SC;
    Eph->Tgd = compl2int(data(pack->Cnst1,197,8),8)*Cnst_2E31;
    Eph->toc = compl2int(data(pack->Cnst1,219,16),16)*Cnst_2E4;
    Eph->af2 = compl2int(data(pack->Cnst1,241,8),8)*Cnst_2E55;
    Eph->af1 = compl2int(data(pack->Cnst1,249,16),16)*Cnst_2E43;
    Eph->af0 = compl2int(data(pack->Cnst1,271,22),22)*Cnst_2E31;
    Eph->WN = data(pack->Cnst1,61,10);
    Eph->IODC = gruppirovka(pack->Cnst1,83, 2, 211, 8);
    Eph->URA = data(pack->Cnst1,73,4);
    Eph->Health = Eph->IODE2 = data(pack->Cnst1,73,6);
    Eph->IODE2 = data(pack->Cnst2,61,8);
    Eph->IODE3 = data(pack->Cnst3,271,8);
    Eph->codeL2 = data(pack->Cnst1,71,2);
    Eph->L2P = pack->Cnst1[90];
}
