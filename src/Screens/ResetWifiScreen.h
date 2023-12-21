#pragma once

#include "Screen.h"

class ResetWifiScreen : public Screen {
 public:
  ResetWifiScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
  void back() override;
};