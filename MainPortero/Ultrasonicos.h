class Ultrasonicos {
//  private:
//    Ultrasonico uAdelante;
//    Ultrasonico uAtras;
//    Ultrasonico uDerecha;
//    Ultrasonico uIzquierda;
//
//    double disAdelante;
//    double disAtras;
//    double disIzquierda;
//    double disDerecha;
//
//    double posX;
//    double posY;
//    double angulo;
//
//    double ancho = 182;
//    double alto = 243;
//    double threshold = 10;
//
//  public:
//
//    Ultrasonicos(int echo1, int trig1, int echo2, int trig2, int echo3, int trig3, int echo4, int trig4) {
//      uAdelante.set(echo1, trig1);
//      uAtras.set(echo2, trig2);
//      uDerecha.set(echo3, trig3);
//      uIzquierda.set(echo4, trig4);
//    }
//
//    void iniciar() {
//      uAdelante.iniciar();
//      uIzqierda.iniciar();
//      uDerecha.iniciar();
//      uIzquierda.iniciar();
//    }
//
//    void calcularDatos() {
//      double prevIzq = disIzquierda;
//      double prevDer = disDerecha;
//      double prevAd = disAdelante;
//      double prevAt = disAtras;
//
//      disAdelante = uAdelante.getDistance();
//      disAtras = uAtras.getDistance();
//      disIzquierda = uIzquierda.getDistance();
//      disDerecha = uDerecha.getDistance();
//
//      //X
//      if (((disIzquierda + disDerecha) >= ancho - threshold) && ((disIzquierda + disDerecha) <= ancho + threshold)) {
//        posX = disIzquierda;
//      } else {
//        double errorI = prevIzq - disIzqierda;
//        double errorD = prevDer - disDerecha;
//
//        posX = if (errorI > errorD) ? ancho - disDerecha : disIzquierda;
//      }
//
//      //Y
//      if (((disAdelante + disAtras) >= alto - threshold) && ((disAdelante + disAtras) <= alto + threshold)) {
//        posY = disAdelante;
//      } else {
//        double errorAd = prevAd - disAdelante;
//        double errorAt = prevAt - disAtras;
//
//        posY = if (errorAd > errorAt) ? alto - disAdelante : disAtras;
//      }
//
//    }
//
//    double getPosX() {
//      return posX;
//    }
//
//    double getPosY() {
//      return posY;
//    }
//
//    double getAngulo(double objX, double objY) {
//      angulo = atan((objY - posY) / (objX - posX));
//      angulo = angulo * 180 / PI;
//      return angulo;
//    }

};
