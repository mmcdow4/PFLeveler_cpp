#pragma once

#include "wx/wx.h"
#include <wx/notebook.h>

#define DEFAULT_WINDOW_SIZE_X 600
#define DEFAULT_WINDOW_SIZE_Y 600

class cMain : public wxFrame
{
public:
  cMain();
  ~cMain();


public:
  int nFieldWidth = 10;
  int nFieldHeight = 10;
  wxButton** btn;
  int* nField = nullptr;
  bool bFirstClick = true;

  wxMenuBar* menubar_;
  wxMenu* file_;
  //wxButton*   m_btn1;
  //wxTextCtrl* m_txt1;
  //wxListBox*  m_list1;

  wxNotebook* notebook_;
  void initializeNotebook();
  void SetupSummaryPage();
  void SetupRacePage();

  void OnButtonClicked(wxCommandEvent& evt);
  void OnRaceSelected(wxCommandEvent& evt);

  wxDECLARE_EVENT_TABLE();
};

