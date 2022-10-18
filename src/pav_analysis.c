#include <math.h>
#include "pav_analysis.h"
#include <stdlib.h>


float compute_power(const float *x, unsigned int N) {
        float pwr=1e-12; 
        int i=0;
        for(i=0;i<N;i++){ //para la longitud de nuestra ventana
            pwr+=x[i]*x[i]; //Hacemos el cuadrado de la señal
        }
        pwr=pwr/N;
    return 10*log10(pwr); //devolvemos el resultado en dB normalizado por la longitud de la ventana
}
float compute_power_ampli(const float *x, unsigned int N,float fm,float mitadenventanadaanterior[],const float *w,float powruido) {
        float pwr=1e-12;
        int i=0; //Nos pasan por parametros la ventana de Hamming, su potencia y la primera mitad de la trama.
        for(i=0;i<N;i++){ //Para la primera mitad de la trama de longitud N
            pwr+=(mitadenventanadaanterior[i]*w[i])*(mitadenventanadaanterior[i]*w[i]); //Calculamos la potencia de la primera mitad de la trama pasandola por la ventana
        }
        for(i=0;i<N;i++){ //Para la segunda mitad de la trama de longitud N
            pwr +=(x[i]*w[N+i])*(x[i]*w[N+i]); //Calculamos la potencia de esta mitad de la trama pasandola por la ventana
            mitadenventanadaanterior[i]=x[i];  //Asignamos los valores de la segunda mitad de la trama a la primera mitad de la siguiente trama
        }
        pwr=pwr/powruido;
        return 10*log10(pwr); //Devolvemos el resultado en dB normalizandolo por la potencia del ruido 
}


float compute_am(const float *x, unsigned int N) {
        float amp=0;
        int i=0;
        for(i=0;i<N;i++){//Para la longitud de la ventana
        //Hacemos el sumatorio de la señal en valor absoluto
            if(x[i]<0){ //Si es menor que 0 la multiplicamos por -1 para hacerla positiva
                amp+=x[i]*-1;
            }else{
                amp+=x[i]; //Mayor que 0 se queda igual
            }
        }
        amp=amp/N;
    return amp; //Devolvemos el valor normalizado
}

float compute_zcr(const float *x, unsigned int N, float fm) {
        float zcr=0;
        int i=0;
        for(i=1;i<N;i++){ //Para la longitud de la ventana
            if((x[i]>=0 && x[i-1]<0)||(x[i]<0&&x[i-1]>=0)){ 
                //Si el valor de esta muestra es de diferente signo que la muestra anterior, sumamos uno al zcr
                zcr+=1; 
            }
        } 
    return zcr*fm/(2*(N-1)); //Devolvemos el valor normalizado a la fm
}
