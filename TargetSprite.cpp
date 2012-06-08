// --------------------------------------------------------------------------------
// An Arduino sketch to play a tilt sensor capture game.
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
// --------------------------------------------------------------------------------

#include <Adafruit_GFX.h>
#ifndef ADAFRUIT_SSD1306_H
#define ADAFRUIT_SSD1306_H
#include <Adafruit_SSD1306.h>
#endif
#include "TargetSprite.h"

//! Adafruit logo bitmap from SSD1306 example sketch.
static unsigned char __attribute__ ((progmem)) target_bmp[]={
  0x30, 0xf0, 0xf0, 0xf0,
  0xf0, 0x30, 0xf8, 0xbe,
  0x9f, 0xff, 0xf8, 0xc0,
  0xc0, 0xc0, 0x80, 0x00, 
  0x20, 0x3c, 0x3f, 0x3f,
  0x1f, 0x19, 0x1f, 0x7b,
  0xfb, 0xfe, 0xfe, 0x07,
  0x07, 0x07, 0x03, 0x00
};

//! Target sprite height.
const uint8_t target_sprite_height = 16;

//! Target sprite width.
const uint8_t target_sprite_width = 16;

//! Set the absolute location of the player sprite.
void TargetSprite::setLocation(int x, int y) {
  _newX = _oldX = x;
  _newY = _oldY = y;
}

//! Get the x location.
int TargetSprite::getX() {
  return _newX;
}

//! Get the y location.
int TargetSprite::getY() {
  return _newY;
}

//! Apply acceleration to the velocity of the player sprite.
void TargetSprite::setVelocity(int x, int y) {
  _velocityX = x;
  _velocityY = y;
}

//! Erase the bitmap at the old location.
void TargetSprite::erase(Adafruit_GFX *display) {
  display->drawBitmap(_oldX, _oldY, target_bmp, target_sprite_width, target_sprite_height, BLACK);
}

//! Move the bitmap based on velocity.
void TargetSprite::move(Adafruit_GFX *display) {
  
  // Add velocity to current position.
  _newX = _oldX + _velocityX;
  _newY = _oldY + _velocityY;
  
  // Enforce screen bounds and bounce sprite off edges.
  if (_newX <= 0) {
    _velocityX = -_velocityX;
    _newX = 0;
  } else if (_newX > 128 - target_sprite_width) {
    _velocityX = -_velocityX;
    _newX = 128 - target_sprite_width;
  }
  if (_newY <= 8) {
    _velocityY = -_velocityY;
    _newY = 8;
  } else if (_newY > 64 - target_sprite_width) {
    _velocityY = -_velocityY;
    _newY = 64 - target_sprite_width;
  }
  
  // Update coordinates.
  _oldX = _newX;
  _oldY = _newY;
}

//! Draw the bitmap at the new location.
void TargetSprite::draw(Adafruit_GFX *display) {
  display->drawBitmap(_newX, _newY, target_bmp, target_sprite_width, target_sprite_height, WHITE);
}
