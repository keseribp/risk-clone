#ifndef GENERALDATA_H
#define GENERALDATA_H

// Window parameters
static const uint SCREEN_CENTER_X = 1920 / 2;
static const uint SCREEN_CENTER_Y = 1080 / 2;
static const uint SPLASH_WINDOW_X = 2 * 800;
static const uint SPLASH_WINDOW_Y = 2 * 315;
static const uint MAP_WINDOW_X = 2 * 1020;
static const uint MAP_WINDOW_Y = 2 * 675;
static const uint CONTROL_PANEL_WINDOW_X = 2 * 500;
static const uint CONTROL_PANEL_WINDOW_Y = 2 * 675;
static const uint WINDOW_X = MAP_WINDOW_X + CONTROL_PANEL_WINDOW_X;
static const uint WINDOW_Y = MAP_WINDOW_Y;
static const uint WINDOW_CENTER_X = WINDOW_X / 2;
static const uint WINDOW_CENTER_Y = WINDOW_Y / 2;
static const float X_EXTENT = 2.0f * static_cast<float>(MAP_WINDOW_X) / static_cast<float>(WINDOW_X) - 1.0f;

// Debug parameters
static const bool DEBUG = false;

typedef unsigned int uint;

#endif
