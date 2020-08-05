#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>

#define DEFAULT_WINDOW_SIZE_X 1000
#define DEFAULT_WINDOW_SIZE_Y 600

/* File Menu IDs */
#define FILE_IMPORT_ID 100
#define FILE_EXPORT_ID 101
#define FILE_RESET_ID 102
#define FILE_EXIT_ID 103

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
#define SUMMARY_FAV_CLASS_LABEL_ID 1023
#define SUMMARY_BUTTON_ID 1024
#define SUMMARY_TODO_LABEL_ID 1025
#define SUMMARY_TODO_LIST_ID 1026
#define SUMMARY_SPELL_LIST_ID 1027
#define SUMMARY_FEAT_LIST_ID 1028
#define SUMMARY_SKILL_LIST_ID 1029

#define RACE_NAME_LABEL_ID 3000
#define RACE_DROPDOWN_ID 3001
#define RACE_SELECT_BTN_ID 3002
#define RACE_TEXT_ID 3003
#define RACE_RACIAL_BONUS_TABLE_ID 3004
#define RACE_RACIAL_BONUS_DESC_ID 3005

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

  wxNotebook* notebook_;
  //wxButton*   m_btn1;
  //wxTextCtrl* m_txt1;
  //wxListBox*  m_list1;

  void menuCallback(wxCommandEvent& evt);
  void InitializeNotebook();
  void setupNotebook();

  void InitializeSummaryPage();
  void InitializeAbilityScoresPage();
  void InitializeRacePage();
  void InitializeClassPage();
  void InitializeSkillsPage();
  void InitializeSpellsPage();
  void InitializeFeatsPage();

  void SetupSummaryPage();
  void SetupRacePage();

  void OnButtonClicked(wxCommandEvent& evt);
  void OnRaceSelected(wxCommandEvent& evt);
  void OnRacialSelected(wxCommandEvent& evt);
  void OnRaceLocked(wxCommandEvent& evt);
  void OnCharLocked(wxCommandEvent& evt);

  wxDECLARE_EVENT_TABLE();

private:
  Pathfinder::Character* currChar_ = NULL;
  int mode_;
};

