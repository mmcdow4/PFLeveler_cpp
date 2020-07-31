#include "cApp.h"

#include <exception>
#include <stdexcept>

#include <pf_include/PFTable.h>

wxIMPLEMENT_APP(cApp);


cApp::cApp()
{
}


cApp::~cApp()
{
}

bool cApp::OnInit()
{

  try
  {
    Pathfinder::PFTable::initialize("E:/dev/pathfinder/csv_files/PathFinder.db");
  }
  catch (const std::invalid_argument& e)
  {
    wxMessageBox(e.what());
    return false;
  }

  m_frame1 = new cMain();
  m_frame1->Show();

  return true;
}