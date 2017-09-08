void resetEncoder()
{
    nMotorEncoder[motorA] = 0;
    nMotorEncoder[motorC] = 0;
}

void initializeMemory(int *motor_A, int *motor_C)
{

    for (int k = 0; k < 25; k++)
    {
        motor_A[k] = 0;
        motor_C[k] = 0;
    }
}
//Initialize the arrays motor_A and motor_C to zero
//(Code can be found in Task Main)


void motorMemory(int *motor_A, int *motor_C, int &entryCount)
{
    motor_A[entryCount] = nMotorEncoder[motorA]; // Records distance travelled via motor encoder, saves it to the array.
  motor_C[entryCount] = nMotorEncoder[motorC];
    entryCount++;

    resetEncoder();
}

void saveRobot(float objectDistance, float &dangerDistance, int *motor_A, int *motor_C, int &entryCount)
{
    eraseDisplay();
    displayString(1, "Saving");
    objectDistance = dangerDistance;

    for(entryCount; k>0; k--)
    {
        if(motor_A[entryCount] > 0)
        {
            motor[motorA] = -50;
        }
        if(motor_A[entryCount] < 0)
        {
            motor[motorA] = 50;
        }
        if(motor_C[entryCount] > 0)
        {
            motor[motorC] = -50;
        }
        if(motor_C[entryCount] < 0)
        {
            motor[motorC] = 50;
        }

        while(fabs(nMotorEncoder[motorA]) < fabs(motor_A[entryCount]) && fabs(nMotorEncoder[motorC]) < fabs(motor_C[entryCount])){}
        motor_A[entryCount] = 0; //reset array value
        motor_C[entryCount] = 0;
        
    }
}

void exitBin(int *motor_A, int *motor_C, int &entryCount)
{
    eraseDisplay();
    displayString(1, "Exit");
    motor[motorA] = -50; // Backs robot away from box where it starts.
  	motor[motorC] = -50;
  	wait1Msec(1000);
  	motorMemory(motor_A, motor_C, entryCount);
}


void findObject(int *motor_A, int *motor_C, int &entryCount, float &objectDistance, float &dangerDistance)
{
            eraseDisplay();
        displayString(1, "Finding");
    motor[motorC] = 50;
  motor[motorA] = -50;      // Begins the spinning the robot to identify an object.
  while((SensorValue[S3] > 125) && fabs(SensorValue[S3]-dangerDistance) > 3)
  	{

  	} // Looks for objects that are withinacertain range.
  objectDistance = SensorValue[S3];
  motorMemory(motor_A, motor_C, entryCount);

  motor[motorA] = 50;
    motor[motorC] = 50;
  while(SensorValue[S1] != 5) // Moves the robot forwards until the colour sensor reads red
  {
      if(SensorValue[S3] > objectDistance+10 && objectDistance != 0) // The robot will likely be about 30deg off. When it can no longer see the object it will turn until it is once again visible.
      {
      eraseDisplay();
        displayString(1, "Adjusting");
      motorMemory(motor_A, motor_C, entryCount);

        motor[motorA] = -50;
        time1[0] = 0;
        while(time1[0] < 100)
        {
        	if(SensorValue[S1] == 5)
        	{
        		break;
        	}
        }
        wait1Msec(100);
        while(SensorValue[S3] > objectDistance) {}
          motorMemory(motor_A, motor_C, entryCount);
          motor[motorA] = 50;
          motor[motorC] = 50;
      }
      if(SensorValue[S2] == 0)
        {
            eraseDisplay();
            displayString(1, "Second IF");

            saveRobot(objectDistance, dangerDistance, motor_A, motor_C, entryCount);

            motor[motorC] = 50;
              motor[motorA] = -50;      // Begins the spinning the robot to identify an object.
              while((SensorValue[S3] == 255 || SensorValue[S3] < 15) && fabs(SensorValue[S3]-dangerDistance) <= 5) {} // Looks for objects that are within a certain range.
              objectDistance = SensorValue[S3];
              motorMemory(motor_A, motor_C, entryCount);

              motor[motorA] = 50;
              motor[motorC] = 50;
        }
     if(nMotorEncoder[motorA] > (2*2140) && nMotorEncoder[motorC] > (2*2140))
  		{
  			nMotorEncoder[motorA] -= 2140;
  			nMotorEncoder[motorC] -= 2140;
  		}
    }

    motorMemory(motor_A, motor_C, entryCount);
    motor[motorA] = 0;                                                // Stops the robot
    motor[motorC] = 0;
}
//Identifies and moves towards object
//Code can be found in task main but needs to be made compatible with the rest of the functions

//function
void returnToBin(int *motor_A, int *motor_C, int &entryCount)
{
        eraseDisplay();
    displayString(1, "Returning");
for(entryCount; entryCount>=0; entryCount--) // Repeats the path it just took in reverse, beginning with the last entry to the array. All 0 entries will be done instantly.
  {
    resetEncoder();

    if(motor_A[entryCount] > 0)
    {
        motor[motorA] = -50;
    }
    if(motor_A[entryCount] < 0)
    {
        motor[motorA] = 50;
    }
    if(motor_C[entryCount] > 0)
    {
        motor[motorC] = -50;
    }
    if(motor_C[entryCount] < 0)
    {
        motor[motorC] = 50;
    }
    while(fabs(nMotorEncoder[motorC]) < fabs(motor_C[entryCount]) && fabs(nMotorEncoder[motorA]) < fabs(motor_A[entryCount])) {}
    motor[motorA] = 0;
    motor[motorC] = 0;
    }

}

 void objectCount(int &num_objects) //function to select # of objects to be retrieved
{

    num_objects = 0;

    eraseDisplay();
    displayString(1, "Select #Objects");
    displayString(2, "L = -1");
    displayString(3, "R = +1");
    displayString(4, "Red = End");


    while(nNxtButtonPressed != 3)
    {
        while(nNxtButtonPressed == -1){}
        if(nNxtButtonPressed == 2)
        {
            while(nNxtButtonPressed == 2) {}
            num_objects --;
        }
        else if(nNxtButtonPressed == 1)
        {
            while(nNxtButtonPressed == 1) {}
            num_objects ++;
        }
        eraseDisplay();
        displayString(4, "#Objects: %d", num_objects);
    }

    eraseDisplay();
}



void grabObject()
{
    eraseDisplay();
     displayString(1, "Grabbing");
    motor[motorB] = -5;
    wait1Msec(1700);
    motor[motorB] = 0;

}
//Grab object with claw

void releaseObject()
{
    eraseDisplay();
    displayString(1, "Releasing");
    motor[motorB] = 5;
    wait1Msec(1700);
    motor[motorB] = 0;
}
//Releases object from claw


task main()
{

    SensorType[S1] = sensorColorNxtFULL;
    SensorType[S2] = sensorTouch;
  SensorType[S3] = sensorSONAR;

  int motor_A[25];
  int motor_C[25];

    int num_objects = 0;


    objectCount(num_objects);

    for(int j = 0; j < num_objects; j++)
    {

        float objectDistance = 0.0;
        float dangerDistance = 0.0;
        int entryCount = 0;

      initializeMemory(motor_A, motor_C);

        displayString(1, "Push the orange", ); // Promps user to start the program by pushing the orange button.
        displayString(2, "button to begin.", );

      while(nNxtButtonPressed != 3){}
      while(nNxtButtonPressed == 3){}

			time1[0] = 0;
      eraseDisplay();

      resetEncoder(); // Initializes the Motor Encoder to 0.

      exitBin(motor_A, motor_C, entryCount); //Exits the bin

        findObject(motor_A, motor_C, entryCount, objectDistance, dangerDistance); //Finds the object and moves to it

        grabObject();//grips object

        returnToBin(motor_A, motor_C);

        releaseObject();//releases object

        displayString(3, "T = %d ms",time1[0]);
    }

}







/*
Notes:
motorB time = 1700 ms
Idea-->If taken to edge:
    Return to spinning portion of find object
    Again look for objects, but remember the distance of the object that took us to the edge
    If an object is found with that distance +/- 5cm, DO NOT PROCEED
*/
