// Function for updating location
// Function for checking special tiles
// Function for initial navigating (from start to exit)
// Function for final navigating (from exit to start)

/* BIG Psuedocode
	
    1. Traverse all, save shortcut along the way, stop at DESTINATION
        while (currentColour == red and currentCoordinate != startCoordinate){
        	
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            1. Identify current state()
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            	- update current coordinate
                	- 
                - update current distance
                - update current color
                - update explored cells
                - update special cells
            
            
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            2. 	Plan_Next_step & orientate()
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                - Use ultrasonic to find free path
                - Direct robot to free path
          

          	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            3. Move()
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                - Move FORWARD 1 step
                	- Feedback loop to adjust two wheels' speed (HARD TO CALIBRATE - LAST STUFF)
                    	* Check distance from 2 sides
                        * Go to the one further from the wall
                    
        }
    
    2. Flash the LED 3 time, 1-1s

	3. Compute the best shortcut, Backtrack
    
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Find optimal back track
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            	- Iterate through all pairs of shortcuts:
            		- Optimal_array = (Distance to that shortcut) - (Shortcut_gap))
                    - NOTE: length array = num of pairs
                - Get the minimum value from optimal_array
                - Get the index
                - Store the "distance" of that shortcut point
                
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // Move 
            // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            	- 180 degree turn
                - NEW FUNCTION to get which array index, base on
                	- cell's "distance" value
                    - current location
                - *kind of* repeat the STEP 1 - identify current state > plan & turn > move
                	- Move toward the OPTIMAL SHORTCUT CELL
                    - Identify color & send BLUETOOTH signal (please, EDEN, get this done correctly)
                    - LED and wait for gate?
                    - Traverse thru - is it the same thing again?
                    	- Go thru a GATE, not go through a FREE PATH
                - Repeat till reach the START
                - Get the money $$$$$$
//////////////////////////////////////////////////////////////////////////////
*/

// Constant
const int START_X = 3;
const int START_Y = 3;
const int MAZE_SIZE = 4
const int SPECIAL_COLORS[] = {'b','g','p','y'};

void setup() {

    /////////////////////////////////////////////////////////////////
  	// Initialisation
	int start_xcoor = START_X; // starting x-cell
    int start_ycoor = START_Y; // starting y-cell
  
    int start_dir = 90; // direction of y axis
    char start_color = 'r'; 

  	/* maze[]:
  			* init with all -1 meaning that it unexplored yet
  			* use to store "distance" values of each CELL (that we have gone through, corresponding to start cell)
    */
    int maze[MAZE_SIZE][MAZE_SIZE]; /////////////WILL HAVE TO CHEKC INITIAL VALUE OF FUNCTION!!!!!!
  
  	/* shortcut_loca[]:
  			* init with all 0
  	 		* use to store special_COLOR_integer_value
    */
    int shortcut_loca[MAZE_SIZE][MAZE_SIZE] = {0}; // 2D array storing cell where there are shortcuts

}

void loop(){
  
  	/////////////////////////////////////////////////////////////////
  	// Polling loop
    int current_xcoor = start_xcoor; // current x-cell
    int current_ycoor = start_ycoor; // current y-cell
    int current_dir = start_dir; 	// current direction of orientation
    char current_color = 0; 		// current colour of cell
  
    // Set starting distance to 0
    maze[current_xcoor][current_ycoor] = 0;

    int cells_checked = 0; // numbers of cells explored
    int current_dist = 0; // the current distance from the most recent explored cell to the start
  
  	////////////////////////////////////////////////////////////////
    // Localisation and Mapping
    ///////////////////////////////////////////////////////////////
  	
  	while (current_color !='r' && current_dist !=0){
      
      	moveForward();							// Move "unlimited" till reach new color (still need define time control inside)
      
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //  1. Identify current state()
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    	if (determineColor() != current_color) // First check if the coordinates have changed since the last check
        {
          //	- update current color
            current_color = determineColor();
          
          //    - update current coordinate
          	current_xcoor += cos(current_dir);	// x-axis is vertical axis
            current_ycoor += sin(current_dir);
            //	- update current distance
            if (maze[current_xcoor,current_ycoor] ==-1)//if cell has been explored before, as a result of a dead end
            {
              current_dist = maze[current_xcoor,current_ycoor];
            }
          	else
            {
            current_dist += 1; 
            
          //	- update explored cells array
          	maze[current_xcoor][current_ycoor] = current_dist;
          //	- update special cells array
          	for (int i; i < 4; i ++) //Go through the special colors array and compare it with current color
            {
            	color = special_colors[i];
              	if (current_color == color){
              		shortcut_loca[current_xcoor][current_ycoor];//Update the special cells array with the current coordinates
                	break;
                }
            }
            }
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //  2. 	Plan_Next_step & orientate
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /*
      	- Use ultrasonic to find free path
        - Direct robot to free path
        	- Not to go to the path already been there --> check the CELL we about to go to
            - ALWAYS turn left
            	- meet dead end, return to intersection
        
          * */
          int index = findNextCell()
          switch(index){
            case 0:
              TurnLeft();	// Left
              break;
            case 1: 		// Forward
                break;
            case 2:
              TurnRight();	// Right
              break;
            case 3:			//all surounding grids are occupied, turn around
              TurnAround();
              break
            default://This should never happen unless something is wrong
              //Something is Wrong
        	}
        }
    }
      
	// 2. Flash the LED 3 time, 1-1s
    blinkGoal(LED_pin, blink_rate);

	// 3. Compute the best shortcut, Backtrack
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Find optimal back track
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  	
  	// Search for index of shortcut_loca
  	int shortcut_pair_color[4][2] = {0};	// Array to store each pair - maximum of 4 pairs for 4 special colors
  											// Follow the order of SPECIAL_COLORS[] = {'b','g','p','y'};
  	int color_idx = -1;						// To store first dimension index for the array above
  	
    
  	// Iterate through the shortcut_loca array, find the index, put that in shortcut_pair_color
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
          	int special_color =  shortcut_loca[i][j];
            // Get distance
            int tmp_distance = maze[i][j];
          
          	// If found non-zero value - mean special colors at that CELL:
          	if (special_color == 0 & tmp_distance ==-1) {
              	
              	// DEAL WITH THE SCENARIO WHERE THERE ARE HIDDEN SHORTCUT ALONG THE WAY
            	// get all adajcent sqaures
              	// check if there is only one unexplored adjecent squares, if yes then it is the other sqaure 
              	// if not then we cannot conclud
              
              	continue;
              	// literally, to do this, we have to go through the shortcut_pair_color array <-- MAYBE LATER, ACCEPT THE ASSUMPTION
              	// literally, to do this, we have to go through the shortcut_pair_color array <-- MAYBE LATER, ACCEPT THE ASSUMPTION
              	// literally, to do this, we have to go through the shortcut_pair_color array <-- MAYBE LATER, ACCEPT THE ASSUMPTION
              	// literally, to do this, we have to go through the shortcut_pair_color array <-- MAYBE LATER, ACCEPT THE ASSUMPTION
              	// literally, to do this, we have to go through the shortcut_pair_color array <-- MAYBE LATER, ACCEPT THE ASSUMPTION
            }
            else if (special_color != 0) {

              	// Update into the "pair array"
                switch (special_color) {
                	case "b": color_idx = 0; break;
                    	break;
                	case "g": color_idx = 1; break;
                    	break;
                    case "p": color_idx = 2; break;
                    	break;
                    case "y": color_idx = 3; break;
                    	break;
                  	default: break; // ignore?
				}
                // check if the current color have any previous stored value
                if (shortcut_pair_color[color_idx][0] == 0)
                    shortcut_pair_color[color_idx][0] = tmp_distance;
                else shortcut_pair_color[color_idx][1] = tmp_distance;
            }
        }
    }
  
  	
  	// Some LENGTHYYYYYYYY math here, sorry, my brain is almost ded...
  	// :((((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))
  
  	// Compute shortest
  	int goal_distance = findMax2D(maze);	// <--------------------- NEED TO DEFINE THIS FUNCTION
	int distance_to_shortcut = 0;
	int shortcut_gap = 0;
  	int pair1, pair2;						// temp value to store "Distance" pair value
  	int eval_array[4];						// array to store step values for evaluation	
    
  	for (int i = 0; i < 4; i++){
      	pair1 = shortcut_pair_color[i][0];
      	pair2 = shortcut_pair_color[i][1];
      	// What if the value is 0?
      	if (pair1 == 0 || pair2 == 0) continue;
      	// Compute:
      	shortcut_gap = abs(pair1 - pair2);
      	// Find the larger shortcut value (between the pair) to find distance_to_shortcut
        if (pair1 > pair2) 	distance_to_shortcut = goal_distance - pair1;
  		else 				distance_to_shortcut = goal_distance - pair2;
      	// Compute
      	eval_array[i] = distance_to_shortcut - shortcut_gap;
    }
  
  	// Find optimal pair to go to (by finding minimum value)
  	int min_idx = 0;					// To store index of the minimum value from eval_array, use this to get the optimal shortcut pair
  	for (int i = 1; i < 4; i++){
  		if (eval_array[i] < eval_array[min_idx]){
          	min_idx = i;
        }
    }
  	
    // Lengthy math ...
  	pair1 = shortcut_pair_color[min_idx][0];					// Get these again to find shortcut_gap
    pair2 = shortcut_pair_color[min_idx][2];
  	shortcut_gap = abs(pair1 - pair2);
  
  	// Get the number of steps needed to go to the optimal shortcut CELL:
  	int number_steps_to_shortcut = eval_array[min_idx] + shortcut_gap 	// "reverse" the equation to find eval_array above
    // Get the distance value of the optiamal shortcut CELL <--- use this as the target to BACK TRACK?
  	int shortcut_distance = goal_distance - step_to_shortcut	

                
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Move - BACK TRACK 	<------------ We can reuse 1. Identify current state() & 3. Move() <------------ from above
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    rotate180();
  
  	// Go to the optimal SHORTCUT CELL
  	while (current_dist != shortcut_distance){
      
      	moveForward();							// Move "unlimited" till reach new color (still need define time control inside)
      
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //  1. Identify current state()
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    	if (determineColor() != current_color) // First check if the coordinates have changed since the last check
        {
          //	- update current color
            current_color = determineColor();
          //    - update current coordinate
          	current_xcoor += cos(current_dir);
            current_ycoor += sin(current_dir);
          //	- update current distance
            current_dist -= 1;						// Back track now, use -1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //  2. 	Plan_Next_step & orientate
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        /*
    	- dont need ultrasonic
        - scan (in the program) adjacent block to find the lower DISTANCE VALUE block        
          * */
      	int distance_adj_scan[4] = {0};
        if (current_xcoor >= 0 && current_xcoor < MAZE_SIZE && current_ycoor >= 0 && current_ycoor < MAZE_SIZE) { // ALSO check if it the wall
            // Top
            distance_adj_scan[0] = (current_xcoor <  MAZE_SIZE - 1) ? maze[current_xcoor + 1][current_ycoor] : 0;
            // Right
            distance_adj_scan[1] = (current_ycoor < MAZE_SIZE - 1) ? armazeray[current_xcoor][current_ycoor + 1] : 0;
            // Bottom
            distance_adj_scan[2] = (current_Xcoor > 0) ? maze[current_xcoor - 1][current_ycoor] : 0;
            // Left 
            distance_adj_scan[3] = (current_ycoor > 0) ? maze[current_xcoor][current_ycoor - 1] : 0;
        } else {
            // Invalid row or column, set all values to 0
            for (int i = 0; i < 4; i++) {
                distance_adj_scan[i] = 0;
            }
        }
      
      	// SCAN FOR THE NEXT LOWER DISTANCE
      	int turning_idx = 0;
        for (int i = 0; i < 4; i++) {
            if (distance_adj_scan[i] == current_dist-1){
            	turning_idx = i
            }
        }
      	// Get the current direction, then find which direction to turn
      	turn_degree = current_dir - (turning_idx * 90);	// <-------- visualisation :)
      	turn_degree = validateDir(turn_degree);
      	
        switch(turn_degree){
          case 0:		break; 					// No turn
          case 90:		rotateRight; break;		// Right
          case 180:		rotate180; break;		// Turn around
          case 270:		rotateLeft; break;		// Right
          default://This should never happen unless something is wrong
          }
  
      
      
    }
  
  	// SEND BLUETOOTH SIGNAL using current_color
  
  	//  NAVIGATE THRU THE OPEN GATE !!!!!!!!!!!! <-- do it later, make sure it can actually TRACK BACK
  
	// end of void loop - GOOD WORK :)
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HELPER FUNCTIONNNNNNNNNNNNNN
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void findNextCell()
{
  int* FreeCells = findFreeCells();
  int index = 0;
  for(int cell : FreeCells)
  {
    if (cell == 1)
    {
      return index;
    }
    index++;
  }
  return index;
}
  
int* findFreeCells()
{
  // A function supposed to get the readings from the three ultrasonic sensors and compare it with a threshold value to determine whether there is a wall or free space in each direction
  // This function outputs a 1x3 array, with values of 0 and 1 for each element, 1 for availabel (no Wall) and 0 for unavaialble(Wall), the three values correspond to [left,front,right]
  float threshold = 40.0;
  float* distances = all_ultra()
  int availability[3]
  for (int i =0; i<2; i++)
  {
    if (distances[i] > threshold)
    {
      availability[i] = 1;
    }
    else
    {
      availability[i] = 0;
    }
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void printUltra(float distance[], int trig[], int echo[], int polling_rate){
  	
    // Set up variables
    int sensor_count = sizeof(distance) / sizeof(distance[0]);
    // Get inputs
    for (int i = 0; i < sensor_count; i++) {
        distance[i] = ultra(trig[i], echo[i]);
        delay(20);
    }
    // Print:
    Serial.print("The distance:\t");
    for (int i = 0; i < sensor_count; i++){
        Serial.print(distance[i]);
        Serial.print("\t\t");
    }
    Serial.print("\n");
    // Wait between each measurement
    delay(polling_rate);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float* all_ultra(int trigPins[3],int echoPins[3])//This probably should be within the the ultrasonic file, but I am not sure 
{
  //go through all sensors from left right
  float distances[3];
  for (int i; i<3;i++)
  {
    distances[i] = ultra[trigPins[i],echoPins[i]];
  }
  return distances;
}
float ultra(int trigPin, int echoPin) {
    long duration;
    int distance;

    // Clears the trigPin.
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds.
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds.
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance.
    distance = duration * 0.034 / 2; //output as cm

    return distance;
}

  
// --------------------------------------------------
// HELPER FUNCTION FOR MOVING - MOTOR

int validateDir(int dir){
	if 		(dir > 360) return dir - 360;
  	else if (dir < 0) 	return dir + 360;
	else 				return dir;
}

void rotateLeft(){
	current_dir += 90;
	current_dir = validateDir(current_dir);
}

void rotateRight(){
  	current_dir -= 90;
	current_dir = validateDir(current_dir);
}

void rotate180(){
  	current_dir -= 180;
	current_dir = validateDir(current_dir);
}

void moveForward(){
  	
}

void moveBackward(){

}

// --------------------------------------------------

char determineColor(){
	color = 'b'
    return color
}


/// BLUETOOTH
#define RX 3
#define TX 2
#define BAUDRATE 9600

char c = ' ';
boolean new_line = true;
String message = "Hello World";

//////////////////////////////////////
// Bluetooth module 
// THIS PART IS JUST TO CONNECT TO OTHER DEVICES AND INTERACT THE WITH THE BLUETOOTH

// Require some initial SETUP <---------------------------- TO DO

// #include <SoftwareSerial.h>

// void setup() {  

// 	 SoftwareSerial BTSerial(RX, TX); // (RX, TX)
//   // Start Serial Monitor for feedback
//   Serial.begin(BAUDRATE);
  
//   // HM-10 default speed in AT command mode
//   BTSerial.begin(BAUDRATE);
  
//   Serial.println("Enter AT commands:");
// }

void loop() {
  
  // Keep reading from HM-10 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HM-10
  if (Serial.available()) {

    // Read from the Serial buffer (from the user input)
    c = Serial.read();

    // Do not send newline ('\n') nor carriage return ('\r') characters
    if(c != 10 && c != 13)
      BTSerial.write(c);

    // If a newline ('\n') is true; print newline + prompt symbol; toggle
    if (new_line) { 
      Serial.print("\r\n>");
      new_line = false;
    }

    // Write to the Serial Monitor the bluetooth's response
    Serial.write(c);
    
    // If a newline ('\n') is read, toggle
    if (c == 10)
      new_line = true;
  }
}

/// THIS PART IS TO SEND THE MESSAGE
void sendMessage() {
  
// Send a string for the bluetooth module to transmit 
  	BTSerial.print(message); 
  
// Print the same string to the Serial Monitor for feedback 
//  	Serial.println(message); 
	delay(1000); 
}



    