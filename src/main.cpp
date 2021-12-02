#include <Arduino.h>
#include <M5Stack.h>
#include <M5GFX.h>
M5GFX display;

int32_t width_half = 0;
int32_t height_half = 0;
const uint16_t FACE_COLOR = TFT_CYAN;
const uint16_t FACE_EDGE_COLOR = TFT_WHITE;
const uint16_t BODY_COLOR = TFT_WHITE;
const uint16_t TEXT_COLOR = TFT_DARKGRAY;


// 太線用
void drawCircleWeight(M5GFX *disp, int32_t x, int32_t y, int32_t r, int32_t weight){
  if(weight%2 == 1){
    disp->fillArc(x, y, r-weight/2.0 -1, r+weight/2.0, 0, 360);
  }else{
    disp->fillArc(x, y, r-weight/2.0, r+weight/2.0, 0, 360);
  }
}

void drawCircleWeight(M5GFX *disp, int32_t x, int32_t y, int32_t r, int32_t weight, uint16_t color565){
  disp->setColor(color565);
  drawCircleWeight(disp, x, y, r, weight);
}

void drawLineWeight(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t weight_px, uint16_t color565){
  uint8_t img[1] = {disp->color16to8(color565)};
  
  float angle = atan2f(y1 - y0, x1 - x0);
  float len = sqrtf(powf(x1 - x0, 2) + powf(y1 - y0, 2));// + weight_px;

  disp->pushImageRotateZoom(
    // x0 - weight_px/2.0 * cosf(angle),
    // y0 - weight_px/2.0 * sinf(angle),//描く原点
    x0,y0,//描く原点
    -0.5, 0, // 1x1 px の原点

    degrees(angle),      // degree
    len, weight_px, // zoom x, y
    1, 1,       // image w, h 
    img );
}
// void drawLineWeight(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t weight_px){
//   drawLineWeight(disp, x0, y0, x1, y1, weight_px, disp->getColor());
// }

void drawLineWeightCup(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t weight_px, uint16_t color565){
  drawLineWeight(disp, x0, y0, x1, y1, weight_px, color565);
  disp->setColor(color565);
  disp->fillCircle(x0, y0, weight_px/2.0);
  disp->fillCircle(x1, y1, weight_px/2.0);
}

void drawTriangleWeight(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t weight_px, uint16_t color565){
  drawLineWeight(disp, x0, y0, x1, y1, weight_px, color565);
  drawLineWeight(disp, x1, y1, x2, y2, weight_px, color565);
  drawLineWeight(disp, x2, y2, x0, y0, weight_px, color565);
}
void drawTriangleWeightCup(M5GFX *disp, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t weight_px, uint16_t color565){
  drawLineWeightCup(disp, x0, y0, x1, y1, weight_px, color565);
  drawLineWeightCup(disp, x1, y1, x2, y2, weight_px, color565);
  drawLineWeightCup(disp, x2, y2, x0, y0, weight_px, color565);
}


void setup() {
  // put your setup code here, to run once:

  display.begin();
  width_half = display.width()/2.0;
  height_half = display.height()/2.0;

  int32_t face_bottom = display.height()-60;
  int32_t mouth_bottom = face_bottom - 40+10;
  uint32_t mouth_width = 60;//80
  uint32_t mouth_height = 30;
  int32_t eye_l_y = face_bottom -100+10;
  int32_t eye_r_y = eye_l_y;
  int32_t eye_center_x = 50;
  int32_t eye_l_x = width_half - eye_center_x;
  int32_t eye_r_x = width_half + eye_center_x;
  int32_t eye_cross_r = 18;
  int32_t eye_weight = 10;
  
  display.fillScreen(TFT_BLACK);

  // test
  // drawLineWeight(&display, 100,100,  200,100, 20, TFT_WHITE);
  // drawLineWeight(&display, 100,100,  100,200, 20, TFT_WHITE);
  // display.setColor(TFT_RED);
  // display.drawLine(0,100,  display.width(), 100);
  // display.drawLine(100,0,  100,display.height());
  // display.setColor(TFT_BLACK);
  // display.drawLine(100,100,  200,100, 10);
  // display.drawLine(100,100,  100,200, 10);

  // body edge
  drawLineWeight(&display, 0,face_bottom, display.width(), face_bottom, 5, FACE_EDGE_COLOR);
  drawCircleWeight(&display, width_half, face_bottom+40-10, 200+10, 5, FACE_EDGE_COLOR);

  // mouth
  // drawCircleWeight(&display, width_half, face_bottom-40, 25, 10, FACE_COLOR);

  drawTriangleWeightCup(&display, 
    width_half, mouth_bottom - mouth_height,
    width_half - mouth_width/2.0, mouth_bottom,
    width_half + mouth_width/2.0, mouth_bottom, 9, FACE_COLOR);

  // eye
  drawLineWeight(&display, eye_l_x - eye_cross_r, eye_l_y - eye_cross_r, eye_l_x + eye_cross_r, eye_l_y + eye_cross_r, eye_weight, FACE_COLOR);
  drawLineWeight(&display, eye_l_x - eye_cross_r, eye_l_y + eye_cross_r, eye_l_x + eye_cross_r, eye_l_y - eye_cross_r, eye_weight, FACE_COLOR);
  drawLineWeight(&display, eye_r_x - eye_cross_r, eye_r_y - eye_cross_r, eye_r_x + eye_cross_r, eye_r_y + eye_cross_r, eye_weight, FACE_COLOR);
  drawLineWeight(&display, eye_r_x - eye_cross_r, eye_r_y + eye_cross_r, eye_r_x + eye_cross_r, eye_r_y - eye_cross_r, eye_weight, FACE_COLOR);

  // body
  display.fillRect(0, face_bottom, display.width(), display.height()-face_bottom,BODY_COLOR);
  display.setTextColor(TFT_DARKGRAY);
  display.setTextSize(2);
  display.drawString("HOSHIMA AI", 10, face_bottom+20);

  // drawLineWeight(&display, 0,0, 200,200, 10, FACE_COLOR);
}

void loop() {
  // put your main code here, to run repeatedly:
}