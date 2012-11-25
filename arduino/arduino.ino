#include <Servo.h>
#include <IRremote.h>
#include "angles.h"
#include "ir_data.h"
#include "pins.h"

Servo servo[8];

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);                    // Intialize Serial communication
  irrecv.enableIRIn();                   // Start the receiver

  for(int i=0;i<8;i++)
    servo[i].attach(servo_pin[i]);       // Attach the Servos on to the respective pins

  stand_up();    // Make the QuadBot Stand up before doing any action
}

void loop()
{
  if (irrecv.decode(&results))           // If IR data is present
  {
    action();
    irrecv.resume();                     // Receive the next value
  }
}

void action()
{
  switch(results.value)
  {
  case stand_up_data:
    Serial.println("Stand up");
    stand_up();   
    break;

  case step_forward_data:                
    Serial.println("Step Forward");
    forward();
    forward();
    forward();
    break;

  case step_backward_data:
    Serial.println("Step Backward");
    backward();
    break;

  case step_right_data:
    Serial.println("Step Right");
    step_right();
    step_right();
    step_right();
    break;

  case step_left_data:
    Serial.println("Step Left");
    step_left();
    step_left();
    step_left();
    break;

  case continous_forward_data:
    Serial.println("Continous Forward");
    continous_forward();
    break;

  case stretch_data:
    Serial.println("Stretch");
    stretch();
    stretch();
    stretch();
    break;
  }
}

void stand_up()
{
  for(int i=0;i<8;i++)
    servo[i].write(stand_position[i]);  // Set the positions of the servos to the postions in the "angles.h" file
}

void forward()
{
  for(int i=0;i<displacement;i++)      // Raise the Left Front and Right Rear legs
  {
    move_servo(0,-1);
    move_servo(2,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Move the Left Front and Right Rear legs forward
  {
    move_servo(4,+1);
    move_servo(6,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Lower the Left front and Right Rear legs
  {
    move_servo(0,+1);
    move_servo(2,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)       // Move the Left Front and Right Rear legs back &
  {                                     // Raise the Right Front and Left Rear legs
    move_servo(4,-1);
    move_servo(6,+1);
    move_servo(1,-1);
    move_servo(3,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Right Front and Left Rear legs forward
  {
    move_servo(5,-1);
    move_servo(7,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Lower the Right Front and Left Rear legs 
  {
    move_servo(1,+1);
    move_servo(3,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Right Front and Left Rear legs back
  {
    move_servo(5,+1);
    move_servo(7,-1);
    delay(delay_time); 
  }  
}

void backward()
{
  for(int i=0;i<displacement;i++)      // Raise the Left Front and Right Rear legs
  {
    move_servo(0,-1);
    move_servo(2,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Move the Left Front and Right Rear legs back
  {
    move_servo(4,-1);
    move_servo(6,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Lower the Left front and Right Rear legs
  {
    move_servo(0,+1);
    move_servo(2,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)       // Move the Left Front and Right Rear legs forward &
  {                                     // Raise the Right Front and Left Rear legs
    move_servo(4,+1);
    move_servo(6,-1);
    move_servo(1,-1);
    move_servo(3,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Right Front and Left Rear legs back
  {
    move_servo(5,+1);
    move_servo(7,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Lower the Right Front and Left Rear legs 
  {
    move_servo(1,+1);
    move_servo(3,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Right Front and Left Rear legs forward
  {
    move_servo(5,-1);
    move_servo(7,+1);
    delay(delay_time); 
  }  
}

void step_right()
{
  for(int i=0;i<displacement;i++)      // Raise the Left Front and Right Rear legs
  {
    move_servo(0,-1);
    move_servo(2,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Move the Left Front leg forward and Right Rear leg backward
  {
    move_servo(4,+1);
    move_servo(6,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Lower the Left front and Right Rear legs
  {
    move_servo(0,+1);
    move_servo(2,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)       // Move the Left Front leg backward, Right Rear leg forward &
  {                                     // Raise the Right Front and Left Rear legs
    move_servo(4,-1);
    move_servo(6,-1);
    move_servo(1,-1);
    move_servo(3,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Left Rear leg forward and Right Front leg backward
  {
    move_servo(5,+1);
    move_servo(7,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Lower the Right Front and Left Rear legs 
  {
    move_servo(1,+1);
    move_servo(3,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Left Rear leg backward and Right Front leg forward
  {
    move_servo(5,-1);
    move_servo(7,-1);
    delay(delay_time); 
  }  
}

void step_left()
{
  for(int i=0;i<displacement;i++)      // Raise the Left Front and Right Rear legs
  {
    move_servo(0,-1);
    move_servo(2,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Move the Left Front leg backward and Right Rear leg forward
  {
    move_servo(4,-1);
    move_servo(6,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)      // Lower the Left front and Right Rear legs
  {
    move_servo(0,+1);
    move_servo(2,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)       // Move the Left Front leg forward, Right Rear leg backward &
  {                                     // Raise the Right Front and Left Rear legs
    move_servo(4,+1);
    move_servo(6,+1);
    move_servo(1,-1);
    move_servo(3,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Left Rear leg backward and Right Front leg forward
  {
    move_servo(5,-1);
    move_servo(7,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Lower the Right Front and Left Rear legs 
  {
    move_servo(1,+1);
    move_servo(3,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)        // Move the Left Rear leg forward and Right Front leg backward
  {
    move_servo(5,+1);
    move_servo(7,+1);
    delay(delay_time); 
  }  
}

void continous_forward()
{
  for(int i=0;i<displacement;i++)          // Raise the Left Front and Right Rear legs
  {
    move_servo(0,-1);
    move_servo(2,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)          // Move the Left Front and Right Rear legs forward
  {
    move_servo(4,+1);
    move_servo(6,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)          // Lower the Left Front and Right Rear legs
  {
    move_servo(0,+1);
    move_servo(2,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)          // Move the Left Front and Right Rear legs backward twice the displacement & 
  {                                        // Raise the Right Front and Left Rear legs
    move_servo(4,-2);
    move_servo(6,+2);
    move_servo(1,-1);
    move_servo(3,-1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)          // Move the Right Front and Left Rear Legs forward
  {
    move_servo(5,-1);
    move_servo(7,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)          // Lower the Right Front and Left Rear legs
  {
    move_servo(1,+1);
    move_servo(3,+1);
    delay(delay_time); 
  }
  for(int i=0;i<displacement;i++)          // Move the Right Front and Left Rear legs backward twice the displacement
  {                                        // Raise the Left Front and Right Rear legs
    move_servo(5,+2);
    move_servo(7,-2);
    move_servo(0,-1);
    move_servo(2,-1);
    delay(delay_time); 
  }

  irrecv.resume(); 
  results.value=0;

  while(1)                       // Enter an infinite while loop
  {
    if(irrecv.decode(&results))  // Break when there is incoming Remote data
      if(results.value)
        break;
    irrecv.resume(); 

    for(int i=0;i<displacement;i++)        // Move the Left Front and Right Rear legs forward twice the displacement
    {
      move_servo(4,+2);
      move_servo(6,-2);
      delay(delay_time); 
    }
    for(int i=0;i<displacement;i++)        // Lower the Left Front and Right Rear legs
    {
      move_servo(0,+1);
      move_servo(2,+1);
      delay(delay_time); 
    }
    for(int i=0;i<displacement;i++)        // Move the Left Front and Right Rear legs backward twice the displacement
    {                                      // Raise the Right Front and Left Rear legs
      move_servo(4,-2);
      move_servo(6,+2);
      move_servo(1,-1);
      move_servo(3,-1);
      delay(delay_time); 
    }

    if(irrecv.decode(&results))
      if(results.value)
        break;
    irrecv.resume(); 

    for(int i=0;i<displacement;i++)        // Move the Right Front and Left Rear legs forward twice the displacement 
    {
      move_servo(5,-2);
      move_servo(7,+2);
      delay(delay_time); 
    }
    for(int i=0;i<displacement;i++)        // Lower the Right Front and Left Rear legs
    {
      move_servo(1,+1);
      move_servo(3,+1);
      delay(delay_time); 
    }
    for(int i=0;i<displacement;i++)        // Move the Right Front and Left Rear legs backward, twice the displacement
    {                                      // Raise the Left Front and Right Rear legs
      move_servo(5,+2);
      move_servo(7,-2);
      move_servo(0,-1);
      move_servo(2,-1);
      delay(delay_time); 
    }
  }  
  stand_up();                    // Stand up when the while loop breaks
}

void stretch()
{
  for(int i=0;i<stretch_displacement;i++)        // Stretch the bot towards the left
  {                                      
    move_servo(0,-1);
    move_servo(1,+1);
    move_servo(2,+1);
    move_servo(3,-1);
    delay(stretch_delay_time); 
  }
  delay(500);
  for(int i=0;i<stretch_displacement;i++)        // Stretch the bot towards the right
  {                                       
    move_servo(0,+2);
    move_servo(1,-2);
    move_servo(2,-2);
    move_servo(3,+2);
    delay(stretch_delay_time); 
  }
  delay(500);
  for(int i=0;i<stretch_displacement;i++)        // Bring it back to initial position
  {                                      
    move_servo(0,-1);
    move_servo(1,+1);
    move_servo(2,+1);
    move_servo(3,-1);
    delay(stretch_delay_time); 
  }
  delay(500);

  for(int i=0;i<stretch_displacement;i++)        // Stretch the bot towards the back
  {                                       
    move_servo(4,+1);
    move_servo(5,-1);
    move_servo(6,-1);
    move_servo(7,+1);
    delay(stretch_delay_time); 
  }
  delay(500);
  for(int i=0;i<stretch_displacement;i++)        // Stretch the bot towards the front
  {                                       
    move_servo(4,-2);
    move_servo(5,+2);
    move_servo(6,+2);
    move_servo(7,-2);
    delay(stretch_delay_time); 
  }
  delay(500);
  for(int i=0;i<stretch_displacement;i++)        // Bring it back to initial position
  {                                       
    move_servo(4,+1);
    move_servo(5,-1);
    move_servo(6,-1);
    move_servo(7,+1);
    delay(stretch_delay_time); 
  }
  delay(500);
}

void move_servo(int x,int y)
{
  servo[x].write(servo[x].read()+y); // Read the current position of Servo 'x' and move it by 'y' degrees
}


