#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {
    float p = 0.0;
    int i = 0;
    for(i=0; i<N; i++){
        p = p + x[i]*x[i];
    }
    return 10*log10f(p/N);
}

float compute_am(const float *x, unsigned int N) {
    float a = 0.0;
    int i = 0;
    for(i=0; i<N; i++){
        if(x[i]>=0){
            a = a + x[i];
        }
        else{
            a = a - x[i];
        }
    }
    return a/N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    float zcr = 0.0;
    int i = 0;
    for(i=1; i<N; i++){
        if((x[i]>=0 && x[i-1]<0) || (x[i]<0 && x[i-1]>=0)){
            zcr++;
        }
    }
    return fm/(2*(N-1))*zcr;
}
