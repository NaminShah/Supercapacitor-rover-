

float startBval;









float startVoltage = 0;







const int enA = 10;


const int in1 = 9;


const int in2 = 8;







const int enB = 5;


const int in3 = 7;


const int in4 = 6;







float brightness;


float PIF;


float PIB;


float vi;


int frontPR = A0; // Front Photoresistor pin


int backPR = A1; // Back Photoresistor pin



























//Variables for Self-Learning


float learn_rate = 0.0001;







float karma[] = { 0.001, 0.001, 0.001, 0.001, 0.001, 0.001 };







float Pred_Reward(float hold[]){


 float total = 0.0;


 Serial.println();


 for(int i = 0; i < 6; i++)


 {


   total = total + (hold[i] * karma[i]);


   Serial.print(" hold: ");


   Serial.print(hold[i]);


   Serial.print("------");


   Serial.print(" karma: ");


   Serial.print(karma[i]);


  


 }


 Serial.println();


 return total;


}







void Update_Karma(float y_actual, float y_pred, float hold[])


{


 for(int i = 0; i < 6; i++){


   float pDelta = (y_pred - y_actual) * hold[i];


   karma[i] = karma[i] - (learn_rate * pDelta);


 }


}












void fullForward(){


   analogWrite(enB,255);


   analogWrite(enA,255);


   digitalWrite(in1,HIGH);


   digitalWrite(in3,LOW);


   digitalWrite(in2,LOW);


   digitalWrite(in4,HIGH);







}







void fullBack(){


   analogWrite(enB,255);


   analogWrite(enA,255);


   digitalWrite(in1,LOW);


   digitalWrite(in3,HIGH);


   digitalWrite(in2,HIGH);


   digitalWrite(in4,LOW);


}







void still(){


   analogWrite(enB,255);


   analogWrite(enA,255);


   digitalWrite(in1,LOW);


   digitalWrite(in3,LOW);


   digitalWrite(in2,LOW);


   digitalWrite(in4,LOW);


}







void setup()


{


   Serial.begin(9600);










   float PIF = analogRead(frontPR)/10;//previous front light value


   float PIB= analogRead(backPR)/10;//previous back light value


   //initial voltage variable here


   float vi = readVoltage(3.5);






}







float current(int startBval)


{


   return(analogRead(A3)-startBval);






}







float Receive_Reward(float f, float b, float voltage, boolean movement)//input front PR val, Back PR val, previos voltage, and if rover actually moved


{


 float reward = 0.0;






 float volt = readVoltage(3.5);














 if( voltage < volt)


 {


   reward = reward + 1.0 ;


     if ( f + b > brightness & movement ) //voltage increased due to movement


       reward = reward + 2.0;            // reward of 2 for an succssful manuever


      


    


 }














 else if(voltage > volt & movement) {


   reward = reward - 1.0; // if motion didnt increase panel voltage or if it decreased panel voltage, subtract 1


 }






 return reward;


}












float readVoltage( int vref)


{


 ////measure voltge on analogpin 4... return float in volts


 //uses voltage divider to measure voltage on analogPin 4


float vPow = vref;


float r1 = 100000;


float r2 = 10000;


float v = (analogRead(A5) * vPow) / 1024.0;


  float v2 = v / (r2 / (r1 + r2));


  return v2;


 






}


void loop()


{ 


brightness = PIF + PIB; // total brightness of environment










 //Scale Distances


 //Use previous distances











 //Create Current State Representation


 //using prevoius loops distances


 //Notice indices 3,4 and 5 are left, forward, right.


 //final index is the bias unit.


 float hold[6] = {PIF,PIB, 1, 0,0, 1};


 //what is predicted for left?


 float front_pred = Pred_Reward(hold);


 //re-encode state for forward


 hold[2] = 0; hold[3] = 1;


 //what is forward reward prediction?


 float back_pred = Pred_Reward(hold);







 hold[3] = 0; hold[4]=1;


 float still_pred = Pred_Reward(hold);






 //re-encode state for right










 Serial.println(" Predicted Rewards: ");


 Serial.print(front_pred);


 Serial.print(":::");


 Serial.print(back_pred);


 Serial.print(":::");


 Serial.print(still_pred);


 Serial.println();






 //Made a decision, lets see what the current state is.


 //Read Distances


   float PDF = analogRead(frontPR)/10;


   delay(10);


   float PDB = analogRead(backPR)/10;


   delay(10);


   float vd = readVoltage(3.5); // 4.7 - 5.0  in arduinos


   delay(10);










 //actual reward


 float a_reward;


 //predicted reward for action actually taken.


 float y_reward;


 if(y_reward > 1000 || y_reward < -1000)


 {


   for(int i = 0; i < 7; i++){


     karma[i] = 0.0001 * i;


   }


 }


 if(front_pred > back_pred)


 {


   Serial.println(" Chose Forward ");


   fullForward();


   delay(10);







   //float current = readCurrent(startBval);


  


   y_reward = front_pred;


   a_reward = Receive_Reward(PDF, PDB, vi, true);


   hold[2] = 1;


   hold[3] = 0;


   hold[4] = 0;


  


 }


 else if(back_pred > front_pred)


 {


   Serial.println(" Chose Back ");


   fullBack();


       delay(10);







   y_reward = back_pred;


   a_reward = Receive_Reward(PDF, PDB, vi, true);


   hold[2] = 0;


   hold[3] = 1;


   hold[4] = 0;


 }







 else if ( front_pred == back_pred) // same predicted rewards


 {


  


   if(PDF > PDB) // choose brighter side


   {


       Serial.println(" Chose Forward ");


       fullForward();


       delay(10);












  // float current = readCurrent(startBval);


  


       y_reward = front_pred;


       a_reward = Receive_Reward(PDF, PDB, vi, true);


       hold[2] = 1;


       hold[3] = 0;


       hold[4] = 0;


   }


   else


   {


       Serial.println(" Chose Back ");


       fullForward();


       delay(10);












  // float current = readCurrent(startBval);


  


       y_reward = back_pred;


       a_reward = Receive_Reward(PDF, PDB, vi, true);


       hold[2] = 0;


       hold[3] = 1;


       hold[4] = 0;


      


   }


 }


 else{


/// stay still


   delay(10);







  


  


   y_reward = still_pred;


   a_reward = Receive_Reward(PDF, PDB, vi, false);


   hold[2] = 0;


   hold[3] = 0;


   hold[4] = 1;


 }


 Serial.println(" ..... ");


 Serial.print(" Actual Reward: ");


 Serial.println(a_reward);


 Serial.print(" Predicted Reward: ");


 Serial.print(y_reward);


 Serial.println(" ..... ");






 //update the model based on experience


 Update_Karma(a_reward, y_reward, hold);






 //set previous PR values to current values.


 PIF = PDF;


 PIB = PDB; 


 vi = vd;










  if ( v2 < 3.5)


  {


      digitalWrite(A0,HIGH);//switch between power buffers


  }


 


 


 }
