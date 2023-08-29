
#include <Arduino.h>
#include <FastLED.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <string>
#include <random>


// Replace with your network credentials
//const char *ssid= "PRUSANet";
//const char *password="middrucktsuper"; 

const char *ssid= "Finjas iPhone";
const char *password="Gruezi1234"; 

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  
</head>
<body>
  <h2>ESP Web Server</h2>
</body>
</html>
)rawliteral";


#define PIN4 12
#define PIN2 26
#define PIN3 27
#define PIN1 25
#define NUM_LEDS1 160    
#define NUM_LEDS2 96       
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS1];
CRGB leds3[NUM_LEDS2];
CRGB leds4[NUM_LEDS2];

int ledMatrix [57][57];



uint8_t ledHue;
int brightness;
int colorMax;
int colorIdx=0;

using namespace std;

//----GreenBlue----
DEFINE_GRADIENT_PALETTE (greenBlue0){0,82,255,255,  50,121,252,210, 100,111,252,207,   150,171,252,196,   200, 181,255,254,   255,82,255,255};
DEFINE_GRADIENT_PALETTE (greenBlue1){0,0,255,255,   50,0,252,171,   100,40,252,184,    150,121,252,160,   200, 81,255,254,  255,0,255,255};
DEFINE_GRADIENT_PALETTE (greenBlue2){0,0,255,255,   50,0,252,171,   100,40,252,184,    150,79,125,255,     200,0,70,255,  255,0,255,255};
DEFINE_GRADIENT_PALETTE (greenBlue3){0,0,255,255,   50,0,252,171,   100,40,252,184,    150,180,44,255,    200, 255,90,213,  255,0,255,255};
DEFINE_GRADIENT_PALETTE (greenBlue4){0,0,255,255,   50,0,252,171,   100,40,252,184,    150,255,46,255,    200, 255,15,57, 255,0,255,255};

CRGBPalette16 greenBlue0Pal= greenBlue0;
CRGBPalette16 greenBlue1Pal= greenBlue1;
CRGBPalette16 greenBlue2Pal= greenBlue2;
CRGBPalette16 greenBlue3Pal= greenBlue3;
CRGBPalette16 greenBlue4Pal= greenBlue4;

CRGBPalette16 greenBlues[5]= {greenBlue0Pal, greenBlue1Pal, greenBlue2Pal, greenBlue3Pal, greenBlue4Pal};

//----Green----
DEFINE_GRADIENT_PALETTE (green0){0,207,253,223,  50,176,252,203, 100,181,251,181,   150,222,251,146,   200, 149,252,219, 255,207,253,223};
DEFINE_GRADIENT_PALETTE (green1){0,159,253,191,   50,73,252,135,   100,0,251,0,    150,192,251,40,   200, 40,252,184, 255,159,253,191};
DEFINE_GRADIENT_PALETTE (green2){0,159,253,191,   50,73,252,135,   100,0,251,0,   150,0,255,255,     200,0,190,255, 255,159,253,191};
DEFINE_GRADIENT_PALETTE (green3){0,159,253,191,   50,73,252,135,   100,0,251,0,    150,0,70,255,    200, 155,92,255,  255,159,253,191};
DEFINE_GRADIENT_PALETTE (green4){0,159,253,191,   50,73,252,135,   100,0,251,0,    150,180,44,255,    200, 255,46,255,  255,159,253,191};

CRGBPalette16 green0Pal= green0;
CRGBPalette16 green1Pal= green1;
CRGBPalette16 green2Pal= green2;
CRGBPalette16 green3Pal= green3;
CRGBPalette16 green4Pal= green4;

CRGBPalette16 greens[5]= {green0Pal, green1Pal, green2Pal, green3Pal, green4Pal};

//----GreenYell----
DEFINE_GRADIENT_PALETTE (greenYell0){0,222,251,131,  50,232,251,151, 100,224,251,138,   150,172,251,153,   200, 146,251,146,  255,222,251,131};
DEFINE_GRADIENT_PALETTE (greenYell1){0,191,251,0,   50,204,251,0,   100,191,251,0,    150,98,251,62,   200, 0,251,0,    255,191,251,0};
DEFINE_GRADIENT_PALETTE (greenYell2){0,191,251,0,   50,204,251,0,   100,191,251,0,   150,0,255,255,     200,40,252,184, 255,191,251,0};
DEFINE_GRADIENT_PALETTE (greenYell3){0,191,251,0,   50,204,251,0,   100,191,251,0,    150,0,70,255,    200, 30,147,255,   255,191,251,0};
DEFINE_GRADIENT_PALETTE (greenYell4){0,191,251,0,   50,204,251,0,   100,191,251,0,    150,192,61,255,    200, 155,92,255,   255,191,251,0};

CRGBPalette16 greenYell0Pal= greenYell0;
CRGBPalette16 greenYell1Pal= greenYell1;
CRGBPalette16 greenYell2Pal= greenYell2;
CRGBPalette16 greenYell3Pal= greenYell3;
CRGBPalette16 greenYell4Pal= greenYell4;

CRGBPalette16 greenYells[5]= {greenYell0Pal, greenYell1Pal, greenYell2Pal, greenYell3Pal, greenYell4Pal};


//----Yellow----
DEFINE_GRADIENT_PALETTE (yellow0){0,255,233,125,  50,255,240,184, 100,246,251,118,   150,255,230,133,   200, 251,252,194,   255,255,233,125};
DEFINE_GRADIENT_PALETTE (yellow1){0,255,211,0,   50,255,226,115,   100,242,251,0,    150,255,219,80,   200, 250,252,121,    255,255,211,0};
DEFINE_GRADIENT_PALETTE (yellow2){0,255,211,0,   50,255,226,115,   100,242,251,0,   150,98,251,62,     200,191,251,0,   255,255,211,0};
DEFINE_GRADIENT_PALETTE (yellow3){0,255,211,0,   50,255,226,115,   100,242,251,0,    150,0,255,255,    200, 0,190,255,    255,255,211,0};
DEFINE_GRADIENT_PALETTE (yellow4){0,255,211,0,   50,255,226,115,   100,242,251,0,    150,79,125,255,    200, 155,92,255,    255,255,211,0};

CRGBPalette16 yellow0Pal= yellow0;
CRGBPalette16 yellow1Pal= yellow1;
CRGBPalette16 yellow2Pal= yellow2;
CRGBPalette16 yellow3Pal= yellow3;
CRGBPalette16 yellow4Pal= yellow4;

CRGBPalette16 yellows[5]= {yellow0Pal, yellow1Pal, yellow2Pal, yellow3Pal, yellow4Pal};

//----Orange----
DEFINE_GRADIENT_PALETTE (orange0){0,255,197,156,  50,255,182,120, 100,255,226,201,   150,255,195,145,   200, 255,227,117,   255,255,197,156};
DEFINE_GRADIENT_PALETTE (orange1){0,255,127,34,   50,255,135,32,   100,255,157,77,    150,255,219,80,   200, 255,203,0,     255,255,127,34};
DEFINE_GRADIENT_PALETTE (orange2){0,255,127,34,   50,255,135,32,   100,255,157,77,   150,242,251,0,     200,191,251,0,    255,255,127,34};
DEFINE_GRADIENT_PALETTE (orange3){0,255,127,34,   50,255,135,32,   100,255,157,77,    150,110,234,255,    200, 81,255,254,    255,255,127,34};
DEFINE_GRADIENT_PALETTE (orange4){0,255,127,34,   50,255,135,32,   100,255,157,77,    150,0,70,255,    200, 0,190,255,    255,255,127,34};

CRGBPalette16 orange0Pal= orange0;
CRGBPalette16 orange1Pal= orange1;
CRGBPalette16 orange2Pal= orange2;
CRGBPalette16 orange3Pal= orange3;
CRGBPalette16 orange4Pal= orange4;

CRGBPalette16 oranges[5]= {orange0Pal, orange1Pal, orange2Pal, orange3Pal, orange4Pal};

//----OrangeRed----
DEFINE_GRADIENT_PALETTE (orangeRed0){0,255,137,122,  50,255,156,156, 100,255,104,99,   150,255,188,143,   200, 255,167,92,    255,255,137,122};
DEFINE_GRADIENT_PALETTE (orangeRed1){0,255,63,41,   50,255,84,83,   100,255,12,5,    150,255,103,0,   200, 255,135,32,    255,255,63,41};
DEFINE_GRADIENT_PALETTE (orangeRed2){0,255,63,41,   50,255,84,83,   100,255,12,5,   150,242,251,0,     200,255,203,0,   255,255,63,41};
DEFINE_GRADIENT_PALETTE (orangeRed3){0,255,63,41,   50,255,84,83,   100,255,12,5,    150,192,251,40,    200, 0,251,0,   255,255,63,41};
DEFINE_GRADIENT_PALETTE (orangeRed4){0,255,63,41,   50,255,84,83,   100,255,12,5,    150,0,255,255,    200, 121,252,160,    255,255,63,41};

CRGBPalette16 orangeRed0Pal= orangeRed0;
CRGBPalette16 orangeRed1Pal= orangeRed1;
CRGBPalette16 orangeRed2Pal= orangeRed2;
CRGBPalette16 orangeRed3Pal= orangeRed3;
CRGBPalette16 orangeRed4Pal= orangeRed4;

CRGBPalette16 orangeReds[5]= {orangeRed0Pal, orangeRed1Pal, orangeRed2Pal, orangeRed3Pal, orangeRed4Pal};

//----Red----
DEFINE_GRADIENT_PALETTE (red0){0,255,164,188,  50,255,159,182, 100,255,169,169,   150,255,120,149,   200, 255,110,110,    255,255,164,188};
DEFINE_GRADIENT_PALETTE (red1){0,255,12,5,   50,255,85,122,   100,255,46,89,    150,255,84,83,   200, 255,15,57,    255,255,12,5};
DEFINE_GRADIENT_PALETTE (red2){0,255,12,5,   50,255,85,122,   100,255,46,89,    150,255,203,0,     200,255,135,32,    255,255,12,5};
DEFINE_GRADIENT_PALETTE (red3){0,255,12,5,   50,255,85,122,   100,255,46,89,     150,0,251,74,    200, 225,249,122,   255,255,12,5};
DEFINE_GRADIENT_PALETTE (red4){0,255,12,5,   50,255,85,122,   100,255,46,89,     150,0,255,255,    200, 0,137,255,    255,255,12,5};

CRGBPalette16 red0Pal= red0;
CRGBPalette16 red1Pal= red1;
CRGBPalette16 red2Pal= red2;
CRGBPalette16 red3Pal= red3;
CRGBPalette16 red4Pal= red4;

CRGBPalette16 reds[5]= {red0Pal, red1Pal, red2Pal, red3Pal, red4Pal};

CRGBPalette16 durColors[7][5] = {
{greenBlue0Pal,greenBlue1Pal,greenBlue2Pal,greenBlue3Pal,greenBlue4Pal} ,
{green0Pal, green1Pal, green2Pal, green3Pal, green4Pal},
{greenYell0Pal, greenYell1Pal, greenYell2Pal, greenYell3Pal, greenYell4Pal},
{yellow0Pal, yellow1Pal, yellow2Pal, yellow3Pal, yellow4Pal},
{orange0Pal, orange1Pal, orange2Pal, orange3Pal, orange4Pal},
{orangeRed0Pal, orangeRed1Pal, orangeRed2Pal, orangeRed3Pal, orangeRed4Pal},
{red0Pal, red1Pal, red2Pal, red3Pal, red4Pal}
} ;

//----Cyan----
DEFINE_GRADIENT_PALETTE (cyan0){0,125,255,251,  50,166,238,255, 100,179,241,255,   150,217,255,253,   200, 241,255,255,    255,125,255,251};
DEFINE_GRADIENT_PALETTE (cyan1){0,0,255,255,   50,21,200,255,   100,75,211,255,    150,159,255,253,   200, 118,222,255,    255,0,255,255};
DEFINE_GRADIENT_PALETTE (cyan2){0,0,255,255,   50,21,200,255,   100,75,211,255,    150,141,139,255,   200, 118,163,255,    255,0,255,255};
DEFINE_GRADIENT_PALETTE (cyan3){0,0,255,255,   50,21,200,255,   100,75,211,255,    150,89,46,255,   200, 149,120,255,    255,0,255,255};;
DEFINE_GRADIENT_PALETTE (cyan4){0,0,255,255,   50,21,200,255,   100,75,211,255,    150,255,46,255,   200, 180,44,255,    255,0,255,255};


CRGBPalette16 cyan0Pal= cyan0;
CRGBPalette16 cyan1Pal= cyan1;
CRGBPalette16 cyan2Pal= cyan2;
CRGBPalette16 cyan3Pal= cyan3;
CRGBPalette16 cyan4Pal= cyan4;

//----CyanBlue----
DEFINE_GRADIENT_PALETTE (cyanBlue0){0,128,228,255,  50,128,207,255, 100,128,201,255,   150,128,255,252,   200, 179,248,255,    255,128,228,255};
DEFINE_GRADIENT_PALETTE (cyanBlue1){0,0,190,255,   50,0,148,255,   100,0,137,255,    150,0,255,255,   200, 110,234,255,    255,0,190,255};
DEFINE_GRADIENT_PALETTE (cyanBlue2){0,0,190,255,   50,0,148,255,   100,0,137,255,    150,155,92,255,   200, 109,92,255,    255,0,190,255};
DEFINE_GRADIENT_PALETTE (cyanBlue3){0,0,190,255,   50,0,148,255,   100,0,137,255,    150,255,46,89,   200, 255,46,255,    255,0,190,255};
DEFINE_GRADIENT_PALETTE (cyanBlue4){0,0,190,255,   50,0,148,255,   100,0,137,255,    150,255,203,0,   200, 255,12,5,    255,0,190,255};

CRGBPalette16 cyanBlue0Pal= cyanBlue0;
CRGBPalette16 cyanBlue1Pal= cyanBlue1;
CRGBPalette16 cyanBlue2Pal= cyanBlue2;
CRGBPalette16 cyanBlue3Pal= cyanBlue3;
CRGBPalette16 cyanBlue4Pal= cyanBlue4;

//----Blue----
DEFINE_GRADIENT_PALETTE (blue0){0,131,169,255,  50,153,185,255, 100,189,213,255,   150,136,201,255,   200, 95,187,255,    255,131,169,255};
DEFINE_GRADIENT_PALETTE (blue1){0,0,70,255,   50,79,125,255,   100,35,96,255,    150,0,127,255,   200, 30,147,255,    255,0,70,255};
DEFINE_GRADIENT_PALETTE (blue2){0,0,70,255,   50,79,125,255,   100,35,96,255,    150,180,44,255,   200, 255,46,255,    255,0,70,255};
DEFINE_GRADIENT_PALETTE (blue3){0,0,70,255,   50,79,125,255,   100,35,96,255,    150,255,135,32,   200, 255,12,5,    255,0,70,255};
DEFINE_GRADIENT_PALETTE (blue4){0,0,70,255,   50,79,125,255,   100,35,96,255,    150,255,203,0,   200, 255,135,32,    255,0,70,255};

CRGBPalette16 blue0Pal= blue0;
CRGBPalette16 blue1Pal= blue1;
CRGBPalette16 blue2Pal= blue2;
CRGBPalette16 blue3Pal= blue3;
CRGBPalette16 blue4Pal= blue4;

//----BluePurp----
DEFINE_GRADIENT_PALETTE (bluePurp0){0,167,125,255,  50,177,168,255, 100,191,150,255,   150,156,148,255,   200, 122,124,255,    255,167,125,255};
DEFINE_GRADIENT_PALETTE (bluePurp1){0,111,43,255,   50,109,92,255,   100,155,92,255,    150,74,60,255,   200, 39,42,255,    255,111,43,255};
DEFINE_GRADIENT_PALETTE (bluePurp2){0,111,43,255,   50,109,92,255,   100,155,92,255,    150,255,46,255,   200, 192,61,255,    255,111,43,255};
DEFINE_GRADIENT_PALETTE (bluePurp3){0,111,43,255,   50,109,92,255,   100,155,92,255,    150,255,15,57,   200, 255,103,0,    255,111,43,255};
DEFINE_GRADIENT_PALETTE (bluePurp4){0,111,43,255,   50,109,92,255,   100,155,92,255,    150,244,255,0,   200, 255,203,0,    255,111,43,255};

CRGBPalette16 bluePurp0Pal= bluePurp0;
CRGBPalette16 bluePurp1Pal= bluePurp1;
CRGBPalette16 bluePurp2Pal= bluePurp2;
CRGBPalette16 bluePurp3Pal= bluePurp3;
CRGBPalette16 bluePurp4Pal= bluePurp4;

//----Purple----
DEFINE_GRADIENT_PALETTE (purp0){0,203,176,255,  50,206,131,255, 100,215,146,255,   150,180,146,255,   200, 168,136,255,    255,203,176,255};
DEFINE_GRADIENT_PALETTE (purp1){0,178,128,255,   50,180,44,255,   100,192,61,255,    150,133,60,255,   200, 11,43,255,    255,178,128,255};
DEFINE_GRADIENT_PALETTE (purp2){0,178,128,255,   50,180,44,255,   100,192,61,255,    150,255,23,125,   200, 255,46,255,    255,178,128,255};
DEFINE_GRADIENT_PALETTE (purp3){0,178,128,255,   50,180,44,255,   100,192,61,255,    150,244,255,0,   200, 255,103,0,    255,178,128,255};
DEFINE_GRADIENT_PALETTE (purp4){0,178,128,255,   50,180,44,255,   100,192,61,255,    150,191,251,0,   200, 0,251,0,    255,178,128,255};

CRGBPalette16 purp0Pal= purp0;
CRGBPalette16 purp1Pal= purp1;
CRGBPalette16 purp2Pal= purp2;
CRGBPalette16 purp3Pal= purp3;
CRGBPalette16 purp4Pal= purp4;

//----PurplePink----
DEFINE_GRADIENT_PALETTE (purpPink0){0,252,159,255,  50,252,138,255, 100,255,161,235,   150,201,118,235,   200, 252,146,255,    255,252,159,255};
DEFINE_GRADIENT_PALETTE (purpPink1){0,255,128,255,   50,255,46,255,   100,255,90,213,    150,180,44,255,   200, 255,93,255,    255,255,128,255};
DEFINE_GRADIENT_PALETTE (purpPink2){0,255,128,255,   50,255,46,255,   100,255,90,213,    150,255,84,83,   200, 255,15,57,    255,255,128,255};
DEFINE_GRADIENT_PALETTE (purpPink3){0,255,128,255,   50,255,46,255,   100,255,90,213,    150,242,251,0,   200, 191,251,255,    255,255,128,255};
DEFINE_GRADIENT_PALETTE (purpPink4){0,255,128,255,   50,255,46,255,   100,255,90,213,    150,192,251,40,   200, 9,251,106,    255,255,128,255};

CRGBPalette16 purpPink0Pal= purpPink0;
CRGBPalette16 purpPink1Pal= purpPink1;
CRGBPalette16 purpPink2Pal= purpPink2;
CRGBPalette16 purpPink3Pal= purpPink3;
CRGBPalette16 purpPink4Pal= purpPink4;

//----Pink----
DEFINE_GRADIENT_PALETTE (pink0){0,255,133,191,  50,255,143,231, 100,255,136,167,   150,255,148,202,   200, 255,182,220,    255,255,133,191};
DEFINE_GRADIENT_PALETTE (pink1){0,255,23,125,   50,255,55,202,   100,255,15,57,    150,255,12,5,   200, 255,87,167,    255,255,23,125};
DEFINE_GRADIENT_PALETTE (pink2){0,255,23,125,   50,255,55,202,   100,255,15,57,    150,255,135,32,   200, 255,203,0,    255,255,23,125};
DEFINE_GRADIENT_PALETTE (pink3){0,255,23,125,   50,255,55,202,   100,255,15,57,    150,242,251,0,   200, 192,251,40,    255,255,23,125};
DEFINE_GRADIENT_PALETTE (pink4){0,255,23,125,   50,255,55,202,   100,255,15,57,    150,81,255,254,   200, 121,252,160,    255,255,23,125};

CRGBPalette16 pink0Pal= pink0;
CRGBPalette16 pink1Pal= pink1;
CRGBPalette16 pink2Pal= pink2;
CRGBPalette16 pink3Pal= pink3;
CRGBPalette16 pink4Pal= pink4;

CRGBPalette16 mollColors[7][5] = {
{cyan0Pal,cyan1Pal,cyan2Pal,cyan3Pal,cyan4Pal} ,
{cyanBlue0Pal,cyanBlue1Pal,cyanBlue2Pal,cyanBlue3Pal,cyanBlue4Pal},
{blue0Pal,blue1Pal,blue2Pal,blue3Pal,blue4Pal},
{bluePurp0Pal,bluePurp1Pal,bluePurp2Pal,bluePurp3Pal,bluePurp4Pal},
{purp0Pal, purp1Pal, purp2Pal, purp3Pal, purp4Pal},
{purpPink0Pal,purpPink1Pal,purpPink2Pal,purpPink3Pal,purpPink4Pal},
{pink0Pal,pink1Pal,pink2Pal,pink3Pal,pink4Pal}
} ;

DEFINE_GRADIENT_PALETTE (test){0,255,0,0,  50,0,255,0, 100,0,0,255,  150, 255,0,255,  200,255,255,255,  255,255,0,0, };
CRGBPalette16 testPal= test;

DEFINE_GRADIENT_PALETTE (whiteMap){0,255,255,255,   127, 20,20,20,    255,255,255,255};
CRGBPalette16 whitePal= whiteMap;

DEFINE_GRADIENT_PALETTE (speakMap){0,255,255,255,  50,255,215,141, 100,255,246,141,  150, 255,200,141,  200,134,119,118,  255,255,255,255};
CRGBPalette16 speakPal= speakMap;


int valenzParam;
int energyParam;
int modeParam=1;
bool flip=false;
int speedParam;

CRGBPalette16 colorPalette;

float mapOwn(float x, float in_min, float in_max, float out_min, float out_max) {

    if(x<=in_min){ return out_min;}
    else if(x>=in_max){ return out_max;}
    else{
      const float dist1= in_max-in_min;
      const float dist2= out_max-out_min;
      const float delta= (x-in_min)/dist1;

      float newOut= (dist2*delta)+ out_min;
      return newOut;
    }
}
void setupMatrix(){
    for(int y=0; y<57; y++){
    for(int x=0; x<57; x++){
      ledMatrix[y][x]= 0;
    }
  }
  for(int x=0; x<160; x++){
    float koorXE;
    float koorYE;
    float angle= 90-((360.0/160.0)*x);
    koorXE= cos(angle*PI/180);
    koorYE= sin(angle*PI/180);


    
    float koorX=  mapOwn(koorXE, -1.0,1.0,0.0,56.0);
    float koorY=  mapOwn(koorYE, -1.0,1.0,0.0,56.0);
    int xArr= (int)(koorX + 0.5);
    int yArr= (int)(koorY + 0.5);
    
    if(ledMatrix[xArr][yArr]==0){
        ledMatrix[xArr][yArr] = x+1000; 
    }
    else{
      ledMatrix[xArr-1][yArr] = x+1000; 
      Serial.print("Hier ist schon wer, Ich bin: ");
      Serial.println(x);
    }
    
  }

    for(int x=0; x<96; x++){
    float koorXE;
    float koorYE;
    float angle= 90-((360.0/96.0)*x);

    koorXE= cos(angle*PI/180);
    koorYE= sin(angle*PI/180);


    
    float koorX=  mapOwn(koorXE, -1.0,1.0,12.0,44.0);
    float koorY=  mapOwn(koorYE, -1.0,1.0,12.0,44.0);
    int xArr= (int)(koorX + 0.5);
    int yArr= (int)(koorY + 0.5);
    
     if(ledMatrix[xArr][yArr]==0){
        ledMatrix[xArr][yArr] = x+2000; 
    }
  }

}




void setPalette() {
  if(modeParam==1){ //Dur
     colorPalette= durColors[valenzParam][energyParam];
  }
  else{ //Moll
     colorPalette= mollColors[valenzParam][energyParam];
  }
}

void fillPalette2(CRGBPalette16 pal){
  fill_palette_circular(leds1,NUM_LEDS1,0,pal,255,LINEARBLEND,false);
  fill_palette_circular(leds2,NUM_LEDS1,0,pal,255,LINEARBLEND,false);
  if(flip){
    fill_palette_circular(leds3,NUM_LEDS2,0,pal,255,LINEARBLEND,false);
    fill_palette_circular(leds4,NUM_LEDS2,0,pal,255,LINEARBLEND,false);
  }
  else{
    fill_palette_circular(leds3,NUM_LEDS2,127,pal,255,LINEARBLEND,false);
    fill_palette_circular(leds4,NUM_LEDS2,127,pal,255,LINEARBLEND,false);
  }
}


void fillPallete(int valence, int energy, int mode) {
   CRGBPalette16 colorPalette;
  if(mode==1){ //Dur
     colorPalette= durColors[valence][energy];
  }
  else{
     colorPalette= mollColors[valence][energy];
  }
 
  //CRGBPalette16 colorPalette= testPal;
  fill_palette_circular(leds1,NUM_LEDS1,0,colorPalette,255,LINEARBLEND,false);
  fill_palette_circular(leds2,NUM_LEDS1,0,colorPalette,255,LINEARBLEND,false);
  if(flip){
    fill_palette_circular(leds3,NUM_LEDS2,0,colorPalette,255,LINEARBLEND,false);
    fill_palette_circular(leds4,NUM_LEDS2,0,colorPalette,255,LINEARBLEND,false);
  }
  else{
    fill_palette_circular(leds3,NUM_LEDS2,127,colorPalette,255,LINEARBLEND,false);
    fill_palette_circular(leds4,NUM_LEDS2,127,colorPalette,255,LINEARBLEND,false);
  }

}





String sliderValue = "0";
// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}
const char* PARAM_INPUT = "value";


int maxMode=9;

int correctionMode=0;

void noiseEffect(CRGBPalette16 pal, int speed, int indexScale, bool anti){

  float helpSpeed= mapOwn(speed, 40,180,15,0);
  int thisSpeed= int(helpSpeed+0.5);
  for (int i = 0; i < NUM_LEDS1; i++) {
    uint8_t index = inoise8(i * indexScale*0.6, millis() /thisSpeed);
    leds1[i] = ColorFromPalette(pal, index, 255);
    leds2[i] = ColorFromPalette(pal, index, 255);
  }
   for (int i = 0; i < NUM_LEDS2; i++) {
    uint8_t index = inoise8(i * indexScale, millis() /thisSpeed);
    if(anti){
       leds3[i] = ColorFromPalette(pal, index+127, 255);
    leds4[i] = ColorFromPalette(pal, index+127, 255);
    }
    else{leds3[i] = ColorFromPalette(pal, index, 255);
    leds4[i] = ColorFromPalette(pal, index, 255);}
  }
}

int rotAnimationIdx=0;
void rotStripes(int speed, bool flip, CRGBPalette16 pal) {
  int thisSpeed= mapOwn(speed,30,180,1,13);
  fill_palette_circular(&leds1[0],NUM_LEDS1/(2),rotAnimationIdx,pal,255,LINEARBLEND,false);
  fill_palette_circular(&leds2[0],NUM_LEDS1/(2),rotAnimationIdx,pal,255,LINEARBLEND,false);
  fill_palette_circular(&leds3[0],NUM_LEDS2/(2),rotAnimationIdx,pal,255,LINEARBLEND,flip);
  fill_palette_circular(&leds4[0],NUM_LEDS2/(2),rotAnimationIdx,pal,255,LINEARBLEND,flip);


  fill_palette_circular(&leds1[NUM_LEDS1/2],NUM_LEDS1/(2),rotAnimationIdx,pal,255,LINEARBLEND,false);
  fill_palette_circular(&leds2[NUM_LEDS1/2],NUM_LEDS1/(2),rotAnimationIdx,pal,255,LINEARBLEND,false);
  fill_palette_circular(&leds3[NUM_LEDS2/2],NUM_LEDS2/(2),rotAnimationIdx,pal,255,LINEARBLEND,flip);
  fill_palette_circular(&leds4[NUM_LEDS2/2],NUM_LEDS2/(2),rotAnimationIdx,pal,255,LINEARBLEND,flip);

  EVERY_N_MILLISECONDS(40){
      rotAnimationIdx+= thisSpeed;
  }

}

int sprarkIdx[5]={0,30,50,70,0};
int sprarkIdx2[5]={0,30,50,70,0};
int sprarkTime[5]={5,10,-5,7,1};
void sparkling(bool flip, int speed, CRGBPalette16 colorPal){
  int bpm=mapOwn(speed,30,180,10,150);
  int points=5;
 // FastLED.clear();

  fill_palette(leds1, NUM_LEDS1,0,150/NUM_LEDS1,colorPal,255,LINEARBLEND);
  fill_palette(leds2, NUM_LEDS1,0,150/NUM_LEDS1,colorPal,255,LINEARBLEND);
  fill_palette(leds3, NUM_LEDS2,0,150/NUM_LEDS2,colorPal,255,LINEARBLEND);
  fill_palette(leds4, NUM_LEDS2,0,150/NUM_LEDS2,colorPal,255,LINEARBLEND);


  for(int i=0; i<points; i++){
    float sinBeat= beatsin8(bpm+sprarkTime[i],0,255,0+(255/points)*i,0);
   // float sinBeat=255.0;
    CRGB kontrast;
    if(i%2==0){
      kontrast= ColorFromPalette(colorPal,150,255,LINEARBLEND);
    }
    else{
      kontrast= ColorFromPalette(colorPal,200,255,LINEARBLEND);
    } 
    
    leds1[sprarkIdx[i]]= nblend(leds1[sprarkIdx[i]],kontrast, sinBeat);
    leds2[sprarkIdx[i]]= nblend(leds1[sprarkIdx[i]],kontrast, sinBeat);
    leds3[sprarkIdx2[i]]= nblend(leds3[sprarkIdx[i]],kontrast, sinBeat);
    leds4[sprarkIdx2[i]]= nblend(leds3[sprarkIdx[i]],kontrast, sinBeat);
    for(int w=1; w<=8; w++){
      float diff= (1.0/8.0)*w;
      float sin= sinBeat*(1.0-diff);

     
      leds1[(sprarkIdx[i]+w)%NUM_LEDS1]= nblend( leds1[(sprarkIdx[i]+w)%NUM_LEDS1],kontrast, sin);
      leds2[(sprarkIdx[i]+w)%NUM_LEDS1]= nblend(leds2[(sprarkIdx[i]+w)%NUM_LEDS1],kontrast, sin); 
      leds3[(sprarkIdx[i]+w)%NUM_LEDS2]= nblend(leds3[(sprarkIdx[i]+w)%NUM_LEDS2],kontrast, sin);
      leds4[(sprarkIdx[i]+w)%NUM_LEDS2]= nblend(leds3[(sprarkIdx[i]+w)%NUM_LEDS2],kontrast, sin); 

      leds1[(sprarkIdx[i]+NUM_LEDS1-w)%NUM_LEDS1]= nblend(leds1[(sprarkIdx[i]+NUM_LEDS1-w)%NUM_LEDS1],kontrast, sin);
      leds2[(sprarkIdx[i]+NUM_LEDS1-w)%NUM_LEDS1]= nblend(leds1[(sprarkIdx[i]+NUM_LEDS1-w)%NUM_LEDS1],kontrast, sin); 
      leds3[(sprarkIdx2[i]+NUM_LEDS2-w)%NUM_LEDS2]= nblend(leds3[(sprarkIdx2[i]+NUM_LEDS2-w)%NUM_LEDS2],kontrast, sin);
      leds4[(sprarkIdx2[i]+NUM_LEDS2-w)%NUM_LEDS2]= nblend(leds3[(sprarkIdx2[i]+NUM_LEDS2-w)%NUM_LEDS2],kontrast, sin); 


    }
    
    if(sinBeat==1){
        sprarkIdx[i]= rand()%NUM_LEDS1;
        if(flip){
          sprarkIdx2[i]= rand()%NUM_LEDS2;
        }
        else{
           sprarkIdx2[i]= mapOwn(sprarkIdx[i],0,NUM_LEDS1-1,0,NUM_LEDS2-1);
        }
    } 
  } 
  
}



int shifta=0;
int shiftb=10;
int shiftc=2;
float shiftd=0.2;
int x=0;
void noiseStripes(int thisspeed, CRGBPalette16 colorPal, bool flip){

    int speed= int(mapOwn(thisspeed,30,180,1,7)+0.5);
  
    CRGB colorK= ColorFromPalette(colorPal,150,255,LINEARBLEND);
    CRGB colorK2= ColorFromPalette(colorPal,200,255,LINEARBLEND);
  
    u_int16_t posBeat1a= beatsin16(shifta*speed,0, NUM_LEDS1*2,0,0);
    u_int16_t posBeat2a= beatsin16(shiftb*speed,0, NUM_LEDS1*2,0,0);
    u_int16_t posBeat3a= beatsin16(shiftc*speed,0, NUM_LEDS1*2,0,0);
    u_int16_t posBeat4a= beatsin16(shiftd*speed,0, NUM_LEDS1*2,0,0);

  
  
    
  
    int pos1= (posBeat1a+posBeat2a+posBeat3a+posBeat4a)/4; 
     pos1= mapOwn(pos1,0,NUM_LEDS1*2,0,NUM_LEDS1-1);

    u_int8_t shiftBeat= beatsin8(speed,0,255,millis(),0);

    int shift= mapOwn(shiftBeat,0,255,-8,8);
   
   
   
   

    int pos2= (pos1+NUM_LEDS1/2)%NUM_LEDS1;

    int pos1_2;
    int pos2_2;   

    if(!flip){
      pos1_2= mapOwn(pos1,0,NUM_LEDS1-1, 0, NUM_LEDS2-1);
      pos2_2= mapOwn(pos2,0,NUM_LEDS1-1, 0, NUM_LEDS2-1);
    }
    else{
      int helpPos1= (pos1+NUM_LEDS1/4)%NUM_LEDS1;
      int helpPos2=  (pos1+NUM_LEDS1/4*3)%NUM_LEDS1;
      pos1_2= mapOwn(helpPos1,0,NUM_LEDS1-1, 0, NUM_LEDS2-1);
      pos2_2= mapOwn(helpPos2,0,NUM_LEDS1-1, 0, NUM_LEDS2-1);
    }

    pos1_2= (pos1_2+shift+NUM_LEDS2)%NUM_LEDS2;
    pos2_2= (pos2_2+shift+NUM_LEDS2)%NUM_LEDS2;


    leds1[pos1]= colorK;
    leds2[pos1]= colorK;
    leds1[pos2]= colorK2;
    leds2[pos2]= colorK2;



    leds3[(pos1_2)%NUM_LEDS2]= colorK;
    leds4[(pos1_2)%NUM_LEDS2]= colorK;
    leds3[(pos2_2)%NUM_LEDS2]= colorK2;
    leds4[(pos2_2)%NUM_LEDS2]= colorK2; 

    for(int i=0; i<NUM_LEDS1; i++){
      float pos=int(mapOwn(i,0,NUM_LEDS1,0,125));
      CRGB color= ColorFromPalette(colorPal,int(pos+0.5),255,LINEARBLEND);
       leds1[i]= nblend(leds1[i],color, 5);
       leds2[i]= nblend(leds2[i],color, 5);
    }
    for(int i=0; i<NUM_LEDS2; i++){
      float pos=int(mapOwn(i,0,NUM_LEDS2,0,125));
      CRGB color= ColorFromPalette(colorPal,int(pos+0.5),255,LINEARBLEND);
       leds3[i]= nblend(leds3[i],color, speed);
       leds4[i]= nblend(leds4[i],color, speed);
    }

}

void fillMatrixPixelWithColor(int ledIdx, CRGB colorCurr){
    if(ledIdx>=1000 && ledIdx<2000){
        int idx= ledIdx-1000;
        leds1[idx]=colorCurr;
        leds2[idx]= colorCurr;
    }
    else if(ledIdx>=2000){
      int idx= ledIdx-2000;
        leds3[idx]= colorCurr;
        leds4[idx]=colorCurr;
    }   
}
int indexAni=0;
u_int8_t indexWave=0;
void fillMatrixWithGradient(CRGBPalette16 pal, int speed, bool flip){

  int shift=60;
  int thisspeed= mapOwn(speed, 30,180,1,15);
  for(int y=0; y<28; y++){
    for(int x=0; x<57; x++){
      int palIdx= mapOwn(y,0,28,0,255);
       // float sinX= sin((2*3.14/56)*x+(indexWave*0.1))*shift;
       float sinX=1;
        int sinXInt= int(sinX);
        fillMatrixPixelWithColor(ledMatrix[x][y], ColorFromPalette( pal, (palIdx+indexAni+255+sinXInt)%255));
    
      
    }
  }

   for(int y=28; y<57; y++){
    for(int x=0; x<57; x++){
      int palIdx= mapOwn(y,28,56,0,255);
      //float sinX= sin((2*3.14/56)*x+(indexWave*0.1))*shift;
      float sinX=1;
        int sinXInt= int(sinX);
        if(flip){
          fillMatrixPixelWithColor(ledMatrix[x][y], ColorFromPalette( pal, (palIdx-indexAni+255+sinXInt)%255));
        }
        else{
          fillMatrixPixelWithColor(ledMatrix[x][y], ColorFromPalette( pal, (palIdx+indexAni+255+sinXInt)%255));

        }
  }
   }

  EVERY_N_MILLISECONDS(40){
    indexAni= (indexAni+thisspeed)%255;
    indexWave=(indexWave+1)%255;
  }
}



int loadingCnt=0;
void loading(){
  CHSV white(20,0,255);
  CRGB yellow(220,255,36);
  int speed=20;


  int index1=  mapOwn(beat8(speed),0,255,0,NUM_LEDS1-1);
  int index2=  mapOwn(beat8(speed),0,255,0,NUM_LEDS2-1);

  leds1[index1]= yellow;
  leds2[index1]= yellow;
  leds3[index2]= yellow;
  leds4[index2]= yellow;

   for(int i=0; i<NUM_LEDS1; i++){
       leds1[i]= nblend(leds1[i],white, 5);
       leds2[i]= nblend(leds2[i],white, 5);
    }
    for(int i=0; i<NUM_LEDS2; i++){
       leds3[i]= nblend(leds3[i],white, 5);
       leds4[i]= nblend(leds4[i],white, 5);
    }

    EVERY_N_SECONDS(1){
      Serial.println("Waiting for connection...");
      loadingCnt++;

    }
}

void waiting(){

  u_int8_t brightSin= beatsin8(7,0,255,0,0);

  //fill_gradient(leds1,NUM_LEDS1,CHSV(255,0,80),CHSV(255,0,255));
  /*fill_solid(leds1,NUM_LEDS1,CHSV(255,0,brightSin));
  fill_solid(leds2,NUM_LEDS1,CHSV(255,0,brightSin));
  fill_solid(leds3,NUM_LEDS2,CHSV(255,0,brightSin));
  fill_solid(leds4,NUM_LEDS2,CHSV(255,0,brightSin)); */

  CRGB colorNow= ColorFromPalette(whitePal,brightSin,255,LINEARBLEND);
  fill_solid(leds1,NUM_LEDS1,colorNow);
  fill_solid(leds2,NUM_LEDS1,colorNow);
  fill_solid(leds3,NUM_LEDS2,colorNow);
  fill_solid(leds4,NUM_LEDS2,colorNow);

}

bool blink;
int blinkCounter;
void error(){
  CRGB white= CRGB(255,255,255);
  CRGB red= CRGB(255,0,0);

if(blinkCounter<10){
    if(blink){
    fill_solid(leds1,NUM_LEDS1,white);
    fill_solid(leds2,NUM_LEDS1,white);
    fill_solid(leds3,NUM_LEDS2,white);
    fill_solid(leds4,NUM_LEDS2,white);
  }
  else{
    fill_solid(leds1,NUM_LEDS1,red);
    fill_solid(leds2,NUM_LEDS1,red);
    fill_solid(leds3,NUM_LEDS2,red);
    fill_solid(leds4,NUM_LEDS2,red);
  }

  EVERY_N_MILLISECONDS(500){
    blink=!blink;
    blinkCounter++;
  }
}
  else{
    fill_solid(leds1,NUM_LEDS1,white);
    fill_solid(leds2,NUM_LEDS1,white);
    fill_solid(leds3,NUM_LEDS2,white);
    fill_solid(leds4,NUM_LEDS2,white);
  }
}

int animationMode=0;
void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2812, PIN1, GRB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2812, PIN2, GRB>(leds2, NUM_LEDS1);
  FastLED.addLeds<WS2812, PIN3, GRB>(leds3, NUM_LEDS2);  
  FastLED.addLeds<WS2812, PIN4, GRB>(leds4, NUM_LEDS2);


  FastLED.setBrightness(60);
  FastLED.setCorrection(TypicalPixelString);

  if(modeParam==1){ //Dur
     colorPalette= durColors[valenzParam][energyParam];
  }
  else{
     colorPalette= mollColors[valenzParam][energyParam];
  }


  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*"); 
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
  DefaultHeaders::Instance().addHeader("Content-Type", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "DELETE, POST, GET, OPTIONS, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");

  speedParam=60;
  setupMatrix();

    //Initalize WIFI
  WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    if(loadingCnt<15){  
      loading();
      
    }
    else{
      error();
    }
    FastLED.show();
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());  

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/change", HTTP_GET, [] (AsyncWebServerRequest *request) {
    animationMode= (animationMode+1)%5;
    request->send(200, "text/plain", "OK");
  });
  server.on("/flip", HTTP_GET, [] (AsyncWebServerRequest *request) {
    flip=!flip;
    Serial.println("Flip: " + String(flip));
    request->send(200, "text/plain", "OK");
  });
  server.on("/correction", HTTP_GET, [] (AsyncWebServerRequest *request) {
    correctionMode=(correctionMode+1)%3;
    if(correctionMode==0){
      FastLED.setCorrection(UncorrectedColor);
    }
    else if(correctionMode==1){
      FastLED.setCorrection(TypicalLEDStrip);
    }
    else if(correctionMode==1){
      FastLED.setCorrection(TypicalPixelString);
    }
    Serial.println("change correction: " + String(correctionMode));
    request->send(200, "text/plain", "OK");
  });
  server.on("/bright", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String sliderStr;
    int bright;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      sliderStr = request->getParam(PARAM_INPUT)->value();
      bright= sliderStr.toInt();
      if(bright>130){bright=130;}
      FastLED.setBrightness(bright);
      
    }
    else {
      sliderStr = "No message sent";
    }
    Serial.println("Bright: " + String(bright));
    request->send(200, "text/plain", "OK");
  });

  server.on("/valenz", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String sliderStr;
    int valenz;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      sliderStr = request->getParam(PARAM_INPUT)->value();
      valenz= sliderStr.toInt();
      if(valenz>6){valenz=6;}
      valenzParam=valenz;
      setPalette();
      
    }
    else {
      sliderStr = "No message sent";
    }
    Serial.println("Valenz: " + String(valenz));
    request->send(200, "text/plain", "OK");
  });

  server.on("/energy", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String sliderStr;
    int energy;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      sliderStr = request->getParam(PARAM_INPUT)->value();
      energy= sliderStr.toInt();
      if(energy>4){energy=4;}
      energyParam=energy;
      setPalette();
      
    }
    else {
      sliderStr = "No message sent";
    }
    Serial.println("Energy: " + String(energy));
    request->send(200, "text/plain", "OK");
  });
  server.on("/mode", HTTP_GET, [] (AsyncWebServerRequest *request) {
    modeParam=modeParam*-1;
    Serial.println("Mode: " + String(modeParam));
    setPalette();
    request->send(200, "text/plain", "OK");
  });
  server.on("/speed", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String sliderStr;
    int speed;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      sliderStr = request->getParam(PARAM_INPUT)->value();
      speed= sliderStr.toInt();
      if(speed<30){
        speed=30;
      }
      else if(speed>170){
        speed=170;
      }
      speedParam=speed;
      
    }
    else {
      sliderStr = "No message sent";
    }
    Serial.println("Speed (bpm): " + String(speedParam));
    request->send(200, "text/plain", "OK");
  });
    // Start server
  server.begin();



}

void loop() {


  if(animationMode==0){
      noiseEffect(colorPalette,speedParam,10,flip);
  }
  else if(animationMode==1)
  {
    rotStripes(speedParam,flip,colorPalette);
  }
  else if(animationMode==2)
  {
    sparkling(flip,speedParam,colorPalette);
  }
  else if(animationMode==3)
  {
    fillMatrixWithGradient(colorPalette,speedParam,flip);
  }
  else{
    noiseStripes(speedParam,colorPalette, flip);
  }
 
  

  FastLED.show();

 

} 
  

 