#include <SoftwareSerial.h>
SoftwareSerial sim(3, 2);
int _timeout;
String _buffer;
String number = "+ZZZxxxxxxxxx"; //-> Numero d'envoie du message

void setup() {
  //delay(7000); // delay de 7 secondes pour s'assurer que les modules reçoivent le signal
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("Système démarré...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Tape s pour l'envoi d'un SMS, r pour recevoir un SMS et c pour passer un appel");
}
void loop() {
  if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage();
        break;
      case 'r':
        RecieveMessage();
        break;
      case 'c':
        callNumber();
        break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());
}
void SendMessage()
{
  //Serial.println ("Envoi du message");
  sim.println("AT+CMGF=1");    //Définit le module GSM en mode texte
  delay(200);
  //Serial.println ("Définir le numéro de SMS");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Numéro de téléphone portable pour envoyer le message
  delay(200);
  String SMS = "Salut Ici Randolf de Tech Inov 237";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);
  delay(200);
  _buffer = _readSerial();
}
void RecieveMessage()
{
  Serial.println ("SIM800L Lire un SMS");
  sim.println("AT+CMGF=1");
  delay (200);
  sim.println("AT+CNMI=1,2,0,0,0"); //Commande AT pour recevoir un SMS en direct
  delay(200);
  Serial.write ("Message non lu terminé");
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}
void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}
