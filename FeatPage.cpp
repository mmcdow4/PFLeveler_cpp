#include "FeatPage.h"
#include <wx/tooltip.h>

#include <pf_include/strHelpers.h>

FeatPage::FeatPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* Feat lists */

  wxStaticText* featsLeftText = new wxStaticText(this, FEAT_REMAINING_COUNTER_TEXT_ID, wxT("No Feat Picks Remaining"));
  vboxOverall->Add(featsLeftText, 0, wxALIGN_LEFT, 10);
  wxCheckBox* filterCheck = new wxCheckBox(this, FEAT_FILTER_CHECKBOX_ID, wxT("Hide Feats With Missing Prereqs"));
  filterCheck->Bind(wxEVT_CHECKBOX, &FeatPage::OnFilterBoxChecked, this);
  filterCheck->Disable();
  vboxOverall->Add(filterCheck, 0, wxALIGN_LEFT, 10);


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

  vboxOverall->Add(hbox1, 4, wxEXPAND);
  /* Description Box */
  wxTextCtrl* featDescription = new wxTextCtrl(this, FEAT_SELECTED_DESCRIPTION_ID, wxT("Description:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  featDescription->SetBackgroundColour(*wxWHITE);
  featDescription->Bind(wxEVT_SIZE, &FeatPage::ResizeCallback, this);
  vboxOverall->Add(featDescription, 2, wxEXPAND | wxALL, 10);
  /* learn/unlearn buttons */
  wxButton* learnBtn = new wxButton(this, FEAT_SELECT_BUTTON_ID, wxT("Take Feat"));
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

  if (featDescWrapper_ != NULL)
  {
    delete featDescWrapper_;
  }

  wxCheckBox* filterCheck = static_cast<wxCheckBox*>(wxWindow::FindWindowById(FEAT_FILTER_CHECKBOX_ID));
  filterCheck->Enable();
  filterCheck->SetValue(false);
  wxTextCtrl* featDesc = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID));
  featDescWrapper_ = new HardBreakWrapper(featDesc, wxT("Description:"), GetClientSize().GetWidth()-20);
  featDesc->SetLabelText(featDescWrapper_->GetWrapped());
  this->PopulateFeatLists();

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

  this->PopulateFeatLists();

  if (featsRemaining_ > 0)
  {
    static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel(wxString::Format(wxT("%d Feats remaining"), featsRemaining_));
    wxWindow::FindWindowById(FEAT_SELECT_BUTTON_ID)->Enable();
  }

  return (featsRemaining_ > 0);
}

void FeatPage::PopulateFeatLists(void)
{
  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  availListBox->ClearAll();
  knownListBox->Clear();
  availListBox->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 150);
  availListBox->InsertColumn(1, "Type", wxLIST_FORMAT_LEFT, 150);

  availFeatIds_.clear();
  availFeatMissingPrereqs_.clear();
  
  bool filterFeats = static_cast<wxCheckBox*>(wxWindow::FindWindowById(FEAT_FILTER_CHECKBOX_ID))->GetValue();
  bool proficiencyChange = false;
  for (int featIdx = 0; featIdx < Pathfinder::PFTable::get_num_feats(); featIdx++) {
    Pathfinder::Feat tmpFeat = Pathfinder::PFTable::get_feat(featIdx);
    if (!charPtr_->isFeatSelected(tmpFeat) || tmpFeat.multiple()) {
      wxListItem listItem;
      listItem.SetId(availFeatIds_.size());
      listItem.SetColumn(0);
      listItem.SetText(Pathfinder::PFTable::get_feat(featIdx).name());
      std::string missingPrereqs;
      if (charPtr_->checkProficiency(featIdx))
      {
        /* This proficiency is granted by a class */
        if (tmpFeat.name().find("Weapon Proficiency") != std::string::npos)
        {
          /* A weapon proficiency - mark the feat as redundant */
          listItem.SetTextColour(*wxLIGHT_GREY);
        }
        else
        {
          proficiencyChange = true;
          charPtr_->selectFeat(tmpFeat);
          knownListBox->AppendString(tmpFeat.fullName());
          continue;
        }
      }
      else if (this->CheckFeatPrereqs(featIdx, missingPrereqs))
      {
        listItem.SetTextColour(*wxBLACK);
      }
      else if(!filterFeats)
      {
        /* Don't have the prerequisites, not filtering so mark it red */
        listItem.SetTextColour(*wxRED);
      }
      else
      {
        /* Don't have the prerequsites, filtering so skip to next feat*/
        continue;
      }
      availFeatIds_.push_back(featIdx);
      availListBox->InsertItem(listItem);
      listItem.SetColumn(1);
      listItem.SetText(tmpFeat.type());
      availListBox->SetItem(listItem);
      availFeatMissingPrereqs_.push_back(missingPrereqs);
    }
  }

  if (proficiencyChange)
  {
    /* update the proficiency string */
    charPtr_->addProficiencies(-1);
  }

  std::vector<Pathfinder::Feat> featList = charPtr_->getSelectedFeats();
  for (std::vector<Pathfinder::Feat>::iterator featIter = featList.begin(); featIter != featList.end(); ++featIter)
  {
    knownListBox->AppendString(featIter->fullName());
  }
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

  UpdateFeatDescription(charPtr_->selectedFeat(featListIdx).id());

  wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID)->GetParent()->Layout();
}

void FeatPage::GrantFeats(void)
{
  this->PopulateFeatLists();
}

void FeatPage::ResizeCallback(wxSizeEvent& evt)
{
  if (featDescWrapper_ != NULL)
  {
    int maxWidth = 0;
    wxTextCtrl *featDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID));
    featDescBox->GetSize(&maxWidth, NULL);
    featDescBox->Clear();
    *featDescBox << featDescWrapper_->UpdateWidth(maxWidth);
  }
  evt.Skip();
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
  
  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID))->Clear();
  *static_cast<wxTextCtrl*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID)) << featDescWrapper_->UpdateText(descriptionText);

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
  Pathfinder::Feat newFeat = Pathfinder::PFTable::get_feat(featIdx);
  if (newFeat.choice() != Pathfinder::Feat::FEAT_NO_CHOICE)
  {
    std::vector<std::string> options = charPtr_->getFeatChoices(newFeat);
    if (options.empty())
    {
      wxMessageBox("You do not meet the prerequisites for any of the potential options for this feat.");
      return;
    }
    wxArrayString choiceStrings;
    for (std::vector<std::string>::iterator strIter = options.begin(); strIter != options.end(); ++strIter)
    {
      choiceStrings.Add(*strIter);
    }
    wxSingleChoiceDialog followOnDialog(this, "Make A Choice For The New Feat", "", choiceStrings);
    int choiceReturn = followOnDialog.ShowModal();
    int choiceIdx = followOnDialog.GetSelection();
    if (choiceReturn == wxID_OK)
    {
      newFeat.choiceString(options[choiceIdx]);
    }
    else
    {
      /* Choice declined, just exit without taking the feat*/
      return;
    }
  }

  charPtr_->selectFeat(newFeat);
  if (newFeat.name().find("Proficiency") != std::string::npos)
  {
    /* update the proficiency string */
    charPtr_->addProficiencies(-1);
  }

  if (newFeat.choice() == Pathfinder::Feat::FEAT_MERCY_CHOICE)
  {
    /* For the mercy feat, we need to go add the mercy choice as well */
    std::vector<Pathfinder::ClassChoice> choiceVec = charPtr_->getClassChoices();
    /* first get the mercy category, ugly I know... */
    int mercyCategory = -1;
    for (std::vector<Pathfinder::ClassChoice>::iterator choiceIter = choiceVec.begin(); choiceIter != choiceVec.end(); ++choiceIter)
    {
      if (choiceIter->name().find("Mercy") != std::string::npos)
      {
        mercyCategory = choiceIter->categoryId();
        break;
      }
    }

    /* Now get the full list of mercies, and look for the one we picked */
    choiceVec = Pathfinder::PFTable::get_class(Pathfinder::PALADIN).getChoiceVec(mercyCategory);
    for (std::vector<Pathfinder::ClassChoice>::iterator choiceIter = choiceVec.begin(); choiceIter != choiceVec.end(); ++choiceIter)
    {
      if (choiceIter->name().compare(newFeat.choiceString()) == 0)
      {
        charPtr_->makeClassChoice(*choiceIter);
        break;
      }
    }
    classChange_ = true;
  }

  /* update the listboxes */
  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  availListBox->SetItemState(featListIdx, 0, wxLIST_STATE_SELECTED);
  
  knownListBox->AppendString(newFeat.fullName());
  
  if (!Pathfinder::PFTable::get_feat(featIdx).multiple()) {
    availFeatIds_.erase(availFeatIds_.begin() + featListIdx);
    availFeatMissingPrereqs_.erase(availFeatMissingPrereqs_.begin() + featListIdx);
    availListBox->DeleteItem(featListIdx);
  }

  /* update the feats left counter */
  featsRemaining_--;

  wxString featsRemainingString = wxString::Format(wxT("%d Feats remaining"), featsRemaining_);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel(featsRemainingString);

  if (featsRemaining_ == 0 || classChange_)
  {
    wxWindow::FindWindowById(FEAT_SELECT_BUTTON_ID)->Disable();
    evt.Skip();
  }

  /* Call this to re-evaluate feat prerequisites */
  this->UpdateFeatPage(wxNOT_FOUND);

  availListBox->GetParent()->Layout();
}

void FeatPage::OnFilterBoxChecked(wxCommandEvent& evt)
{
  if(featDescWrapper_ != NULL)
  {
    static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID))->SetLabelText(featDescWrapper_->UpdateText("Description: "));
  }

  wxListCtrl* availListBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  availListBox->ClearAll();
  availListBox->InsertColumn(0, "Name", wxLIST_FORMAT_LEFT, 150);
  availListBox->InsertColumn(1, "Type", wxLIST_FORMAT_LEFT, 150);
  availFeatIds_.clear();
  availFeatMissingPrereqs_.clear();

  bool hideMissingPrereqs = static_cast<wxCheckBox*>(wxWindow::FindWindowById(FEAT_FILTER_CHECKBOX_ID))->GetValue();

  for (int featIdx = 0; featIdx < Pathfinder::PFTable::get_num_feats(); featIdx++) {
    Pathfinder::Feat tmpFeat = Pathfinder::PFTable::get_feat(featIdx);
    if (!charPtr_->isFeatSelected(tmpFeat) || tmpFeat.multiple()) {
      wxListItem listItem;
      listItem.SetId(availFeatIds_.size());
      listItem.SetColumn(0);
      listItem.SetText(Pathfinder::PFTable::get_feat(featIdx).name());
      std::string missingPrereqs;
      if (charPtr_->checkProficiency(featIdx))
      {
        /* This feat is a proficiency feat which is redundant */
        listItem.SetTextColour(*wxLIGHT_GREY);
      }
      else if (this->CheckFeatPrereqs(featIdx, missingPrereqs))
      {
        listItem.SetTextColour(*wxBLACK);
      }
      else if (!hideMissingPrereqs)
      {
        /* Write feats with missing prereqs in red */
        listItem.SetTextColour(*wxRED);
      }
      else
      {
        /* Don't show feats with missing prereqs */
        continue;
      }

      availListBox->InsertItem(listItem);
      listItem.SetColumn(1);
      listItem.SetText(tmpFeat.type());
      availListBox->SetItem(listItem);
      availFeatIds_.push_back(featIdx);
      availFeatMissingPrereqs_.push_back(missingPrereqs);
    }
  }
}

void FeatPage::MouseOverEvent(wxMouseEvent& evt)
{
  wxListCtrl* listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(evt.GetId()));
  int flags = 0;
  int item = listBox->HitTest(evt.GetPosition(), flags);

  if (flags == wxLIST_HITTEST_ONITEMLABEL && evt.Moving() && item != wxNOT_FOUND && !availFeatMissingPrereqs_[item].empty())
  {
    listBox->SetToolTip(availFeatMissingPrereqs_[item]);
  }
  else
  {
    listBox->UnsetToolTip();
  }
}

bool FeatPage::CheckFeatPrereqs(int featIndex, std::string &missingPrereqs)
{
  if (charPtr_ == NULL)
  {
    return true;
  }
  Pathfinder::Feat newFeat = Pathfinder::PFTable::get_feat(featIndex);
  return (charPtr_->checkFeatPrereqs(newFeat, missingPrereqs));
}