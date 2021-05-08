#pragma once

#ifndef SKILLPAGE_H
#define SKILLPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>

#include <vector>

#define NUMBER_SKILL_GRID_COLUMNS 12

#define SKILL_IS_CLASS_SKILL_BOX_ID 5000
#define SKILL_SKILL_NAME_ID 5001
#define SKILL_TOTAL_ID 5002
#define SKILL_ABILITY_ID 5003
#define SKILL_ABILITY_MOD_ID 5004
#define SKILL_RANK_VALUE_ID 5005
#define SKILL_MISC_MOD_ID 5006
#define SKILL_PLUS_RANK_BUTTON_ID 5007
#define SKILL_MINUS_RANK_BUTTON_ID 5008

#define SKILL_GRID_OFFSET SKILL_MINUS_RANK_BUTTON_ID + NUMBER_SKILL_GRID_COLUMNS*Pathfinder::NUMBER_SKILLS

#define SKILL_NUM_SKILL_POINTS_LABEL_ID (SKILL_GRID_OFFSET + 1)
#define SKILL_LOCK_BUTTON_ID (SKILL_GRID_OFFSET + 2)
#define SKILL_SCROLL_BAR_ID (SKILL_GRID_OFFSET + 3)

class SkillPage : public wxScrolled<wxPanel>
{

public:
  SkillPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~SkillPage() {}

  void ResetPage(Pathfinder::Character* currChar);
  void UpdateSkillPage();

  void OnSkillIncrement(wxCommandEvent& evt);
  void OnSkillDecrement(wxCommandEvent& evt);
  void OnSkillsLocked(wxCommandEvent& evt);


private:
  Pathfinder::Character* charPtr_ = NULL;
  std::vector<int> startingRanksVec_;
  int startingUnusedPoints_;
};

#endif