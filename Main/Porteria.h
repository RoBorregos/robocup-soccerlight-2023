class Porteria {
  private:
    int color = -1;
    int x = -1;
    int y = -1;
    int w = -1;
    int h = -1;
    int area = -1;

    //Default
 


    public:
    
   Porteria() {
    }
    
    //Almacenar los datos de la cámara
    void actualizar(String str) {
      if (str.length() == 0) {
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

        area = w*h;

      }

    }

    void setX(int xs){
      x = xs;
    }

    int getX() {
      return x;
    }
};
