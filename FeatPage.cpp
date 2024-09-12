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

  if (featDescWrapper_ != NULL)
  {
    delete featDescWrapper_;
  }

  wxTextCtrl* featDesc = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID));
  featDescWrapper_ = new HardBreakWrapper(featDesc, wxT("Description:"), GetClientSize().GetWidth()-20);
  featDesc->SetLabelText(featDescWrapper_->GetWrapped());
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
      if (charPtr_->checkProficiency(featIdx))
      {
        /* This proficiency is redundant for your class */
        listItem.SetTextColour(*wxLIGHT_GREY);
      }
      else if (this->CheckFeatPrereqs(featIdx, missingPrereqs))
      {
        listItem.SetTextColour(*wxBLACK);
      }
      else
      {
        /* Don't have the prerequisites */
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
    if (availListBox->GetItemTextColour(idx) != *wxLIGHT_GREY && charPtr_->checkProficiency(availFeatIds_[idx]))
    {
      /* This feat is a proficiency feat which became redundant */
      availListBox->SetItemTextColour(idx, *wxLIGHT_GREY);
      availFeatMissingPrereqs_[idx] = "";
    }

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

void FeatPage::ResizeCallback(wxSizeEvent& evt)
{
  if (featDescWrapper_ != NULL)
  {
    int maxWidth = 0;
    wxTextCtrl *featDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID));
    featDescBox->GetSize(&maxWidth, NULL);
    featDescBox->Clear();
    *featDescBox << featDescWrapper_->UpdateWidth(maxWidth);

    //int item = wxNOT_FOUND;
    //int featIdx = wxNOT_FOUND;
    //if ((item = static_cast<wxListCtrl*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID))->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    //{
    //  featIdx = availFeatIds_[item];
    //}
    //else if ((item = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID))->GetSelection()) != wxNOT_FOUND)
    //{
    //  featIdx = charPtr_->selectedFeat(item);
    //}

    //if (featIdx != wxNOT_FOUND)
    //{
    //  UpdateFeatDescription(featIdx);
    //}
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
  return (charPtr_->checkFeatPrereqs(featIndex, missingPrereqs));
}