#include "Arduino.h"
#include "Porteria.h"

Porteria::Porteria() {
}

//Almacenar los datos de la cámara
void Porteria::actualizar(String str) {
  //x = -1;
  // if (str.length() == 0) {
  //   x = -1;
  //   return;
  // }

  int arr[4];
  String data = "";
  int index = 0;
  //Porteria p;

  for (int i = 2; i < str.length() && index < 4; i++) {

    if (!(str[i] == ',')) {
      data += str[i];
    } else if (str[i] == ',' || i == str.length() - 1) {
      arr[index++] = data.toInt();
      data = "";
    }

    x = arr[0];
    y = arr[1];
    h = arr[2];
    w = arr[3];

    area = w * h;

  }

}

void Porteria::setX(int xs) {
  x = xs;
}

int Porteria::getY() {
  return y;
}

int Porteria::getX() {
  return x;
}

int Porteria::getColor() {
  return color;
}
