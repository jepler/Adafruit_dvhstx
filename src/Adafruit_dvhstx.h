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

class DVHSTX16 : public Adafruit_GFX {
public:
    /**************************************************************************/
    /*!
       @brief    Instatiate a DVHSTX 16-bit canvas context for graphics
       @param    res   Display resolution
       @param    double_buffered Whether to allocate two buffers
    */
    /**************************************************************************/
    DVHSTX16(DVHSTXPinout pinout, DVHSTXResolution res, bool double_buffered=false) : Adafruit_GFX(dvhstx_width(res), dvhstx_height(res)), pinout(pinout), res{res}, double_buffered{double_buffered} {}
    ~DVHSTX16() { end(); }

    bool begin() {
        return hstx.init(dvhstx_width(res), dvhstx_height(res), pimoroni::DVHSTX::MODE_RGB565, double_buffered, pinout);
    }
    void end() { hstx.reset(); }

  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillScreen(uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  uint16_t getPixel(int16_t x, int16_t y) const;
  /**********************************************************************/
  /*!
    @brief    Get a pointer to the internal buffer memory (current back buffer)
    @returns  A pointer to the allocated buffer
  */
  /**********************************************************************/
  uint16_t *getBuffer(void) const { return hstx.get_back_buffer<uint16_t>(); }

  /**********************************************************************/
  /*!
    @brief    If double-buffered, wait for retrace and swap buffers. Otherwise, do nothing (returns immediately)
    @param copy_framebuffer if true, copy the new screen to the new back buffer. Otherwise, the content is undefined.
  */
  /**********************************************************************/
  void swap(bool copy_framebuffer = false);

protected:
  uint16_t getRawPixel(int16_t x, int16_t y) const;
  void drawFastRawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastRawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

private:
DVHSTXPinout pinout;
DVHSTXResolution res;
bool double_buffered;
    mutable pimoroni::DVHSTX hstx;
};

#if 0
class DVHSTX8 : GFXcanvas8 {
    /**************************************************************************/
    /*!
       @brief    Instatiate a DVHSTX 8-bit canvas context for graphics
       @param    res   Display resolution
       @param    double_buffered Whether to allocate two buffers
    */
    /**************************************************************************/
    DVHSTX8(DVHSTXResolution res, bool double_buffered=false);
    ~DVHSTX8() { end(); }
    bool begin() {
        return hstx.init(width, height, MODE_PALETTE, double_buffered);
    }
    void end() { hstx.reset(); }

  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void fillScreen(uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  bool getPixel(int16_t x, int16_t y) const;
  /**********************************************************************/
  /*!
    @brief    Get a pointer to the internal buffer memory (current back buffer)
    @returns  A pointer to the allocated buffer
  */
  /**********************************************************************/
  uint8_t *getBuffer(void) const { return hstx.get_back_buffer<uint8_t>(); }


  /**********************************************************************/
  /*!
    @brief    If double-buffered, wait for retrace and swap buffers. Otherwise, do nothing (returns immediately)
    @param copy_framebuffer if true, copy the new screen to the new back buffer. Otherwise, the content is undefined.
  */
  /**********************************************************************/
  void swap(bool copy_framebuffer = false);
protected:
  bool getRawPixel(int16_t x, int16_t y) const;
  void drawFastRawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastRawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

};

class DVHSTXTEXT1 : GFXcanvas16 {
    DVHSTX16();
    bool begin() {
        return hstx.init(1280, 720, MODE_TEXT_MONO, true);
    }
    using TextColor = pimoroni::DVHSTX::TextColour;
    void end() { hstx.reset(); }
};

class DVHSTXTEXT3 : GFXcanvas16 {
    DVHSTX16();
    bool begin() {
        return hstx.init(1280, 720, MODE_TEXT_RGB111, true);
    }
    void end() { hstx.reset(); }
};
#endif
