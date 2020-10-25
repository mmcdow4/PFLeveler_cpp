#pragma once

#ifndef CLASSPAGE_H
#define CLASSPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>

#include <vector>

#define CLASS_DROPDOWN_LABEL_ID 4000
#define CLASS_DROPDOWN_ID 4001
#define CLASS_LEVELUP_BUTTON_ID 4002
#define CLASS_DESCRIPTION_ID 4003
#define CLASS_TODO_FEATURE_LABEL_ID 4004
#define CLASS_TODO_FEATURE_LIST_ID 4005
#define CLASS_SELECTED_FEATURE_LABEL_ID 4006
#define CLASS_SELECTED_FEATURE_LIST_ID 4007
#define CLASS_FEATURE_BUTTON_ID 4008
#define CLASS_ABILITIES_LABEL_ID 4009
#define CLASS_ABILITIES_LIST_ID 4010
#define CLASS_ABILITIES_DESCRIPTION_ID 4011

class ClassPage : public wxPanel
{

public:
  ClassPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~ClassPage() {}

  void ResetPage(Pathfinder::Character* currChar);

  //void OnRaceSelected(wxCommandEvent& evt);
  //void OnRacialSelected(wxCommandEvent& evt);
  //void OnRaceLocked(wxCommandEvent& evt);

private:
  //std::vector<int> rolledScores;
  Pathfinder::Character* charPtr_ = NULL;
};

#endif