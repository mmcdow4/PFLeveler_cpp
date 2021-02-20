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
#define CLASS_FAVORED_CLASS_BUTTON_ID 4003
#define CLASS_DESCRIPTION_ID 4004
#define CLASS_TODO_FEATURE_LABEL_ID 4005
#define CLASS_TODO_FEATURE_LIST_ID 4006
#define CLASS_SELECTED_FEATURE_LABEL_ID 4007
#define CLASS_SELECTED_FEATURE_LIST_ID 4008
#define CLASS_FEATURE_DESCRIPTION_ID 4009
#define CLASS_FEATURE_BUTTON_ID 4010
#define CLASS_ABILITIES_LABEL_ID 4011
#define CLASS_ABILITIES_LIST_ID 4012
#define CLASS_ABILITIES_DESCRIPTION_ID 4013

class ClassPage : public wxPanel
{

public:
  ClassPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~ClassPage() {}

  void ResetPage(Pathfinder::Character* currChar);

  void OnLevelAdded(wxCommandEvent& evt);
  void OnFavoredClassAdded(wxCommandEvent& evt);
  void OnClassSelected(wxCommandEvent& evt);

  void OnAbilitySelected(wxCommandEvent& evt);
  void OnUnselectedFeatureSelected(wxCommandEvent& evt);
  void OnFinishedFeatureSelected(wxCommandEvent& evt);
  void SelectFeatureButtonPress(wxCommandEvent& evt);
  void MakeFeatureChoice(int classIdx, int classLvl, int numChoices, std::vector<Pathfinder::ClassChoice>& choiceVec, wxString catName);

private:
  Pathfinder::Character* charPtr_ = NULL;
  std::vector<Pathfinder::ClassFeature> todoFeatures_;
  std::vector<Pathfinder::ClassAbility> abilities_;

  std::vector<wxString> featureNames_;
  std::vector<wxString> featureDescriptions_;

  std::multiset<int> choicesMade_;
};

#endif