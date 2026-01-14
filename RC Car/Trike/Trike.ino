//Code for Remote Control Trike
//Using DRV8835 Motor Controller with Arduino Nano 328P
//Using Phase/Enable Mode
//Enable Pin is


//DRV8835 Device Info, https://www.pololu.com/product/2135/pictures
//Arduino Nano Pins http://christianto.tjahyadi.com/wp-content/uploads/2014/11/nano.jpg
//Arduino interrupt driven PWM reading from http://www.camelsoftware.com/2015/12/25/reading-pwm-signals-from-an-rc-receiver-with-arduino/

//This works while Nano is connected to a computer.
//Once on Battery power, turning the servo causes the motor to stop.
//It only stops if the servo is connected.
//Check the output voltage of the 7805 when the servo turns.
//The onboard LED flashes when the servo is moved, making me think that the board is not getting enough power.


#define THROTTLE_PIN 2
#define MOTORPHASE_PIN 4
#define MOTORPWM_PIN 5
#define DEADSPOT 10

volatile unsigned long timer_start;
volatile int last_interrupt_time;
volatile int pulse_time;
const int throttle_max = 1928;
const int throttle_min = 1128;
const int signed throttle_rest = 1526 ;

void my_PWM_read(){
  last_interrupt_time = micros();
  if(digitalRead(THROTTLE_PIN) == HIGH){
    timer_start = micros();
  }else{
    if(timer_start != 0){
      pulse_time = ((volatile int)micros() - timer_start);
      timer_start = 0;
    }
  }
  
}


void setup(){
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(MOTORPHASE_PIN, OUTPUT);
  pinMode(MOTORPWM_PIN, OUTPUT);
  //Interrupt 0 is on Pin 2
  //Interrupt 1 is on Pin 3
  attachInterrupt(0, my_PWM_read, CHANGE);
  Serial.begin(9600);
//  for (int count=0 ; count<8 ; count++){
//    throttle_rest += pulseIn(THROTTLE_PIN, HIGH);
//  }
//  throttle_rest = throttle_rest/8;
}


void loop(){
  int signed throttle = map(pulse_time, throttle_min, throttle_max, -250, 250);
  
  //create deadspot threshold
  if(abs(throttle)>DEADSPOT){
    //Move forward or backward
    pinMode(MOTORPHASE_PIN, OUTPUT);
    if(throttle>0){
      //Forwards
      digitalWrite(MOTORPHASE_PIN, LOW);
    }else{
      //Backwards
      digitalWrite(MOTORPHASE_PIN, HIGH);
    }
    analogWrite(MOTORPWM_PIN, abs(throttle));
  }else{
    //Brake
    pinMode(MOTORPHASE_PIN, INPUT);
    analogWrite(MOTORPWM_PIN, 0);
  }
  //  throttle_in = pulseIn(THROTTLE_PIN, HIGH,25000);
//  Serial.print("Pulse_time: ");
//  Serial.print(pulse_time);
//  Serial.print(", throttle_min: ");
//  Serial.print(throttle_min);
//  Serial.print(", throttle_rest: ");
//  Serial.print(throttle_rest);
//  Serial.print(", throttle_max: ");
//  Serial.print(throttle_max);
//  Serial.print(", throttle: ");
//  Serial.print(throttle);
//  Serial.print("\n");
  delay(10);
  
  
  
}
