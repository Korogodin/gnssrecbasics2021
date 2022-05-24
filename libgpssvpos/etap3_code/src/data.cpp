#include "../header/head.h"

int32_t data(std::string Cnst, int32_t start, int dlit) {
    int32_t retrn = 0;
    int32_t Rretrn = 0;
    for (int i = start; i < start+dlit; i++) {
        retrn = (retrn | ((Cnst[i - 1] == '1') ? 1 : 0));
        if (i < start+dlit-1){
            retrn = retrn<<1;
        }
    }
    return retrn;
}
