#include <Arduino.h>
#include "Valvula.h"

Valvula::Valvula(int pin)
{
  _pin = pin;
  digitalWrite(_pin, releOFF);
  pinMode(_pin, OUTPUT);
}

void Valvula::abrirValvula()
{
  digitalWrite(_pin, releON);
  estado=HIGH;
}

void Valvula::cerrarValvula()
{
  digitalWrite(_pin, releOFF);
  estado=LOW;
}

bool Valvula::estadoValvula()
{
  return estado;
}