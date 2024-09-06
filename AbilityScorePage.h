#pragma once

#ifndef ABILITYSCOREPAGE_H
#define ABILITYSCOREPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Attributes.h>
#include <pf_include/Race.h>
#include <pf_include/Character.h>

#include <vector>


#define ABSCR_MODE_STANDARD 0
#define ABSCR_MODE_CLASSIC 1
#define ABSCR_MODE_HEROIC 2
#define ABSCR_MODE_DICEPOOL 3
#define ABSCR_MODE_PURCHASE 4
#define ABSCR_MODE_DIRECT_INPUT 5

#define ABSCR_OUT_OF_RANGE_COST 1000

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
#define ABSCR_ATTRIBUTE_VALUE_INPUT 2022 /* range form 2022 to 2027*/
#define ABSCR_ATTRIBUTE_VALUE_TEXT 2028 /* range from 2028 to 2033 */
#define ABSCR_ATTRIBUTE_PLUS_BTN 2034 /* range from 2034 to 2039 */
#define ABSCR_ATTRIBUTE_RACIALS 2040 /* range from 2040 to 2045 */
#define ABSCR_ATTRIBUTE_RACIAL_RADIO 2046 /* range from 2046 to 2051 */
#define ABSCR_ATTRIBUTE_TOTALS 2052 /* range from 2052 to 2057 */
#define ABSCR_ATTRIBUTE_MODIFIERS 2058 /* range from 2058 to 2063 */
#define ABSCR_AC_TOTAL 2064
#define ABSCR_AC_ARMOR_VAL 2065
#define ABSCR_AC_SHIELD_VAL 2066
#define ABSCR_AC_DEX_MOD 2067
#define ABSCR_AC_SIZE_MOD 2068
#define ABSCR_AC_NATURAL_ARMOR 2069
#define ABSCR_AC_DEFLECT_MOD 2070
#define ABSCR_AC_MISC_BONUS 2071
#define ABSCR_MISC_TOUCH_VALUE 2072
#define ABSCR_MISC_FLATFOOT_VALUE 2073
#define ABSCR_MISC_INITMOD_VALUE 2074
#define ABSCR_SAVES_NAME 2075 /* name range from 2075 to 2077 */
#define ABSCR_SAVES_TOTAL 2078 /* range from 2078 to 2080 */
#define ABSCR_SAVES_BASE 2081 /* range from 2081 to 2083 */
#define ABSCR_SAVES_ABILITY_MOD 2084 /* range from 2084 to 2086 */
#define ABSCR_SAVES_MAGIC_MOD 2087 /* range from 2087 to 2089 */
#define ABSCR_SAVES_MISC_MOD 2090 /* range from 2090 to 2092 */
#define ABSCR_SAVES_TEMP_MOD 2093 /* range from 2093 to 2095 */
#define ABSCR_COMBAT_CMB_LABEL 2096
#define ABSCR_COMBAT_CMB_TOTAL 2097
#define ABSCR_COMBAT_CMB_BAB 2098
#define ABSCR_COMBAT_CMB_STR 2099
#define ABSCR_COMBAT_CMB_SIZE 2100
#define ABSCR_COMBAT_CMD_LABEL 2101
#define ABSCR_COMBAT_CMD_TOTAL 2102
#define ABSCR_COMBAT_CMD_BAB 2103
#define ABSCR_COMBAT_CMD_STR 2104
#define ABSCR_COMBAT_CMD_DEX 2105
#define ABSCR_COMBAT_CMD_SIZE 2106
#define ABSCR_ATTRIBUTE_LOCK_BUTTON 2107

class AbilityScorePage : public wxPanel
{

public:
  AbilityScorePage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~AbilityScorePage() {}

  void ResetPage(Pathfinder::Character* currChar);

  /* ability score page functions */
  void OnAttributeModeSelected(wxCommandEvent& evt);
  void OnAbilityScoreSelected(wxCommandEvent& evt);
  void OnRacialRadioSelected(wxCommandEvent& evt);
  void OnAttributesLocked(wxCommandEvent& evt);
  void OnTextInput(wxCommandEvent& evt);
  void OnPlusButtonPress(wxCommandEvent& evt);
  void OnMinusButtonPress(wxCommandEvent& evt);

  void ApplyRacialBonuses();
  void UpdateFields();
  void UpdateAbilityScorePage(int classId);
private:
  void populateScorePool(int modeIdx);
  int calculatePointCost(int value, bool minus);
  int prevSelections_[Pathfinder::NUMBER_ABILITY_SCORES] = { 0, 0, 0, 0, 0, 0 };
  const int valueCosts_[12] = {-4, -2, -1, 0, 1, 2, 3, 5, 7, 10, 13, 17};
  bool flexibleApplied_ = false;
  bool newPoint_ = false;
  bool newPointUsed_ = false;
  int modeChosen_ = -1;
  int pointsRemaining_ = 0;
  int diceRemaining_ = 0;

  Pathfinder::Character* charPtr_ = NULL;
};

#endif