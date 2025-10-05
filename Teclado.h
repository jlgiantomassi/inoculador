//Definimos variables para manejar el teclado matricial 4x4
const byte rowsCount = 4;
const byte columsCount = 4;
 
char keys[rowsCount][columsCount] = {//4*4
   { '1','2','3', 'A' },
   { '4','5','6', 'B' },
   { '7','8','9', 'C' },
   { '*','0','#', 'D' }
};

/*
char keys[rowsCount][columsCount] = {
   { '1','2','3'},
   { '4','5','6'},
   { '7','8','9'},
   { '*','0','#'}
};
*/
 
const byte rowPins[rowsCount] = { 10, 9, 8, 7 };  //pines de filas para teclado matricial 4x4
const byte columnPins[columsCount] = { 6, 5, 4, A3 }; //pines de columnas para teclado matricial 4x4
//no usamos el pin 2y3 porque lo utilizamos para interrupciones y los pines 11,12y13 los usamos para la bomba y la valvula
 
Keypad teclado = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount);
//Variables del Menu
Button readButtons()
{
  char val;
  btnPressed = Button::Unknown;
  val = teclado.getKey();
  if ( val == '6' )
    btnPressed = Button::Right;
  else if ( val == '2' )
    btnPressed = Button::Up;
  else if ( val == '8' )
    btnPressed = Button::Down;
  else if ( val == '4' )
    btnPressed = Button::Left;
  else if ( val == '#'  )
    btnPressed = Button::Ok;
  else if ( val == '*'  )
    btnPressed = Button::Cancel;
  
  if(btnPressed!=Button::Unknown)
    delay(100);
  return btnPressed;
}