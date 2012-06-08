// --------------------------------------------------------------------------------
// An Arduino sketch for a tilt sensor capture game.
//
// MIT license.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
//      ******************************************************
//      Designed for the Adafruit Monochrome 128x64 OLED graphic display
//      http://www.adafruit.com/products/326
//      and Adafruit ADXL335 - 5V ready triple-axis accelerometer (+-3g analog out) break out board
//      http://www.adafruit.com/products/163
//      ******************************************************
//
//
// --------------------------------------------------------------------------------
// Dependencies
// --------------------------------------------------------------------------------
// Adafruit Industries's core graphics library:
//       https://github.com/adafruit/Adafruit-GFX-Library
// Adafruit Industries's SSD1306 OLED driver library:
//       https://github.com/adafruit/Adafruit_SSD1306
// --------------------------------------------------------------------------------
#include <Wire.h>
#include "PlayerSprite.h"
#include "TargetSprite.h"
#include <Adafruit_GFX.h>

#ifndef ADAFRUIT_SSD1306_H
#define ADAFRUIT_SSD1306_H
#include <Adafruit_SSD1306.h>
#endif

//! OLED display wired per tutorial.
// http://ladyada.net/products/oled12864/
#define OLED_DC 11
#define OLED_CS 12
#define OLED_CLK 10
#define OLED_MOSI 9
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//! Sensor variables.
#define X_IN_PIN 0
#define Y_IN_PIN 1
int origin_x;
int origin_y;

int raw_sensor_x;
int raw_sensor_y;

int tilt_sensor_x;
int tilt_sensor_y;
  
//! Player sprite.
PlayerSprite playerSprite;

//! Target sprite.
TargetSprite targetSprite;

//! The current state.
void (*state)() = NULL;

//! The state that the game was in prior to the current state.
void (*last_state)() = NULL;

//! The time in milliseconds since the last state change.
unsigned long last_state_change_time;

//! The current time in milliseconds since boot.
unsigned long time;

//! The current level.
uint8_t level;

//! The capture count.
uint8_t capture_count;

//! The current score.
long score;

//! Initialize the sketch.
void setup() {      
  // Initialize the serial connection for debugging.
  Serial.begin(9600);
  Serial.println(F("Serial initialized"));

  // Initialize the OLED.
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();
  Serial.println(F("OLED initialized"));
  
  // Configure the arduino to use an external source for the max value on the analog pins
  analogReference(EXTERNAL);

  // Assume the sensor is level, capture the current values as origin
  origin_y = analogRead(X_IN_PIN);
  origin_x = analogRead(Y_IN_PIN);

  // Initial game state
  state = begin_splash_screen;
}


//! Main loop of execution.
void loop() {
  time = millis();
  
  // Record time of state change so animations
  // know when to stop
  if (last_state != state) {
    last_state = state;
    last_state_change_time = time;
  }
  
  // Read the sensor values and swap coordinates to match OLED.
  raw_sensor_x = analogRead(Y_IN_PIN);
  raw_sensor_y = analogRead(X_IN_PIN);
  
  // Center sendor reads on origin and reduce jitter.
  tilt_sensor_x = (raw_sensor_x - origin_x) / 10;
  tilt_sensor_y = (raw_sensor_y - origin_y) / 10;
  
  // Call current state function
  state();

  delay(10);
}

// -------------------------------------------------------------------------------
// Game start states / functions
// -------------------------------------------------------------------------------

void begin_splash_screen() {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(F("Mosfet's"));
  display.setCursor(24, 48);
  display.print(F("Roundup"));
  display.setTextSize(1);

  display.display();
  
  state = animate_splash_screen;
}

void animate_splash_screen() {
  static boolean blink = true;
  static unsigned long last_blink_time;
  
  if (time - last_blink_time >= 1000) {
    display.setCursor(24, 24);
    if (blink) {
      display.print(F("Tilt to Start"));
    } else {
      display.fillRect(0, 24, 128, 8, BLACK);
    }
    blink = !blink;
    last_blink_time = time;
  }
  
  if (abs(tilt_sensor_x) >= 3 || abs(tilt_sensor_y) >= 3) {
    state = start_game;
  }
  
  display.display();
}

void start_game() {  
  // Reset game state
  score = 0;
  level = 1;

  state = begin_level;
}

// -------------------------------------------------------------------------------
// Level Transition States
// -------------------------------------------------------------------------------

void begin_level() {
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setCursor(16, 0);
  display.print(F("Capture"));
  display.setCursor(48, 32);
  display.print(level * 10);
  display.setTextSize(1);
  
  display.display();
    
  // Center the player sprite.
  playerSprite.setLocation(64, 32);
  
  // Set up the target sprite.
  spawn_target();

  // Reset level state.
  capture_count = 0;

  state = begin_level_delay;
}

void begin_level_delay() {
  if (time - last_state_change_time >= 3000) {
    display.clearDisplay();
    
    display.setCursor(0, 0);
    display.print(F("Captured "));

    display.display();
    state = play_game;
  }
}

void end_level() {
  // score += shield_power * 10; Time bonus???
  
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.print(F("Well Done!"));
  display.setTextSize(1);
  
  display.display();
  
  state = end_level_delay;
}

void end_level_delay() {
  if (time - last_state_change_time >= 3000) {
    level++;
    state = begin_level;
  }
}

// -------------------------------------------------------------------------------
// Main Game Loop State
// -------------------------------------------------------------------------------

//! Spawn a new target sprite.
void spawn_target() {
  targetSprite.setLocation(random(112), 8 + random(40));
  targetSprite.setVelocity(random(8) - 4, random(8) - 4);
}

//! Check collisions.
boolean collision_check() {
  int left1 = playerSprite.getX();
  int right1 = left1 + 16;
  int top1 = playerSprite.getY();
  int bottom1 = top1 + 16;
  int left2 = targetSprite.getX();
  int right2 = left2 + 16;
  int top2 = targetSprite.getY();
  int bottom2 = top2 + 16;
  
  if (bottom1 < top2) {
    return false;
  } else if (top1 > bottom2) {
    return false;
  } else if (right1 < left2) {
    return false;
  } else if (left1 > right2) {
    return false;
  } else {
    return true;
  }
}

//! Main game state.
void play_game() {

  display.fillRect(56, 0, 72, 8, BLACK);
  playerSprite.erase(&display);
  targetSprite.erase(&display);
  
  playerSprite.accelerate(tilt_sensor_x, tilt_sensor_y);
  
  playerSprite.move(&display);
  targetSprite.move(&display);

  if (collision_check()) {
    spawn_target();
    capture_count++;
    score += 10;
    
    if (capture_count >= level * 10) {
      state = end_level;
    }
  }

  playerSprite.draw(&display);
  targetSprite.draw(&display);
  
  display.setCursor(56, 0);
  display.print(capture_count);

  display.setCursor(100, 0);
  display.print(30 - ((time - last_state_change_time) / 1000));

  if (((time - last_state_change_time) / 1000) >= 30) {
    state = game_over;
  }

  // Update the OLED.
  display.display();
}

// -------------------------------------------------------------------------------
// Game over states / functions
// -------------------------------------------------------------------------------

void game_over() {
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.print(F("Game Over"));
  display.setTextSize(1);
  
  display.display();
  
  state = game_over_delay;
}

void game_over_delay() {
  if (time - last_state_change_time >= 5000) {
    state = begin_splash_screen;
  }
}
