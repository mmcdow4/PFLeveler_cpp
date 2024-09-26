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
  vboxOverall->Add(hbox1, 0, wxEXPAND | wxALIGN_LEFT, 10);


  /* Available Spells List */
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availSpellsLabel = new wxStaticText(this, wxID_ANY, wxT("Available Spells:"));
  vboxAvail->Add(availSpellsLabel, 0, wxBOTTOM, 5);
  wxString *dummyStr = NULL;
  wxListBox* availSpellsList = new wxListBox(this, SPELL_AVAIL_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
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
  wxTextCtrl* spellDescription = new wxTextCtrl(this, SPELL_SELECTED_DESCRIPTION_ID, wxT("Description:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  spellDescription->SetBackgroundColour(*wxWHITE);
  spellDescription->Bind(wxEVT_SIZE, &SpellPage::ResizeCallback, this);
  vboxOverall->Add(spellDescription, 2, wxEXPAND, 10);

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

  if (spellDescWrapper_ != NULL)
  {
    delete spellDescWrapper_;
  }

  wxTextCtrl* spellDesc = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID));
  spellDescWrapper_ = new HardBreakWrapper(spellDesc, wxT("Description:"), GetClientSize().GetWidth() - 20);
  spellDesc->SetLabelText(spellDescWrapper_->GetWrapped());

  static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID))->Clear();
  static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->Clear();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SPELL_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Spells Left to Learn");

  wxChoice* classDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID));
  classDropDown->Clear();

  for (int classIdx = 0; classIdx < Pathfinder::NUMBER_CLASSES; classIdx++)
  {
    std::vector<int> tempVec;
    availSpellIds_.emplace(classIdx, tempVec);
    tempVec.assign(10, 0);
    spellsLeft_.emplace(classIdx, tempVec);

    tempVec = charPtr_->getKnownSpells(classIdx);
    if (!tempVec.empty())
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

  availSpellList->Clear();
  knownSpellList->Clear();
  availSpellsTable_.clear();
  knownSpellsTable_.clear();

  int listIdx = -1;
  if (std::find(classList_.begin(), classList_.end(), classId) != classList_.end())
  {
    /* If the class is already in the dropdown menu, select it */
    listIdx = std::distance(classList_.begin(), std::find(classList_.begin(), classList_.end(), classId));
    classDropDown->SetSelection(listIdx);
  }
  for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
  {
    Pathfinder::lvlUpMarker spellLevelMarker = static_cast<Pathfinder::lvlUpMarker>(static_cast<int>(Pathfinder::SPELLS_KNOWN_0) + spellLevel);
    std::vector<int> spellVec = Pathfinder::PFTable::get_spell_list(classId, spellLevel);
 
    if (!spellVec.empty() && listIdx == -1)
    {
      /* If we have spells and are not already in the dropdown menu, add the class to the dropdown and select it */
      classList_.push_back(classId);
      classDropDown->AppendString(Pathfinder::CLASS_NAMES[classId]);
      listIdx = classList_.size() - 1;
      classDropDown->SetSelection(listIdx);
    }

    /* Now automatically set the class dropdown to the current class */
    int num_spells_known = Pathfinder::PFTable::get_class(classId).levelItem(classLevel, spellLevelMarker);
    if (classLevel == 1 && static_cast<Pathfinder::classMarker>(classId) == Pathfinder::WIZARD && spellLevel == 1)
    {
      /* Ugly and hardcoded, but it's a literal one off case: for only level 1 Wizards learning level 1 spells, learn an additional number equal to your intelligence modifier */
      num_spells_known += charPtr_->abilityModifier(Pathfinder::INTELLIGENCE);
    }
    if(num_spells_known == 0)
    {
      Pathfinder::abilityScoreMarker casterAbility = Pathfinder::PFTable::get_class(classId).casterAbility();
      if (charPtr_->getAbilityScore(casterAbility) < 10 + spellLevel)
      {
        /* Can't learn or cast spells of this level due to caster ability score, skip this and all following levels */
        break;
      }
      /* Spells Known == 0 means the entire spell list is known */
      for (std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
      {
        charPtr_->learnSpell(classId, *spellIter);
      }
    }
    else if (num_spells_known > 0)
    {
      Pathfinder::abilityScoreMarker casterAbility = Pathfinder::PFTable::get_class(classId).casterAbility();
      if (charPtr_->getAbilityScore(casterAbility) < 10 + spellLevel)
      {
        /* Can't learn or cast spells of this level due to caster ability score, skip this and all following levels */
        break;
      }
      /* Spells known > 0 means you must select a subset of spells to learn */
      if (classLevel > 1)
      {
        num_spells_known -= std::max(0, Pathfinder::PFTable::get_class(classId).levelItem(classLevel - 1, spellLevelMarker));
      }
      spellsLeft_[classId][spellLevel] = num_spells_known;
      if(num_spells_known > 0)
      {
        int num_spells_available = 0;
        for (std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
        {
          int spellIdx = *spellIter;
          if (!charPtr_->isSpellKnown(classId, spellIdx)) //You don't already know this spell
          {
            availSpellIds_[classId].push_back(spellIdx);
            num_spells_available++;

            /* We already selected this class in the dropdown above, fill out the available spell list as we go */
            wxString spellName = Pathfinder::PFTable::get_spell(*spellIter).name(classId);
            availSpellList->AppendString(spellName);
            availSpellsTable_.emplace(spellName, *spellIter);
          }
        }
        spellsLeft_[classId][spellLevel] = std::min(num_spells_available, num_spells_known);
      }
    }
  }
 
  /* We already have this class selected, fill out the known and available spell lists */
  std::vector<int> spellVec = charPtr_->getKnownSpells(classId);
  for(std::vector<int>::iterator spellIter = spellVec.begin(); spellIter != spellVec.end(); ++spellIter)
  {
    wxString spellName = Pathfinder::PFTable::get_spell(*spellIter).name(classId);
    knownSpellList->AppendString(spellName);
    knownSpellsTable_.emplace(spellName, *spellIter);
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

void SpellPage::GrantSpells(int classId)
{
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));

  /* Set the class dropdown to the relevant class */
  static_cast<wxChoice*>(wxWindow::FindWindowById(SPELL_CLASS_DROPDOWN_ID))->SetSelection(std::distance(classList_.begin(), std::find(classList_.begin(), classList_.end(), classId)));
  
  knownListBox->Clear();
  knownSpellsTable_.clear();

  std::vector<int> knownSpells = charPtr_->getKnownSpells(classId);
  for (auto spellIter = knownSpells.begin(); spellIter != knownSpells.end(); ++spellIter)
  {
    /* Repopulate the known spells list */
    wxString spell_name = Pathfinder::PFTable::get_spell(*spellIter).name(classId);
    knownListBox->AppendString(spell_name);
    knownSpellsTable_.emplace(spell_name, *spellIter);

    /* If the spell was in the available list, delete it */
    std::vector<int>::iterator availLoc = std::find(availSpellIds_[classId].begin(), availSpellIds_[classId].end(), *spellIter);
    if (availLoc != availSpellIds_[classId].end())
    {
      availSpellIds_[classId].erase(availLoc);
      availSpellsTable_.erase(spell_name);
      int loc = availListBox->FindString(spell_name);
      availListBox->Delete(loc);
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

  wxTextCtrl* spellDescription = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID));

  spellDescription->Clear();
  *spellDescription << spellDescWrapper_->UpdateText(descriptionText);
}

void SpellPage::ResizeCallback(wxSizeEvent& evt)
{
  if (spellDescWrapper_ != NULL)
  {
    int maxWidth = 0;
    wxTextCtrl* spellDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SPELL_SELECTED_DESCRIPTION_ID));
    spellDescBox->GetSize(&maxWidth, NULL);
    spellDescBox->Clear();
    *spellDescBox << spellDescWrapper_->UpdateWidth(maxWidth);

    //wxString selectedSpell = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID))->GetStringSelection();
    //int spellId = wxNOT_FOUND;
    //if (!selectedSpell.empty())
    //{
    //  spellId = knownSpellsTable_[selectedSpell];
    //}
    //else
    //{
    //  selectedSpell = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID))->GetStringSelection();
    //  if (!selectedSpell.empty())
    //  {
    //    spellId = availSpellsTable_[selectedSpell];
    //  }
    //}

    //if (spellId != wxNOT_FOUND)
    //{
    //  UpdateSpellDescription(spellId);
    //}
  }
  evt.Skip();
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

  /* If you are a first level wizard who has not chosen your arcane school yet, you cannot start learning spells */
  if (static_cast<Pathfinder::classMarker>(classIdx) == Pathfinder::WIZARD && charPtr_->getClassLevel(classIdx) == 1)
  {
    bool arcane_school_chosen = false;
    std::vector<Pathfinder::ClassChoice> choiceVec = charPtr_->getClassChoices();
    for (std::vector<Pathfinder::ClassChoice>::iterator iter = choiceVec.begin(); iter != choiceVec.end(); ++iter)
    {
      if (iter->name().find("Arcane School:"))
      {
        arcane_school_chosen = true;
        break;
      }
    }
    if (!arcane_school_chosen)
    {
      wxMessageBox("As a first level wizard you must choose your arcane school before learning any spells");
      return;
    }
  }
  /* update the internal lists */
  charPtr_->learnSpell(classIdx, spellIdx);
  availSpellIds_[classIdx].erase(availSpellIds_[classIdx].begin() + spellListIdx);

  /* update the listboxes */
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_AVAIL_SPELL_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(SPELL_KNOWN_SPELL_LIST_ID));
  knownListBox->AppendString(availListBox->GetString(spellListIdx));
  knownSpellsTable_.emplace(Pathfinder::PFTable::get_spell(spellIdx).name(classIdx), spellIdx);
  availSpellsTable_.erase(Pathfinder::PFTable::get_spell(spellIdx).name(classIdx));
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
        availSpellsTable_.erase(Pathfinder::PFTable::get_spell(spellIdx).name(classIdx));
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
      knownListBox->AppendString(Pathfinder::PFTable::get_spell(*spellIter).name(classIdx));
      knownSpellsTable_.emplace(Pathfinder::PFTable::get_spell(*spellIter).name(classIdx), *spellIter);
    }
    /* do the available spells now*/
    for (std::vector<int>::iterator spellIter = availSpellIds_[classIdx].begin(); spellIter != availSpellIds_[classIdx].end(); ++spellIter)
    {
      availListBox->AppendString(Pathfinder::PFTable::get_spell(*spellIter).name(classIdx));
      availSpellsTable_.emplace(Pathfinder::PFTable::get_spell(*spellIter).name(classIdx), *spellIter);
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