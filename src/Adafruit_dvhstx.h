#pragma once

#include "Adafruit_GFX.h"

#include "drivers/dvhstx/dvhstx.hpp"

enum DVHSTXResolution {
    /* well supported, square pixels on a 16:9 display, actual resolution 1280x720@50Hz */
    DVHSTX_RESOLUTION_320x180,
    DVHSTX_RESOLUTION_640x360,

    /* sometimes supported, square pixels on a 16:9 display, actual resolution 960x540@60Hz */
    DVHSTX_RESOLUTION_480x270,

    /* sometimes supported, square pixels on a 16:9 display, actual resolution 800x450@60Hz */
    DVHSTX_RESOLUTION_400x225,

    /* well supported, but pixels aren't square on a 16:9 display */
    DVHSTX_RESOLUTION_320x240, /* 4:3, actual resolution 640x480@60Hz */
    DVHSTX_RESOLUTION_360x240, /* 3:2, actual resolution 720x480@60Hz */
    DVHSTX_RESOLUTION_360x200, /* 18:10, actual resolution 720x400@70Hz */
    DVHSTX_RESOLUTION_360x288, /* 5:4, actual resolution 720x576@60Hz */
    DVHSTX_RESOLUTION_400x300, /* 4:3, actual resolution 800x600@60Hz */
    DVHSTX_RESOLUTION_512x384, /* 4:3, actual resolution 1024x768@60Hz */

    /* sometimes supported, but pixels aren't square on a 16:9 display */
    DVHSTX_RESOLUTION_400x240, /* 5:3, actual resolution 800x480@60Hz */
};

using pimoroni::DVHSTXPinout;

// If the board definition provides pre-defined pins for the HSTX connection,
// use them to define a default pinout object.
// This object can be used as the first argument of the DVHSTX constructor.
// Otherwise you must provide the pin nubmers directly as a list of 4 numbers
// in curly brackets such as {12, 14, 16, 18}. These give the location of the
// positive ("P") pins in the order: Clock, Data 0, Data 1, Data 2; check your
// board's schematic for details.
#if defined(PIN_CKP)
#define DVHSTX_PINOUT_DEFAULT {PIN_CKP, PIN_D0P, PIN_D1P, PIN_D2P}
#endif

int16_t dvhstx_width(DVHSTXResolution r);
int16_t dvhstx_height(DVHSTXResolution r);

class DVHSTX16 : public GFXcanvas16 {
public:
    /**************************************************************************/
    /*!
       @brief    Instatiate a DVHSTX 16-bit canvas context for graphics
       @param    res   Display resolution
       @param    double_buffered Whether to allocate two buffers
    */
    /**************************************************************************/
    DVHSTX16(DVHSTXPinout pinout, DVHSTXResolution res, bool double_buffered=false) : GFXcanvas16(dvhstx_width(res), dvhstx_height(res), false), pinout(pinout), res{res}, double_buffered{double_buffered} {}
    ~DVHSTX16() { end(); }

    bool begin() {
        bool result = hstx.init(dvhstx_width(res), dvhstx_height(res), pimoroni::DVHSTX::MODE_RGB565, double_buffered, pinout);
        if (!result) return false;
        buffer = hstx.get_back_buffer<uint16_t>();
        fillScreen(0);
        return true;
    }
    void end() { hstx.reset(); }

  /**********************************************************************/
  /*!
    @brief    If double-buffered, wait for retrace and swap buffers. Otherwise, do nothing (returns immediately)
    @param copy_framebuffer if true, copy the new screen to the new back buffer. Otherwise, the content is undefined.
  */
  /**********************************************************************/
  void swap(bool copy_framebuffer = false);

  /**********************************************************************/
  /*!
    @brief    Convert 24-bit RGB value to a framebuffer value
    @param r The input red value, 0 to 255
    @param g The input red value, 0 to 255
    @param b The input red value, 0 to 255
    @return  The corresponding 16-bit pixel value
  */
  /**********************************************************************/
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
  }

private:
DVHSTXPinout pinout;
DVHSTXResolution res;
    mutable pimoroni::DVHSTX hstx;
bool double_buffered;
};


class DVHSTX8 : public GFXcanvas8 {
public:
    /**************************************************************************/
    /*!
       @brief    Instatiate a DVHSTX 8-bit canvas context for graphics
       @param    res   Display resolution
       @param    double_buffered Whether to allocate two buffers
    */
    /**************************************************************************/
    DVHSTX8(DVHSTXPinout pinout, DVHSTXResolution res, bool double_buffered=false) : GFXcanvas8(dvhstx_width(res), dvhstx_height(res), false), pinout(pinout), res{res}, double_buffered{double_buffered} {}
    ~DVHSTX8() { end(); }

    bool begin() {
        bool result = hstx.init(dvhstx_width(res), dvhstx_height(res), pimoroni::DVHSTX::MODE_PALETTE, double_buffered, pinout);
        if (!result) return false;
        for(int i=0; i<255; i++ ) {
            uint8_t r = (i >> 6) * 255 / 3;
            uint8_t g = ((i >> 2) & 7) * 255 / 7;
            uint8_t b = (i & 3) * 255 / 3;
            setColor(i, r, g, b);
        }
        buffer = hstx.get_back_buffer<uint8_t>();
        fillScreen(0);
        return true;
    }
    void end() { hstx.reset(); }

    void setColor(uint8_t idx, uint8_t red, uint8_t green, uint8_t blue) {
        hstx.get_palette()[idx] = (red << 16) | (green << 8) | blue;
    }
    void setColor(uint8_t idx, uint32_t rgb) {
        hstx.get_palette()[idx] = rgb;
    }

  /**********************************************************************/
  /*!
    @brief    If double-buffered, wait for retrace and swap buffers. Otherwise, do nothing (returns immediately)
    @param copy_framebuffer if true, copy the new screen to the new back buffer. Otherwise, the content is undefined.
  */
  /**********************************************************************/
  void swap(bool copy_framebuffer = false);

private:
DVHSTXPinout pinout;
DVHSTXResolution res;
    mutable pimoroni::DVHSTX hstx;
bool double_buffered;
};

using TextColor = pimoroni::DVHSTX::TextColour;

class DVHSTXText3 : public GFXcanvas16 {
public:
    struct Cell {
        uint16_t value;
        Cell(uint8_t c, uint8_t attr = TextColor::TEXT_WHITE) : value(c | (attr << 8)) {}
    };
    /**************************************************************************/
    /*!
       @brief    Instatiate a DVHSTX 8-bit canvas context for graphics
       @param    res   Display resolution
       @param    double_buffered Whether to allocate two buffers
    */
    /**************************************************************************/
    DVHSTXText3(DVHSTXPinout pinout) : GFXcanvas16(91,30,false), pinout(pinout), res{res}, attr{TextColor::TEXT_WHITE} {}
    ~DVHSTXText3() { end(); }

    bool begin() {
        bool result = hstx.init(91, 30, pimoroni::DVHSTX::MODE_TEXT_RGB111, false, pinout);
        if (!result) return false;
        buffer = hstx.get_back_buffer<uint16_t>();
        return true;
    }
    void end() { hstx.reset(); }

    void cursor_off() { hstx.cursor_off(); }
    void set_cursor(int x, int y) { hstx.set_cursor(x, y); }

#if 0 // TODO
    void setattr(uint8_t a) { attr = a; };
    uint8_t getattr(void) { return attr; }

    void scrollup(int rows=1);
    void scrollregion(uint8_t xsrc, uint8_t ysrc, uint8_t xdest, uint8_t ydest, uint8_t cols, uint8_t rows);

    Cell getchar(uint8_t x, uint8_t y);
    void setchar(uint8_t x, uint8_t y, Cell c);
    void setchar(uint8_t x, uint8_t y, uint8_t ch) { setchar(x, y, {ch, attr}); }
    void setchar(uint8_t x, uint8_t y, uint8_t ch, uint8_t a) { setchar(x, y, {ch, a}); }
#endif

private:
DVHSTXPinout pinout;
DVHSTXResolution res;
    mutable pimoroni::DVHSTX hstx;
bool double_buffered;
uint8_t attr;
};
