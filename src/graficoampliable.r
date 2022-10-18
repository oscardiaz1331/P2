 frecm=16000
 Tlong=0.02
 Tdesp=Tlong/2
 variableint=Tdesp*100
 N=frecm*0.01;
 j=0;
 i=0;
 trm=1
 resultado<-c()
 while(j<15){
   i=0;
   while(i<frecm*Tlong){
     resultado[trm]<-i+j*frecm*variableint/100;
     i=1+i;
     trm=trm+1;
   }
   j=j+1;
 }
 
plot(1:length(resultado)-1,resultado,xlab="Quince ventanas de 0.02s en # muestras",ylab="Posición de las muestras de las ventanas")
length(resultado)
Tdesp*frecm
frecm*Tlong
