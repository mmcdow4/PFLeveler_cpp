#pragma once

#include "wx/wx.h"


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


  //wxButton*   m_btn1;
  //wxTextCtrl* m_txt1;
  //wxListBox*  m_list1;

  void OnButtonClicked(wxCommandEvent& evt);

  wxDECLARE_EVENT_TABLE();
};

