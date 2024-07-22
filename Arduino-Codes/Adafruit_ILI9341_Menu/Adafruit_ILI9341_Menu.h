
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#ifndef ADAFRUITILI9341_MENU_H
#define ADAFRUITILI9341_MENU_H

#if ARDUINO >= 100
    #include "Arduino.h"
    #include "Print.h"
#else
    // Include necessary headers for older versions of Arduino
#endif

#ifdef __cplusplus
    // Additional includes for C++ compatibility
#endif

// Constants
#define MAX_OPT 15                // Maximum number of elements in a menu
#define MAX_CHAR_LEN 30           // Maximum number of characters in menu items
#define TRIANGLE_H 3.7
#define TRIANGLE_W 2.5
#define MENU_C_DKGREY 0x4A49      // Color for disabled menu items
#define EXIT_TEXT "Exit"          // Default text for exit menu option
#define ICON_NONE 0
#define ICON_MONO 1
#define ICON_565  2
#define BUTTON_PRESSED 1
#define BUTTON_NOTPRESSED 0

class EditMenu {
public:
    EditMenu(Adafruit_ILI9341 *Display, bool EnableTouch = false);

    void init(uint16_t TextColor, uint16_t BackgroundColor, 
              uint16_t HighlightTextColor, uint16_t HighlightColor,
              uint16_t SelectedTextColor, uint16_t SelectedColor,
              uint16_t MenuColumn, uint16_t ItemRowHeight, uint16_t MaxRow,
              const char *TitleText, const GFXfont &ItemFont, const GFXfont &TitleFont);

    int addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, 
              float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL);

    int addMono(const char *ItemText, float Data, float LowLimit, float HighLimit, 
                float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL, 
                const unsigned char *Bitmap = nullptr, uint8_t BitmapWidth = 0, uint8_t BitmapHeight = 0);

    // Added by Adab Bawa
    int addMI(const char *ItemLabel, const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight);
    // Added by Adab Bawa

    int add565(const char *ItemText, float Data, float LowLimit, float HighLimit, 
               float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL, 
               const uint16_t *Bitmap = nullptr, uint8_t BitmapWidth = 0, uint8_t BitmapHeight = 0);

    int selectRow();

    void draw();
    void MoveUp();
    void MoveDown();
    void setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor);
    void setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight);
    void setTitleText(char *TitleText, char *ExitText);
    void setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin);
    void setIncrementDelay(uint16_t Delay);
    void setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, uint16_t BorderRadius, uint16_t BorderThickness);
    void setItemColors(uint16_t DisableTextColor, uint16_t BorderColor, uint16_t EditModeBorderColor = 0);
    void setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin);
    void setItemText(int ItemID, const char *ItemText);
    void setIconMargins(uint16_t LeftMargin, uint16_t TopMargin);
    void SetItemValue(int ItemID, float ItemValue);
    void SetAllColors(uint16_t TextColor, uint16_t BackgroundColor, 
                      uint16_t HighlightTextColor, uint16_t HighlightColor, uint16_t HighlightBorderColor,
                      uint16_t SelectedTextColor, uint16_t SelectedColor, uint16_t SelectBorderColor,
                      uint16_t DisableTextColor, uint16_t TitleTextColor, uint16_t TitleFillColor);
    void disable(int ID);
    void enable(int ID);
    bool getEnableState(int ID);
    int press(int16_t ScreenX, int16_t ScreenY);
    void drawRow(int ID);

    float value[MAX_OPT];
    int item;

private:
    void drawHeader(bool hl, uint8_t Style);
    void up();
    void down();
    void incrementUp();
    void incrementDown();
    void drawItems();
    void drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color);
    void draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h);

    Adafruit_ILI9341 *d;
    char itemlabel[MAX_OPT][MAX_CHAR_LEN];
    char ttx[MAX_CHAR_LEN];
    char etx[MAX_CHAR_LEN]; 
    const GFXfont *itemf;
    const GFXfont *titlef;
    uint16_t itc, ibc, ihtc, ihbc, istc, isbc;   // Item variables
    uint16_t tbl, tbt, tbw, tbh, ttc, tfc, tox, toy;   // Title variables
    // Margins
    uint16_t imr, isx, itx, isy, irh, irw, ioy, iox, mm, icox, icoy;
    int i;
    int totalID;
    int MaxRow;
    int currentID;
    int cr;
    byte debounce;
    int sr, pr;
    bool moreup = false, moredown = false;
    uint16_t col;
    float data[MAX_OPT];
    float low[MAX_OPT];
    float high[MAX_OPT];
    float inc[MAX_OPT];
    byte dec[MAX_OPT];
    char **itemtext[MAX_OPT];
    bool rowselected = false;
    bool haslist[MAX_OPT];
    bool enablestate[MAX_OPT];
    bool drawTitleFlag = true;
    bool redraw = false;
    uint16_t ditc = 0;
    uint16_t temptColor = 0, bcolor, sbcolor;
    const unsigned char *itemBitmap[MAX_OPT];
    const uint16_t *item565Bitmap[MAX_OPT];
    uint8_t bmp_w[MAX_OPT];
    uint8_t bmp_h[MAX_OPT];
    byte IconType[MAX_OPT];
    uint16_t radius = 0;
    uint16_t thick = 0;
    uint16_t incdelay = 50;
    bool enabletouch, redrawh;
};

class ItemMenu {
public:
    ItemMenu(Adafruit_ILI9341 *Display, bool EnableTouch = false);

    void init(uint16_t TextColor, uint16_t BackgroundColor,
              uint16_t HighlightTextColor, uint16_t HighlightColor, 
              uint16_t ItemRowHeight, uint16_t MaxRow,
              const char *TitleText, const GFXfont &ItemFont, const GFXfont &TitleFont);

    int addNI(const char *ItemLabel);
    int addMono(const char *ItemLabel, const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight);
    int add565(const char *ItemLabel, const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight);
    void draw();
    void MoveUp();
    void MoveDown();
    int selectRow();
    void setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor);
    void setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight);
    void setTitleText(char *TitleText, char *ExitText);
    void setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin);
    void setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, byte BorderRadius, byte BorderThickness);
    void setItemColors(uint16_t DisableTextColor, uint16_t BorderColor);
    void setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin);

    // Added by Adab Bawa
    const char* stringValues[MAX_OPT];

    void setItemText(int ItemID, const char *ItemText);
    void setIconMargins(uint16_t LeftMargin, uint16_t TopMargin);
    void SetAllColors(uint16_t TextColor, uint16_t BackgroundColor, uint16_t HighlightTextColor, uint16_t HighlightColor, 
                      uint16_t HighLightBorderColor, uint16_t DisableTextColor, uint16_t TitleTextColor, uint16_t TitleFillColor);
    void disable(int ID);
    void enable(int ID);
    bool getEnableState(int ID);
    int press(int16_t ScreenX, int16_t ScreenY);
    void drawRow(int ID, uint8_t style);

    float value[MAX_OPT];
    int item;

private:
    void drawHeader(bool hl, uint8_t style);
    void drawItems();
    void drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color);
    void draw565Bitmap(int16_t x, int16_t y, const uint16_t *Bitmap, uint8_t w, uint8_t h);

    Adafruit_ILI9341 *d;
    bool enabletouch;
    char itemlabel[MAX_OPT][MAX_CHAR_LEN];
    char ttx[MAX_CHAR_LEN];
    char etx[MAX_CHAR_LEN];
    const GFXfont *itemf;
    const GFXfont *titlef;
    uint16_t bkgr, isx, itx, isy, irh, itc, ibc, ihbc, ihtc, isc, imr, irw, ioy, iox;  // Item variables
    uint16_t tbl, tbt, tbw, tbh, ttc, tfc, tox, toy, icox, icoy, di, mm;  // Title variables
    uint16_t ditc, difc, temptColor, bcolor;
    bool hasIcon = false, moreup = false, moredown = false;
    int i;
    int totalID;
    int MaxRow;
    int currentID;
    int cr;
    byte debounce;
    int sr, pr;
    const unsigned char *itemBitmap[MAX_OPT];
    const uint16_t *item565Bitmap[MAX_OPT];
    bool rowselected = false;
    bool drawTitleFlag = true;
    bool redraw = false;
    bool enablestate[MAX_OPT];
    uint8_t bmp_w[MAX_OPT];
    uint8_t bmp_h[MAX_OPT];
    byte IconType[MAX_OPT];
    byte radius, thick;
};

#endif
