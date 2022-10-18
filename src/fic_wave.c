#include <stdio.h>
#include <stdint.h>
#include "fic_wave.h"

FILE    *abre_wave(const char *ficWave, float *fm) {
    FILE    *fpWave;
    //Creamos las variables con el tipo de int que deben tener
    int32_t bits;
    int16_t canales,pcm;
    int64_t tmp;
    //Con fseek nos movemos el lugar que se inspecciona del archivo 
    //Con fread leemos el archivo que se da como último parámetro
    //y colocamos la información de longitud segundo parámetro en la variable
    //del primer parámetro
    if ((fpWave = fopen(ficWave, "r")) == NULL) return NULL;
    if (fseek(fpWave, 44, SEEK_SET) < 0) return NULL;
    fseek(fpWave,24,SEEK_SET); //Posición 24 es la de fm
    fread(&tmp,sizeof(float),1,fpWave); //Es del tamaño de un float y solo queremos leer una vez
    //Principio ampliación
    fseek(fpWave,16,SEEK_SET); //Posición 16 es la del núm de bits
    fread(&bits,sizeof(int),1,fpWave);//Tamaño de un int de 32
    fseek(fpWave,20,SEEK_SET); //Posición 20 es la de si es pcm o no
    fread(&pcm,sizeof(short),1,fpWave);//Tamaño de short, 16
    fseek(fpWave,22,SEEK_SET); //Posición del número de canales
    fread(&canales,sizeof(short),1,fpWave); //Tamaño de un short
    if(bits==16 && canales==1 && pcm==1){ //Miramos que las 3 condiciones se cumplan
    }else{//Si no se cumplen mandamos una señal de error y paramos el programa.
        fprintf(stderr,"La señal no es PCM-16 bits o no es mono\n");
        return NULL;
    }
    //Final ampliación
    *fm=tmp;
    fseek(fpWave,44,SEEK_SET);   //Colocamos el lugar de lectura en los datos.
    return fpWave;
}

size_t   lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave) {
    return fread(x, size, nmemb, fpWave);
}

void    cierra_wave(FILE *fpWave) {
    fclose(fpWave);
}
