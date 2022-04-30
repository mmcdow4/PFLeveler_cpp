#pragma once

#ifndef FEATPAGE_H
#define FEATPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>
#include <pf_include/Feat.h>

#define FEAT_REMAINING_COUNTER_TEXT_ID 7000
#define FEAT_AVAIL_FEAT_LIST_ID 7001
#define FEAT_SELECT_BUTTON_ID 7002
#define FEAT_REMOVE_BUTTON_ID 7003
#define FEAT_KNOWN_FEAT_LIST_ID 7004
#define FEAT_SELECTED_DESCRIPTION_ID 7005

class FeatPage : public wxPanel
{

public:
  FeatPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~FeatPage() {}

  void ResetPage(Pathfinder::Character* currChar);
  bool UpdateFeatPage(int classId);

  void OnFeatSelected(wxCommandEvent& evt);
  void SelectFeatButtonPress(wxCommandEvent& evt);
  
  void UpdateFeatDescription(int spellIndex);
private:
  Pathfinder::Character* charPtr_ = NULL;
  std::vector<int> availFeatIds_;
  int featsRemaining_;
};

#endif