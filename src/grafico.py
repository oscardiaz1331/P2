import matplotlib.pyplot as plt
frecm=16000
Tlong=0.02
Tdesp=Tlong/2
variableint=Tdesp*100
variableint2=Tlong*100
N=frecm*0.01
trm=1
numtramas=15
posicionmuestra=[None]*int(2*N*numtramas)
print(N*numtramas)
for i in range(0,numtramas):
    for j in range(0,int(frecm*Tlong-1)):
        posicionmuestra[trm]=j+i*frecm*Tdesp
        trm+=1
plt.plot(range(1,len(posicionmuestra)+1),posicionmuestra)
plt.xlabel("Quince ventanas de 0.02s en # muestras")
plt.ylabel("Posición de las muestras de las ventanas")
plt.show()