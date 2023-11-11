/*Prueba de un sensor optico
* el resultado se muestra en el led del puerto 6
*la señal del sensor se recibe en el puerto 7
 */

//se incluye librería de manejo de servos
#include <Servo.h>
//se asigna al servo con el nombre de orientador
Servo orientador;
/*Se asignan las variables enteras para sensores, espera y angulo, las variables para
* sensores y espera pueden ser del tipo booleana
 */
int sensorCuello=0;
//variable para el sensor del cuello de la botella
int sensorAza=0;
//variable del sensor para el aza
int sensorOrientador=1; //variable de sensor orientador
int giro=0;
//variable para actuar orientador
int espera=0;
//variable para esperar presencia de botella (sensor cuello)y verificar posición (sensor aza)
int angulo=0;
//variable del ángulo del servo motor

void setup()
{
  //Inicializa serial para mostrar valores en monitor,
  //no obligatorio, pero sirve como verificación, se puede eliminar después
  Serial.begin(9600);
  //Se asigna servo a salida pwm10 y se hacen movimientos de inicio para verificar servo
  orientador.attach(11);
  orientador.write(angulo);
  delay(800);
  orientador.write(180);
  delay(800);
  orientador.write(angulo);
  delay(800);
  //Se asignan puertos de salida para leds indicadores y entradas para los 3 sensores
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  //Se hace un ciclo de verificación de los leds
  digitalWrite(4,1);
  delay(1000);
  digitalWrite(5,1);
  delay(1000);
  digitalWrite(4,0);
  delay(1000);
  digitalWrite(5,0);
}
void loop() {
/*Se imprimen en monitor los valores de espera, cuello, aza y orientador para verificar
* sus valores y hacer los ajustes pertinentes, esto se puede omitir una vez probado
 */

  /*Se verifica que espera sea ®, de ser así se leen los valores del sensor del cuello,
  * del sensor del aza, si el valor del cuello es diferente a 1, significa que ya hay
  botella presente y si esto es así, se verifica si el sensor del aza detecta o no esta,
  * de ser detectada el aza, quiere decir que la botella está alrevez
   */
  if (espera==0){
    sensorCuello=digitalRead(7); //Se lee valor de entrada del sensorCuello, entrada 7
    sensorAza=digitalRead(8); //Se lee valor de entrada del sensorAza, entrada 8
    Serial.print("espera=");
    Serial.println(espera);
    Serial.print("cuello=");
    Serial.println(sensorCuello);
    Serial.print("aza=");
    Serial.println(sensorAza);
    Serial.print("orientador=");
    Serial.println(sensorOrientador);
    if (sensorCuello==0) //Si el valor del sensorCuello es cero (diferente a 1) ya hay botella presente
    {
        if (sensorAza==0) //Si el valor del sensorAza es uno, la botella está volteada
        {
          digitalWrite(4,1); //enciende led rojo, botella volteada
          digitalWrite(5,0); //apaga led verde
          espera=1;//asigna el valor de espera en 1, para esperar ahora sensor orientador
          giro=1;//asigna valor de 1 a giro para que se haga el giro de la botella
          }
         else
         {
          digitalWrite(5,1); //enciende led verde, botella correta
          digitalWrite(4,0); //apaga led rojo
          espera=1; //asigna el valor de espera en 1, para esperar ahora sensor orientador
          //y que la botella pase por el sensor sin ser girada, giro se mantiene en 0
         }
    }
  }
    else
    {
    /*Ciclo que espera a que la botella pase por el sensor orientador
     */
      while (sensorOrientador==1){
      sensorOrientador=digitalRead(9);
      }
      /*Una vez que la botella es detectada por el sensor orientador,
      * se verifica si la botella debe ser girada (giro-1)
      * si el ángulo es O se cambia a 180
      * de otro modo lo devuelve al valor de 0, esto para
      * hacer el giro hacia una dirección y la otra
       */
        if (giro==1)
        {
          if (angulo==0)
          {
          angulo=180;
          }
          else
          {
          angulo=0;
          }
          orientador.write(angulo);
          delay(5000);
          angulo=0;
          orientador.write(angulo);
          giro=0; //gira el orientador//reinicia el valor de giro a ® para volver a comenzar
          
        }
          
        /*A continuación realiza proceso de reinicio de leds, gira la botella, devuelve
        a O el valor de espera para comenzar nueva verificación de botella, devuelve
        * valor de 1 a sensorOrientador para comenzar nuevamente.
         */
        digitalWrite(4,0);    //apaga led rojo
        digitalWrite(5,0);    //apaga led verde
        espera=0;             //devuelve el valor de 0 a espera para comenzar ciclo
        sensorOrientador=1;   //devuelve el valor de 1 al sensor Orientador
        delay(100);          //hace una pausa de 2 segundos
    }
}
