#include "Adafruit_dvhstx.h"

// This code is heavily adapted from Adafruit_GFX_Library. The existin
// GFXcanvas classes were not suitable, as the hstx instance controls
// framebuffer allocation including double-buffering


#if 0
/**************************************************************************/
/*!
    @brief  Draw a pixel to the canvas framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color 8-bit Color index to fill with. Only lower byte of uint16_t is used.
*/
/**************************************************************************/
void DVHSTX8::drawPixel(int16_t x, int16_t y, uint16_t color) {
  uint8_t *buffer = getBuffer();
  if (buffer) {
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
      return;

    int16_t t;
    switch (rotation) {
    case 1:
      t = x;
      x = _width - 1 - y;
      y = t;
      break;
    case 2:
      x = _width - 1 - x;
      y = _height - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = _height - 1 - t;
      break;
    }

    buffer[x + y * _width] = color;
  }
}

/**********************************************************************/
/*!
        @brief    Get the pixel color index at a given coordinate
        @param    x   x coordinate
        @param    y   y coordinate
        @returns  The desired pixel's 8-bit color index
*/
/**********************************************************************/
uint8_t DVHSTX8::getPixel(int16_t x, int16_t y) const {
  int16_t t;
  switch (rotation) {
  case 1:
    t = x;
    x = _width - 1 - y;
    y = t;
    break;
  case 2:
    x = _width - 1 - x;
    y = _height - 1 - y;
    break;
  case 3:
    t = x;
    x = y;
    y = _height - 1 - t;
    break;
  }
  return getRawPixel(x, y);
}

/**********************************************************************/
/*!
        @brief    Get the pixel index value at a given, unrotated coordinate.
              This method is intended for hardware drivers to get pixel value
              in physical coordinates.
        @param    x   x coordinate
        @param    y   y coordinate
        @returns  The desired pixel's 8-bit color value
*/
/**********************************************************************/
uint8_t DVHSTX8::getRawPixel(int16_t x, int16_t y) const {
  if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
    return 0;
  uint8_t *buffer = getBuffer();
  if (buffer) {
    return buffer[x + y * _width];
  }
  return 0;
}

/**************************************************************************/
/*!
    @brief  Fill the framebuffer completely with one color
    @param  color 8-bit Color to fill with. Only lower byte of uint16_t is used.
*/
/**************************************************************************/
void DVHSTX8::fillScreen(uint16_t color) {
  uint8_t *buffer = getBuffer();
  if (buffer) {
    memset(buffer, color, _width * _height);
  }
}

/**************************************************************************/
/*!
   @brief  Speed optimized vertical line drawing
   @param  x      Line horizontal start point
   @param  y      Line vertical start point
   @param  h      Length of vertical line to be drawn, including first point
   @param  color  8-bit Color to fill with. Only lower byte of uint16_t is
                  used.
*/
/**************************************************************************/
void DVHSTX8::drawFastVLine(int16_t x, int16_t y, int16_t h,
                               uint16_t color) {
  uint8_t *buffer = getBuffer();
  if (!buffer) return;

  if (h < 0) { // Convert negative heights to positive equivalent
    h *= -1;
    y -= h - 1;
    if (y < 0) {
      h += y;
      y = 0;
    }
  }

  // Edge rejection (no-draw if totally off canvas)
  if ((x < 0) || (x >= width()) || (y >= height()) || ((y + h - 1) < 0)) {
    return;
  }

  if (y < 0) { // Clip top
    h += y;
    y = 0;
  }
  if (y + h > height()) { // Clip bottom
    h = height() - y;
  }

  if (getRotation() == 0) {
    drawFastRawVLine(x, y, h, color);
  } else if (getRotation() == 1) {
    int16_t t = x;
    x = _width - 1 - y;
    y = t;
    x -= h - 1;
    drawFastRawHLine(x, y, h, color);
  } else if (getRotation() == 2) {
    x = _width - 1 - x;
    y = _height - 1 - y;

    y -= h - 1;
    drawFastRawVLine(x, y, h, color);
  } else if (getRotation() == 3) {
    int16_t t = x;
    x = y;
    y = _height - 1 - t;
    drawFastRawHLine(x, y, h, color);
  }
}

/**************************************************************************/
/*!
   @brief  Speed optimized horizontal line drawing
   @param  x      Line horizontal start point
   @param  y      Line vertical start point
   @param  w      Length of horizontal line to be drawn, including 1st point
   @param  color  8-bit Color to fill with. Only lower byte of uint16_t is
                  used.
*/
/**************************************************************************/
void DVHSTX8::drawFastHLine(int16_t x, int16_t y, int16_t w,
                               uint16_t color) {
  uint8_t *buffer = getBuffer();
  if (!buffer) return;

  if (w < 0) { // Convert negative widths to positive equivalent
    w *= -1;
    x -= w - 1;
    if (x < 0) {
      w += x;
      x = 0;
    }
  }

  // Edge rejection (no-draw if totally off canvas)
  if ((y < 0) || (y >= height()) || (x >= width()) || ((x + w - 1) < 0)) {
    return;
  }

  if (x < 0) { // Clip left
    w += x;
    x = 0;
  }
  if (x + w >= width()) { // Clip right
    w = width() - x;
  }

  if (getRotation() == 0) {
    drawFastRawHLine(x, y, w, color);
  } else if (getRotation() == 1) {
    int16_t t = x;
    x = _width - 1 - y;
    y = t;
    drawFastRawVLine(x, y, w, color);
  } else if (getRotation() == 2) {
    x = _width - 1 - x;
    y = _height - 1 - y;

    x -= w - 1;
    drawFastRawHLine(x, y, w, color);
  } else if (getRotation() == 3) {
    int16_t t = x;
    x = y;
    y = _height - 1 - t;
    y -= w - 1;
    drawFastRawVLine(x, y, w, color);
  }
}

/**************************************************************************/
/*!
   @brief    Speed optimized vertical line drawing into the raw canvas buffer
   @param    x   Line horizontal start point
   @param    y   Line vertical start point
   @param    h   length of vertical line to be drawn, including first point
   @param    color   8-bit Color to fill with. Only lower byte of uint16_t is
   used.
*/
/**************************************************************************/
void DVHSTX8::drawFastRawVLine(int16_t x, int16_t y, int16_t h,
                                  uint16_t color) {
  // x & y already in raw (rotation 0) coordinates, no need to transform.
  uint8_t *buffer_ptr = buffer + y * _width + x;
  for (int16_t i = 0; i < h; i++) {
    (*buffer_ptr) = color;
    buffer_ptr += _width;
  }
}

/**************************************************************************/
/*!
   @brief    Speed optimized horizontal line drawing into the raw canvas buffer
   @param    x   Line horizontal start point
   @param    y   Line vertical start point
   @param    w   length of horizontal line to be drawn, including first point
   @param    color   8-bit Color to fill with. Only lower byte of uint16_t is
   used.
*/
/**************************************************************************/
void DVHSTX8::drawFastRawHLine(int16_t x, int16_t y, int16_t w,
                                  uint16_t color) {
  // x & y already in raw (rotation 0) coordinates, no need to transform.
  memset(buffer + y * _width + x, color, w);
}
#endif

/**************************************************************************/
/*!
    @brief  Draw a pixel to the canvas framebuffer
    @param  x   x coordinate
    @param  y   y coordinate
    @param  color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void DVHSTX16::drawPixel(int16_t x, int16_t y, uint16_t color) {
  uint16_t *buffer = getBuffer();
  if (buffer) {
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
      return;

    int16_t t;
    switch (rotation) {
    case 1:
      t = x;
      x = _width - 1 - y;
      y = t;
      break;
    case 2:
      x = _width - 1 - x;
      y = _height - 1 - y;
      break;
    case 3:
      t = x;
      x = y;
      y = _height - 1 - t;
      break;
    }

    buffer[x + y * _width] = color;
  }
}

/**********************************************************************/
/*!
        @brief    Get the pixel color value at a given coordinate
        @param    x   x coordinate
        @param    y   y coordinate
        @returns  The desired pixel's 16-bit 5-6-5 color value
*/
/**********************************************************************/
uint16_t DVHSTX16::getPixel(int16_t x, int16_t y) const {
  uint16_t *buffer = getBuffer();
  if (!buffer) return 0;
  int16_t t;
  switch (rotation) {
  case 1:
    t = x;
    x = _width - 1 - y;
    y = t;
    break;
  case 2:
    x = _width - 1 - x;
    y = _height - 1 - y;
    break;
  case 3:
    t = x;
    x = y;
    y = _height - 1 - t;
    break;
  }
  return getRawPixel(x, y);
}

/**********************************************************************/
/*!
        @brief    Get the pixel color value at a given, unrotated coordinate.
              This method is intended for hardware drivers to get pixel value
              in physical coordinates.
        @param    x   x coordinate
        @param    y   y coordinate
        @returns  The desired pixel's 16-bit 5-6-5 color value
*/
/**********************************************************************/
uint16_t DVHSTX16::getRawPixel(int16_t x, int16_t y) const {
  if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
    return 0;
  uint16_t *buffer = getBuffer();
  if (buffer) {
    return buffer[x + y * _width];
  }
  return 0;
}

/**************************************************************************/
/*!
    @brief  Fill the framebuffer completely with one color
    @param  color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void DVHSTX16::fillScreen(uint16_t color) {
  uint16_t *buffer = getBuffer();
  if (buffer) {
    uint8_t hi = color >> 8, lo = color & 0xFF;
    if (hi == lo) {
      memset(buffer, lo, _width * _height * 2);
    } else {
      uint32_t i, pixels = _width * _height;
      for (i = 0; i < pixels; i++)
        buffer[i] = color;
    }
  }
}

#if 0
/**************************************************************************/
/*!
    @brief  Reverses the "endian-ness" of each 16-bit pixel within the
            canvas; little-endian to big-endian, or big-endian to little.
            Most microcontrollers (such as SAMD) are little-endian, while
            most displays tend toward big-endianness. All the drawing
            functions (including RGB bitmap drawing) take care of this
            automatically, but some specialized code (usually involving
            DMA) can benefit from having pixel data already in the
            display-native order. Note that this does NOT convert to a
            SPECIFIC endian-ness, it just flips the bytes within each word.
*/
/**************************************************************************/
void DVHSTX16::byteSwap(void) {
  uint16_t *buffer = getBuffer();
  if (buffer) {
    uint32_t i, pixels = _width * _height;
    for (i = 0; i < pixels; i++)
      buffer[i] = __builtin_bswap16(buffer[i]);
  }
}
#endif

/**************************************************************************/
/*!
   @brief    Speed optimized vertical line drawing
   @param    x   Line horizontal start point
   @param    y   Line vertical start point
   @param    h   length of vertical line to be drawn, including first point
   @param    color   color 16-bit 5-6-5 Color to draw line with
*/
/**************************************************************************/
void DVHSTX16::drawFastVLine(int16_t x, int16_t y, int16_t h,
                                uint16_t color) {
  uint16_t *buffer = getBuffer();
  if (!buffer) return;
  if (h < 0) { // Convert negative heights to positive equivalent
    h *= -1;
    y -= h - 1;
    if (y < 0) {
      h += y;
      y = 0;
    }
  }

  // Edge rejection (no-draw if totally off canvas)
  if ((x < 0) || (x >= width()) || (y >= height()) || ((y + h - 1) < 0)) {
    return;
  }

  if (y < 0) { // Clip top
    h += y;
    y = 0;
  }
  if (y + h > height()) { // Clip bottom
    h = height() - y;
  }

  if (getRotation() == 0) {
    drawFastRawVLine(x, y, h, color);
  } else if (getRotation() == 1) {
    int16_t t = x;
    x = _width - 1 - y;
    y = t;
    x -= h - 1;
    drawFastRawHLine(x, y, h, color);
  } else if (getRotation() == 2) {
    x = _width - 1 - x;
    y = _height - 1 - y;

    y -= h - 1;
    drawFastRawVLine(x, y, h, color);
  } else if (getRotation() == 3) {
    int16_t t = x;
    x = y;
    y = _height - 1 - t;
    drawFastRawHLine(x, y, h, color);
  }
}

/**************************************************************************/
/*!
   @brief  Speed optimized horizontal line drawing
   @param  x      Line horizontal start point
   @param  y      Line vertical start point
   @param  w      Length of horizontal line to be drawn, including 1st point
   @param  color  Color 16-bit 5-6-5 Color to draw line with
*/
/**************************************************************************/
void DVHSTX16::drawFastHLine(int16_t x, int16_t y, int16_t w,
                                uint16_t color) {
  uint16_t *buffer = getBuffer();
  if (!buffer) return;
  if (w < 0) { // Convert negative widths to positive equivalent
    w *= -1;
    x -= w - 1;
    if (x < 0) {
      w += x;
      x = 0;
    }
  }

  // Edge rejection (no-draw if totally off canvas)
  if ((y < 0) || (y >= height()) || (x >= width()) || ((x + w - 1) < 0)) {
    return;
  }

  if (x < 0) { // Clip left
    w += x;
    x = 0;
  }
  if (x + w >= width()) { // Clip right
    w = width() - x;
  }

  if (getRotation() == 0) {
    drawFastRawHLine(x, y, w, color);
  } else if (getRotation() == 1) {
    int16_t t = x;
    x = _width - 1 - y;
    y = t;
    drawFastRawVLine(x, y, w, color);
  } else if (getRotation() == 2) {
    x = _width - 1 - x;
    y = _height - 1 - y;

    x -= w - 1;
    drawFastRawHLine(x, y, w, color);
  } else if (getRotation() == 3) {
    int16_t t = x;
    x = y;
    y = _height - 1 - t;
    y -= w - 1;
    drawFastRawVLine(x, y, w, color);
  }
}

/**************************************************************************/
/*! 
   @brief    Speed optimized horizontal line drawing into the raw canvas buffer
   @param    x   Line horizontal start point
   @param    y   Line vertical start point
   @param    w   length of horizontal line to be drawn, including first point
   @param    color   color 16-bit 5-6-5 Color to draw line with
*/
/**************************************************************************/
void DVHSTX16::drawFastRawHLine(int16_t x, int16_t y, int16_t w,
                                   uint16_t color) {
  // x & y already in raw (rotation 0) coordinates, no need to transform.
  uint16_t *buffer = getBuffer();
  uint32_t buffer_index = y * WIDTH + x;
  for (uint32_t i = buffer_index; i < buffer_index + w; i++) {
    buffer[i] = color;
  }
}

/**************************************************************************/
/*!
   @brief    Speed optimized vertical line drawing into the raw canvas buffer
   @param    x   Line horizontal start point
   @param    y   Line vertical start point
   @param    h   length of vertical line to be drawn, including first point
   @param    color   color 16-bit 5-6-5 Color to draw line with
*/
/**************************************************************************/
void DVHSTX16::drawFastRawVLine(int16_t x, int16_t y, int16_t h,
                                   uint16_t color) {
  // x & y already in raw (rotation 0) coordinates, no need to transform.
  uint16_t *buffer = getBuffer();
  uint16_t *buffer_ptr = buffer + y * _width + x;
  for (int16_t i = 0; i < h; i++) {
    (*buffer_ptr) = color;
    buffer_ptr += _width;
  }
}


int16_t dvhstx_width(DVHSTXResolution r) {
    switch(r) {
default:
        case DVHSTX_RESOLUTION_320x180: return 320;
        case DVHSTX_RESOLUTION_640x360: return 640;
        case DVHSTX_RESOLUTION_480x270: return 480;
        case DVHSTX_RESOLUTION_400x225: return 400;
        case DVHSTX_RESOLUTION_320x240: return 320;
        case DVHSTX_RESOLUTION_360x240: return 360;
        case DVHSTX_RESOLUTION_360x200: return 360;
        case DVHSTX_RESOLUTION_360x288: return 360;
        case DVHSTX_RESOLUTION_400x300: return 400;
        case DVHSTX_RESOLUTION_512x384: return 512;
        case DVHSTX_RESOLUTION_400x240: return 400;
    }
    return 0;
}

int16_t dvhstx_height(DVHSTXResolution r) {
    switch(r) {
default:
        case DVHSTX_RESOLUTION_320x180: return 180;
        case DVHSTX_RESOLUTION_640x360: return 360;
        case DVHSTX_RESOLUTION_480x270: return 270;
        case DVHSTX_RESOLUTION_400x225: return 225;
        case DVHSTX_RESOLUTION_320x240: return 240;
        case DVHSTX_RESOLUTION_360x240: return 240;
        case DVHSTX_RESOLUTION_360x200: return 200;
        case DVHSTX_RESOLUTION_360x288: return 288;
        case DVHSTX_RESOLUTION_400x300: return 300;
        case DVHSTX_RESOLUTION_512x384: return 384;
        case DVHSTX_RESOLUTION_400x240: return 240;
    }
}

