#pragma once

#ifndef RACEPAGE_H
#define RACEPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>
#include "WrapText.h"

#include <vector>



#define RACE_NAME_LABEL_ID 3000
#define RACE_DROPDOWN_ID 3001
#define RACE_SELECT_BTN_ID 3002
#define RACE_TEXT_ID 3003
#define RACE_RACIAL_BONUS_TABLE_ID 3004
#define RACE_RACIAL_BONUS_DESC_ID 3005
#define RACE_BONUS_LANGUAGE_COUNT_ID 3006
#define RACE_BONUS_LANGUAGE_LABEL_ID 3007
#define RACE_BONUS_LANGUAGE_DROPDOWN_ID 3008
#define RACE_BONUS_LANGUAGE_BTN_ID 3009

class RacePage : public wxPanel
{

public:
  RacePage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~RacePage() {}

  void ResetPage(Pathfinder::Character* currChar);
  void SetupBonusLanguages(void);

  void OnRaceSelected(wxCommandEvent& evt);
  void OnRacialSelected(wxCommandEvent& evt);
  void OnRaceLocked(wxCommandEvent& evt);
  void LearnLanguageButtonPress(wxCommandEvent& evt);

  wxString populateRaceText(Pathfinder::Race chosenRace);
  void populateRacialTable(wxListBox* racialAbilityList, Pathfinder::Race raceObj);
  
private:
  Pathfinder::Character* charPtr_ = NULL;

  std::vector<int> availBonusLanguages_;
  bool bonusLanguagesLearned_ = false;

  HardBreakWrapper* raceTextWrapper_ = NULL;
  HardBreakWrapper* racialDescWrapper_ = NULL;
};

#endif