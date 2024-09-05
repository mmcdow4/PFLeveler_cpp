#include "FeatPage.h"
#include <wx/tooltip.h>

#include <pf_include/strHelpers.h>

FeatPage::FeatPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* Feat lists */

  wxStaticText* featsLeftText = new wxStaticText(this, FEAT_REMAINING_COUNTER_TEXT_ID, wxT("No Feat Picks Remaining"));
  vboxOverall->Add(featsLeftText, 1, wxEXPAND | wxALIGN_LEFT, 10);


  /* Available Feats List */
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availFeatsLabel = new wxStaticText(this, wxID_ANY, wxT("Available Feats:"));
  vboxAvail->Add(availFeatsLabel, 0, wxBOTTOM, 5);
  wxString *dummyStr = NULL;
  wxListCtrl* availFeatsList = new wxListCtrl(this, FEAT_AVAIL_FEAT_LIST_ID, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
  availFeatsList->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 150);
  availFeatsList->InsertColumn(1, "Type", wxLIST_FORMAT_LEFT, 150);
  vboxAvail->Add(availFeatsList, 1, wxEXPAND, 0);

  availFeatsList->Bind(wxEVT_LIST_ITEM_SELECTED, &FeatPage::OnAvailFeatSelected, this);
  availFeatsList->Bind(wxEVT_MOTION, &FeatPage::MouseOverEvent, this);
  //availFeatsList->Bind(wxEVT_ENTER_WINDOW, &FeatPage::MouseOverEvent, this);
  //availFeatsList->Bind(wxEVT_LEAVE_WINDOW, &FeatPage::MouseOverEvent, this);
  //availFeatsList->Bind(wxEVT_MOTION, &FeatPage::MouseOverEvent, this);

  hbox1->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 10);

  /* Known Feats List */
  wxBoxSizer* vboxKnown = new wxBoxSizer(wxVERTICAL);
  wxStaticText* knownFeatsLabel = new wxStaticText(this, wxID_ANY, wxT("Known Feats:"));
  vboxKnown->Add(knownFeatsLabel, 0, wxBOTTOM, 5);
  wxListBox* knownFeatsList = new wxListBox(this, FEAT_KNOWN_FEAT_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vboxKnown->Add(knownFeatsList, 1, wxEXPAND, 0);

  knownFeatsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FeatPage::OnTakenFeatSelected, this);

  hbox1->Add(vboxKnown, 1, wxEXPAND | wxUP | wxDOWN | wxLEFT, 10);

  vboxOverall->Add(hbox1, 10, wxEXPAND);
  /* Description Box */
  wxStaticText* featDescription = new wxStaticText(this, FEAT_SELECTED_DESCRIPTION_ID, wxT("Description:"));
  featDescription->SetBackgroundColour(*wxWHITE);
  vboxOverall->Add(featDescription, 1, wxEXPAND, 10);

  /* learn/unlearn buttons */
  wxButton* learnBtn = new wxButton(this, FEAT_SELECT_BUTTON_ID, wxT("Select Feat"));
  learnBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FeatPage::SelectFeatButtonPress, this);
  learnBtn->Disable();
  vboxOverall->Add(learnBtn, 0, wxALIGN_LEFT | wxRIGHT | wxBOTTOM, 10);

  this->SetSizer(vboxOverall);
}

void FeatPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  featsRemaining_ = 0;
  availFeatIds_.clear();

  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  knownListBox->Clear();
  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  availListBox->ClearAll();
  availListBox->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 150);
  availListBox->InsertColumn(1, "Type", wxLIST_FORMAT_LEFT, 150);

  for (int featIdx = 0; featIdx < Pathfinder::PFTable::get_num_feats(); featIdx++) {
    if (!charPtr_->isFeatSelected(featIdx) || Pathfinder::PFTable::get_feat(featIdx).multiple()) {
      availFeatIds_.push_back(featIdx);
      wxListItem listItem;
      listItem.SetId(availFeatIds_.size()-1);
      listItem.SetColumn(0);
      listItem.SetText(Pathfinder::PFTable::get_feat(featIdx).name());
      std::string missingPrereqs;
      if (this->CheckFeatPrereqs(featIdx, missingPrereqs))
      {
        listItem.SetTextColour(*wxBLACK);
      }
      else
      {
        listItem.SetTextColour(*wxRED);
      }
      availListBox->InsertItem(listItem);
      listItem.SetColumn(1);
      listItem.SetText(Pathfinder::PFTable::get_feat(featIdx).type());
      availListBox->SetItem(listItem);
      availFeatMissingPrereqs_.push_back(missingPrereqs);
    }
    if (charPtr_->isFeatSelected(featIdx))
    {
      knownListBox->AppendString(Pathfinder::PFTable::get_feat(featIdx).name());
    }
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID))->SetLabel("Description:");
  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Feat Picks Remaining");

}

bool FeatPage::UpdateFeatPage(int classId)
{

  if(classId != wxNOT_FOUND)
  {
    int classLevel = charPtr_->getClassLevel(classId);

    featsRemaining_ += Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::NEW_FEAT);
    if (charPtr_->getCharacterLevel() == 1 && charPtr_->race().bonusFeat()) {
      featsRemaining_++;
    }
  }

  /* Re-evaluate feat pre-requisites */
  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  for (int idx = 0; idx < availListBox->GetItemCount(); idx++)
  {
    if (!availFeatMissingPrereqs_[idx].empty())
    {
      
      std::string missingPrereqs;
      if (this->CheckFeatPrereqs(availFeatIds_[idx], missingPrereqs))
      {
        availListBox->SetItemTextColour(idx, *wxBLACK);
      }
      availFeatMissingPrereqs_[idx] = missingPrereqs;
    }
  }

  if (featsRemaining_ > 0)
  {
    static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel(wxString::Format(wxT("%d Feats remaining"), featsRemaining_));
    wxWindow::FindWindowById(FEAT_SELECT_BUTTON_ID)->Enable();
  }

  return (featsRemaining_ > 0);
}

void FeatPage::OnAvailFeatSelected(wxListEvent& evt)
{
  int featListIdx = evt.GetIndex();

  static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID))->SetSelection(wxNOT_FOUND);
  UpdateFeatDescription(availFeatIds_[featListIdx]);
}

void FeatPage::OnTakenFeatSelected(wxCommandEvent& evt)
{
  int featListIdx = evt.GetSelection();

  /* Deselect anything seletced in the available feats list */
  wxListCtrl* availList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  long item = -1;
  while ((item = availList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
    availList->SetItemState(item, 0, wxLIST_STATE_SELECTED);
  }

  UpdateFeatDescription(charPtr_->selectedFeat(featListIdx));

  wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID)->GetParent()->Layout();
}

void FeatPage::GrantFeats(void)
{
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  std::vector<int> knownFeats = charPtr_->getSelectedFeats();
  /* Remove all taken feats from the available list */
  for (auto featIter = knownFeats.begin(); featIter != knownFeats.end(); ++featIter)
  {
    std::vector<int>::iterator iter = std::find(availFeatIds_.begin(), availFeatIds_.end(), *featIter);
    int loc = std::distance(availFeatIds_.begin(), iter);
    //int loc = availListBox->FindString(Pathfinder::PFTable::get_feat(*featIter).name());

    if (loc < static_cast<int>(availFeatIds_.size()))
    {
      knownListBox->AppendString(Pathfinder::PFTable::get_feat(*featIter).name());
      /* Now delete from the available list */
      availListBox->DeleteItem(loc);
      availFeatIds_.erase(availFeatIds_.begin() + loc);
      availFeatMissingPrereqs_.erase(availFeatMissingPrereqs_.begin() + loc);
    }
  }
}

void FeatPage::UpdateFeatDescription(int featIdx)
{
  Pathfinder::Feat currFeat = Pathfinder::PFTable::get_feat(featIdx);

  wxString descriptionText = "Description:\n" + currFeat.name() + "\n";

  descriptionText += "\n" + currFeat.benefits();
  if (!currFeat.prerequisites().empty())
  {
      descriptionText += "\nPrerequesites: " + currFeat.prerequisites();
  }
  if (!currFeat.prereqFeats().empty())
  {
      descriptionText += "\nPrerequesite Feats: " + currFeat.prereqFeats();
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID))->SetLabel(descriptionText);
}

void FeatPage::SelectFeatButtonPress(wxCommandEvent& evt)
{
  /* get the selected feat index */
  int featListIdx = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID))->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (featListIdx == wxNOT_FOUND)
  {
    wxMessageBox("You need to select an available feat first");
    return;
  }
  else if (!availFeatMissingPrereqs_[featListIdx].empty())
  {
    wxMessageBox("You are missing the following prerequisites for this feat: " + availFeatMissingPrereqs_[featListIdx]);
    return;
  }

  int featIdx = availFeatIds_[featListIdx];
  /* update the internal lists */
  charPtr_->selectFeat(featIdx);

  /* update the listboxes */
  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  availListBox->SetItemState(featListIdx, 0, wxLIST_STATE_SELECTED);
  
  knownListBox->AppendString(availListBox->GetItemText(featListIdx));
  
  if (!Pathfinder::PFTable::get_feat(featIdx).multiple()) {
    availFeatIds_.erase(availFeatIds_.begin() + featListIdx);
    availFeatMissingPrereqs_.erase(availFeatMissingPrereqs_.begin() + featListIdx);
    availListBox->DeleteItem(featListIdx);
  }

  /* update the feats left counter */
  featsRemaining_--;

  wxString featsRemainingString = wxString::Format(wxT("%d Feats remaining"), featsRemaining_);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel(featsRemainingString);

  if (featsRemaining_ == 0)
  {
    wxWindow::FindWindowById(FEAT_SELECT_BUTTON_ID)->Disable();
    evt.Skip();
  }

  /* Call this to re-evaluate feat prerequisites */
  this->UpdateFeatPage(wxNOT_FOUND);

  availListBox->GetParent()->Layout();
}

void FeatPage::MouseOverEvent(wxMouseEvent& evt)
{
  wxListCtrl* listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(evt.GetId()));
  int flags = 0;
  int item = listBox->HitTest(evt.GetPosition(), flags);

  if (flags == wxLIST_HITTEST_ONITEMLABEL && evt.Moving() && item != wxNOT_FOUND && !availFeatMissingPrereqs_[item].empty())
  {
    listBox->SetToolTip(availFeatMissingPrereqs_[item]);//Pathfinder::PFTable::get_feat(availFeatIds_[item]).description());
  }
  else
  {
    listBox->UnsetToolTip();
  }
  //if (evt.Entering())
  //{
  //  listBox->SetToolTip(Pathfinder::PFTable::get_feat(availFeatIds_[item]).description());
  //  //toolTip_ = new wxToolTip(choiceDescriptions_[item]);
  //}
  //else if (evt.Moving() && !availFeatMissingPrereqs_[item].empty())
  //{
  //  listBox->SetToolTip(availFeatMissingPrereqs_[item]);//Pathfinder::PFTable::get_feat(availFeatIds_[item]).description());
  //  //toolTip_->SetTip(choiceDescriptions_[item]);
  //}
  //else if (evt.Leaving())
  //{
  //  listBox->UnsetToolTip();
  //  //delete toolTip_;
  //  //toolTip_ = NULL;
  //}
}

bool FeatPage::CheckFeatPrereqs(int featIndex, std::string &missingPrereqs)
{
  if (charPtr_ == NULL)
  {
    return true;
  }
  return (charPtr_->checkFeatPrereqs(featIndex, missingPrereqs));
  //std::string dummy;
  //bool testRet = (charPtr_->checkFeatPrereqs(featIndex, dummy));
  //std::vector<std::string> prereq_tokens;
  //if (!Pathfinder::PFTable::get_feat(featIndex).prerequisites().empty())
  //{
  //  prereq_tokens = split(Pathfinder::PFTable::get_feat(featIndex).prerequisites());
  //} 
  //std::vector<std::string> prereq_feat_tokens;
  //if (!Pathfinder::PFTable::get_feat(featIndex).prereqFeats().empty())
  //{
  //  prereq_feat_tokens = split(Pathfinder::PFTable::get_feat(featIndex).prereqFeats());
  //} 

  //std::vector<bool> prereqs_met;
  //if(prereq_tokens.empty())
  //{
  //  prereqs_met.push_back(true);
  //}
  //else
  //{
  //  prereqs_met.assign(prereq_tokens.size(), false);
  //}

  //std::vector<bool> prereq_feats_met;
  //if(prereq_feat_tokens.empty())
  //{
  //  prereq_feats_met.push_back(true);
  //}
  //else
  //{
  //  prereq_feats_met.assign(prereq_feat_tokens.size(), false);
  //}

  //for (size_t token_idx = 0; token_idx < prereq_tokens.size(); token_idx++)
  //{
  //  /* Validate this token */
  //  bool category_found = false;

  //  /* Just assume weapon proficiency for now */
  //  if (prereq_tokens[token_idx].find("proficient with weapon") != std::string::npos)
  //  {
  //    prereqs_met[token_idx] = true;
  //  }

  //  /* See if it's an ability score requirement */
  //  for (size_t attributeIdx = 0; attributeIdx < static_cast<size_t>(Pathfinder::NUMBER_ABILITY_SCORES); attributeIdx++)
  //  {
  //    if (prereq_tokens[token_idx].find(std::string(Pathfinder::ABILITY_SCORE_ABBREVIATIONS[attributeIdx])) != std::string::npos)
  //    {
  //      category_found = false;
  //      int prereq_val = std::stoi(prereq_tokens[token_idx].substr(4));
  //      prereqs_met[token_idx] = (charPtr_->getAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(attributeIdx)) >= prereq_val);
  //      break;
  //    }
  //  }
  //  
  //  if(category_found)
  //  {
  //    continue;
  //  }

  //  /* See if it's a base attack bonus requirement */
  //  if (prereq_tokens[token_idx].find("base attack bonus") != std::string::npos)
  //  {
  //    size_t pos = prereq_tokens[token_idx].find("base attack bonus");
  //    int prereq_val = std::stoi(prereq_tokens[token_idx].substr(pos+18));
  //    prereqs_met[token_idx] = (charPtr_->getBaseAttackBonus() >= prereq_val);
  //    continue;
  //  }

  //  /* See if it's a class level requirement */
  //  for (int classIdx = 0; classIdx < Pathfinder::PFTable::get_num_classes(); classIdx++)
  //  {
  //    size_t pos = prereq_tokens[token_idx].find("-level " + lower_string(Pathfinder::PFTable::get_class(classIdx).name()));
  //    if (pos != std::string::npos)
  //    {
  //      category_found = true;
  //      int prereq_val = std::stoi(prereq_tokens[token_idx].substr(0, pos-2));
  //      prereqs_met[token_idx] = (charPtr_->getClassLevel(classIdx) >= prereq_val);
  //    }
  //  }

  //  if (category_found)
  //  {
  //    continue;
  //  }

  //  /* See if it's a character level requirement */
  //  if (prereq_tokens[token_idx].find("-level Character") != std::string::npos)
  //  {
  //    size_t pos = prereq_tokens[token_idx].find("-level Charracter");
  //    int prereq_val = std::stoi(prereq_tokens[token_idx].substr(0, pos - 2));
  //    prereqs_met[token_idx] = (charPtr_->getCharacterLevel() >= prereq_val);
  //    continue;
  //  }

  //  /* See if it's a caster level requirement */
  //  if (prereq_tokens[token_idx].find("-level Caster") != std::string::npos)
  //  {
  //    size_t pos = prereq_tokens[token_idx].find("-level Caster");
  //    int prereq_val = std::stoi(prereq_tokens[token_idx].substr(0, pos-2));
  //    prereqs_met[token_idx] = (charPtr_->getCasterLevel() >= prereq_val);
  //    continue;
  //  }

  //  /* See if it's a skill rank requirement */
  //  if (prereq_tokens[token_idx].find(" rank") != std::string::npos)
  //  {
  //    std::vector<std::string> optionList = split(prereq_tokens[token_idx], "|");
  //    for (std::vector<std::string>::iterator optionIter = optionList.begin(); optionIter != optionList.end() &&
  //      !prereqs_met[token_idx]; ++optionIter)
  //    {
  //      for (int skillIdx = 0; skillIdx < Pathfinder::NUMBER_SKILLS && !prereqs_met[token_idx]; skillIdx++)
  //      {
  //        size_t pos = optionIter->find(std::string(Pathfinder::skillStrings[skillIdx]));
  //        if (pos != std::string::npos)
  //        {
  //          int prereq_val = std::stoi(optionIter->substr(0, pos-1));
  //          prereqs_met[token_idx] = (charPtr_->rawSkillRank(static_cast<Pathfinder::skillMarker>(skillIdx)) >= prereq_val);
  //        }
  //      }
  //    }
  //    continue;
  //  }

  //  /* See if it's a class feature requirement */
  //  if (prereq_tokens[token_idx].find("class feature") != std::string::npos)
  //  {
  //    size_t pos = prereq_tokens[token_idx].find("class feature");
  //    std::string featureName = prereq_tokens[token_idx].substr(0, pos-1);
  //    featureName = trim(featureName);
  //    std::vector<int> featureVec = charPtr_->getClassFeatures();
  //    for (std::vector<int>::iterator iter = featureVec.begin(); iter != featureVec.end() && !prereqs_met[token_idx]; ++iter)
  //    {
  //      prereqs_met[token_idx] = (Pathfinder::PFTable::get_class_feature(*iter).name().find(featureName) != std::string::npos);
  //    }
  //    featureVec = charPtr_->getClassAbilities();
  //    for (std::vector<int>::iterator iter = featureVec.begin(); iter != featureVec.end() && !prereqs_met[token_idx]; ++iter)
  //    {
  //      prereqs_met[token_idx] = (Pathfinder::PFTable::get_class_ability(*iter).name().find(featureName) != std::string::npos);
  //    }
  //    featureVec = charPtr_->getClassChoices();
  //    for (std::vector<int>::iterator iter = featureVec.begin(); iter != featureVec.end() && !prereqs_met[token_idx]; ++iter)
  //    {
  //      prereqs_met[token_idx] = (Pathfinder::PFTable::get_class_choice(*iter).name().find(featureName) != std::string::npos);
  //    }
  //    continue;
  //  }
  //}

  ///* Loop through the required feats list */
  //std::vector<int> takenFeats = charPtr_->getSelectedFeats();
  //for (size_t token_idx = 0; token_idx < prereq_feat_tokens.size(); token_idx++)
  //{
  //  /* Validate this token */
  //  std::vector<std::string> optionList = split(prereq_feat_tokens[token_idx], "|");
  //  int num_required = 1;
  //  int num_matched = 0;
  //  if (prereq_feat_tokens[token_idx].compare("any two critical feats") == 0)
  //  {
  //    /* Ugly one off for Critical Mastery */
  //    num_required = 2;
  //    optionList.clear();
  //    for (int feat_idx = 0; feat_idx < Pathfinder::PFTable::get_num_feats(); feat_idx++)
  //    {
  //      std::string name = Pathfinder::PFTable::get_feat(feat_idx).name();
  //      if (name.find("Critical") != std::string::npos)
  //      {
  //        optionList.push_back(name);
  //      }
  //    }
  //  }
  //  
  //  for(std::vector<std::string>::iterator optionIter = optionList.begin(); optionIter != optionList.end(); ++optionIter)
  //  {
  //    for(std::vector<int>::iterator iter = takenFeats.begin(); iter != takenFeats.end(); ++iter)
  //    {
  //      if (optionIter->compare(Pathfinder::PFTable::get_feat(*iter).name()) == 0)
  //      {
  //        /* You have the named feat */
  //        num_matched++;
  //      }
  //    }
  //  }

  //  prereq_feats_met[token_idx] = (num_matched >= num_required);
  //}

  //bool retVal = true;
  //for (size_t idx = 0; idx < prereqs_met.size(); idx++)
  //{
  //  if (!prereqs_met[idx])
  //  {
  //    retVal = false;
  //    if (!missingPrereqs.empty())
  //    {
  //      missingPrereqs += ", ";
  //    }
  //    missingPrereqs += prereq_tokens[idx];
  //  }
  //}
  //for (size_t idx = 0; idx < prereq_feats_met.size(); idx++)
  //{
  //  if (!prereq_feats_met[idx])
  //  {
  //    retVal = false;
  //    if (!missingPrereqs.empty())
  //    {
  //      missingPrereqs += ", ";
  //    }
  //    missingPrereqs += prereq_feat_tokens[idx];
  //  }
  //}

  //if (testRet != retVal || missingPrereqs.compare(dummy) != 0)
  //{
  //  wxMessageBox(wxString::Format(wxT("Error testing prereqs for [%d] [%s] : char said [%d] [%s] and page said [%d] [%s]"),
  //    featIndex, Pathfinder::PFTable::get_feat(featIndex).name().c_str(), static_cast<int>(testRet), dummy.c_str(), static_cast<int>(retVal), missingPrereqs.c_str()));
  //}
  //return retVal;
}