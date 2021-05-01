#include "SpellPage.h"

SpellPage::SpellPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* spell lists */

  wxStaticText* spellsLeftText = new wxStaticText(this, SPELL_REMAINING_COUNTER_TEXT_ID, wxT("No Spells Left to Learn"));
  vboxOverall->Add(spellsLeftText, 1, wxEXPAND | wxALIGN_LEFT, 10);


  /* Available Spells List */
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Available Spells:"));
  vboxAvail->Add(availSpellsLabel, 0, wxBOTTOM, 5);
  wxString *dummyStr = NULL;
  wxListBox* availSpellsList = new wxListBox(this, SPELL_AVAIL_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
  vboxAvail->Add(availSpellsList, 1, wxEXPAND, 0);

  availSpellsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &SpellPage::OnSpellSelected, this);

  hbox1->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 10);

  /* Known Spells List */
  wxBoxSizer* vboxKnown = new wxBoxSizer(wxVERTICAL);
  wxStaticText* knownSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Known Spells:"));
  vboxKnown->Add(knownSpellsLabel, 0, wxBOTTOM, 5);
  wxListBox* knownSpellsList = new wxListBox(this, SPELL_KNOWN_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
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
  learnBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellPage::LearnSpellButtonPress, this);
  learnBtn->Disable();
  vboxOverall->Add(learnBtn, 0, wxALIGN_LEFT | wxRIGHT | wxBOTTOM, 10);

  this->SetSizer(vboxOverall);
}

void SpellPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  availSpellIds_.clear();

  memset(spellsLeft_, 0, 10 * sizeof(int));

  wxListBox* knownSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->Clear();
  knownSpellList->Clear();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Spells Left to Learn");
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID))->SetLabel("Description:");

  std::vector<int> knownSpells = charPtr_->getKnownSpells();
  for (auto spellIter = knownSpells.begin(); spellIter != knownSpells.end(); ++spellIter)
  {
    Pathfinder::Spell currSpell = Pathfinder::PFTable::get_spell(*spellIter);
    knownSpellList->AppendString(wxString::Format(wxT("level %d spell: %s"), currSpell.SlaLvl(), currSpell.name()));
  }
}

bool SpellPage::UpdateSpellPage(int classId)
{
  int classLevel = charPtr_->getClassLevel(classId);
  wxListBox* availSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));

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
        std::vector<int> spellVec = Pathfinder::PFTable::get_spell_list(spellLevel);

        for (std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
        {
          if (Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId) > -1 && // Is this available to your class?
            Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId) <= classLevel && // is your level high enough to learn it?
            !charPtr_->isSpellKnown(*spellIter)) //You don't already know this spell
          {
            availSpellIds_.push_back(*spellIter);
            availSpellList->AppendString(wxString::Format(wxT("level %d spell: %s"), spellLevel,
                Pathfinder::PFTable::get_spell(*spellIter).name()));
          }
        }
      }
    }
  }

  bool spellsLeft = UpdateSpellsRemainingText();

  if (spellsLeft)
  {
    wxWindow::FindWindowById(SPELL_LEARN_BUTTON_ID)->Enable();
  }

  return spellsLeft;
}

void SpellPage::OnSpellSelected(wxCommandEvent& evt)
{
  int spellListIdx = evt.GetSelection();

  if (evt.GetId() == SPELL_AVAIL_SPELL_LIST_ID)
  {
    static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID))->SetSelection(wxNOT_FOUND);
    UpdateSpellDescription(availSpellIds_[spellListIdx]);
  }
  else if (evt.GetId() == SPELL_KNOWN_SPELL_LIST_ID)
  {
    static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->SetSelection(wxNOT_FOUND);
    UpdateSpellDescription(charPtr_->knownSpell(spellListIdx));
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

void SpellPage::LearnSpellButtonPress(wxCommandEvent& evt)
{
  /* get the selected spell index */
  unsigned int spellListIdx = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->GetSelection();
  if (spellListIdx == wxNOT_FOUND)
  {
    return;
  }

  int spellIdx = availSpellIds_[spellListIdx];
  /* update the internal lists */
  charPtr_->learnSpell(spellIdx);
  availSpellIds_.erase(availSpellIds_.begin() + spellListIdx);

  /* update the listboxes */
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  knownListBox->AppendString(availListBox->GetString(spellListIdx));
  wxArrayString tmpList = availListBox->GetStrings();
  tmpList.RemoveAt(spellListIdx);
  availListBox->Clear();
  availListBox->InsertItems(tmpList, 0);

  /* update the spells left counter */
  int spellLevel = Pathfinder::PFTable::get_spell(spellIdx).SlaLvl();
  spellsLeft_[spellLevel]--;
  
  if (spellsLeft_[spellLevel] == 0)
  {
    /* Done learning this spell level, remove all spells of this level */
    for (size_t spellIter = 0; spellIter < availSpellIds_.size(); )
    {
      if (Pathfinder::PFTable::get_spell(availSpellIds_[spellIter]).SlaLvl() == spellLevel)
      {
        availSpellIds_.erase(availSpellIds_.begin() + spellIter);
        tmpList.RemoveAt(spellIter);
      }
      else
      {
        ++spellIter;
      }
    }
  }
  availListBox->Clear();
  availListBox->InsertItems(tmpList, 0);

  if(UpdateSpellsRemainingText() == false)
  {
    /* No more spells to learn -> disable the learn button and kick back to main */
    wxWindow::FindWindowById(SPELL_LEARN_BUTTON_ID)->Disable();
    evt.Skip();
  }

  availListBox->GetParent()->Layout();
}

bool SpellPage::UpdateSpellsRemainingText(void)
{
  bool anySpellsLeft = false;
  for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
  {
    if (spellsLeft_[spellLevel] > 0)
    {
      anySpellsLeft = true;
      break;
    }
  }

  if (!anySpellsLeft)
  {
    /* all spells have been chosen */
    /* Update text box */
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Spells Left to Learn");
    /* Clear the available spells list */
    static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->Clear();
    /* disable the learn button */
    static_cast<wxButton*>(wxWindow::FindWindowById(SPELL_LEARN_BUTTON_ID))->Disable();
  }
  else
  {
    /* update the spells available text */
    wxString spellsRemainingString = "";
    for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
    {
      if (spellsLeft_[spellLevel] > 0)
      {
        spellsRemainingString += wxString::Format(wxT("%d%s level : %d spells remaining\n"), spellLevel, SPELL_LEVEL_RANK_SUFFIX[spellLevel], spellsLeft_[spellLevel]);
      }
    }
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel(spellsRemainingString);
  }
  return anySpellsLeft;
}