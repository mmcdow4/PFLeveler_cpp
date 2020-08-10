#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>

#define DEFAULT_WINDOW_SIZE_X 1000
#define DEFAULT_WINDOW_SIZE_Y 600

#define ABSCR_MODE_STANDARD 0
#define ABSCR_MODE_CLASSIC 1
#define ABSCR_MODE_HEROIC 2
#define ABSCR_MODE_DICEPOOL 3
#define ABSCR_MODE_PURCHASE 4

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

#define ABSCR_METHOD_DROPDOWN_LABEL_ID 2000
#define ABSCR_METHOD_DROPDOWN_ID 2001
#define ABSCR_METHOD_BTN_ID 2002
#define ABSCR_SCORES_REMAINING_TEXT_ID 2003
#define ABSCR_ATTRIBUTE_LABELS 2004
#define ABSCR_ATTRIBUTE_STRENGTH_LABEL 2004
#define ABSCR_ATTRIBUTE_DEXTERITY_LABEL 2005
#define ABSCR_ATTRIBUTE_CONSTITUTION_LABEL 2006
#define ABSCR_ATTRIBUTE_INTELLIGENCE_LABEL 2007
#define ABSCR_ATTRIBUTE_WISDOM_LABEL 2008
#define ABSCR_ATTRIBUTE_CHARISMA_LABEL 2009
#define ABSCR_ATTRIBUTE_MINUS_BTN 2010 /* minus buttons range from 2010 for strength to 2015 for charisma */
#define ABSCR_ATTRIBUTE_VALUE_DROPDOWN 2016 /* range from 2016 for strength to 2021 for charisma */
#define ABSCR_ATTRIBUTE_PLUS_BTN 2022 /* range from 2022 to 2027 */
#define ABSCR_ATTRIBUTE_RACIALS 2028 /* range from 2028 to 2033 */
#define ABSCR_ATTRIBUTE_TOTALS 2034 /* range from 2034 to 2039 */
#define ABSCR_ATTRIBUTE_MODIFIERS 2040 /* range from 2040 to 2045 */
#define ABSCR_AC_TOTAL 2046
#define ABSCR_AC_ARMOR_VAL 2047
#define ABSCR_AC_SHIELD_VAL 2048
#define ABSCR_AC_DEX_MOD 2049
#define ABSCR_AC_SIZE_MOD 2050
#define ABSCR_AC_NATURAL_ARMOR 2051
#define ABSCR_AC_DEFLECT_MOD 2052
#define ABSCR_AC_MISC_BONUS 2053
#define ABSCR_MISC_TOUCH_VALUE 2054
#define ABSCR_MISC_FLATFOOT_VALUE 2055
#define ABSCR_MISC_INITMOD_VALUE 2056
#define ABSCR_MISC_DEX_MOD 2057
#define ABSCR_MISC_MISC_MOD 2058
#define ABSCR_SAVES_NAME 2059 /* name range from 2059 to 2061 */
#define ABSCR_SAVES_TOTAL 2062 /* range from 2062 to 2064 */
#define ABSCR_SAVES_BASE 2065 /* range from 2065 to 2067 */
#define ABSCR_SAVES_ABILITY_MOD 2068 /* range from 2068 to 2070 */
#define ABSCR_SAVES_MAGIC_MOD 2071 /* range from 2071 to 2073 */
#define ABSCR_SAVES_MISC_MOD 2074 /* range from 2074 to 2076 */
#define ABSCR_SAVES_TEMP_MOD 2077 /* range from 2077 to 2079 */
#define ABSCR_COMBAT_CMB_LABEL 2080
#define ABSCR_COMBAT_CMB_TOTAL 2081
#define ABSCR_COMBAT_CMB_BAB 2082
#define ABSCR_COMBAT_CMB_STR 2083
#define ABSCR_COMBAT_CMB_SIZE 2084
#define ABSCR_COMBAT_CMD_LABEL 2085
#define ABSCR_COMBAT_CMD_TOTAL 2086
#define ABSCR_COMBAT_CMD_BAB 2087
#define ABSCR_COMBAT_CMD_STR 2088
#define ABSCR_COMBAT_CMD_DEX 2089
#define ABSCR_COMBAT_CMD_SIZE 2090


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

  void OnAttributeModeSelected(wxCommandEvent& evt);
  wxDECLARE_EVENT_TABLE();

private:
  Pathfinder::Character* currChar_ = NULL;
  int mode_;
};

