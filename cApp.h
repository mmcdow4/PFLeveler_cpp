#pragma once

#include "wx/wx.h"
#include "cMain.h"

#include <pf_include/Character.h>
#include <pf_include/Race.h>

class cApp : public wxApp
{
public:
  cApp();
  ~cApp();

  cMain* m_frame1 = nullptr;
public:
  virtual bool OnInit();

private:
  Pathfinder::Character currChar_;
  int mode_;
};

