#include <Arduino.h>
#include "Bomba.h"

Bomba::Bomba(int pinEncendido,int pinRegulador, int estado)
{
  _pinEncendido = pinEncendido;
  _pinRegulador=pinRegulador;
  _estado=estado;
  digitalWrite(_pinEncendido, apagado);
  analogWrite(_pinRegulador,_estado);
  pinMode(_pinEncendido, OUTPUT);
  pinMode(_pinRegulador, OUTPUT);
}

void Bomba::encender()
{
  digitalWrite(_pinEncendido, encendido);
}

void Bomba::apagar()
{
  digitalWrite(_pinEncendido, apagado);
}

int Bomba::getEstado()
{
  return _estado;
}

void Bomba::setEstado(int est)
{
  if(est < estadoMin)
    est=estadoMin;
  if(est > estadoMax)
    est=estadoMax;
  _estado=est;
  analogWrite(_pinRegulador,_estado);
}