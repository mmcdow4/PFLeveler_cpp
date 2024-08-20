#include "SpellPage.h"

SpellPage::SpellPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* spells left text, class dropdown */
  wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL); /* spell lists */

  wxStaticText* spellsLeftText = new wxStaticText(this, SPELL_REMAINING_COUNTER_TEXT_ID, wxT("No Spells Left to Learn"));
  hbox1->Add(spellsLeftText, 1, wxEXPAND | wxALIGN_LEFT, 10);

  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* Class dropdown and label */
  wxStaticText* classLabel = new wxStaticText(this, wxID_ANY, "Class:");
  vbox1->Add(classLabel, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE);
  wxChoice* classDropDown = new wxChoice(this, SPELL_CLASS_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);
  classDropDown->Bind(wxEVT_CHOICE, &SpellPage::OnClassSelected, this);
  vbox1->Add(classDropDown, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  hbox1->Add(vbox1);
  vboxOverall->Add(hbox1, 1, wxEXPAND | wxALIGN_LEFT, 10);


  /* Available Spells List */
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Available Spells:"));
  vboxAvail->Add(availSpellsLabel, 0, wxBOTTOM, 5);
  wxString *dummyStr = NULL;
  wxListBox* availSpellsList = new wxListBox(this, SPELL_AVAIL_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
  vboxAvail->Add(availSpellsList, 1, wxEXPAND, 0);

  availSpellsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &SpellPage::OnSpellSelected, this);

  hbox2->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 10);

  /* Known Spells List */
  wxBoxSizer* vboxKnown = new wxBoxSizer(wxVERTICAL);
  wxStaticText* knownSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Known Spells:"));
  vboxKnown->Add(knownSpellsLabel, 0, wxBOTTOM, 5);
  wxListBox* knownSpellsList = new wxListBox(this, SPELL_KNOWN_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
  vboxKnown->Add(knownSpellsList, 1, wxEXPAND, 0);

  knownSpellsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &SpellPage::OnSpellSelected, this);

  hbox2->Add(vboxKnown, 1, wxEXPAND | wxUP | wxDOWN | wxLEFT, 10);

  vboxOverall->Add(hbox2, 5, wxEXPAND);
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
  spellsLeft_.clear();
  availSpellsTable_.clear();
  knownSpellsTable_.clear();
  classList_.clear();

  static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID))->Clear();
  static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->Clear();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Spells Left to Learn");
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID))->SetLabel("Description:");

  wxChoice* classDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID));
  classDropDown->Clear();

  for (int classIdx = 0; classIdx < Pathfinder::NUMBER_CLASSES; classIdx++)
  {
    std::vector<int> tempVec;
    availSpellIds_.emplace(classIdx, tempVec);
    tempVec.assign(10, 0);
    spellsLeft_.emplace(classIdx, tempVec);

    int classLevel = charPtr_->getClassLevel(classIdx);
    if (classLevel > 0 && (Pathfinder::PFTable::get_class(classIdx).levelItem(classLevel, Pathfinder::SPELLS_KNOWN_0) >= 0 ||
      Pathfinder::PFTable::get_class(classIdx).levelItem(classLevel, Pathfinder::SPELLS_KNOWN_1) >= 0))
    {
      classList_.push_back(classIdx);
      classDropDown->AppendString(Pathfinder::CLASS_NAMES[classIdx]);
    }
  }
}

bool SpellPage::UpdateSpellPage(int classId)
{
  int classLevel = charPtr_->getClassLevel(classId);
  wxChoice* classDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID));
  wxListBox* availSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));
  wxListBox* knownSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  
  int classChoice = classDropDown->GetSelection();
  int classIdx = -1;
  if (classChoice != wxNOT_FOUND)
  {
    classIdx = classList_[classChoice];
  }
  if (classIdx == classId)
  {
    availSpellList->Clear();
    knownSpellList->Clear();
    availSpellsTable_.clear();
    knownSpellsTable_.clear();
  }

  if (Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::SPELLS_KNOWN_0) >= 0 ||
    Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::SPELLS_KNOWN_1) >= 0)
  {
    if (std::find(classList_.begin(), classList_.end(), classId) == classList_.end())
    {
      classList_.push_back(classId);
      classDropDown->AppendString(Pathfinder::CLASS_NAMES[classId]);
    }
    /* cycle through spell levels, append all available spells to available list as level N : spell name */
    for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
    {
      Pathfinder::lvlUpMarker spellLevelMarker = static_cast<Pathfinder::lvlUpMarker>(static_cast<int>(Pathfinder::SPELLS_KNOWN_0) + spellLevel);
      spellsLeft_[classId][spellLevel] = Pathfinder::PFTable::get_class(classId).levelItem(classLevel, spellLevelMarker);
      if (classLevel > 1 && Pathfinder::PFTable::get_class(classId).levelItem(classLevel - 1, spellLevelMarker) > 0)
      {
        spellsLeft_[classId][spellLevel] -= Pathfinder::PFTable::get_class(classId).levelItem(classLevel - 1, spellLevelMarker);
      }
      if (spellsLeft_[classId][spellLevel] > 0)
      {
        std::vector<int> spellVec = Pathfinder::PFTable::get_spell_list(classId, spellLevel);

        for (std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
        {
          if (charPtr_->isSpellKnown(classId, *spellIter)) //You don't already know this spell
          {
            availSpellIds_[classId].push_back(*spellIter);
            if (classIdx == classId)
            {
              wxString spellName = wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId), Pathfinder::PFTable::get_spell(*spellIter).name());
              availSpellList->AppendString(spellName);
              availSpellsTable_.emplace(spellName, *spellIter);
            }
          }
        }
      }
    }
  }
  else if (Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::SPELLS_PER_DAY_0) >= 0 ||
    Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::SPELLS_PER_DAY_1) >= 0)
  {
    if (std::find(classList_.begin(), classList_.end(), classId) == classList_.end())
    {
      classList_.push_back(classId);
      classDropDown->AppendString(Pathfinder::CLASS_NAMES[classId]);
    }
    for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
    {
      Pathfinder::lvlUpMarker spellLevelMarker = static_cast<Pathfinder::lvlUpMarker>(static_cast<int>(Pathfinder::SPELLS_PER_DAY_0) + spellLevel);
      if (Pathfinder::PFTable::get_class(classId).levelItem(classLevel, spellLevelMarker) >= 0)
      {
        std::vector<int> spellVec = Pathfinder::PFTable::get_spell_list(classId, spellLevel);
        for (std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
        {
          if (Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId) > -1 && // Is this spell available to your class?
            Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId) <= classLevel && // is your level high enough to learn it?
            !charPtr_->isSpellKnown(classId, *spellIter)) // You don't already know this spell
          {
            if (std::find(availSpellIds_[classId].begin(), availSpellIds_[classId].end(), *spellIter) != availSpellIds_[classId].end())
            {
              availSpellIds_[classId].erase(availSpellIds_[classId].begin() + *spellIter);
            }
            charPtr_->learnSpell(classId, *spellIter);
          }
        }
      }
    }
  }

  if (classIdx == classId)
  {
    std::vector<int> spellVec = charPtr_->getKnownSpells(classIdx);
    for(std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
    {
      wxString spellName = wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId), Pathfinder::PFTable::get_spell(*spellIter).name());
      knownSpellList->AppendString(spellName);
      knownSpellsTable_.emplace(spellName, *spellIter);
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
  int classChoice = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID))->GetSelection();
  if (classChoice != wxNOT_FOUND)
  {
    int classIdx = classList_[classChoice];
    if (evt.GetId() == SPELL_AVAIL_SPELL_LIST_ID)
    {
      wxString spellName = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->GetString(spellListIdx);
      static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID))->SetSelection(wxNOT_FOUND);
      UpdateSpellDescription(availSpellsTable_[spellName]);
    }
    else if (evt.GetId() == SPELL_KNOWN_SPELL_LIST_ID)
    {
      wxString spellName = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID))->GetString(spellListIdx);
      static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->SetSelection(wxNOT_FOUND);
      UpdateSpellDescription(knownSpellsTable_[spellName]);
    }
  }
  wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID)->GetParent()->Layout();
}

void SpellPage::GrantSpells(void)
{
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));

  int classChoice = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID))->GetSelection();
  int classIdx = -1;
  if (classChoice != wxNOT_FOUND)
  {
    classIdx = classList_[classChoice];
   }
  /* FIXME: After adding the class drop down, switch this to get selected class */
  for(int classId = 0; classId < Pathfinder::NUMBER_CLASSES; classId++)
  {
    std::vector<int> knownSpells = charPtr_->getKnownSpells(classId);
    for (auto spellIter = knownSpells.begin(); spellIter != knownSpells.end(); ++spellIter)
    {
      wxString spell_name = wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classId), Pathfinder::PFTable::get_spell(*spellIter).name());
      std::vector<int>::iterator availLoc = std::find(availSpellIds_[classId].begin(), availSpellIds_[classId].end(), *spellIter);
      if (availLoc != availSpellIds_[classId].end())
      {
        availSpellIds_[classId].erase(availLoc);
        availSpellsTable_.erase(spell_name);
        int loc = availListBox->FindString(spell_name);
        if (loc != wxNOT_FOUND && classIdx == classId)
        {
          availListBox->Delete(loc);
        }
      }

      int loc = knownListBox->FindString(spell_name);
      if(loc == wxNOT_FOUND && classIdx == classId)
      {
        knownListBox->AppendString(spell_name);
        knownSpellsTable_.emplace(spell_name, *spellIter);
      }
    }
  }
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
  int classChoice = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID))->GetSelection();
  if (spellListIdx == wxNOT_FOUND || classChoice == wxNOT_FOUND)
  {
    return;
  }
  int classIdx = classList_[classChoice];
  int spellIdx = availSpellIds_[classIdx][spellListIdx];
  /* update the internal lists */
  charPtr_->learnSpell(classIdx, spellIdx);
  availSpellIds_[classIdx].erase(availSpellIds_[classIdx].begin() + spellListIdx);

  /* update the listboxes */
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  knownListBox->AppendString(availListBox->GetString(spellListIdx));
  knownSpellsTable_.emplace(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(spellIdx).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(spellIdx).name()), spellIdx);
  availSpellsTable_.erase(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(spellIdx).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(spellIdx).name()));
  availListBox->DeselectAll();
  availListBox->Delete(spellListIdx);

  /* update the spells left counter */
  int spellLevel = Pathfinder::PFTable::get_spell(spellIdx).requiredClassLevel(classIdx);
  spellsLeft_[classIdx][spellLevel]--;
  
  if (spellsLeft_[classIdx][spellLevel] == 0)
  {
    /* Done learning this spell level, remove all spells of this level */
    for (size_t spellIter = 0; spellIter < availSpellIds_[classIdx].size(); )
    {
      if (Pathfinder::PFTable::get_spell(availSpellIds_[classIdx][spellIter]).requiredClassLevel(classIdx) == spellLevel)
      {
        int spellIdx = availSpellIds_[classIdx][spellIter];
        availSpellIds_[classIdx].erase(availSpellIds_[classIdx].begin() + spellIter);
        availListBox->Delete(spellIter);
        availSpellsTable_.erase(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(spellIdx).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(spellIdx).name()));
      }
      else
      {
        ++spellIter;
      }
    }
  }

  if(UpdateSpellsRemainingText() == false)
  {
    /* No more spells to learn -> disable the learn button and kick back to main */
    wxWindow::FindWindowById(SPELL_LEARN_BUTTON_ID)->Disable();
    evt.Skip();
  }

  availListBox->GetParent()->Layout();
}

void SpellPage::OnClassSelected(wxCommandEvent& evt)
{
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));
  int classChoice = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID))->GetSelection();

  if (classChoice != wxNOT_FOUND)
  {
    knownListBox->Clear();
    availListBox->Clear();
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID))->SetLabel(wxT("Description:"));
    knownSpellsTable_.clear();
    availSpellsTable_.clear();
    int classIdx = classList_[classChoice];
    std::vector<int> spellList = charPtr_->getKnownSpells(classIdx);
    for (std::vector<int>::iterator spellIter = spellList.begin(); spellIter != spellList.end(); ++spellIter)
    {
      knownListBox->AppendString(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(*spellIter).name()));
      knownSpellsTable_.emplace(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(*spellIter).name()), *spellIter);
    }
    /* do the available spells now*/
    for (std::vector<int>::iterator spellIter = availSpellIds_[classIdx].begin(); spellIter != availSpellIds_[classIdx].end(); ++spellIter)
    {
      availListBox->AppendString(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(*spellIter).name()));
      availSpellsTable_.emplace(wxString::Format(wxT("level %d spell: %s"), Pathfinder::PFTable::get_spell(*spellIter).requiredClassLevel(classIdx), Pathfinder::PFTable::get_spell(*spellIter).name()), *spellIter);
    }
  }
}

bool SpellPage::UpdateSpellsRemainingText(void)
{
  bool anySpellsLeft = false;
  for(std::vector<int>::iterator classIter = classList_.begin(); classIter != classList_.end(); ++classIter)
  {
    for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
    {
      if (spellsLeft_[*classIter][spellLevel] > 0)
      {
        anySpellsLeft = true;
        break;
      }
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
    for (std::vector<int>::iterator classIter = classList_.begin(); classIter != classList_.end(); ++classIter)
    {
      for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
      {
        if (spellsLeft_[*classIter][spellLevel] > 0)
        {
          spellsRemainingString += wxString::Format(wxT("%d%s level : %d %s spells remaining\n"), spellLevel, SPELL_LEVEL_RANK_SUFFIX[spellLevel], spellsLeft_[*classIter][spellLevel], Pathfinder::CLASS_NAMES[*classIter]);
        }
      }
    }
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel(spellsRemainingString);
  }
  return anySpellsLeft;
}