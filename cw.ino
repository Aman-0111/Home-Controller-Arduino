#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//#define DEBUG

void setup() {
  //Setup code:
lcd.begin(16,2);
lcd.setBacklight(7);
Serial.begin(9600);
Serial.println(F("ENHANCED: LAMP,OUTSIDE,QUERY"));
}

//Structure for lamps 
typedef struct lamp_s{
  //Stores location and action values
  const char* location;
  float on_time;
  float off_time;
  int8_t lvl;
} lamp_t; //Defining the lamp type

//Structure for rooms
typedef struct room_s {
  //Stores room name and light and heat values
  const char* r_name;
  //Pointers to lamps
  lamp_t *lamp1;
  lamp_t *lamp2;
  float light_on_time;
  float light_off_time;
  float heat_on_time;
  float heat_off_time;
  int8_t light_lvl;
  int8_t heat_lvl;
} room_t; //Defining the room type

//Structure for floor
typedef struct floor_s {
  //Stores floor name and room pointers
  const char* f_name;
  room_t *room_1;
  room_t *room_2;
  room_t *room_3;
} floor_t; //Defining the floor type

//Structure to house
typedef struct house_s {
  //Stores pointers to floors
  floor_t *first;
  floor_t *ground;
  floor_t *outside;
} house_t; //Defining house type

/* Macro used to display the house structure to serial monitor.
 * Loops are used with the get_room(), get_floor() and get_lamp() macros
 * to display data about each room.
 */
#define display_data() ({\
for (int8_t i = 0 ; i<2 ; i++){\
  get_floor();\
  for (int8_t c = 0 ; c<3 ; c++){\
    get_room();\
    Serial.print((*floor_p).f_name);\
    Serial.print(F("/"));\
    Serial.print((*room_p).r_name);\
    Serial.print(F("/Heat/Main/On:"));\
    if ((*room_p).heat_on_time < 10){\
      Serial.println("0"+(String) (*room_p).heat_on_time);\
    }else{\
      Serial.println((String) (*room_p).heat_on_time);\
    }\
    Serial.print((*floor_p).f_name);\
    Serial.print(F("/"));\
    Serial.print((*room_p).r_name);\
    Serial.print(F("/Heat/Main/Off:"));\
    if ((*room_p).heat_off_time < 10){\
      Serial.println("0"+(String) (*room_p).heat_off_time);\
    }else{\
      Serial.println((String) (*room_p).heat_off_time);\
    }\
    Serial.print((*floor_p).f_name);\
    Serial.print(F("/"));\
    Serial.print((*room_p).r_name);\
    Serial.print(F("/Heat/Main/Level:"));\
    Serial.println((*room_p).heat_lvl);\
    Serial.print((*floor_p).f_name);\
    Serial.print(F("/"));\
    Serial.print((*room_p).r_name);\
    Serial.print(F("/Light/Main/On:"));\
    if ((*room_p).light_on_time < 10){\
      Serial.println("0"+(String) (*room_p).light_on_time);\
    }else{\
      Serial.println((String) (*room_p).light_on_time);\
    }\
    Serial.print((*floor_p).f_name);\
    Serial.print(F("/"));\
    Serial.print((*room_p).r_name);\
    Serial.print(F("/Light/Main/Off:"));\
    if ((*room_p).light_off_time < 10){\
      Serial.println("0"+(String) (*room_p).light_off_time);\
    }else{\
      Serial.println((String) (*room_p).light_off_time);\
    }\
    Serial.print((*floor_p).f_name);\
    Serial.print(F("/"));\
    Serial.print((*room_p).r_name);\
    Serial.print(F("/Light/Main/Level:"));\
    Serial.println((*room_p).light_lvl);\
    for (int8_t x = 0; x<2;x++){\
      get_lamp();\
      Serial.print((*floor_p).f_name);\
      Serial.print(F("/"));\
      Serial.print((*room_p).r_name);\
      Serial.print(F("/Lamp/"));\
      Serial.print((*lamp_p).location);\
      Serial.print(F("/On:"));\
      if ((*lamp_p).on_time < 10){\
        Serial.println("0"+(String) (*lamp_p).on_time);\
      }else{\
        Serial.println((String) (*lamp_p).on_time);\
      }\
      Serial.print((*floor_p).f_name);\
      Serial.print(F("/"));\
      Serial.print((*room_p).r_name);\
      Serial.print(F("/Lamp/"));\
      Serial.print((*lamp_p).location);\
      Serial.print(F("/Off:"));\
      if ((*lamp_p).off_time < 10){\
        Serial.println("0"+(String) (*lamp_p).off_time);\
      }else{\
        Serial.println((String) (*lamp_p).off_time);\
      }\
      Serial.print((*floor_p).f_name);\
      Serial.print(F("/"));\
      Serial.print((*room_p).r_name);\
      Serial.print(F("/Lamp/"));\
      Serial.print((*lamp_p).location);\
      Serial.print(F("/Level:"));\
      Serial.println((*lamp_p).lvl);\
    }\
  }\
}\
floor_p = (*house_p).outside;\
room_p = (*floor_p).room_1;\
Serial.print((*floor_p).f_name);\
Serial.print(F("/"));\
Serial.print((*room_p).r_name);\
Serial.print(F("/Water/Main/On:"));\
if ((*room_p).light_on_time < 10){\
  Serial.println("0"+(String) (*room_p).light_on_time);\
}else{\
  Serial.println((String) (*room_p).light_on_time);\
}\
Serial.print((*floor_p).f_name);\
Serial.print(F("/"));\
Serial.print((*room_p).r_name);\
Serial.print(F("/Water/Main/Off:"));\
if ((*room_p).light_off_time < 10){\
  Serial.println("0"+(String) (*room_p).light_off_time);\
}else{\
  Serial.println((String) (*room_p).light_off_time);\
}\
room_p = (*floor_p).room_2;\
Serial.print((*floor_p).f_name);\
Serial.print(F("/"));\
Serial.print((*room_p).r_name);\
Serial.print(F("/Light/Main/On:"));\
if ((*room_p).light_on_time < 10){\
  Serial.println("0"+(String) (*room_p).light_on_time);\
}else{\
  Serial.println((String) (*room_p).light_on_time);\
}\
Serial.print((*floor_p).f_name);\
Serial.print(F("/"));\
Serial.print((*room_p).r_name);\
Serial.print(F("/Light/Main/Off:"));\
if ((*room_p).light_off_time < 10){\
  Serial.println("0"+(String) (*room_p).light_off_time);\
}else{\
  Serial.println((String) (*room_p).light_off_time);\
}\
Serial.print((*floor_p).f_name);\
Serial.print(F("/"));\
Serial.print((*room_p).r_name);\
Serial.print(F("/Light/Main/Level:"));\
Serial.println((*room_p).light_lvl);\
})


//Macro to direct floor pointer to the correct floor
#define get_floor() ({\
switch(i){\
case 0: floor_p = (*house_p).first;break;\
case 1: floor_p = (*house_p).ground;break;\
}\
})

//Macro to direct room pointer to the correct room
#define get_room() ({\
switch(c){\
case 0: room_p = (*floor_p).room_1;break;\
case 1: room_p = (*floor_p).room_2;break;\
case 2: room_p = (*floor_p).room_3;break;\
}\
})

//Macro to direct lamp pointer to the correct lamp
#define get_lamp() ({\
switch(x){\
case 0: lamp_p = (*room_p).lamp1;break;\
case 1: lamp_p = (*room_p).lamp2;break;\
}\
})

/*Macro is used to clear the lcd screen when the level is selected
 * Type casts the level into a string
 * Uses the level to create a string of only " "
 * Prints the eraser string to cover the level on display
 */
#define clearLevelSelection() ({\
lcd.setCursor(current_selection.length(),0);\
String selection = (String) lvl_selection;\
String eraser = " ";\
for (int8_t i = 0; i < selection.length(); i++){\
eraser += " ";\
}\
lcd.print(eraser);\
selection = "";\
lcd.setCursor(current_selection.length(),0);\
})

/*Macro is used to clear the lcd screen when the on or off time is selected
 * Type casts the time into a string
 * Uses the time to create a string of only " "
 * Prints the eraser string to cover the level on display
 */
#define clearTimeSelection() ({\
lcd.setCursor(current_selection.length(),0);\
String selection = (String) time_selection;\
String eraser = " ";\
for (int8_t i = 0; i < selection.length(); i++){\
eraser += " ";\
}\
lcd.print(eraser);\
selection = "";\
lcd.setCursor(current_selection.length(),0);\
})

//Macros to scroll the text
#define scrollRight()  {lcd.scrollDisplayLeft();scroll_pos++; delay(400);}
#define scrollLeft()  {lcd.scrollDisplayRight();scroll_pos--; delay(400);}

void loop() {
//Creates lamps to place in each room
static lamp_t lamp1 = {"Mirror",0.0,0.0,0};
static lamp_t lamp2 = {"Wall",0.0,0.0,0};
static lamp_t lamp3 = {"Desk",0.0,0.0,0};
static lamp_t lamp4 = {"Bed",0.0,0.0,0};
static lamp_t lamp5 = {"Mirror",0.0,0.0,0};
static lamp_t lamp6 = {"Bed",0.0,0.0,0};
static lamp_t lamp7 = {"Table",0.0,0.0,0};
static lamp_t lamp8 = {"Shelf",0.0,0.0,0};
static lamp_t lamp9 = {"Wall",0.0,0.0,0};
static lamp_t lamp10 = {"Shelf",0.0,0.0,0};
static lamp_t lamp11 = {"Table",0.0,0.0,0};
static lamp_t lamp12 = {"TV",0.0,0.0,0};
//Creates a filler lamp to place in the rooms where no lamps exist
static lamp_t filler_lamp = {"",0.0,0.0,0};
//Creates all the rooms with the lamps above
static room_t bathroom = {"Bathroom",&lamp1,&lamp2,0.0,0.0,0.0,0.0,0,0};
static room_t bedroom1 = {"Bedroom1",&lamp3,&lamp4,0.0,0.0,0.0,0.0,0,0};
static room_t bedroom2 = {"Bedroom2",&lamp5,&lamp6,0.0,0.0,0.0,0.0,0,0};
static room_t kitchen = {"Kitchen",&lamp7,&lamp8,0.0,0.0,0.0,0.0,0,0};
static room_t hall = {"Hall",&lamp9,&lamp10,0.0,0.0,0.0,0.0,0,0};
static room_t living_room = {"Living room",&lamp11,&lamp12,0.0,0.0,0.0,0.0,0,0};
static room_t garden = {"Garden",&filler_lamp,&filler_lamp,0.0,0.0,0.0,0.0,0,0};
static room_t garage = {"Garage",&filler_lamp,&filler_lamp,0.0,0.0,0.0,0.0,0,0};
//filler room to use in the outside room
static room_t filler_room = {"",&filler_lamp,&filler_lamp,0.0,0.0,0.0,0.0,0,0};
//Creates the floors with the rooms above
static floor_t first_floor = {"First Floor",&bathroom,&bedroom1,&bedroom2};
static floor_t ground_floor = {"Ground Floor",&kitchen,&hall,&living_room};
static floor_t outside = {"Outside",&garden,&garage,&filler_room};
//Creates the house with the floors
static house_t house = {&first_floor,&ground_floor,&outside};
//Creates pointers for each type of structure
static lamp_t *lamp_p;
static room_t *room_p;
static floor_t *floor_p;
static house_t *house_p = &house;
//level and time a pointers to change values in the structure
static float *time_p;
static int8_t *lvl_p;
//cursor stores which part of the menu the user is in e.g: floor,room,type,etc
static int8_t cursor_pos = 0;
//points to the diffrent options in each part of the menu e.g: first,ground,outside
static int8_t menu_pos = 0;
//scroll variables are used to control the scrolling text and prevent it from scrolling past the limit.
static int8_t scroll_pos = 0;
static int8_t scroll_left_limit = 0;
static int8_t scroll_limit_index = 23;
static bool scroll_limit_reached = false;
//selection variables hold what the user has selected so far in the program
static String current_selection = "";
static String floor_selection = "";
static String room_selection = "";
static String device_selection = "";
static String name_selection = "";
static String action_selection = "";
static float time_selection = 00.00;
static int8_t lvl_selection = 0;
//used to control the incrementation of the time
static int minutes = 0;
//bools used to store whether a button is pressed
bool up;
check_up(&up);
bool down;
check_down(&down);
bool left;
check_left(&left);
bool right;
check_right(&right);
bool select;
check_select(&select);
//Switch case with cursor_pos to control the lcd display
switch (cursor_pos){
  case 0:
    /*if statments used to display a floor based on the menu_pos
     *the floor_p pointer and floor selection is also set based on the menu_pos
     */
    if ((menu_pos == 0) && !(floor_selection == "Ground")) {
      lcd.clear();
      lcd.print(F("Ground"));
      floor_selection = "Ground";
      floor_p = (*house_p).ground;
    }
    if ((menu_pos == 1) && !(floor_selection == "First")){
      lcd.clear();
      lcd.print(F("First"));
      floor_selection = "First";
      floor_p = (*house_p).first;
    }
    if ((menu_pos == 2) && !(floor_selection == "Outside")){
      lcd.clear();
      lcd.print(F("Outside"));
      floor_selection = "Outside";
      floor_p = (*house_p).outside;
    }
    if ((menu_pos == 3) && !(floor_selection == "Serial Output")){
      lcd.clear();
      lcd.print(F("Serial Output"));
      floor_selection = "Serial Output";
    }
    //Up and Down buttons increment and decrement menu_pos
    if (up && !(menu_pos == 3)){
      menu_pos++;
    } else if (down && !(menu_pos == 0)){
      menu_pos--;
    //Right is used to select the floor
    } else if (right){
      //Displays data to serial monitor is serial output was selected
      if (floor_selection == "Serial Output"){
        display_data();
      } else{
        //adds the floor to the current selection and sets cursor and menu_pos for next case
        current_selection += floor_selection;
        cursor_pos++;
        menu_pos = 0;
        //Scrolls text using the length of the floor selected
        for(int8_t i = 0; i<floor_selection.length();i++){
          scrollRight()
        }
      }
    }
    break;
  case 1:
    /*if statments used to display a room based on the menu_pos and floor
     *the room_p pointer and room selection is also set based on the menu_pos
     */
    if (floor_selection == "First"){
      if ((menu_pos == 0) && !(room_selection == "bathroom")){
        lcd.setCursor(current_selection.length(),0);
        lcd.print(F("/Bathroom"));
        room_selection = "/bathroom";
        room_p = (*floor_p).room_1;
      }
      if ((menu_pos == 1) && !(room_selection == "bedroom1")){
        lcd.setCursor(current_selection.length(),0);
        lcd.print(F("/Bedroom1"));
        room_selection = "/bedroom1";
        room_p = (*floor_p).room_2;
      }
      if ((menu_pos == 2) && !(room_selection == "bedroom2")){
        lcd.setCursor(current_selection.length(),0);
        lcd.print(F("/Bedroom2"));
        room_selection = "/bedroom2";
        room_p = (*floor_p).room_3;
      }
    }if (floor_selection == "Ground"){      
      if ((menu_pos == 0) && !(room_selection == "/kitchen")){
        //clear_selection function used to remove any previously selected room due to diffrent room lengths
        room_selection = clear_selection(current_selection,room_selection);
        lcd.print(F("/Kitchen"));
        room_selection = "/kitchen";
        room_p = (*floor_p).room_1;
      }
      if ((menu_pos == 1) && !(room_selection == "/hall")){
        room_selection = clear_selection(current_selection,room_selection);
        lcd.print(F("/Hall"));
        room_selection = "/hall";
        room_p = (*floor_p).room_2;
      }
      if ((menu_pos == 2) && !(room_selection == "/living room")){
        room_selection = clear_selection(current_selection,room_selection);
        lcd.print(F("/Living room"));
        room_selection = "/living room";
        room_p = (*floor_p).room_3;
      }
    }
    if (floor_selection == "Outside"){
      if ((menu_pos == 0) && !(room_selection == "/garden")){
        lcd.setCursor(current_selection.length(),0);
        lcd.print(F("/Garden"));
        room_selection = "/garden";
        room_p = (*floor_p).room_1;
      }
      if ((menu_pos == 1) && !(room_selection == "/garage")){
        lcd.setCursor(current_selection.length(),0);
        lcd.print(F("/Garage"));
        room_selection = "/garage";
        room_p = (*floor_p).room_2;
      }
    }
    //changes the menu_pos incrementing limit as outside has one less room than other floors
    if (floor_selection == "Outside"){
      if (up && !(menu_pos == 1)){
        menu_pos++;
      } else if (down && !(menu_pos == 0)){
        menu_pos--;
      } 
    } else {
      if (up && !(menu_pos == 2)){
        menu_pos++;
      } else if (down && !(menu_pos == 0)){
        menu_pos--;
      } 
    } 
    
    if (right){
      //button has same functionality as one in the first case
      current_selection += room_selection;
      cursor_pos++;
      menu_pos = 0;
      for(int8_t i = 0; i<room_selection.length();i++){
        scrollRight()
      }
    } else if (left) {
      //changes cursor and menu pos to return to previous selection
      cursor_pos--;
      menu_pos = 0;
      //Uses the floor_selection to scroll back the the left
      for(int8_t i = 0; i<floor_selection.length();i++){
        scrollLeft()
      }
      //clears room_selection and current_selection
      room_selection = clear_selection(current_selection,room_selection);
      current_selection = "";
    }
    break;
  case 2:
    //Displays specific devices dues to limited types in outside rooms
    if (room_selection == "/garden"){
      if ((menu_pos == 0) && !(device_selection == "/water")){
        device_selection = clear_selection(current_selection,device_selection);
        lcd.print(F("/Water"));
        device_selection = "/water";
      }
    } else if (room_selection == "/garage"){
      if ((menu_pos == 0) && !(device_selection == "/light")){
        device_selection = clear_selection(current_selection,device_selection);
        lcd.print(F("/Light"));
        device_selection = "/light";
      }
    } else{
      //All other rooms will display devices based on menu_pos in the if statments below
      if ((menu_pos == 0) && !(device_selection == "/light")){
        device_selection = clear_selection(current_selection,device_selection);
        lcd.print(F("/Light"));
        device_selection = "/light";
      }
      if ((menu_pos == 1) && !(device_selection == "/heat")){
        device_selection = clear_selection(current_selection,device_selection);
        lcd.print(F("/Heat"));
        device_selection = "/heat";
      }
      if ((menu_pos == 2) && !(device_selection == "/lamp")){
        device_selection = clear_selection(current_selection,device_selection);
        lcd.print(F("/Lamp"));
        device_selection = "/lamp";
      }
    }   
    // The set of if statments belows have the same functionaity as the ones in the previous case
    if (up && !(menu_pos == 2)){
      menu_pos++;
    } else if (down && !(menu_pos == 0)){
      menu_pos--;
    } else if (right){
      current_selection += device_selection;
      cursor_pos++;
      menu_pos = 0;
      for(int8_t i = 0; i<device_selection.length();i++){
        scrollRight()
      }
    } else if (left){
      cursor_pos--;
      menu_pos = 0;
      for(int8_t i = 0; i<room_selection.length();i++){
        scrollLeft();
      }
      device_selection = clear_selection(current_selection,device_selection);
      current_selection = floor_selection;
    }
    break;
  case 3:
    //if the device is not a lamp then only /Main is displayed
    if (!(device_selection == "/lamp")){
      if ((menu_pos == 0) && !(name_selection == "/Main")){
        name_selection = clear_selection(current_selection,name_selection);
        lcd.print(F("/Main"));
        name_selection = "/Main";
      }
    } else{
      //When lamp is selected the room_p is used to obtain the locations of the lamps in the room
      lamp_p = (*room_p).lamp1;
      const char* location1 = (*lamp_p).location;
      lamp_p = (*room_p).lamp2;
      const char* location2 = (*lamp_p).location;
      /*These locations are used with menu_pos to allow the user to select a specific lamp
       * A lamp_p and name selection is also based on the menu_pos and location
       */
      if ((menu_pos == 0) && !(name_selection == ("/" + (String) location1))){
        name_selection = clear_selection(current_selection,name_selection);
        lcd.print(F("/"));
        lcd.print(location1);
        lamp_p = (*room_p).lamp1;
        name_selection ="/" + (String) location1;
      }
      if ((menu_pos == 1) && !(name_selection == ("/" + (String) location2))){
        name_selection = clear_selection(current_selection,name_selection);
        lcd.print(F("/"));
        lcd.print(location2);
        lamp_p = (*room_p).lamp2;
        name_selection ="/" + (String) location2;
      }
      //Up and Down functionality is only implemented if the device is lamp
      if (up && !(menu_pos == 2)){
        menu_pos++;
      } else if (down && !(menu_pos == 0)){
        menu_pos--;
      }
    }
    if (right){
      //right has same functionality with an extra step
      current_selection += name_selection;
      cursor_pos++;
      menu_pos = 0;
      for(int8_t i = 0; i < name_selection.length();i++){
        //If statment used with functions to make sure scroll limit is not exceeded
        if (ScrollCheck(scroll_pos,scroll_limit_index)){
          scroll_left_limit = ScrolledRight(i,name_selection.length());
          scroll_limit_reached = true;
          break;
        }
        scrollRight();
      }
    } else if (left){
      //Left has same functionality as previous cases
      cursor_pos--;
      menu_pos = 0;
      for(int8_t i = 0; i<device_selection.length();i++){
        scrollLeft();
      }
      name_selection = clear_selection(current_selection,name_selection);
      current_selection = floor_selection + room_selection;
    }
    break;
  case 4:
    //Menu_pos used to select the action
    if ((menu_pos == 0) && !(action_selection == "/On")){
      action_selection = clear_selection(current_selection,action_selection);
      lcd.print(F("/On"));
      action_selection = "/On";
      //Sets the time_p to point at a location that stores time based on room and device
      if (device_selection == "/heat"){
        time_p = &((*room_p).heat_on_time);
      //Water uses the same variables as light as there is no light device in the garden
      } else if ((device_selection == "/light") or (device_selection == "/water")){
        time_p = &((*room_p).light_on_time);
      } else {
        time_p = &((*lamp_p).on_time);
      }
    } else if ((menu_pos == 1) && !(action_selection == "/Off")){
      action_selection = clear_selection(current_selection,action_selection);
      lcd.print(F("/Off"));
      action_selection = "/Off";
      //Same principle as the set of if statmentments above
      if (device_selection == "/heat"){
        time_p = &((*room_p).heat_off_time);
      } else if ((device_selection == "/light") or (device_selection == "/water")){
        time_p = &((*room_p).light_off_time);
      } else {
        time_p = &((*lamp_p).off_time);
      }
    }else if ((menu_pos == 2) && !(action_selection == "/Level")){
      action_selection = clear_selection(current_selection,action_selection);
      lcd.print(F("/Level"));
      action_selection = "/Level";
      //Same principle as the set of if statments above except water has no level
      if (device_selection == "/heat"){
        lvl_p = &((*room_p).heat_lvl);
      } else if (device_selection == "/light"){
        lvl_p = &((*room_p).light_lvl);
      } else {
        lvl_p = &((*lamp_p).lvl);
      }
    }
    //limits menu_pos incrementing as water has no level
    if (device_selection == "/water"){
      if (up && !(menu_pos == 1)){
        menu_pos++;
      } else if (down && !(menu_pos == 0)){
        menu_pos--;
      }
    } else {
      if (up && !(menu_pos == 2)){
        menu_pos++;
      } else if (down && !(menu_pos == 0)){
        menu_pos--;
      }    
    } 
    //Right has the same functionality as it does in the previous case
    if (right){
      action_selection += ":";
      current_selection += action_selection;
      lcd.print(F(":"));     
      menu_pos = 0;
      cursor_pos++;
      if (!(scroll_limit_reached)){
        for(int i = 0; i < action_selection.length();i++){
          if (ScrollCheck(scroll_pos,scroll_limit_index)){
            scroll_left_limit = ScrolledRight(i,action_selection.length());
            scroll_limit_reached = true;
            break;
          }
          scrollRight();
        }
      } else{
        //This helps with the next case
        scroll_limit_reached = false;
      }
    } else if (left){
      //Changes the variables in the same way as previous cases
      cursor_pos--;
      menu_pos = 0;
      //if the scroll limit was reached then the left scroll will be limited
      if ((scroll_limit_reached) && !(scroll_left_limit == 0)){
        for(int8_t i = 0; i<(name_selection.length() - scroll_left_limit);i++){
          scrollLeft();
        }
        scroll_limit_reached = false;
      }else{
        //Same loop as in previous cases
        for(int8_t i = 0; i<name_selection.length();i++){
          scrollLeft();
        }
      }
      action_selection = clear_selection(current_selection,action_selection);
      current_selection = floor_selection + room_selection + device_selection;
    }
    break;
  case 5:
    //Displays time controls
    if ((action_selection == "/On:") || (action_selection == "/Off:")){
      if (up){
        //Creates an upper limit to time
        if (time_selection < 23.59){
          clearTimeSelection();
          //Uses minutes and modulus to control time selection in the correct format
          #ifdef DEBUG
          minutes += 15;
          if (!(minutes%60 == 0)){
            time_selection += 0.15;
          } else {
            time_selection -= 0.45;
            time_selection += 1;
          }
          //Prevents upper limit being exceeded
          if (time_selection > 23.59){
            time_selection = 23.45;
          }
          #else
          minutes ++;
          if (!(minutes%60 == 0)){
            time_selection += 0.01;
          } else {
            time_selection -= 0.59;
            time_selection += 1;
          }
          //Prevents upper limit being exceeded
          if (time_selection > 23.59){
            time_selection = 23.59;
          }
          #endif
          //Displays time in correct format
          if (time_selection < 10){
            lcd.print("0"+(String)time_selection);
          }else{
            lcd.print(time_selection); 
          }
        }
      } else if (down){
        //Creates a lower limit to time
        if (time_selection > 0.00){
          clearTimeSelection();
          //Uses minutes and modulus to control time selection in the correct format
          #ifdef DEBUG
          minutes -= 15;
          if (!(minutes%60 == 45)){
            time_selection -= 0.15;
          } else{
            time_selection += 0.45;
            time_selection -= 1;          
          }
          #else
          minutes --;
          if (!(minutes%60 == 59)){
            time_selection -= 0.01;
          } else{
            time_selection += 0.59;
            time_selection -= 1;          
          }
          #endif
          //Prevents time going below lower limit
          if (time_selection < 0){
            time_selection = 0.00;
          }
          //Displays time in correct format
          if (time_selection < 10){
            lcd.print("0"+(String)time_selection);
          }else{
            lcd.print(time_selection); 
          }
        }
      }
    //Displays level controls
    } else if (action_selection == "/Level:"){
      //Up and down increment and decrement level
      if (up){
        if (lvl_selection < 100){
          clearLevelSelection();
          #ifdef DEBUG
          lvl_selection += 5;
          #else
          lvl_selection++;
          #endif
          lcd.print(lvl_selection);
        }
      } else if (down){
        if (lvl_selection > 0){
          clearLevelSelection();
          #ifdef DEBUG
          lvl_selection -= 5;
          #else
          lvl_selection--;
          #endif
          lcd.print(lvl_selection);
        }
      }
    }
    //Right will use the time or level pointers to set the new time/level in the selected room or lamp
    if (right){
      if ((action_selection == "/On:") || (action_selection == "/Off:")){
        *time_p = time_selection;
      } else {
        *lvl_p = lvl_selection;
      }
      #ifdef DEBUG
      Serial.print(current_selection);
      if ((action_selection == "/On:") || (action_selection == "/Off:")){
        if (time_selection < 10){
            Serial.println("0"+(String)time_selection);
          }else{
            Serial.println(time_selection); 
          }
      } else{
        Serial.println(lvl_selection);
      }
      #endif
    } else if (left){
      //Resets variables like previous cases
      minutes = 0;
      cursor_pos--;
      menu_pos = 0;
      //If scroll limit was reached in the menu before this then scroll left will be used
      if ((scroll_limit_reached) && !(scroll_left_limit == 0)){
        for(int i = 0; i<(action_selection.length() - scroll_left_limit);i++){
          scrollLeft();
        }
        scroll_limit_reached = false;
      //Sets scroll limit to true as scroll limit was reached in the menu before the previous one
      }else{
        scroll_limit_reached = true;
      }
      //clears time or level
      if ((action_selection == "/On:") || (action_selection == "/Off:")){
        clearTimeSelection();
      } else{
        clearLevelSelection();
      }
      current_selection = floor_selection + room_selection + device_selection + name_selection;
      time_selection = 00.00;
      lvl_selection = 0;
    }
}
//Will display all data to serial monitor if a Q followed by a new line is recieved
if (Serial.available() > 0){
  if (Serial.readString() == "Q\n"){
    display_data();
  }
}
//If select is pressed at any point the user will be taken back to floor selection
if (select){
  cursor_pos = 0;
  menu_pos = 0;
  lcd.setCursor(0,0);
  
  scroll_pos = 0;
  scroll_left_limit = 0;
  scroll_limit_index = 23;
  scroll_limit_reached = false;

  current_selection = "";
  floor_selection = "";
  room_selection = "";
  device_selection = "";
  name_selection = "";
  action_selection = "";
  time_selection = 00.00;
  lvl_selection = 0;
  minutes = 0;
}
}

/* This function uses the same principles as the clearTimeSelection() and clearLevelSelection() Macros.
 * Except it is applied to string selections such as floor_selection,room_selection etc...
 */
String clear_selection(String current_selection, String selection){
  lcd.setCursor(current_selection.length(),0);
  String eraser = "";
  for (int8_t i = 0; i < selection.length(); i++){
    eraser += " ";
  }
  lcd.print(eraser);
  selection = "";
  lcd.setCursor(current_selection.length(),0);
  return selection;
}

//Checks if when scrolling right the scroll limit is reached
bool ScrollCheck(int8_t scroll_pos, int8_t scroll_limit){
  if (scroll_pos >= scroll_limit){
    return true;
  } else{
    return false;
  }
}

//Finds how much is scrolled before reaching the maximum
int8_t ScrolledRight(int8_t index, int8_t selection_length){
  int8_t scrolled = selection_length - (index);
  return scrolled;
}

/* All function below use the same principles.
 * They all check when the button is pressed and registers the press when the button is released.
 */
void check_up(bool *up){
  static int8_t old_buttons = lcd.readButtons();
  int8_t buttons = lcd.readButtons();
  int8_t changes = old_buttons & ~buttons;
  *up = changes & BUTTON_UP;
  old_buttons = buttons;
}

void check_down(bool *down){
  static int8_t old_buttons = lcd.readButtons();
  int8_t buttons = lcd.readButtons();
  int8_t changes = old_buttons & ~buttons;
  *down = changes & BUTTON_DOWN;
  old_buttons = buttons;
}

void check_right(bool *right){
  static int8_t old_buttons = lcd.readButtons();
  int8_t buttons = lcd.readButtons();
  int8_t changes = old_buttons & ~buttons;
  *right = changes & BUTTON_RIGHT;
  old_buttons = buttons;
}

void check_left(bool *left){
  static int8_t old_buttons = lcd.readButtons();
  int8_t buttons = lcd.readButtons();
  int8_t changes = old_buttons & ~buttons;
  *left = changes & BUTTON_LEFT;
  old_buttons = buttons;
}

void check_select(bool *select){
  static int8_t old_buttons = lcd.readButtons();
  int8_t buttons = lcd.readButtons();
  int8_t changes = old_buttons & ~buttons;
  *select = changes & BUTTON_SELECT;
  old_buttons = buttons;
}
