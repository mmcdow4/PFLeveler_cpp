#pragma once

#ifndef FEATPAGE_H
#define FEATPAGE_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>
#include <pf_include/Feat.h>
#include "WrapText.h"

#define FEAT_REMAINING_COUNTER_TEXT_ID 7000
#define FEAT_FILTER_CHECKBOX_ID 7001
#define FEAT_AVAIL_FEAT_LIST_ID 7002
#define FEAT_SELECT_BUTTON_ID 7003
#define FEAT_REMOVE_BUTTON_ID 7004
#define FEAT_KNOWN_FEAT_LIST_ID 7005
#define FEAT_SELECTED_DESCRIPTION_ID 7006

class FeatPage : public wxPanel
{

public:
  FeatPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~FeatPage() {}

  void ResetPage(Pathfinder::Character* currChar);
  bool UpdateFeatPage(int classId);
  void PopulateFeatLists();

  void OnAvailFeatSelected(wxListEvent& evt);
  void OnTakenFeatSelected(wxCommandEvent& evt);
  void SelectFeatButtonPress(wxCommandEvent& evt);
  void OnFilterBoxChecked(wxCommandEvent& evt);
  void MouseOverEvent(wxMouseEvent& evt);
  void ResizeCallback(wxSizeEvent& evt);

  void GrantFeats(void);
  void UpdateFeatDescription(int featIndex);
  bool CheckFeatPrereqs(int featIndex, std::string& missingPrereqs);
private:
  Pathfinder::Character* charPtr_ = NULL;
  std::vector<int> availFeatIds_;
  std::vector<std::string> availFeatMissingPrereqs_;
  int featsRemaining_ = 0;
  HardBreakWrapper* featDescWrapper_ = NULL;
};

#endif