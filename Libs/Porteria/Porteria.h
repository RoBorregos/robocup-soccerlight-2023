#ifndef Porteria_h
  #define Porteria_h

class Porteria {
  private:
    int color = -1;
    int x = -1;
    int y = -1;
    int w = -1;
    int h = -1;
    int area = -1;

    public:

    Porteria();
    void actualizar(String str);
    void setX(int xs);
    int getX();
    int getY();
    int getColor();
    int getArea();
    int getLargo();
    int getAlto();
    
};

#endif
