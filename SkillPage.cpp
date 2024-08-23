/* local includes*/
#include "SkillPage.h"

/* stdlib includes*/
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <string>

/* wxWidgets includes */
#include <wx/window.h>
#include <wx/stattext.h>
#include <wx/notebook.h>

/* Pathfinder includes */
#include <pf_include/PFTable.h>
#include <pf_include/Character.h>
#include <pf_include/Skills.h>

SkillPage::SkillPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxScrolled<wxPanel>(parentNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL), charPtr_(currChar)
{
  this->SetVirtualSize(1000, 600);
  this->SetScrollRate(10, 10);

  this->SetBackgroundColour(0xE5E5E5);
  this->SetFont(*wxSMALL_FONT);

  startingRanksVec_.assign(Pathfinder::NUMBER_SKILLS, 0);
  startingUnusedPoints_ = 0;

  wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL); /* will hold class dropdown, description, and feature list */

  wxFlexGridSizer* skillsGrid = new wxFlexGridSizer(Pathfinder::NUMBER_SKILLS + 1, NUMBER_SKILL_GRID_COLUMNS, 1, 1);
  
  /* Add labels for the columns*/
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Class Skill")), 0, wxALIGN_CENTER); //Class Skill Indicator
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Skill")), 0, wxALIGN_CENTER);
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Total")), 0, wxALIGN_CENTER);
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER); // equals sign
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Ability")), 0, wxALIGN_CENTER); 
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Ability Mod.")), 0, wxALIGN_CENTER);
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER); // plus sign
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Ranks")), 0, wxALIGN_CENTER);
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER); // plus sign
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Misc. Mod")), 0, wxALIGN_CENTER);
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER); // plus button
  skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER); // minus button

  /* Set up the rows of skills */
  for (int skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS; skillIdx++)
  {
    skillsGrid->Add(new wxCheckBox(this, SKILL_IS_CLASS_SKILL_BOX_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, ""), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, SKILL_SKILL_NAME_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxString(Pathfinder::skillStrings[skillIdx])), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, SKILL_TOTAL_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxT(" 0 ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" = ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, SKILL_ABILITY_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS,
      wxString(Pathfinder::ABILITY_SCORE_ABBREVIATIONS[static_cast<int>(Pathfinder::SKILL_BASE_ABILITIES[skillIdx])])), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, SKILL_ABILITY_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxT(" 0 ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" + ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, SKILL_RANK_VALUE_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxT(" 0 ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" + ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxStaticText(this, SKILL_MISC_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxT(" 0 ")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxButton(this, SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxT("+ Rank")), 0, wxALIGN_CENTER);
    skillsGrid->Add(new wxButton(this, SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS, wxT("- Rank")), 0, wxALIGN_CENTER);

    wxWindow::FindWindowById(SKILL_TOTAL_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetBackgroundColour(*wxWHITE);
    wxWindow::FindWindowById(SKILL_ABILITY_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetBackgroundColour(*wxWHITE);
    wxWindow::FindWindowById(SKILL_RANK_VALUE_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetBackgroundColour(*wxWHITE);
    wxWindow::FindWindowById(SKILL_MISC_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetBackgroundColour(*wxWHITE);

    wxWindow::FindWindowById(SKILL_IS_CLASS_SKILL_BOX_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->Disable();

    wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->Disable();
    wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SkillPage::OnSkillIncrement, this);
    wxWindow::FindWindowById(SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->Disable();
    wxWindow::FindWindowById(SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SkillPage::OnSkillDecrement, this);
  }
  
  hbox->Add(skillsGrid, 0, wxEXPAND);

  wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* vboxFillerTop = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* vboxFillerBottom = new wxBoxSizer(wxHORIZONTAL);

  hbox->Add(vbox, 1, wxEXPAND);

  vboxFillerTop->Add(new wxPanel(this, -1));
  vbox->Add(vboxFillerTop, 1, wxEXPAND);

  wxStaticText* unusedSkillPointsText = new wxStaticText(this, SKILL_NUM_SKILL_POINTS_LABEL_ID, "Number of Unspent Skill Points: 0");
  vbox->Add(unusedSkillPointsText, 0, wxALIGN_CENTER);

  wxButton* skillLockButton = new wxButton(this, SKILL_LOCK_BUTTON_ID, "Lock Skill Points");
  skillLockButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SkillPage::OnSkillsLocked, this);
  skillLockButton->Disable();
  vbox->Add(skillLockButton, 0, wxALIGN_CENTER);

  vboxFillerBottom->Add(new wxPanel(this, -1));
  vbox->Add(vboxFillerBottom, 2, wxEXPAND);

  this->SetSizerAndFit(hbox);
  this->Layout();
}


void SkillPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;
  startingRanksVec_.assign(Pathfinder::NUMBER_SKILLS, 0);
  startingUnusedPoints_ = 0;

  for (int skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS; skillIdx++)
  {
    Pathfinder::skillMarker currSkill = static_cast<Pathfinder::skillMarker>(skillIdx);
    wxWindow::FindWindowById(SKILL_TOTAL_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetLabel(wxString::Format(wxT(" %d "), charPtr_->effectiveSkillRank(currSkill)));
    wxWindow::FindWindowById(SKILL_ABILITY_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetLabel(wxString::Format(wxT(" %d "), charPtr_->abilityModifier(Pathfinder::SKILL_BASE_ABILITIES[skillIdx])));
    wxWindow::FindWindowById(SKILL_RANK_VALUE_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetLabel(wxString::Format(wxT(" %d "), charPtr_->rawSkillRank(currSkill)));
    wxWindow::FindWindowById(SKILL_MISC_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS)->SetLabel(" 0 ");

    static_cast<wxCheckBox*>(wxWindow::FindWindowById(SKILL_IS_CLASS_SKILL_BOX_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetValue(charPtr_->isClassSkill(currSkill));
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Show();
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Show();
  }
}

void SkillPage::UpdateSkillPage(void)
{
  /* Update Unused Skill Counter */
  startingUnusedPoints_ = charPtr_->unusedSkillPoints();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_NUM_SKILL_POINTS_LABEL_ID))->SetLabel("Number of Unspent Skill Points:" +
    wxString::Format(wxT(" %d "), startingUnusedPoints_));
  /* Update The Grid */
  for (int skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS; skillIdx++)
  {
    Pathfinder::skillMarker currSkill = static_cast<Pathfinder::skillMarker>(skillIdx);
    /* Update Class Skill Check Boxes*/
    if (charPtr_->isClassSkill(currSkill))
    {
      static_cast<wxCheckBox*>(wxWindow::FindWindowById(SKILL_IS_CLASS_SKILL_BOX_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetValue(true);
    }

    /* Update Total Values */
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_TOTAL_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetLabel(wxString::Format(wxT(" %d "), charPtr_->effectiveSkillRank(currSkill)));

    /* Update Ability Mods */
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_ABILITY_MOD_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetLabel(
      wxString::Format(wxT(" %d "), charPtr_->abilityModifier(Pathfinder::SKILL_BASE_ABILITIES[skillIdx])));

    /* Activate increment buttons */
    if (startingUnusedPoints_ > 0)
    {
      static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Enable();
    }
    startingRanksVec_[skillIdx] = charPtr_->rawSkillRank(static_cast<Pathfinder::skillMarker>(skillIdx));
  }

  if(startingUnusedPoints_ > 0)
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_LOCK_BUTTON_ID))->Enable();
  }
}

void SkillPage::OnSkillIncrement(wxCommandEvent& evt)
{
  int skillIdx = (evt.GetId() - SKILL_PLUS_RANK_BUTTON_ID) / NUMBER_SKILL_GRID_COLUMNS;
  int newTot = charPtr_->incrementSkill(static_cast<Pathfinder::skillMarker>(skillIdx));
  Pathfinder::skillMarker skillMark = static_cast<Pathfinder::skillMarker>(skillIdx);

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_TOTAL_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetLabel(
    wxString::Format(wxT(" %d "), newTot));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_RANK_VALUE_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetLabel(
    wxString::Format(wxT(" %d "), charPtr_->rawSkillRank(skillMark)));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_NUM_SKILL_POINTS_LABEL_ID))->SetLabel("Number of Unspent Skill Points:" +
    wxString::Format(wxT(" %d "), charPtr_->unusedSkillPoints()));
  if (charPtr_->rawSkillRank(skillMark) == charPtr_->getCharacterLevel())
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Disable();
  }
  if (charPtr_->rawSkillRank(skillMark) > startingRanksVec_[skillIdx])
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Enable();
  }

  if (charPtr_->unusedSkillPoints() == 0)
  {
    for (skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS; skillIdx++)
    {
      static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Disable();
    }
  }
}

void SkillPage::OnSkillDecrement(wxCommandEvent& evt)
{
  int skillIdx = (evt.GetId() - SKILL_MINUS_RANK_BUTTON_ID) / NUMBER_SKILL_GRID_COLUMNS;
  int newTot = charPtr_->decrementSkill(static_cast<Pathfinder::skillMarker>(skillIdx));
  Pathfinder::skillMarker skillMark = static_cast<Pathfinder::skillMarker>(skillIdx);

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_TOTAL_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetLabel(
    wxString::Format(wxT(" %d "), newTot));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_RANK_VALUE_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->SetLabel(
    wxString::Format(wxT(" %d "), charPtr_->rawSkillRank(skillMark)));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SKILL_NUM_SKILL_POINTS_LABEL_ID))->SetLabel("Number of Unspent Skill Points:" +
    wxString::Format(wxT(" %d "), charPtr_->unusedSkillPoints()));
  if (charPtr_->rawSkillRank(skillMark) == startingRanksVec_[skillIdx])
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Disable();
  }
  if (charPtr_->rawSkillRank(skillMark) < charPtr_->getCharacterLevel() && charPtr_->unusedSkillPoints() > 0)
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Enable();
  }
  
  if (charPtr_->unusedSkillPoints() == 1)
  {
    for (skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS; skillIdx++)
    {
      if (charPtr_->rawSkillRank(static_cast<Pathfinder::skillMarker>(skillIdx)) < charPtr_->getCharacterLevel())
      {
        static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_PLUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Enable();
      }
    }
  }
}

void SkillPage::OnSkillsLocked(wxCommandEvent& evt)
{
  for (int skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS; skillIdx++)
  {
    startingRanksVec_[skillIdx] = charPtr_->rawSkillRank(static_cast<Pathfinder::skillMarker>(skillIdx));
    static_cast<wxButton*>(wxWindow::FindWindowById(SKILL_MINUS_RANK_BUTTON_ID + skillIdx * NUMBER_SKILL_GRID_COLUMNS))->Disable();
  }

  if (charPtr_->unusedSkillPoints() == 0)
  {
    wxWindow::FindWindowById(evt.GetId())->Disable();
    evt.Skip();
  }
}