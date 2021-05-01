#pragma once

#ifndef SPELLPAGE_H
#define SPELLPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>

#define SPELL_REMAINING_COUNTER_TEXT_ID 6000
#define SPELL_AVAIL_SPELL_LIST_ID 6001
#define SPELL_LEARN_BUTTON_ID 6002
#define SPELL_UNLEARN_BUTTON_ID 6003
#define SPELL_KNOWN_SPELL_LIST_ID 6004
#define SPELL_SELECTED_DESCRIPTION_ID 6005

static const char* SPELL_LEVEL_RANK_SUFFIX[10] = {
    "th",
    "st",
    "nd",
    "rd",
    "th",
    "th",
    "th",
    "th",
    "th",
    "th" };
class SpellPage : public wxPanel
{

public:
  SpellPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~SpellPage() {}

  void ResetPage(Pathfinder::Character* currChar);
  bool UpdateSpellPage(int classId);

  void OnSpellSelected(wxCommandEvent& evt);
  void LearnSpellButtonPress(wxCommandEvent& evt);
  
  void UpdateSpellDescription(int spellIndex);
  bool UpdateSpellsRemainingText(void);
private:
  Pathfinder::Character* charPtr_ = NULL;
  std::vector<int> availSpellIds_;
  std::vector<int> knownSpellIds_;
  int spellsLeft_[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 };
};

#endif