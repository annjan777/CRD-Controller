///////////////////////////////////////////////////////////////////////////////////////////////////
// Begining of the code
//////////////////////////////////////////////////////////////////////////////////////////////////

// Required libraries - BEGIN

#include <Arduino.h>
#include <EEPROM.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_ILI9341_Menu.h"
#include <Keypad.h> 

// Required libraries - END


//Custom Libraries - BEGIN

//available on Github
#include "image.h"
#include "Adafruit_ILI9341_Menu.h"

//Custom Libraries - END


//Fonts Used for Menu - BEGIN

// found in \Arduino\libraries\Adafruit-GFX-Library-master
#include "fonts\FreeMonoBold9pt7b.h"
#include "fonts\FreeMono18pt7b.h"
#include "fonts\FreeMono12pt7b.h"
#include "fonts\FreeSerif18pt7b.h"
#include "fonts\FreeSerif12pt7b.h"
#include "fonts\FreeSerifBold18pt7b.h"

//Fonts Used for Menu - END


// Display Layout Constants - BEGIN
#define DATA_COLUMN1     175
#define ROW_HEIGHT1      40
#define DATA_COLUMN2     220
#define ROW_HEIGHT2      40
// Display Layout Constants - END


// Define TFT display pins - BEGIN
#define TFT_CS     15
#define TFT_RST    2
#define TFT_DC     4
#define TFT_MOSI   23
#define TFT_CLK    18
#define TFT_MISO   19
// Define TFT display pins - END


// Define keypad pins and keys - BEGIN

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {5, 21, 22, 33}; 

// Define keypad pins and keys - END


// Fonts definitions for various UI elements

#define FONT_SMALL  FreeMonoBold9pt7b                
#define FONT_EDITTITLE  FreeSerif18pt7b          
#define FONT_ITEM   FreeSerif12pt7b        
#define FONT_TITLE  FreeSerif18pt7b      
#define FONT_WARNING FreeSerifBold18pt7b 

// Fonts definitions for various UI elements


// Declare Menu and Sub Menu Variables - BEGIN

int MenuOption = 0, AllowParameterMenu = 0, AllowSystemMenu = 0;

// Variables for Main Menu
int MenuOption1 = 0, MenuOption2 = 0, MenuOption3 = 0, MenuOption4 = 0;

// Variables for Parameter Menu
int ParameterOption1, ParameterOption2, ParameterOption3, ParameterOption4, ParameterOption5, ParameterOption6;

// Variables for System Menu
int SystemOption1, SystemOption2, SystemOption3, SystemOption4, SystemOption5, SystemOption6, SystemOption7, SystemOption8;

// Declare Menu and Sub Menu Variables - END


// Assigning Character Values - BEGIN

String currentLengthStr = "";
String p1;
char p2, p3, p4, p5, p6;

// Assigning Character Values - END


// Menu Item Arrays - BEGIN

const char *OperationItems [] = {"Winding", "Unwind"};
const char *TripItems [] = {"Safe", "Warning"};
const char* LengthItems[0]; // Adjust size based on your requirement
const char *ModeItems[] = {"VFD", "Stall Torque"};
const char *FeedItems[] = {"Center Feed", "End Feed"};
const char *LRModeItems[] = {"Local", "Remote"};
const char *SensorItems[] = {"Inclination", "Proximity", "PLS"};
const char *TravelItems[] = {"GLS", "Encoder", "Proximity"};
const char *OffOnItems[] =   {"Off", "On"};
const char *H_NAMES[] = {"Bad", "Good"};
const char *TSTATUS[] = {"Trip Status: Safe"};
const char *HSTATUS[] = {"System Health: Good"};

// Menu Item Arrays - END


// EEPROM storage addresses for menu options - BEGIN

#define EEPROM_ADDR_OPTION1 0
#define EEPROM_ADDR_OPTION2 4
#define EEPROM_ADDR_OPTION3 8
#define EEPROM_ADDR_OPTION4 12
#define EEPROM_ADDR_OPTION5 16
#define EEPROM_ADDR_OPTION6 20
#define EEPROM_ADDR_OPTION7 24
#define EEPROM_ADDR_OPTION8 28
#define EEPROM_ADDR_OPTION9 32
#define EEPROM_ADDR_OPTION10 36
#define EEPROM_ADDR_OPTION11 40
#define EEPROM_ADDR_OPTION12 44
#define EEPROM_ADDR_OPTION13 48
#define EEPROM_ADDR_LENGTH_ITEMS 52 // Starting address for storing LengthItems
#define MAX_LENGTH 50 // Maximum length of the string to store

// EEPROM storage addresses for menu options - END


// Functions to Store and Retrieve data from EEPROM prototypes - BEGIN

void storeSelectedOption(int optionID, int value);
int retrieveSelectedOption(int optionID);

//Functions to Store and Retrieve data from EEPROM prototypes - END


// Colors for Designing the menu - BEGIN

const char *C_NAMES[] = {"White", "Black", "Grey", "Blue", "Red", "Green", "Cyan", "Magenta",
                         "Yellow", "Teal", "Orange", "Pink", "Purple", "Lt Grey", "Lt Blue", "Lt Red",
                         "Lt Green", "Lt Cyan", "Lt Magenta", "Lt Yellow", "Lt Teal", "Lt Orange", "Lt Pink", "Lt Purple",
                         "Medium Grey", "Medium Blue", "Medium Red", "Medium Green", "Medium Cyan", "Medium Magenta", "Medium Yellow", "Medium Teal",
                         "Medium Orange", "Medium Pink", "Medium Purple", "Dk Grey", "Dk Blue", "Dk Red", "Dk Green", "Dk Cyan",
                         "Dk Magenta", "Dk Yellow", "Dk Teal", "Dk Orange", "Dk Pink", "Dk Purple", "Amber"
                        };

const uint16_t  C_VALUES[] = {  0XFFFF, 0X0000, 0XC618, 0X001F, 0XF800, 0X07E0, 0X07FF, 0XF81F, //7
                                0XFFE0, 0X0438, 0XFD20, 0XF81F, 0X801F, 0XE71C, 0X73DF, 0XFBAE, //15
                                0X7FEE, 0X77BF, 0XFBB7, 0XF7EE, 0X77FE, 0XFDEE, 0XFBBA, 0XD3BF, //23
                                0X7BCF, 0X1016, 0XB000, 0X0584, 0X04B6, 0XB010, 0XAD80, 0X0594, //31
                                0XB340, 0XB00E, 0X8816, 0X4A49, 0X0812, 0X9000, 0X04A3, 0X0372, //39
                                0X900B, 0X94A0, 0X0452, 0X92E0, 0X9009, 0X8012, 0xFF7E //46
                             };

uint16_t MENU_TEXT = C_VALUES[1];
uint16_t MENU_BACKGROUND = C_VALUES[0];
uint16_t MENU_HIGHLIGHTTEXT = C_VALUES[1];
uint16_t MENU_HIGHLIGHT = C_VALUES[21];
uint16_t MENU_HIGHBORDER = C_VALUES[10];
uint16_t MENU_SELECTTEXT = C_VALUES[0];
uint16_t MENU_SELECT = C_VALUES[4];
uint16_t MENU_SELECTBORDER = C_VALUES[37];
uint16_t MENU_DISABLE = C_VALUES[2];
uint16_t TITLE_TEXT = C_VALUES[13];
uint16_t TITLE_BACK = C_VALUES[10];

// Colors for Designing the Menu - END


// Initialize TFT Display & Menu Objects - BEGIN

Adafruit_ILI9341 Display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
ItemMenu MainMenu(&Display);
EditMenu SystemMenu(&Display); 
EditMenu ParameterMenu(&Display); 

// Initialize TFT Display & Menu Objectssrz - END


// Create keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
bool keypadEnabled = true; // Flag to enable/disable keypad

// Define the buzzer pin
#define BUZZER_PIN 32

// Default values
const int DEFAULT_VALUE = 0;


// Code for EEPROM - BEGIN

// Function to initialize EEPROM values
void initializeEEPROMValues() {
  // Loop through EEPROM addresses for options 1 to 8
  for (int address = EEPROM_ADDR_OPTION1; address <= EEPROM_ADDR_OPTION8; address += sizeof(int)) {
    int value; // Variable to hold the value read from EEPROM
    EEPROM.get(address, value); // Get the value stored at the current address
    if (value < 0 || value > 1000) { // Check if the value is out of the valid range (0-1000)
      EEPROM.put(address, DEFAULT_VALUE); // If invalid, store the default value at the current address
    }
  }
  EEPROM.commit(); // Commit changes to EEPROM
}

// Function to store a selected option value in EEPROM
void storeSelectedOption(int optionID, int value) {
  EEPROM.put(optionID, value); // Store the provided value at the specified option ID address
  EEPROM.commit(); // Commit changes to EEPROM
}

// Function to retrieve a selected option value from EEPROM
int retrieveSelectedOption(int optionID) {
  int value = 0; // Ensure value is initialized to 0
  EEPROM.get(optionID, value); // Get the value stored at the specified option ID address
  return value; // Return the retrieved value
}

// Function to store a string in EEPROM with its length
void storeLengthItems(int addr, const char* value) {
  int len = strlen(value); // Get the length of the string
  EEPROM.write(addr, len); // Store the length of the string at the specified address
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr + 1 + i, value[i]); // Store each character of the string sequentially
  }
  EEPROM.commit(); // Commit changes to EEPROM
}

// Function to retrieve a string from EEPROM with its length
void retrieveLengthItems(int addr, char* value) {
  int len = EEPROM.read(addr); // Read the length of the string stored at the specified address
  for (int i = 0; i < len; i++) {
    value[i] = EEPROM.read(addr + 1 + i); // Read each character of the string sequentially
  }
  value[len] = '\0'; // Null-terminate the string
}

// Code for EEPROM - END


// Code for Setup Function - BEGIN

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
  EEPROM.begin(512); // Initialize EEPROM with a size of 512 bytes (adjust as needed)
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as an output

  // Load stored LengthItems value
  char loadedLengthStr[MAX_LENGTH] = ""; // Create a buffer to store the loaded string
  retrieveLengthItems(EEPROM_ADDR_LENGTH_ITEMS, loadedLengthStr); // Retrieve the string from EEPROM
  LengthItems[0] = loadedLengthStr; // Store the retrieved string in LengthItems array

  Display.begin(); // Initialize the display
  Display.setRotation(1); // Set display rotation
  Display.fillScreen(ILI9341_WHITE); // Fill the screen with white color

  // Delay for 5 seconds
  // delay(5000); // Uncomment if a delay is needed

  // Clear the display or proceed with other code
  Display.fillScreen(ILI9341_BLACK); // Fill the screen with black color

  // Initialize the main menu
  MainMenu.init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, 64, 3, "Main Menu", FONT_TITLE, FONT_TITLE);

  // Display Trip Status
  MenuOption4 = MainMenu.addNI(HSTATUS[0]); // Add menu item for trip status
  MenuOption3 = MainMenu.addNI(TSTATUS[0]); // Add menu item for temperature status
  MenuOption1 = MainMenu.add565("CRD Status", epd_bitmap_drivers, 64, 64); // Add menu item for CRD status
  MenuOption2 = MainMenu.add565("CRD Settings", epd_bitmap_slogo, 64, 64); // Add menu item for CRD settings

  // Debugging Menu - Begin
  if (AllowSystemMenu == 1) {
    MainMenu.disable(MenuOption1); // Disable menu option 1 if AllowSystemMenu is 1
  } else {
    MainMenu.enable(MenuOption1); // Enable menu option 1 if AllowSystemMenu is not 1
  }

  if (AllowParameterMenu == 1) {
    MainMenu.disable(MenuOption2); // Disable menu option 2 if AllowParameterMenu is 1
  } else {
    MainMenu.enable(MenuOption2); // Enable menu option 2 if AllowParameterMenu is not 1
  }
  // Debugging Menu - END

  MainMenu.setTitleColors(TITLE_TEXT, TITLE_BACK); // Set title colors for the main menu
  MainMenu.setTitleBarSize(0, 0, 320, 40); // Set the title bar size
  MainMenu.setTitleTextMargins(60, 30); // Set title text margins
  MainMenu.setIconMargins(0, 0); // Set icon margins
  MainMenu.setMenuBarMargins(0, 310, 15, 4); // Set menu bar margins
  MainMenu.setItemColors(MENU_DISABLE, MENU_HIGHBORDER); // Set item colors
  MainMenu.setItemTextMargins(10, 45, 5); // Set item text margins

  // Code to design Option Menu
  // Retrieve the stored values
  SystemMenu.SetItemValue(SystemOption8, retrieveSelectedOption(EEPROM_ADDR_OPTION8)); // Retrieve and set value for SystemOption8

  // Initialize System Menu
  SystemMenu.init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT,
                  DATA_COLUMN2, ROW_HEIGHT2, ROWS, "System Settings", FONT_ITEM, FONT_TITLE);

  SystemOption1 = SystemMenu.add565("Power Supply", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems, epd_bitmap_power, 32, 32);
  SystemMenu.SetItemValue(SystemOption1, retrieveSelectedOption(EEPROM_ADDR_OPTION1)); // Retrieve and set value for SystemOption1

  SystemOption2 = SystemMenu.add565("System Health", 0, 0, sizeof(H_NAMES) / sizeof(H_NAMES[0]), 1, 0, H_NAMES, epd_bitmap_battery, 32, 32);
  SystemMenu.SetItemValue(SystemOption2, retrieveSelectedOption(EEPROM_ADDR_OPTION2)); // Retrieve and set value for SystemOption2

  SystemOption3 = SystemMenu.add565("CRD", 0, 0, sizeof(OffOnItems) / sizeof(OffOnItems[0]), 1, 0, OffOnItems, epd_bitmap_power, 32, 32);
  SystemMenu.SetItemValue(SystemOption3, retrieveSelectedOption(EEPROM_ADDR_OPTION3)); // Retrieve and set value for SystemOption3

  SystemOption4 = SystemMenu.add565("Operation", 0, 0, sizeof(OperationItems) / sizeof(OperationItems[0]), 1, 0, OperationItems, epd_bitmap_arrow, 32, 32);
  SystemMenu.SetItemValue(SystemOption4, retrieveSelectedOption(EEPROM_ADDR_OPTION4)); // Retrieve and set value for SystemOption4

  SystemOption5 = SystemMenu.add565("Over Load Trip", 0, 0, sizeof(TripItems) / sizeof(TripItems[0]), 1, 0, TripItems, epd_bitmap_Tune, 32, 32);
  SystemMenu.SetItemValue(SystemOption5, retrieveSelectedOption(EEPROM_ADDR_OPTION5)); // Retrieve and set value for SystemOption5

  SystemOption6 = SystemMenu.add565("Cable Tension", 0, 0, sizeof(TripItems) / sizeof(TripItems[0]), 1, 0, TripItems, epd_bitmap_Tune, 32, 32);
  SystemMenu.SetItemValue(SystemOption6, retrieveSelectedOption(EEPROM_ADDR_OPTION6)); // Retrieve and set value for SystemOption6

  SystemOption7 = SystemMenu.add565("Slagness", 0, 0, sizeof(TripItems) / sizeof(TripItems[0]), 1, 0, TripItems, epd_bitmap_Tune, 32, 32);
  SystemMenu.SetItemValue(SystemOption7, retrieveSelectedOption(EEPROM_ADDR_OPTION7)); // Retrieve and set value for SystemOption7

  SystemOption8 = SystemMenu.add565("Chain Snap", 0, 0, sizeof(TripItems) / sizeof(TripItems[0]), 1, 0, TripItems, epd_bitmap_Tune, 32, 32);
  SystemMenu.SetItemValue(SystemOption8, retrieveSelectedOption(EEPROM_ADDR_OPTION8)); // Retrieve and set value for SystemOption8

  SystemMenu.setTitleColors(TITLE_TEXT, TITLE_BACK); // Set title colors for the system menu
  SystemMenu.setTitleBarSize(0, 0, 320, 40); // Set the title bar size
  SystemMenu.setTitleTextMargins(60, 30); // Set title text margins
  SystemMenu.setIconMargins(5, 0); // Set icon margins
  SystemMenu.setItemTextMargins(7, 25, 10); // Set item text margins
  SystemMenu.setItemColors(MENU_DISABLE, MENU_HIGHBORDER, MENU_SELECTBORDER); // Set item colors

  // Code to design Parameter Menu
  ParameterMenu.init(MENU_TEXT, MENU_BACKGROUND, MENU_HIGHLIGHTTEXT, MENU_HIGHLIGHT, MENU_SELECTTEXT, MENU_SELECT,
                  DATA_COLUMN1, ROW_HEIGHT1, ROWS, "Parameter Menu", FONT_ITEM, FONT_TITLE);

  ParameterOption1 = ParameterMenu.addNI("Manual TLI", 0, 0, sizeof(LengthItems) / sizeof(LengthItems[0]), 1, 0, LengthItems); 

  ParameterOption2 = ParameterMenu.addNI("Mode", 1, 0, sizeof(ModeItems) / sizeof(ModeItems[0]), 1, 0, ModeItems);
  ParameterMenu.SetItemValue(ParameterOption2, retrieveSelectedOption(EEPROM_ADDR_OPTION9)); // Retrieve and set value for ParameterOption2

  ParameterOption3 = ParameterMenu.addNI("Feed", 1, 0, sizeof(FeedItems) / sizeof(FeedItems[0]), 1, 0, FeedItems);
  ParameterMenu.SetItemValue(ParameterOption3, retrieveSelectedOption(EEPROM_ADDR_OPTION10)); // Retrieve and set value for ParameterOption3

  ParameterOption4 = ParameterMenu.addNI("System", 1, 0, sizeof(LRModeItems) / sizeof(LRModeItems[0]), 1, 0, LRModeItems);
  ParameterMenu.SetItemValue(ParameterOption4, retrieveSelectedOption(EEPROM_ADDR_OPTION11)); // Retrieve and set value for ParameterOption4

  ParameterOption5 = ParameterMenu.addNI("L-R Sensor", 1, 0, sizeof(SensorItems) / sizeof(SensorItems[0]), 1, 0, SensorItems);
  ParameterMenu.SetItemValue(ParameterOption5, retrieveSelectedOption(EEPROM_ADDR_OPTION12)); // Retrieve and set value for ParameterOption5

  ParameterOption6 = ParameterMenu.addNI("Travel Sensor", 1, 0, sizeof(TravelItems) / sizeof(TravelItems[0]), 1, 0, TravelItems);
  ParameterMenu.SetItemValue(ParameterOption6, retrieveSelectedOption(EEPROM_ADDR_OPTION13)); // Retrieve and set value for ParameterOption6

  ParameterMenu.setTitleTextMargins(50, 30); // Set title text margins for the parameter menu
  ParameterMenu.setItemTextMargins(16, 25, 5); // Set item text margins
  ParameterMenu.setMenuBarMargins(10, 305, 4, 2); // Set menu bar margins

  ProcessMainMenu(); // Process the main menu

  Display.fillScreen(MENU_BACKGROUND); // Fill the screen with the menu background color
}

// Code for Setup Function - END


// Code for Function Loop - BEGIN

void loop() {
}

//Code for Function Loop - END


// Code for Buzzer - BEGIN 

// Function to produce a single beep
void beep(int duration) {
  int frequency = 1000; // Frequency in Hertz
  tone(BUZZER_PIN, frequency, duration); // Generate tone at the specified frequency and duration
}

// Function to produce a continuous beep
void beep2() {
  int frequency = 1000; // Frequency in Hertz
  tone(BUZZER_PIN, frequency); // Generate continuous tone at the specified frequency
}

// Function to produce an alternating beep
void beep1(int duration) {
  int interval = 500;  // Interval for frequency change in milliseconds
  int startFrequency = 1500;  // Starting frequency in Hertz
  int endFrequency = 2000;   // Ending frequency in Hertz

  // Loop to alternate the frequency for the specified duration
  for (int i = 0; i < duration; i += interval * 2) {
    tone(BUZZER_PIN, startFrequency, interval); // Generate tone at start frequency for the interval
    delay(interval); // Wait for the interval duration
    tone(BUZZER_PIN, endFrequency, interval); // Generate tone at end frequency for the interval
    delay(interval); // Wait for the interval duration
  }
  noTone(BUZZER_PIN); // Stop the tone after the loop
}

// Code for Buzzer - END


// Warning Function - BEGIN

// Function to display a warning message with alternating screen colors and beeps
void showWarning(int duration) {
  unsigned long startTime = millis(); // Record the start time
  
  // Loop for the specified duration in milliseconds
  while (millis() - startTime < duration) {  
    // Set up the display for amber background
    Display.fillScreen(0xFFE0);  // Fill the screen with amber color
    Display.setFont(&FONT_WARNING); // Set the font for warning text
    
    // Calculate centered position for text
    int16_t x1, y1;
    uint16_t w, h;
    Display.getTextBounds("Warning Trip !", 0, 0, &x1, &y1, &w, &h); // Get text bounds
    int x_center = (Display.width() - w) / 2; // Calculate X position to center text
    int y_center = (Display.height() - h) / 2; // Calculate Y position to center text

    // Display warning text
    Display.setCursor(x_center, y_center); // Set cursor to centered position
    Display.setTextColor(C_VALUES[4], 0xFFE0);  // Set text color to red for contrast with amber background
    Display.print(F("Warning Trip !")); // Print warning message
    beep1(500);  // Beep for half second

    // Set up the display for red background
    Display.fillScreen(C_VALUES[4]);  // Fill the screen with red color
    
    // Calculate centered position for text (recalculate to ensure proper positioning)
    Display.getTextBounds("Warning Trip !", 0, 0, &x1, &y1, &w, &h); // Get text bounds again
    x_center = (Display.width() - w) / 2; // Recalculate X position
    y_center = (Display.height() - h) / 2; // Recalculate Y position

    // Display warning text
    Display.setCursor(x_center, y_center); // Set cursor to centered position
    Display.setTextColor(ILI9341_WHITE, C_VALUES[4]);  // Set text color to white for contrast with red background
    Display.print(F("Warning Trip !")); // Print warning message
    beep1(500);  // Beep for half second
  }
}

// Warning Function - END


// Status Function - BEGIN

// Function to display a status message on the screen
void showStatus() {
    // Set up the display with a red background
    Display.fillScreen(ILI9341_RED);  // Fill the screen with red color
    Display.setFont(&FONT_TITLE); // Set the font for the title

    // Calculate centered position for the first line of text
    int16_t x1, y1;
    uint16_t w1, h1;
    Display.getTextBounds("Warning", 0, 0, &x1, &y1, &w1, &h1); // Get bounds for the first line
    int x_center1 = (Display.width() - w1) / 2; // Calculate X position to center the first line
    int y_center1 = (Display.height() - h1) / 2 - 10; // Calculate Y position to center the first line, with adjustment for spacing

    // Calculate centered position for the second line of text
    uint16_t w2, h2;
    Display.getTextBounds("System Health", 0, 0, &x1, &y1, &w2, &h2); // Get bounds for the second line
    int x_center2 = (Display.width() - w2) / 2; // Calculate X position to center the second line
    int y_center2 = y_center1 + h1 + 10; // Position the second line below the first line with additional spacing

    // Display the first line of text
    Display.setCursor(x_center1, y_center1); // Set cursor to the calculated position for the first line
    Display.setTextColor(ILI9341_WHITE, ILI9341_RED);  // Set text color to white for contrast with the red background
    Display.print(F("Warning")); // Print the first line of text

    // Display the second line of text
    Display.setCursor(x_center2, y_center2); // Set cursor to the calculated position for the second line
    Display.print(F("System Health")); // Print the second line of text
}

// Status Function - END


// Code for Debugging Mode - BEGIN

// Function to display a debugging mode message on the screen
void showDebugging() {
    // Set up the display with a white background
    Display.fillScreen(ILI9341_WHITE);  // Fill the screen with white color
    Display.setFont(&FONT_WARNING); // Set the font for the warning message

    // Calculate centered position for the text
    int16_t x1, y1;
    uint16_t w, h;
    Display.getTextBounds("Debugging Mode", 0, 0, &x1, &y1, &w, &h); // Get text bounds
    int x_center = (Display.width() - w) / 2; // Calculate X position to center the text
    int y_center = (Display.height() - h) / 2; // Calculate Y position to center the text

    // Display the text
    Display.setCursor(x_center, y_center); // Set cursor to centered position
    Display.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  // Set text color to black for contrast with white background
    Display.print(F("Debugging Mode")); // Print the debugging mode message
}

// Code for Debugging Mode - END


// Logo Function - BEGIN

// Function to display a bitmap image on the screen
void displayBitmap(const uint16_t *bitmap, uint16_t w, uint16_t h) {
  // Iterate over each row of the bitmap
  for (uint16_t row = 0; row < h; row++) {
    // Iterate over each column in the current row
    for (uint16_t col = 0; col < w; col++) {
      // Read the color from the bitmap array at the current position
      uint16_t color = pgm_read_word(bitmap + (row * w + col)); // Retrieve color from program memory
      // Draw the pixel on the display at the specified position
      Display.drawPixel(col, row, color); // Set the pixel color on the screen
    }
  }
}

// Logo Function - END


// MainMenu Function - BEGIN

// Function to process and display the main menu
void ProcessMainMenu() {
  int MainMenuOption = 1; // Initial flag for the menu item
  bool debuggingMode = false; // Flag to indicate debugging mode

  // Clear the display and draw the initial menu
  Display.fillScreen(MENU_BACKGROUND); // Set background color for the menu
  MainMenu.draw(); // Draw the initial menu on the screen

  // Main loop to process menu interactions
  while (MainMenuOption != 0) {
    // Check if keypad is enabled for user input
    if (keypadEnabled) {
      char key = keypad.getKey(); // Read the key pressed on the keypad

      if (key) { // If a key is pressed
        // Handle key press based on the key value
        if (key == 'A') { // Up arrow key
          beep(100); // Short beep
          MainMenu.MoveUp(); // Move up in the menu
        } else if (key == 'C') { // Down arrow key
          beep(100); // Short beep
          MainMenu.MoveDown(); // Move down in the menu
        } else if (key == 'B') { // Select key
          beep(100); // Short beep
          MainMenuOption = MainMenu.selectRow(); // Select the current row in the menu

          // Process selected menu options
          if (MainMenuOption == MenuOption1) {
            ProcessSystemMenu(); // Process the system menu
            Display.fillScreen(MENU_BACKGROUND); // Clear the screen
            MainMenu.draw(); // Redraw the main menu
          } else if (MainMenuOption == MenuOption2) {
            ProcessParameterMenu(); // Process the parameter menu
            Display.fillScreen(MENU_BACKGROUND); // Clear the screen
            MainMenu.draw(); // Redraw the main menu
          }
        }
      }
    }

    // Check for serial input
    if (Serial.available() > 0) { // If there is data available
      char input = Serial.read(); // Read the incoming byte

      if (input) { // If there is input from the serial
        // Handle specific serial inputs
        if (input == 'i' || input == 'j') { // For SystemOption5
          SystemMenu.value[SystemOption5] = (input == 'j') ? 1 : 0; // Set value based on input
          if (SystemMenu.value[SystemOption5] == 1) { // If value is 1
            showWarning(5000); // Display warning for 5 seconds
          }
          SystemMenu.SetItemValue(SystemOption5, SystemMenu.value[SystemOption5]); // Update menu item value
          storeSelectedOption(EEPROM_ADDR_OPTION5, SystemMenu.value[SystemOption5]); // Store value in EEPROM
          if (!debuggingMode) {
            Display.fillScreen(MENU_BACKGROUND); // Clear the screen
            MainMenu.draw(); // Redraw the main menu
          }
        } else if (input == 'a') { // For SystemOption2
          SystemMenu.value[SystemOption2] = 0; // Set value to 0
          SystemMenu.SetItemValue(SystemOption2, SystemMenu.value[SystemOption2]); // Update menu item value
          beep(100); // Short beep
          Display.fillScreen(MENU_BACKGROUND); // Clear the screen
          MainMenu.draw(); // Redraw the main menu
        } else if (input == 'b') { // For SystemOption2
          SystemMenu.value[SystemOption2] = 1; // Set value to 1
          SystemMenu.SetItemValue(SystemOption2, SystemMenu.value[SystemOption2]); // Update menu item value
          beep(1000); // Longer beep
          showStatus(); // Display system status
        } else if (input == 'c' || input == 'd') { // For SystemOption1
          if (debuggingMode) {
            SystemMenu.value[SystemOption1] = (input == 'c') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption1, SystemMenu.value[SystemOption1]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION1, SystemMenu.value[SystemOption1]); // Store value in EEPROM
          }
        } else if (input == 'e' || input == 'f') { // For SystemOption3
          if (debuggingMode) {
            SystemMenu.value[SystemOption3] = (input == 'e') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption3, SystemMenu.value[SystemOption3]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION3, SystemMenu.value[SystemOption3]); // Store value in EEPROM
          }
        } else if (input == 'g' || input == 'h') { // For SystemOption4
          if (debuggingMode) {
            SystemMenu.value[SystemOption4] = (input == 'h') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption4, SystemMenu.value[SystemOption4]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION4, SystemMenu.value[SystemOption4]); // Store value in EEPROM
          }
        } else if (input == 'k' || input == 'l') { // For SystemOption6
          if (debuggingMode) {
            SystemMenu.value[SystemOption6] = (input == 'l') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption6, SystemMenu.value[SystemOption6]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION6, SystemMenu.value[SystemOption6]); // Store value in EEPROM
          }
        } else if (input == 'm' || input == 'n') { // For SystemOption7
          if (debuggingMode) {
            SystemMenu.value[SystemOption7] = (input == 'n') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption7, SystemMenu.value[SystemOption7]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION7, SystemMenu.value[SystemOption7]); // Store value in EEPROM
          }
        } else if (input == 'o' || input == 'p') { // For SystemOption8
          if (debuggingMode) {
            SystemMenu.value[SystemOption8] = (input == 'p') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption8, SystemMenu.value[SystemOption8]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION8, SystemMenu.value[SystemOption8]); // Store value in EEPROM
          }
        } else if (input == '-') { // Toggle debugging mode
          beep(500); // Short beep
          MainMenu.disable(MenuOption1); // Disable CRD Status menu option
          MainMenu.disable(MenuOption2); // Disable CRD Settings menu option
          showDebugging(); // Display debugging mode message
          debuggingMode = true; // Enable debugging mode
          keypadEnabled = false; // Disable keypad input
        } else if (input == '+') { // Exit debugging mode
          beep(500); // Short beep
          MainMenu.enable(MenuOption1); // Enable CRD Status menu option
          MainMenu.enable(MenuOption2); // Enable CRD Settings menu option
          debuggingMode = false; // Disable debugging mode
          keypadEnabled = true; // Enable keypad input
          Display.fillScreen(MENU_BACKGROUND); // Clear the screen
          MainMenu.draw(); // Redraw the main menu
        }
      }
    }
  }
}

// Main Menu Function - END


// System Menu Function - BEGIN

// Function to process and display the system menu
void ProcessSystemMenu() {
  int EditMenuOption = 1; // Flag to track selected menu item
  bool serialInputEnabled = false; // Flag to control serial input processing

  // Clear the display and draw the initial system menu
  Display.fillScreen(MENU_BACKGROUND); // Set background color for the menu
  SystemMenu.draw(); // Draw the initial system menu on the screen

  // Main loop to process menu interactions
  while (EditMenuOption != 0) {
    char key = keypad.getKey(); // Read the key from the keypad
    char input = 0; // Variable to store serial input

    // Handle keypad input
    if (key) { // If a key is pressed
      if (key == 'A') { // Up arrow key
        beep(100); // Short beep
        SystemMenu.MoveUp(); // Move up in the menu
      } else if (key == 'C') { // Down arrow key
        beep(100); // Short beep
        SystemMenu.MoveDown(); // Move down in the menu
      } else if (key == 'B') { // Select key
        beep(100); // Short beep
        EditMenuOption = SystemMenu.selectRow(); // Select the current row in the menu

        // Store selected menu options in EEPROM
        storeSelectedOption(EEPROM_ADDR_OPTION1, SystemMenu.value[SystemOption1]);
        storeSelectedOption(EEPROM_ADDR_OPTION2, SystemMenu.value[SystemOption2]);
        storeSelectedOption(EEPROM_ADDR_OPTION3, SystemMenu.value[SystemOption3]);
        storeSelectedOption(EEPROM_ADDR_OPTION4, SystemMenu.value[SystemOption4]);
        storeSelectedOption(EEPROM_ADDR_OPTION5, SystemMenu.value[SystemOption5]);
        storeSelectedOption(EEPROM_ADDR_OPTION6, SystemMenu.value[SystemOption6]);
        storeSelectedOption(EEPROM_ADDR_OPTION7, SystemMenu.value[SystemOption7]);
        storeSelectedOption(EEPROM_ADDR_OPTION8, SystemMenu.value[SystemOption8]);
      }
    }

    // Handle serial input
    if (Serial.available() > 0) { // If there is data available
      input = Serial.read(); // Read the incoming byte

      if (input) { // If there is input from the serial
        if (input == '-') { // Toggle debugging mode
          serialInputEnabled = true; // Enable serial input processing
          SystemMenu.disable(SystemOption1); // Disable specific menu options
          SystemMenu.disable(SystemOption2);
          beep(500); // Short beep
          showDebugging(); // Display debugging mode message
          keypadEnabled = false; // Disable keypad input
        } else if (input == '+') { // Exit debugging mode
          serialInputEnabled = false; // Disable serial input processing
          beep(500); // Short beep
          SystemMenu.enable(SystemOption1); // Enable specific menu options
          SystemMenu.enable(SystemOption2);
          keypadEnabled = true; // Enable keypad input
          Display.fillScreen(MENU_BACKGROUND); // Clear the screen
          SystemMenu.draw(); // Redraw the system menu
        } else if (serialInputEnabled) { // If serial input processing is enabled
          if (input == 'c' || input == 'd') { // For SystemOption1
            SystemMenu.value[SystemOption1] = (input == 'c') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption1, SystemMenu.value[SystemOption1]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION1, SystemMenu.value[SystemOption1]); // Store value in EEPROM
          } else if (input == 'a' || input == 'b') { // For SystemOption2
            SystemMenu.value[SystemOption2] = (input == 'a') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption2, SystemMenu.value[SystemOption2]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION2, SystemMenu.value[SystemOption2]); // Store value in EEPROM
          } else if (input == 'e' || input == 'f') { // For SystemOption3
            SystemMenu.value[SystemOption3] = (input == 'e') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption3, SystemMenu.value[SystemOption3]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION3, SystemMenu.value[SystemOption3]); // Store value in EEPROM
          } else if (input == 'g' || input == 'h') { // For SystemOption4
            SystemMenu.value[SystemOption4] = (input == 'h') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption4, SystemMenu.value[SystemOption4]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION4, SystemMenu.value[SystemOption4]); // Store value in EEPROM
          } else if (input == 'i' || input == 'j') { // For SystemOption5
            SystemMenu.value[SystemOption5] = (input == 'j') ? 1 : 0; // Set value based on input
            if (SystemMenu.value[SystemOption5] == 1) { // If value is 1
              showWarning(5000); // Display warning for 5 seconds
            }
            SystemMenu.SetItemValue(SystemOption5, SystemMenu.value[SystemOption5]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION5, SystemMenu.value[SystemOption5]); // Store value in EEPROM
          } else if (input == 'k' || input == 'l') { // For SystemOption6
            SystemMenu.value[SystemOption6] = (input == 'l') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption6, SystemMenu.value[SystemOption6]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION6, SystemMenu.value[SystemOption6]); // Store value in EEPROM
          } else if (input == 'm' || input == 'n') { // For SystemOption7
            SystemMenu.value[SystemOption7] = (input == 'n') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption7, SystemMenu.value[SystemOption7]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION7, SystemMenu.value[SystemOption7]); // Store value in EEPROM
          } else if (input == 'o' || input == 'p') { // For SystemOption8
            SystemMenu.value[SystemOption8] = (input == 'p') ? 1 : 0; // Set value based on input
            SystemMenu.SetItemValue(SystemOption8, SystemMenu.value[SystemOption8]); // Update menu item value
            storeSelectedOption(EEPROM_ADDR_OPTION8, SystemMenu.value[SystemOption8]); // Store value in EEPROM
          }
        }
      }
    }
  }
  beep(500); // Short beep to signal end of menu processing
}

// System Menu Function - END


// Parameter Menu Function - BEGIN

// Function to process and display the parameter menu
void ProcessParameterMenu() {
  int EditMenuOption = 1; // Flag to track selected menu item
  String currentLengthStr = ""; // Current length string for user input

  // Clear the display and draw the parameter menu
  Display.fillScreen(MENU_BACKGROUND); // Set background color
  ParameterMenu.draw(); // Draw the initial parameter menu on the screen

  // Main loop to process menu interactions
  while (EditMenuOption != 0) {
    char key = keypad.getKey(); // Read the key from the keypad

    if (key) { // If a key is pressed
      if (key == 'A') { // Up arrow key
        beep(100); // Short beep
        ParameterMenu.MoveUp(); // Move up in the menu
      } else if (key == 'C') { // Down arrow key
        beep(100); // Short beep
        ParameterMenu.MoveDown(); // Move down in the menu
      } else if (key == 'B') { // Select key
        beep(100); // Short beep
        EditMenuOption = ParameterMenu.selectRow(); // Select the current row

        // Store selected menu options in EEPROM
        storeSelectedOption(EEPROM_ADDR_OPTION9, ParameterMenu.value[ParameterOption2]);
        storeSelectedOption(EEPROM_ADDR_OPTION10, ParameterMenu.value[ParameterOption3]);
        storeSelectedOption(EEPROM_ADDR_OPTION11, ParameterMenu.value[ParameterOption4]);
        storeSelectedOption(EEPROM_ADDR_OPTION12, ParameterMenu.value[ParameterOption5]);
        storeSelectedOption(EEPROM_ADDR_OPTION13, ParameterMenu.value[ParameterOption6]);
      } else if (key == '#') { // Reset current length string
        beep(100); // Short beep
        currentLengthStr = ""; // Reset string

        // Update LengthItems with the reset value
        LengthItems[0] = currentLengthStr.c_str();
        ParameterMenu.drawRow(ParameterOption1); // Redraw row with updated value
      } else if (key >= '0' && key <= '9') { // Numeric input
        beep(100); // Short beep
        currentLengthStr += key; // Append new digit

        // Update LengthItems with the new value
        LengthItems[0] = currentLengthStr.c_str();
        ParameterMenu.value[ParameterOption1] = 1; // Update parameter value
        ParameterMenu.drawRow(ParameterOption1); // Redraw row with updated value

        // Store updated value in EEPROM
        storeLengthItems(EEPROM_ADDR_LENGTH_ITEMS, LengthItems[0]);
      }
      // Ensure the current length string is updated in the menu display
      LengthItems[0] = currentLengthStr.c_str();
      ParameterMenu.drawRow(ParameterOption1);  
    }

    // Handle serial input
    if (Serial.available() > 0) {
      char serialKey = Serial.read(); // Read the incoming byte

      if (serialKey == '#') { // Reset current length string via serial
        beep(100); // Short beep
        currentLengthStr = ""; // Reset string

        // Update LengthItems with the reset value
        LengthItems[0] = currentLengthStr.c_str();
        ParameterMenu.drawRow(ParameterOption1); // Redraw row with updated value
      } else if (serialKey >= '0' && serialKey <= '9') { // Numeric input from serial
        beep(100); // Short beep
        currentLengthStr += serialKey; // Append new digit

        // Update LengthItems with the new value
        LengthItems[0] = currentLengthStr.c_str();
        ParameterMenu.value[ParameterOption1] = 1; // Update parameter value
        ParameterMenu.drawRow(ParameterOption1); // Redraw row with updated value

        // Store updated value in EEPROM
        storeLengthItems(EEPROM_ADDR_LENGTH_ITEMS, LengthItems[0]);
      }
    }

    // Update parameter values based on menu selections
    p2 = (ParameterMenu.value[ParameterOption2] == 1) ? 'r' : 'q'; 
    p3 = (ParameterMenu.value[ParameterOption3] == 1) ? 's' : 't';
    p4 = (ParameterMenu.value[ParameterOption4] == 1) ? 'u' : 'v';  
    p5 = (ParameterMenu.value[ParameterOption5] == 1) ? 'w' : 
         (ParameterMenu.value[ParameterOption5] == 0) ? 'x' : 'y';
    p6 = (ParameterMenu.value[ParameterOption6] == 1) ? 'z' : 
         (ParameterMenu.value[ParameterOption6] == 0) ? 'A' : 'B';
  }

  // Print the final length and parameters to the serial monitor
  beep(500); // Longer beep to indicate end
  Serial.println(currentLengthStr); // Print length string
  Serial.println(p2); // Print parameter p2
  Serial.println(p3); // Print parameter p3
  Serial.println(p4); // Print parameter p4
  Serial.println(p5); // Print parameter p5
  Serial.println(p6); // Print parameter p6
}

// Parameter Menu Function - END


///////////////////////////////////////////////////////////////////////////////////////////////////
// End of the code
//////////////////////////////////////////////////////////////////////////////////////////////////
