/*  Argint Adrian-Costel
    313 CB */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void harta( unsigned long long H ) // Afiseaza harta.
{
  int i, x;
  for (i = 63; i >= 0; i--){
    x = H >> i;
    if (x & 1)
      printf("#");
    else
      printf(".");
    if (i % 8 == 0)
      printf ("\n");
  }
  printf ("\n");
}

int verificareStanga (unsigned long long P){ //verif mutare st/dr
  unsigned long long v;
  v = 9259542123273814144UL; //numar coloana stanga 1 
  if( (v & P) > 0 )  
    return 1;
  else
    return 0;
}

int verificareDreapta (unsigned long long P){ //verif mutare st/dr
  unsigned long long v;
  v = 72340172838076673UL; // numar coloana dreapta 1
  if ( (v & P) > 0 )
    return 1;
  else
    return 0;
}

unsigned long long eliminare( unsigned long long H ) // Elimina liniile complete
{
  int i, c = 0;
  unsigned long long mask, maskH, x;
  for (i = 63; i >= 0; i--)
  {
    x = H >> i;
    if (x & 1){
      c++;
    }
    if (i % 8 == 0){
      if(c == 8 && i > 7){
        mask = (H << (64 - i));
        maskH = H >> (i + 8);
        H = (maskH << i) | (mask >> (64 - i));
        break;
      }
      else {
        if(c == 8){
          H = H >> 8;
        }
      }
      c = 0;
    }
  }
  return H;
}

int zerouri( unsigned long long H ) //  Numara zerourile.
{
  int i, bit, c = 0;
  for (i = 63; i >= 0; i--){
    bit = H >> i;
    if ((bit & 1 )== 0)
      c++; // Contor 
  }
  return c;
}

int main ()
{
  unsigned long long H, maskP, copieH;
  long long v[100][9];
  double scor;
  int M, i, j, k, ok, c = 0, liniiComplete = 0;
  scanf("%llu", &H); //Introducem harta 
  scanf("%d", &M);  //Introducem nr de piese
  for (i = 0; i < M; i++)
    for (j = 0; j < 9; j++)
      scanf("%lld", &v[i][j]); // Se salveaza intr-o matrice de M linii si 9 coloane piesa + 8 mutari
  harta(H);
  for (i = 0; i < M; i++){ //fiecare piesa trece pe primul rand al hartii
    maskP = v[i][0] << 56; //Salvam prima linie din piesa 
    if (v[i][0]>128) //Piesa mai mare de o linie
      c = 1;
    for (j = 1; j < 9; j++){ // piesa de la 1 la 8
      ok = 1; //ok este variabila conditie, daca piesa are loc pe harta 
      if (v[i][j] == 0 && (maskP & H) == 0) { //fara mutare stanga/dreapta 
        copieH = H | maskP; // se face adaugarea piesei
        harta(copieH);
        ok = 0; 
      }

      if (v[i][j] > 0 ){ //mutare la dreapta
        for (k = 1; k <=v[i][j]; k++){
          if ( (maskP & H) == 0){
            if((verificareDreapta(maskP) == 0) && (((maskP >> 1) & H) == 0)){
              maskP = maskP >> 1;
              v[i][0] = v[i][0] >> 1;
              ok = 0;
            }
            else {
                ok = 0;
            }
          }
          else
            break;
        }
        if(ok == 0){ 
          copieH = H | maskP;// se face adaugarea piesei
          harta(copieH);
        }
        else{
          break;
        }
      }

      if (v[i][j] < 0 ){ //mutare la stanga
        v[i][j] = abs(v[i][j]);
        for (k = 1; k <= v[i][j]; k++){
          if ( (maskP &  H) == 0){
            if( ((maskP << 1) & H) == 0 && (verificareStanga(maskP) == 0) ){
              maskP = maskP << 1; //Se muta cate o data atat mask 
              v[i][0] = v[i][0] << 1; //cat si copia piesa efectiv
              ok = 0; 
            }
            else{
              ok = 0;
            }
          }
          else
            break;
        }
        if(ok ==0){
          copieH = H | maskP; // se face adaugarea piesei
          harta(copieH);
        }
        else{
          break;
        }
      }

      if(c){ //Daca e de 2 cifre, dupa prima mutare, se afiseaza toata piesa
        if ( ((maskP >> 8) & H )== 0 ){
        maskP = v[i][0]<< 48;
        c--;
        }
        else{
          break;
        }
      }
      else
        maskP = maskP >> 8;

      if (ok){
        break;
      }
    }
    if(H == copieH){ // Conditie pentru a verifica daca exista modificari
      harta(H);      // Astfel, daca nu exista modificari, programul se opreste
      break;
    }
    else{
      H = copieH;// Altfel, harta salveaza modificarile
    }
    if (c){ // verifica daca piesa are loc pe harta
      harta(H);
      c--;
      break;
    }
    if(H != eliminare(H)){ // Daca exista linii complete, acestea se vor elimina
      while (H != eliminare(H)){
        H = eliminare(H);
        liniiComplete ++; // Contorul pentru linii complete creste
      }
    harta(H);
    }
  }
  scor = sqrt(zerouri(H)) + pow(1.25 , liniiComplete);
  printf("GAME OVER!\nScore:%.2lf", scor);
  return 0;
}