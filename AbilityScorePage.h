#pragma once

#ifndef ABILITYSCOREPAGE_H
#define ABILITYSCOREPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>

#include <vector>


#define ABSCR_MODE_STANDARD 0
#define ABSCR_MODE_CLASSIC 1
#define ABSCR_MODE_HEROIC 2
#define ABSCR_MODE_DICEPOOL 3
#define ABSCR_MODE_PURCHASE 4

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

class AbilityScorePage : public wxPanel
{

public:
  AbilityScorePage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~AbilityScorePage() {}

  void ResetPage(Pathfinder::Character* currChar);

  /* ability score page functions */
  void OnAttributeModeSelected(wxCommandEvent& evt);
  void OnAbilityScoreSelected(wxCommandEvent& evt);

  void ApplyRacialBonuses();
  void UpdateFields();

private:
  void populateScorePool(int modeIdx);
  std::vector<int> rolledScores_;
  int prevSelections_[Pathfinder::NUMBER_ABILITY_SCORES] = { 0, 0, 0, 0, 0, 0 };

  Pathfinder::Character* charPtr_ = NULL;
};

#endif