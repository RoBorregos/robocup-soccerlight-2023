class Porteria {
  public:
    int color;
    int x;
    int y;
    int w;
    int h;
    int area;

    //Default
    Porteria() {
      color = -1;
      x = -1;
      y = -1;
      w = -1;
      h = -1;
    }


    //Almacenar los datos de la cámara
    void actualizar(String str) {
      int arr[5];
      String data = "";
      int index = 0;
      Porteria p;


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

        area = w*h;

      }

    }
};
