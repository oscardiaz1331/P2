#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[]) {
    //Declaración de variables
    int   N,i,trm=0,correcto=0;
    float *x,*mitadventanaanterior=0,*w=0,powruido=0,fm,durTrm=0.010;
    short *buffer;
    FILE  *fpWave;
    FILE *fpOut=stdout;
   
 
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }
    if ((fpWave = abre_wave(argv[1], &fm)) == NULL) {
        fprintf(stderr, "Error al abrir el fichero WAVE de entrada %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }  
    //Si pasan dos parametros al ejecutar el segundo sera leido y 
    //guardado en fpOut para escribir en él. 
    if(argc==3){
        if((fpOut=fopen(argv[2],"w"))==NULL){
           fprintf(stderr, "Error en la lectura del nombre del archivo .txt\n");
                    return -1;
        }
    }
    N = durTrm * fm;  
    //Repartimos memoria en las variables.
    if ((buffer = malloc(N * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0 ||
        (mitadventanaanterior = malloc(N * sizeof(*mitadventanaanterior))) == 0||
        (w = malloc(2 * N * sizeof(*w))) == 0) {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno));
        return -1;
    }
    //Calculamos la ventana de Hamming y su potencia.
    for(i=0;i<2*N;i++){
        w[i]=0.53836-0.46164*cos(2*3.141592*i/(N-1));
        powruido+=w[i]*w[i];
    }
    //Escribimos en fpOut la potencia, amplitud y ZCR
    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) {
        for (int n = 0; n < N; n++) x[n] = buffer[n] / (float) (1 << 15);
            fprintf(fpOut,"%d \t%f\t%f\t%f\n",trm,compute_power_ampli(x,N,fm,mitadventanaanterior,w,powruido), //ampliación
                 //   fprintf(fpOut ,"%d \t%f\t%f\t%f\n",trm,compute_power(x,N), //ejercicio sin ampliación
                    compute_am(x, N),
                    compute_zcr(x, N, fm));
        trm += 1;
    }
    fclose(fpOut);
    cierra_wave(fpWave);
    free(buffer);
    free(x);
    free(mitadventanaanterior);
    free(w);
    return 0;
}
