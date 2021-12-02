#include <Arduino.h>
#include <M5Stack.h>
#include <M5GFX.h>
M5GFX display;
const int FACE_COLOR = TFT_CYAN;

// 太線用
void drawCircleWeight(M5GFX *disp, int32_t x, int32_t y, int32_t r, int32_t weight){
  disp->fillArc(x, y, r-weight/2.0, r+weight/2.0, 0, 360);
}

void drawLineWeight(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t weight_px, uint16_t color565){
  uint8_t img[1] = {disp->color16to8(color565)};
  
  float angle = degrees( atan2f(y1 - y0, x1 - x0) );
  float len = sqrtf(powf(x1 - x0, 2) + powf(y1 - y0, 2));

  disp->pushImageRotateZoom(
    x0, y0,     //描く原点
    -0.5, -0.5, // 1x1 px の原点
    angle,      // degree
    len, weight_px, // zoom x, y
    1, 1,       // image w, h 
    img );
}
// void drawLineWeight(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t weight_px){
//   drawLineWeight(disp, x0, y0, x1, y1, weight_px, disp->getColor());
// }

void setup() {
  // put your setup code here, to run once:

  display.begin();
  display.fillScreen(TFT_BLACK);

  display.setColor(TFT_WHITE);
  drawCircleWeight(&display, display.width()/2.0, display.height()/2.0, 50, 10);

  drawLineWeight(&display, 0,0, 200,200, 10, TFT_WHITE);
}

void loop() {
  // put your main code here, to run repeatedly:
}