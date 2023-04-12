#ifndef Color_h
  #define Color_h
  
  class Color {
  
    private:
      const int pinLED = 52;
      const int pinBoton = 7;
      int proportion = 2.5; //BNO

  
      //ROBOT BNO
      const int sig[3] = {A0, A1, A15}; // pines signal: DERECHA, PRINCIPAL, IZQUIERDA
      const int sC[3] = {8, 13, 53}; // pines de 'control' ABC
      const int sB[3] = {9, 11, 50}; //robot bno
      const int sA[3] = {10, 12, 51};
  
   
      //ROBOT IMU
  //    const int sig[3] = {A0, A1, A15}; // pines signal: DERECHA, PRINCIPAL, IZQUIERDA
  //    const int sC[3] = {8, 12, 53}; // pines de 'control' ABC
  //    const int sB[3] = {9, 11, 49}; //robot imu
  //    const int sA[3] = {10, 13, 51};
  
        
         
      int foto[3][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};
      int fotoMinV[3][8] = {{2, 6, 7, 8, 3, 6, 5, 8}, {0, 1, 1, 3, 4, 0, 3, 8}, {1, 7, 10, 6, 2, 6, 1, 10}};
      int fotoMaxV[3][8] = {{4, 8, 9, 10, 5, 8, 8, 10}, {1, 3, 4, 5, 5, 0, 5, 11}, {6, 13, 16, 12, 5, 10, 8, 16}};
      int fotoMinB[3][8] = {{27, 50, 43, 35, 32, 41, 43, 47}, {19, 44, 35, 58, 56, 100, 52, 58}, {25, 49, 52, 49, 31, 53, 45, 69}};
      int fotoMaxB[3][8] = {{30, 53, 46, 38, 35, 44, 46, 50}, {22, 47, 38, 62, 58, 100, 55, 60}, {33, 56, 63, 56, 36, 60, 52, 79}};
   
      double angulo[3][8] = {{271.2, 279.6, 287.8, 296.3, 307.6, 315.7, 323.8, 332.3}, {212.7, 205.8, 195.2, 180, 180, 167, 156, 149}, {44.3, 27.7, 36.2, 54.2, 63.7, 88.8, 72.2, 80.4}};

    int lectura(int ft, int placa);
    void binario(int pin, int lado);
    

  
  
      // -------------------------------------------------------------------------------------------------------------
  
  
  
  
    public:
  
      Color();
      void iniciar();
      void calibrar();
      double checkForLinea();
      bool checkForLineaBool();
 

  };

#endif
