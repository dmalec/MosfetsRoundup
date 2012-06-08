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
#include "PlayerSprite.h"

//! Player bitmap.
/* 
  Bitmap converted by LCD Assistant
  http://en.radzio.dxp.pl/bitmap_converter/
*/
static unsigned char __attribute__ ((progmem)) player_bmp[]={
  0x00, 0xC0, 0xFE, 0xFE,
  0x7E, 0xFC, 0xF0, 0xF0,
  0xF0, 0xF0, 0xFC, 0x7E,
  0xFE, 0xFE, 0xC0, 0x00,
  0x00, 0x1F, 0x3F, 0x7E,
  0x7F, 0x7E, 0x7F, 0x7F,
  0x7F, 0x7F, 0x7E, 0x7F,
  0x7E, 0x3F, 0x1F, 0x00
};

//! Player sprite height.
const uint8_t player_sprite_height = 16;

//! Player sprite width.
const uint8_t player_sprite_width = 16;

//! Set the absolute location of the player sprite.
void PlayerSprite::setLocation(int x, int y) {
  _oldX = x;
  _oldY = y;
}

//! Get the x location.
int PlayerSprite::getX() {
  return _newX;
}

//! Get the y location.
int PlayerSprite::getY() {
  return _newY;
}

//! Apply acceleration to the velocity of the player sprite.
void PlayerSprite::accelerate(int x, int y) {
  _velocityX = applyAcceleration(_velocityX, x);
  _velocityY = applyAcceleration(_velocityY, y);
}

//! Erase the bitmap at the old location.
void PlayerSprite::erase(Adafruit_GFX *display) {
  display->drawBitmap(_oldX, _oldY, player_bmp, player_sprite_width, player_sprite_height, BLACK);
}

//! Move the bitmap based on velocity.
void PlayerSprite::move(Adafruit_GFX *display) {
  
  // Add velocity to current position.
  _newX = _oldX + (_velocityX * 2);
  _newY = _oldY + (_velocityY * 2);
  
  // Enforce screen bounds.
  _newX = (_newX >= 0) ? _newX : 0;
  _newY = (_newY >= 8) ? _newY : 8;
  _newX = (_newX < 128 - player_sprite_width) ? _newX : 128 - player_sprite_width;
  _newY = (_newY < 64 - player_sprite_height) ? _newY : 64 - player_sprite_height;
  
  // Apply friction to the velocity.
  _velocityX = applyFriction(_velocityX);
  _velocityY = applyFriction(_velocityY);
  
  // Update coordinates.
  _oldX = _newX;
  _oldY = _newY;
}

//! Draw the bitmap at the new location.
void PlayerSprite::draw(Adafruit_GFX *display) {
  display->drawBitmap(_newX, _newY, player_bmp, player_sprite_width, player_sprite_height, WHITE);
}

//! Apply friction to a velocity value.
int PlayerSprite::applyFriction(int velocity) {
  if (velocity > 0) {
    return velocity - 1;
  } else if (velocity < 0) {
    return velocity + 1;
  } else {
    return 0;
  }
}

//! Apply acceleration to a velocity value.
int PlayerSprite::applyAcceleration(int velocity, int acceleration) {
  int new_velocity = velocity + acceleration;
  
  // Enforce limits.
  new_velocity = (new_velocity <= 4) ? new_velocity : 4;
  new_velocity = (new_velocity >= -4) ? new_velocity : -4;
  
  return new_velocity;
}
