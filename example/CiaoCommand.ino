/*

 Possible commands to send from the xmpp client:

 * "digital/PIN"        -> to read a digital PIN
 * "digital/PIN/VALUE"  -> to write a digital PIN (VALUE: 1/0)
 * "analog/PIN/VALUE"   -> to write in a PWM PIN(VALUE range: 0 - 255);
 * "analog/PIN"         -> to read a analog PIN
 * "mode/PIN/VALUE"     -> to set the PIN mode (VALUE: input / output)
 * "led on"             -> turn on led 13
 * "led off"            -> turn off led 13
 * "ciao"               -> random answers in 5 different languages
 
 */
#include <Ciao.h>

String mess_random[5]={"Hi, I am MCU :-P","hallo , ik ben MCU :-P","bonjour, je suis MCU :-P","Ciao, io sono MCU :-P","Kon'nichiwa, watashi wa MCU yo :-P" };

void setup() {
  
  Ciao.begin();
}

void loop() {

  CiaoData data = Ciao.read("xmpp");   
  if(!data.isEmpty()){  
    String id = data.get(0);
    String sender = data.get(1);
    String message = data.get(2);
    
    message.toLowerCase();
    if(message == "ciao")
      Ciao.writeResponse("xmpp",id,mess_random[random(0,5)]);
    
    else if(message == "led on"){
      digitalWrite(13,HIGH);
      Ciao.writeResponse("xmpp",id,"Led D13 ON");
    }
    
    else if(message == "led off"){
      digitalWrite(13,LOW);
      Ciao.writeResponse("xmpp",id,"Led D13 OFF");     
    }
    
    else{
      CiaoData command;        
      command.split_command(message,"/");
      process(command,id);
    }
  }
}



void process(CiaoData cmd, String id) {

  String command = cmd.get(0); 
   if (command == "digital") {
    digitalCommand(cmd,id);
  }
  else if (command == "analog") {
    analogCommand(cmd,id);
  }
  else if (command == "mode") {
    modeCommand(cmd,id);
  }
  else
    Ciao.writeResponse("xmpp",id,"sorry, i don't understand :(");
  
}

void digitalCommand(CiaoData cmd, String id) {
  int pin, value;
  
  pin = (cmd.get(1)).toInt();
  
  if (cmd.get(2) != "-1") {
    value = (cmd.get(2)).toInt();
    digitalWrite(pin, value);
    if (value == 1)
      Ciao.writeResponse("xmpp",id,"Pin D"+String(pin)+" ON");
    else if(value == 0)
      Ciao.writeResponse("xmpp",id,"Pin D"+String(pin)+" OFF");   
  }
  else if (cmd.get(2) == "-1") {
    value = digitalRead(pin);
    Ciao.writeResponse("xmpp",id,"D"+String(pin)+" value = "+String(value));
  }
}

void analogCommand(CiaoData cmd, String id) {
  int pin, value;

  pin = (cmd.get(1)).toInt();
  
  if (cmd.get(2) != "-1") {
    value =(cmd.get(2)).toInt();
    analogWrite(pin, value);
    Ciao.writeResponse("xmpp",id,"D"+String(pin)+" set to analog");
  }
  else if (cmd.get(2) == "-1") {
    value = analogRead(pin);
    Ciao.writeResponse("xmpp",id,"A"+String(pin)+" value = "+String(value));
  }
}

void modeCommand(CiaoData cmd, String id) {
  int pin;

  pin = (cmd.get(1)).toInt();
  
  String mode = cmd.get(2);
  if (mode == "input") {
    pinMode(pin, INPUT);
    Ciao.writeResponse("xmpp",id," pin D"+String(pin)+" set in INPUT mode");
    return;
  }

  if (mode == "output") {
    pinMode(pin, OUTPUT);
    Ciao.writeResponse("xmpp",id," pin D"+String(pin)+" set in OUTPUT mode");
    return;
  }
  
  Ciao.writeResponse("xmpp",id,"invalid mode");

}


  
