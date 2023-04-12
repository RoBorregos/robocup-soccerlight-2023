#include "Arduino.h"
#include "Porteria.h"

Porteria::Porteria() {
}

//Almacenar los datos de la cámara
void Porteria::actualizar(String str) {
  x = -1;
  if (str.length() == 0) {
    x = -1;
    return;
  }

  int arr[5];
  String data = "";
  int index = 0;
  //Porteria p;


  for (int i = 0; i < str.length(); i++) {

    if (!(str[i] == ',')) {
      data += str[i];
    } else if (str[i] == ',' || i == str.length() - 1) {
      arr[index++] = data.toInt();
      data = "";
    }

    color = arr[0];
    x = arr[1];
    y = arr[2];
    h = arr[3];
    w = arr[4];

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
