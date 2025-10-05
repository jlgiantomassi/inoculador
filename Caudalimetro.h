#ifndef Caudalimetro_h
#define Caudalimetro_h

class Caudalimetro {
  public:
    Caudalimetro(int pin, float k, float c);

    int getPin();
    void setCaudal(float c);
    void setFactorK(float k);
    float getFactorK();

    void sumarVolumen(float dV, unsigned long intervalo);
    void resetVolumen();
    float getVolumen();

    float calcularCaudal(int intervaloMedicion);
    float errorCaudal();

    void setPulse();
    void setPulse(int p);

    void setSinAgua(bool f);
    bool getSinAgua();
  private:
    int  pinCaudal; //pin del cuadalimetro
    float factorK;  //constante de factor del caudalimetro
    float caudal;  //caudal a manejar
    int measureInterval;
    volatile int pulseConter;
    float volumen;
    float ultimoFlujo;
    bool sinAgua;
    //unsigned long t0;
    //volatile unsigned long tiempoInicio;
    //unsigned long tiempoT; //tiempo para eliminar ruido en las interrupciones

};
#endif
