
class Constantes {
   public:
       static const int velocidades = 140;
       static const int velMin = 80;
       static const int trigPin = 36;
       static const int echoPin = 38;
       static const int led = 9;
       static const int analogo = A4;

       enum Estados {
        linea,
        buscarPelota,
        hasPelota,
        golPorteria,
        nada
       };

       enum Lados {
        amarillo = 0,
        azul = 1
       };

};