#ifndef Bomba_h
#define Bomba_h

class Bomba
{
  public:
    Bomba(int pinEncendido,int pinRegulador,int estado);
    void encender();
    void apagar();
    int getEstado();
    void setEstado(int est);
    
  private:
    int _pinEncendido;
    int _pinRegulador;
    const int encendido = HIGH; //enciende el rele del modulo (logica inversa)
    const int apagado = LOW;  //apaga el rele del modulo (logica inversa)
    int _estado=0;  //varia de 147 (6V) a 255 (12V)
    int estadoMin=147;
    int estadoMax=255;
};

#endif
