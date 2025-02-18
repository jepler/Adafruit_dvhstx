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

#define DVHSTX_PINOUT_METRO_RP2350 ((DVHSTXPinout){14, 18, 16, 12})
// TODO: check and enable these pinouts
#define DVHSTX_PINOUT_FEATHER_RP2350 ((DVHSTXPinout){14, 18, 16, 12})
#define DVHSTX_PINOUT_FRUITJAM_RP2350 ((DVHSTXPinout){14, 18, 16, 12})

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
