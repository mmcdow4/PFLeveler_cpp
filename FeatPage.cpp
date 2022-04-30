#include "FeatPage.h"

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
  wxListBox* availFeatsList = new wxListBox(this, FEAT_AVAIL_FEAT_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
  vboxAvail->Add(availFeatsList, 1, wxEXPAND, 0);

  availFeatsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FeatPage::OnFeatSelected, this);

  hbox1->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 10);

  /* Known Feats List */
  wxBoxSizer* vboxKnown = new wxBoxSizer(wxVERTICAL);
  wxStaticText* knownFeatsLabel = new wxStaticText(this, wxID_ANY, wxT("Known Feats:"));
  vboxKnown->Add(knownFeatsLabel, 0, wxBOTTOM, 5);
  wxListBox* knownFeatsList = new wxListBox(this, FEAT_KNOWN_FEAT_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
  vboxKnown->Add(knownFeatsList, 1, wxEXPAND, 0);

  knownFeatsList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FeatPage::OnFeatSelected, this);

  hbox1->Add(vboxKnown, 1, wxEXPAND | wxUP | wxDOWN | wxLEFT, 10);

  vboxOverall->Add(hbox1, 5, wxEXPAND);
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

  featsRemaining_ = charPtr_->getNumFeatSelectionsRemaining();
  availFeatIds_.clear();

  wxListBox* knownFeatList = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  knownFeatList->Clear();
  wxListBox* availFeatList = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  availFeatList->Clear();
  for (int featIdx = 0; featIdx < Pathfinder::PFTable::get_num_feats(); featIdx++) {
    if (!charPtr_->isFeatSelected(featIdx) || Pathfinder::PFTable::get_feat(featIdx).multiple()) {
      availFeatIds_.push_back(featIdx);
      availFeatList->AppendString(Pathfinder::PFTable::get_feat(featIdx).name());
    }
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID))->SetLabel("Description:");
  if (featsRemaining_ > 0) {
    static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel(wxString::Format(wxT("%d Feats remaining"), featsRemaining_));
  }
  else
  {
    featsRemaining_ = 0;
    static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel("No Feat Picks Remaining");
  }
  

  std::vector<int> knownFeats = charPtr_->getSelectedFeats();
  for (auto featIter = knownFeats.begin(); featIter != knownFeats.end(); ++featIter)
  {
    Pathfinder::Feat currFeat = Pathfinder::PFTable::get_feat(*featIter);
    knownFeatList->AppendString(wxString::Format(wxT("%s"), currFeat.name()));
  }

}

bool FeatPage::UpdateFeatPage(int classId)
{
  int classLevel = charPtr_->getClassLevel(classId);
  wxListBox* availSpellList = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));

  featsRemaining_ += Pathfinder::PFTable::get_class(classId).levelItem(classLevel, Pathfinder::NEW_FEAT);

  if (featsRemaining_ > 0)
  {
    //wxString featsRemainingString = wxString::Format(wxT("%d Feats remaining"), featsRemaining_);
    static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_REMAINING_COUNTER_TEXT_ID))->SetLabel(wxString::Format(wxT("%d Feats remaining"), featsRemaining_));
    wxWindow::FindWindowById(FEAT_SELECT_BUTTON_ID)->Enable();
  }

  return (featsRemaining_ > 0);
}

void FeatPage::OnFeatSelected(wxCommandEvent& evt)
{
  int featListIdx = evt.GetSelection();

  if (evt.GetId() == FEAT_AVAIL_FEAT_LIST_ID)
  {
    static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID))->SetSelection(wxNOT_FOUND);
    UpdateFeatDescription(availFeatIds_[featListIdx]);
  }
  else if (evt.GetId() == FEAT_KNOWN_FEAT_LIST_ID)
  {
    static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID))->SetSelection(wxNOT_FOUND);
    UpdateFeatDescription(charPtr_->selectedFeat(featListIdx));
  }

  wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID)->GetParent()->Layout();
}

void FeatPage::UpdateFeatDescription(int featIdx)
{
  Pathfinder::Feat currFeat = Pathfinder::PFTable::get_feat(featIdx);

  wxString descriptionText = "Description:\n" + currFeat.name() + "\n";

  descriptionText += "\n" + currFeat.description();
  if (!currFeat.prerequisites().empty())
  {
      descriptionText += "Prerequesites: " + currFeat.prerequisites() + "\n";
  }
  if (!currFeat.prereqFeats().empty())
  {
      descriptionText += "Prerequesite Feats: " + currFeat.prereqFeats() + "\n";
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(FEAT_SELECTED_DESCRIPTION_ID))->SetLabel(descriptionText);
}

void FeatPage::SelectFeatButtonPress(wxCommandEvent& evt)
{
  /* get the selected feat index */
  unsigned int featListIdx = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID))->GetSelection();
  if (featListIdx == wxNOT_FOUND)
  {
    return;
  }

  int featIdx = availFeatIds_[featListIdx];
  /* update the internal lists */
  charPtr_->selectFeat(featIdx);

  /* update the listboxes */
  wxListBox* availListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_AVAIL_FEAT_LIST_ID));
  wxListBox* knownListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(FEAT_KNOWN_FEAT_LIST_ID));
  knownListBox->AppendString(availListBox->GetString(featListIdx));

  if (!Pathfinder::PFTable::get_feat(featIdx).multiple()) {
    availFeatIds_.erase(availFeatIds_.begin() + featListIdx);

    wxArrayString tmpList = availListBox->GetStrings();
    tmpList.RemoveAt(featListIdx);
    availListBox->Clear();
    availListBox->InsertItems(tmpList, 0);
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

  availListBox->GetParent()->Layout();
}