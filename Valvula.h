#ifndef Valvula_h
#define Valvula_h

class Valvula
{
  public:
    Valvula(int pin);
    void abrirValvula();
    void cerrarValvula();
    bool estadoValvula();
    
  private:
    int _pin;
    int estado;
    const int releOFF = HIGH; //logica inversa porque se maneja la valvula con un rele
    const int releON = LOW;
};
#endif