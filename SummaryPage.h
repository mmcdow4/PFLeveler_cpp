#pragma once

#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>
#include "WrapText.h"

#include <vector>

/* Summary Tab IDs */
#define SUMMARY_NAME_LABEL_ID 1000
#define SUMMARY_NAME_INPUT_ID 1001
#define SUMMARY_PLAYER_LABEL_ID 1002
#define SUMMARY_PLAYER_INPUT_ID 1003
#define SUMMARY_ALIGNMENT_LABEL_ID 1004
#define SUMMARY_ALIGNMENT_DROPDOWN_ID 1005
#define SUMMARY_HEIGHT_LABEL_ID 1006
#define SUMMARY_HEIGHT_INPUT_ID 1007
#define SUMMARY_WEIGHT_LABEL_ID 1008
#define SUMMARY_WEIGHT_INPUT_ID 1009
#define SUMMARY_HAIR_LABEL_ID 1010
#define SUMMARY_HAIR_INPUT_ID 1011
#define SUMMARY_EYES_LABEL_ID 1012
#define SUMMARY_EYES_INPUT_ID 1013
#define SUMMARY_DEITY_LABEL_ID 1014
#define SUMMARY_DEITY_INPUT_ID 1015
#define SUMMARY_HOMELAND_LABEL_ID 1016
#define SUMMARY_HOMELAND_INPUT_ID 1017
#define SUMMARY_GENDER_LABEL_ID 1018
#define SUMMARY_GENDER_INPUT_ID 1019
#define SUMMARY_AGE_LABEL_ID 1020
#define SUMMARY_AGE_INPUT_ID 1021
#define SUMMARY_RACE_LABEL_ID 1022
#define SUMMARY_SIZE_LABEL_ID 1023
#define SUMMARY_SPEED_LABEL_ID 1024
#define SUMMARY_FAV_CLASS_LABEL_ID 1025
#define SUMMARY_LANGUAGE_LABEL_ID 1026
#define SUMMARY_BUTTON_ID 1027
#define SUMMARY_CLASS_LEVEL_LIST_ID 1028
#define SUMMARY_ABILITY_SCORES_LIST_ID 1029
#define SUMMARY_SKILL_LIST_ID 1030
#define SUMMARY_FEAT_LIST_ID 1031
#define SUMMARY_ABILITY_LIST_ID 1032
#define SUMMARY_SPELL_SLOT_LIST_ID 1033
#define SUMMARY_SPELL_LIST_ID 1034
#define SUMMARY_CLASS_DROPDOWN_ID 1035

class SummaryPage : public wxPanel
{

public:
  SummaryPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~SummaryPage() {}

  void ResetPage(Pathfinder::Character* currChar);

  void OnCharLocked(wxCommandEvent& evt);
  void OnClassSelected(wxCommandEvent& evt);

  void PopulateFavoredClassData(void);
  void PopulateClassLevelData(void);
  void PopulateRaceData(void);
  void PopulateAbilityScoreData(void);
  void PopulateSkillData(void);
  void PopulateFeatData(void);
  void PopulateSpellData(void);
private:
  Pathfinder::Character* charPtr_ = NULL;
  std::map<wxString, int> knownSpellsTable_;
  std::vector<int> classList_;

  HardBreakWrapper* languageTextWrapper_ = NULL;
};

#endif