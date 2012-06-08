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

#ifndef TARGET_SPRITE_H
#define TARGET_SPRITE_H

#include <Adafruit_GFX.h>
#ifndef ADAFRUIT_SSD1306_H
#define ADAFRUIT_SSD1306_H
#include <Adafruit_SSD1306.h>
#endif

class TargetSprite {
  
  public:
    void setLocation(int x, int y);
    int getX();
    int getY();
    void setVelocity(int x, int y);
    
    void erase(Adafruit_GFX *display);
    void move(Adafruit_GFX *display);
    void draw(Adafruit_GFX *display);

  private:
    int _oldX;
    int _oldY;
    int _newX;
    int _newY;
    int _velocityX;
    int _velocityY;
};

#endif // TARGET_SPRITE_H
