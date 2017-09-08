void resetEncoder()
{
    nMotorEncoder[motorA] = 0;
    nMotorEncoder[motorC] = 0;
}
//Resets the encoders to 0

void initializeMemory(int *motor_A, int *motor_C)
{
 
    for (int k = 0; k < 25; k++)
    {
        motor_A[k] = 0;
        motor_C[k] = 0;
    }
}
//Initialize the arrays motor_A and motor_C to zero
 
void motorMemory(int *motor_A, int *motor_C, int &entryCount)
{
    motor_A[entryCount] = nMotorEncoder[motorA]; 
    motor_C[entryCount] = nMotorEncoder[motorC];
    entryCount++;
 
    resetEncoder();
}
//Records distance travelled via motor encoder, saves it to the array.
 
void saveRobot(float objectDistance, float &dangerDistance, int *motor_A, int *motor_C, int &entryCount)
{
    eraseDisplay();
    displayString(1, "Saving");
    dangerDistance = objectDistance;
 
    motor[motorA] = -50;
    motor[motorC] = -50;
    
    wait1Msec(2500);
    
    motor[motorA] = 0;
    motor[motorC] = 0;  
}
//Immediately moves robot away from table edge in the case it drives to it
 
void exitBin(int *motor_A, int *motor_C, int &entryCount)
{
    eraseDisplay();
    resetEncoder();
    displayString(1, "Exit");
    motor[motorA] = -50; // Backs robot away from box where it starts.
    motor[motorC] = -50;
    wait1Msec(1000);
    motorMemory(motor_A, motor_C, entryCount);
}
//Pre-determined movement to remove the robot from the bin location
 
 
void findObject(int *motor_A, int *motor_C, int &entryCount,
 float &objectDistance, float &dangerDistance)
{
    eraseDisplay();
    displayString(1, "Finding");
    motor[motorC] = 50;
    motor[motorA] = -50;      
	// Begins the spinning the robot to identify an object.
    while((SensorValue[S3] > 125) && fabs(SensorValue[S3]-dangerDistance) > 3)
    {
 
    } 
	// Looks for objects within range that have not been identified as 'dangers'
    objectDistance = SensorValue[S3];
    motorMemory(motor_A, motor_C, entryCount);
 
    motor[motorA] = 50;
    motor[motorC] = 50;
    while(SensorValue[S1] != 5) 
	// Moves the robot forwards until the colour sensor reads red
    {
        if(SensorValue[S3] > objectDistance+10 && objectDistance != 0) 
		/* The robot will likely be about 30deg off. 
		When it can no longer see the object it will turn until it is once
		again visible.*/
        {
	    	eraseDisplay();
	        displayString(1, "Adjusting");
	        motorMemory(motor_A, motor_C, entryCount);
	        
	        motor[motorA] = -50;
	        time1[0] = 0; 
	        if(nMotorEncoder[motorA] > (2*2140) && nMotorEncoder[motorC] > (2*2140))
	        {
	            nMotorEncoder[motorA] -= 2140;
	            nMotorEncoder[motorC] -= 2140;
	        }
	        /*Due to precision errors with excessive spinning, this stops the robot
	        from completing more than 2 spins on the return trip by limiting the 
	        encoder values*/
	        while(time1[0] < 100)
	        {
	            if(SensorValue[S1] == 5)
	            {
	                break;
	            }
	        }
	        //Stops the spin if the sensor sees an object in claw range
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
	 		break;   
        }
        //runs saveRobot() when table edge is detected
        
       
    }
 
    motorMemory(motor_A, motor_C, entryCount);
    motor[motorA] = 0;                                                // Stops the robot
    motor[motorC] = 0;
}
//Identifies and moves towards object

void returnToBin(int *motor_A, int *motor_C)
{
    eraseDisplay();
    displayString(1, "Returning");
	    
	for(int k = 24; k>=0; k--) 
	/* Repeats the path it just took in reverse, beginning with the last entry 
	to the array. All 0 entries will be done instantly.*/
	{
	    resetEncoder();
	 
	    if(motor_A[k] > 0)
	    {
	        motor[motorA] = -50;
	    }
	    if(motor_A[k] < 0)
	    {
	        motor[motorA] = 50;
	    }
	    if(motor_C[k] > 0)
	    {
	        motor[motorC] = -50;
	    }
	    if(motor_C[k] < 0)
	    {
	        motor[motorC] = 50;
	    }
	    
	    while(fabs(nMotorEncoder[motorC]) < fabs(motor_C[k])) {}
	    motor[motorA] = 0;
	    motor[motorC] = 0;
	}
}
//returns the robot(now with object) to the bin location
 
void objectCount(int &num_objects)
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
 //function to let the user select the number of objects to be retrieved
 
 
void grabObject()
{
    eraseDisplay();
    displayString(1, "Grabbing");
    motor[motorB] = -5;
    wait1Msec(1700);
    motor[motorB] = 0;
}
//Grab object with claw (closes claw)
 
void releaseObject()
{
    eraseDisplay();
    displayString(1, "Releasing");
    motor[motorB] = 5;
    wait1Msec(1700);
    motor[motorB] = 0;
}
//Releases object from claw (opens claw)
 
 
task main()
{
 
    SensorType[S1] = sensorColorNxtFULL;
    SensorType[S2] = sensorTouch;
    SensorType[S3] = sensorSONAR;
 
    int motor_A[25];
    int motor_C[25];
 	//Arrays of encoder values from each motor
 	
    int num_objects = 0; //predetermined number of objects entered by user
 
 
    objectCount(num_objects);
 
    for(int j = 0; j < num_objects; j++)
    {
 
        float objectDistance = 0.0;
        float dangerDistance = 0.0;
        int entryCount = 0; //array index number
 
        initializeMemory(motor_A, motor_C);
  
        displayString(1, "Push the orange", ); 
        displayString(2, "button to begin.", );
        // Promps user to start the program by pushing the orange button.
 
        while(nNxtButtonPressed != 3){}
        while(nNxtButtonPressed == 3){}
 
        time1[0] = 0;
        eraseDisplay();
 
        resetEncoder(); // Initializes the Motor Encoder to 0.
 
        exitBin(motor_A, motor_C, entryCount); //Exits the bin
 
        findObject(motor_A, motor_C, entryCount, objectDistance, dangerDistance); 
		//Finds the object and moves to it
 		
		grabObject();//grips object
 
        returnToBin(motor_A, motor_C);//returns to starting point
 
        releaseObject();//releases object
 
        eraseDisplay();
        displayString(3, "T = 1.0%d s",(time1[0])/ 1000.0); 
		// Displays the time taken in seconds
        wait1Msec(2000);
    }
 
}
 
 
 
 
 
 

