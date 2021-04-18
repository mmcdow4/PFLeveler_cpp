#include "SpellPage.h"

SpellPage::SpellPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* spell lists */

  wxStaticText* spellsLeftText = new wxStaticText(this, SPELL_REMAINING_COUNTER_TEXT_ID, wxT("0 Spells Left To Learn"));
  vboxOverall->Add(spellsLeftText, 1, wxEXPAND | wxALIGN_LEFT, 10);


  /* Available Spells List */
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Available Spells:"));
  vboxAvail->Add(availSpellsLabel, 0, wxBOTTOM, 5);
  wxListBox* availSpellsList = new wxListBox(this, SPELL_AVAIL_SPELL_LIST_ID);
  vboxAvail->Add(availSpellsList, 1, wxEXPAND, 0);

  availSpellsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &SpellPage::OnSpellSelected, this);

  hbox1->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 10);

  /* Known Spells List */
  wxBoxSizer* vboxKnown = new wxBoxSizer(wxVERTICAL);
  wxStaticText* knownSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Known Spells:"));
  vboxKnown->Add(knownSpellsLabel, 0, wxBOTTOM, 5);
  wxListBox* knownSpellsList = new wxListBox(this, SPELL_KNOWN_SPELL_LIST_ID);
  vboxKnown->Add(knownSpellsList, 1, wxEXPAND, 0);

  knownSpellsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &SpellPage::OnSpellSelected, this);

  hbox1->Add(vboxKnown, 1, wxEXPAND | wxUP | wxDOWN | wxLEFT, 10);

  vboxOverall->Add(hbox1, 5, wxEXPAND);
  /* Description Box */
  wxStaticText* spellDescription = new wxStaticText(this, SPELL_SELECTED_DESCRIPTION_ID, wxT("Description:"));
  spellDescription->SetBackgroundColour(*wxWHITE);
  vboxOverall->Add(spellDescription, 1, wxEXPAND, 10);

  /* learn/unlearn buttons */
  wxButton* learnBtn = new wxButton(this, SPELL_LEARN_BUTTON_ID, wxT("Learn Selected Spell"));
  //selectBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &RacePage::OnRaceLocked, this);
  learnBtn->Disable();
  vboxOverall->Add(learnBtn, 0, wxALIGN_LEFT | wxRIGHT | wxBOTTOM, 10);

  this->SetSizer(vboxOverall);
}

void SpellPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  availSpellIds_.clear();
  knownSpellIds_.clear();

  memset(spellsLeft_, 0, 10 * sizeof(int));

  wxListBox* knownSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->Clear();
  knownSpellList->Clear();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Spells Left to Learn");
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID))->SetLabel("Description:");

  int maxSpellIdx = charPtr_->getMaxSpellIdx();
  for (int spellIdx = 0; spellIdx <= maxSpellIdx; spellIdx++)
  {
    Pathfinder::Spell* spellPtr = NULL;
    if (charPtr_->getSpell(spellIdx, &spellPtr))
    {
      knownSpellIds_.push_back(spellIdx);
      knownSpellList->AppendString(spellPtr->name());
    }
  }
}

void SpellPage::UpdateSpellPage(int classId)
{
  int classLevel = charPtr_->getClassLevel(classId);
  wxListBox* availSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));

  wxString spellsRemainingString = "";
  if (Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::SPELLS_KNOWN_0) > 0)
  {
    /* cycle through spell levels, append all available spells to available list as level N : spell name */
    for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
    {
      Pathfinder::lvlUpMarker spellLevelMarker = static_cast<Pathfinder::lvlUpMarker>(static_cast<int>(Pathfinder::SPELLS_KNOWN_0) + spellLevel);
      spellsLeft_[spellLevel] = Pathfinder::PFTable::get_class(classId).levelItem(classLevel, spellLevelMarker);
      if (spellsLeft_[spellLevel] < 0)
      {
        break;
      }
      if (classLevel > 1 && Pathfinder::PFTable::get_class(classId).levelItem(classLevel - 1, spellLevelMarker) > 0)
      {
        spellsLeft_[spellLevel] -= Pathfinder::PFTable::get_class(classId).levelItem(classLevel - 1, spellLevelMarker);
      }
      if (spellsLeft_[spellLevel] > 0)
      {
        spellsRemainingString += wxString::Format(wxT("%d level %d spells remaining\n"), spellsLeft_[spellLevel], spellLevel);
        std::vector<int> spellVec = Pathfinder::PFTable::get_spell_list(spellLevel);

        for (std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
        {
          if (Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId) > -1 && 
            Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId) <= classLevel)
          {
            availSpellIds_.push_back(*spellIter);
            availSpellList->AppendString(wxString::Format(wxT("level %d %s spell: %s"), spellLevel,
              Pathfinder::CLASS_NAMES[classId], Pathfinder::PFTable::get_spell(*spellIter).name()));
          }
        }
      }
    }
  }
  else
  {
    spellsRemainingString = "No Spells Left to Learn";
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel(spellsRemainingString);
}

void SpellPage::OnSpellSelected(wxCommandEvent& evt)
{
  int spellListIdx = evt.GetSelection();

  if (evt.GetId() == SPELL_AVAIL_SPELL_LIST_ID)
  {
    UpdateSpellDescription(availSpellIds_[spellListIdx]);
  }
  else if (evt.GetId() == SPELL_KNOWN_SPELL_LIST_ID)
  {
    UpdateSpellDescription(knownSpellIds_[spellListIdx]);
  }

  wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID)->GetParent()->Layout();
}

void SpellPage::UpdateSpellDescription(int spellIdx)
{
  Pathfinder::Spell currSpell = Pathfinder::PFTable::get_spell(spellIdx);

  wxString descriptionText = "Description:\n" + currSpell.name() + "\n";
  descriptionText += wxString(Pathfinder::SPELL_SCHOOL_NAMES[currSpell.eSchool()]);
  if (currSpell.eSubSchool() != Pathfinder::NULL_SUBSCHOOL)
  {
    descriptionText += " (" + wxString(Pathfinder::SPELL_SUBSCHOOL_NAMES[currSpell.eSubSchool()]) + ")";
  }
  descriptionText += "\n" + currSpell.description();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID))->SetLabel(descriptionText);
}