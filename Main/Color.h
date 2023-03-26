class Color{

  private:
    const int pinBoton = 4;
    const int sig[3] = {A0,A1,A2}; // pines signal: DERECHA, PRINCIPAL, IZQUIERDA  
    const int sC[3] = {49,5,38};  // pines de 'control' ABC 
    const int sB[3] = {12,2,43};
    const int sA[3] = {47,3,42};
    int fotoMinV[3][8] = {{2,6,7,8,3,6,5,8},{0,1,1,3,4,0,3,8},{1,7,10,6,2,6,1,10}};
    int fotoMaxV[3][8] = {{4,8,9,10,5,8,8,10},{1,3,4,5,5,0,5,11},{6,13,16,12,5,10,8,16}};
    int fotoMinB[3][8] = {{27,50,43,35,32,41,43,47},{19,44,35,58,56,100,52,58},{25,49,52,49,31,53,45,69}};
    int fotoMaxB[3][8] = {{30,53,46,38,35,44,46,50},{22,47,38,62,58,100,55,60},{33,56,63,56,36,60,52,79}};

    int lectura(int ft, int placa){   // 1: derecha (1,3,4)
      switch (ft){
        case 0: binario(0,placa); return analogRead(sig[placa]); break;
        case 1: binario(1,placa); return analogRead(sig[placa]); break;
        case 2: binario(2,placa); return analogRead(sig[placa]); break;
        case 3: binario(3,placa); return analogRead(sig[placa]); break;
        case 4: binario(4,placa); return analogRead(sig[placa]); break;
        case 5: binario(5,placa); return analogRead(sig[placa]); break;
        case 6: binario(6,placa); return analogRead(sig[placa]); break;
        case 7: binario(7,placa); return analogRead(sig[placa]); break;
      }
    }

    void binario(int pin, int lado){
      switch (pin){
        case 0: 
          digitalWrite(sC[lado],0); 
          digitalWrite(sB[lado],0); 
          digitalWrite(sA[lado],0); break;
        case 1:
          digitalWrite(sC[lado],0); 
          digitalWrite(sB[lado],0); 
          digitalWrite(sA[lado],1); break;
        case 2:
          digitalWrite(sC[lado],0); 
          digitalWrite(sB[lado],1); 
          digitalWrite(sA[lado],0); break;
        case 3:
          digitalWrite(sC[lado],0); 
          digitalWrite(sB[lado],1); 
          digitalWrite(sA[lado],1); break;
        case 4: 
          digitalWrite(sC[lado],1); 
          digitalWrite(sB[lado],0); 
          digitalWrite(sA[lado],0); break;
        case 5: 
          digitalWrite(sC[lado],1); 
          digitalWrite(sB[lado],0); 
          digitalWrite(sA[lado],1); break;
        case 6: 
          digitalWrite(sC[lado],1); 
          digitalWrite(sB[lado],1); 
          digitalWrite(sA[lado],0); break;
        case 7: 
          digitalWrite(sC[lado],1); 
          digitalWrite(sB[lado],1); 
          digitalWrite(sA[lado],1); break;
      }
    }



  // -------------------------------------------------------------------




    public:

    Color(){}

    void iniciar(){
      for (int i=0; i<3; i++){  
        pinMode(sC[i],OUTPUT);
        pinMode(sB[i],OUTPUT);
        pinMode(sA[i],OUTPUT);
      }
    }


    void calibrar(){
        for (int i=0; i<3; i++){   // calibrar VERDE
          for (int j=0; j<8; j++){
            for (int k=0; k<1000; k++){
              int lec = lectura(j,i);
              if (lec < fotoMinV[i][j]){
                fotoMinV[i][j] = lec;
              }
              else if (lec > fotoMaxV[i][j]){
                fotoMaxV[i][j] = lec;
              }
            }
          }
        }

        for (int i=0; i<3; i++){   // calibrar BLANCO
          for (int j=0; j<8; j++){
            for (int k=0; k<1000; k++){
              int lec = lectura(j,i);
              if (lec < fotoMinB[i][j]){
                fotoMinB[i][j] = lec;
              }
              else if (lec > fotoMaxB[i][j]){
                fotoMaxB[i][j] = lec;
              }
            }
          }
          while (digitalRead(pinBoton) == 1){}
        }

    }


    bool checkForLinea(){
      for (int i=0; i<8; i++){
        for (int j=0; j<3; j++){
          if (lectura(i,j) >= fotoMinB[j][i] && lectura(i,j) <= fotoMaxB[j][i]){
            Serial.println("Jaló");
            return true;
          }
        }
      }
      return false;
    }

};
