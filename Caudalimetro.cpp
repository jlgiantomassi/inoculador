#include <Arduino.h>
#include "Caudalimetro.h"


Caudalimetro::Caudalimetro(int pin, float k, float c) //pin,factorK, caudalinicial
{
  pinCaudal = pin;
  volumen = 0;
  //t0 = 0;
  //tiempoInicio = 0;
  //tiempoT = 5; //tiempo para eliminar ruido en las interrupciones
  measureInterval = 1500;
  factorK = k;
  caudal = c;
}

void Caudalimetro::setCaudal(float c)
{
  caudal = c;
}

void Caudalimetro::setFactorK(float k)
{
  factorK=k;
}

float Caudalimetro::getFactorK()
{
  return factorK;
}




//metodos publicos
//calcula el volumen total que paso por el caudalimetro
void Caudalimetro::sumarVolumen(float dV, unsigned long intervalo)
{
  //volumen += dV / 60 * (millis() - t0) / 1000.0;
  volumen += dV / 60 * intervalo / 1000.0;
  //t0 = millis();
}

//calcula el caudal en un lapso de tiempo dado(measureInterval)
float Caudalimetro::calcularCaudal(int intervaloMedicion)
{
  //pulseConter = 0;
  //delay(measureInterval);
  float freq = (float)pulseConter * 1000 / intervaloMedicion; //measureInterval;
  ultimoFlujo = freq / factorK;
  return ultimoFlujo;
}

float Caudalimetro::errorCaudal()
{
  if (caudal == 0) //evitamos el desbordamiento por division de 0. Si el caudal es 0 debemos cerrar la valvula
    return 1;
  else
    return (ultimoFlujo / caudal) - 1;

}

//metodos privados
//interrupciones del caudalimetro
void Caudalimetro::setPulse()
{
  //if (millis() - tiempoInicio > tiempoT) //tiempoT para hacer el retardo por las interferencias cuando se usaba el motor con el rele
  //{
  pulseConter++;
  //tiempoInicio = millis();
  //}
}

void Caudalimetro::setPulse(int p)
{
  //if (millis() - tiempoInicio > tiempoT) //tiempoT para hacer el retardo por las interferencias cuando se usaba el motor con el rele
  //{
  pulseConter = p;
  //tiempoInicio = millis();
  //}
}


int Caudalimetro::getPin()
{
  return pinCaudal;
}


float Caudalimetro::getVolumen()
{
  return volumen;
}

void Caudalimetro::resetVolumen()
{
  volumen = 0;
}


void Caudalimetro::setSinAgua(bool f)
{
  sinAgua = f;
}

bool Caudalimetro::getSinAgua()
{
  return sinAgua;
}
