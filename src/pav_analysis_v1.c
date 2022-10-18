#include <math.h>
#include "pav_analysis.h"
#include <stdlib.h>
#include <stdio.h>


float compute_power(const float *x, unsigned int N) {
         float tmp=0; 
        int i=0;
        for(i=0;i<N;i++){
            tmp+=x[i]*x[i];
        }
       tmp=tmp/N;
    return 10*log10(tmp);
}
float compute_power_ampli(const float *x, unsigned int N,float fm) {
    float tmp=0, Tlong=0.02,Tdesp=Tlong/2;
    float pi=0; 
    int frecm=fm;
    int i=0,j=0;
    float *w=0,w_2=0;
    w=(float*)calloc(frecm*Tlong,sizeof(float));
   
    //Creación de la ventana y la suma de sus cuadrados.
        for(i=0;i<frecm*Tlong;i++){
            w[i]=0.53836-0.46164*cos(2*(3141592/1000000)*i/(frecm*Tlong-1));
            w_2+=w[i]*w[i];
        }
      int variableint=Tdesp*100;
    //Obtención de cada Pdb[i] de la fórmula del pdf p1_2
        for(j=0;j*frecm*Tlong<N;j++){
            tmp=0;
            for(i=0;i<frecm*Tlong;i++){
                tmp+=(x[i+j*frecm*variableint/100]*w[i])*(x[i+j*frecm*variableint/100]*w[i]);
            }
            tmp=tmp/w_2; 
            pi=10*log10(tmp);
        }
        
      
    return pi;
}


float compute_am(const float *x, unsigned int N) {
        float tmp=0;
        int i=0;
        for(i=0;i<N;i++){
            if(x[i]<0){
                tmp+=x[i]*-1;
            }else{
                tmp+=x[i];
            }
        }
        tmp=tmp/N;
    return tmp;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
        float tmp=(1/(N-1))*fm/2;
        int i=0;
        for(i=1;i<N;i++){
            if((x[i]>=0 && x[i-1]<0)||(x[i]<0&&x[i-1]>=0)){
                tmp+=1;
            }
        } 
    return tmp;
}
