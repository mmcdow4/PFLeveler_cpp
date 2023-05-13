#include "cApp.h"

#include <exception>
#include <stdexcept>
#include <filesystem>

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
    //std::string path1 = "E:/dev/pathfinder/csv_files/";
    Pathfinder::PFTable::initialize( std::filesystem::current_path().string() + "/PathFinder.db" );
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