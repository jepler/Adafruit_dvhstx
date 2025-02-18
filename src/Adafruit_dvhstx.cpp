#include "Adafruit_dvhstx.h"

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

void DVHSTX16::swap(bool copy_framebuffer) {
    if (!double_buffered) { return; }
    hstx.flip_blocking();
    if (copy_framebuffer) {
        memcpy(hstx.get_front_buffer<uint8_t>(), hstx.get_back_buffer<uint8_t>(), sizeof(uint16_t) * _width * _height);
    } 
}
