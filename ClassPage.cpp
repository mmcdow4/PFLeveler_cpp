/* local includes*/
#include "ClassPage.h"

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
#include <pf_include/Class.h>


//wxString populateRaceText(Pathfinder::Race raceObj);
//void populateRacialTable(wxListBox* racialAbilityList, Pathfinder::Race raceObj);


ClassPage::ClassPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);/* will contain the veritcal boxes below */
  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* will hold class dropdown, description, and feature list */
  wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL); /* will hold the list of class abilities */

  hbox1->Add(vbox1, 1, wxEXPAND | wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  hbox1->Add(vbox2, 1, wxEXPAND | wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);


  /* fill out The left side of the page */
  /* Add a horizontal sizer to contain the class dropdown menu and "add level" button */
  wxBoxSizer* hbox_levelup = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText* classLabel = new wxStaticText(this, CLASS_DROPDOWN_LABEL_ID, wxT("Classes:"));
  /* fill in the class menu */
  wxChoice* classDropDown = new wxChoice(this, CLASS_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);
  for (int classIdx = 0; classIdx < Pathfinder::PFTable::get_num_classes(); classIdx++)
  {
    try
    {
      classDropDown->Append(Pathfinder::PFTable::get_class(classIdx).name());
    }
    catch (const std::invalid_argument& e)
    {
      wxMessageBox(e.what());
    }
  }

  classDropDown->Bind(wxEVT_CHOICE, &ClassPage::OnClassSelected, this);
  classDropDown->Disable();
  classDropDown->Hide();

  wxButton* levelUpBtn = new wxButton(this, CLASS_LEVELUP_BUTTON_ID, wxT("Add Class Level"));
  levelUpBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ClassPage::OnLevelAdded, this);
  levelUpBtn->Disable();
  levelUpBtn->Hide();


  wxButton* favoredClassBtn = new wxButton(this, CLASS_FAVORED_CLASS_BUTTON_ID, wxT("Select Favored Class"));
  favoredClassBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ClassPage::OnFavoredClassAdded, this);
  favoredClassBtn->Disable();
  favoredClassBtn->Hide();

  hbox_levelup->Add(classLabel, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  hbox_levelup->Add(classDropDown, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  hbox_levelup->Add(levelUpBtn, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  hbox_levelup->Add(favoredClassBtn, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  vbox1->Add(hbox_levelup, 0, wxRIGHT, 10);

  /* Add a text box for the class description below the dropdown */
  wxStaticText* classDescription = new wxStaticText(this, CLASS_DESCRIPTION_ID, wxT("Class Description:"));
  classDescription->SetBackgroundColour(*wxWHITE);
  vbox1->Add(classDescription, 1, wxEXPAND | wxUP | wxDOWN, 10);

  wxStaticText* todoFeatureLabel = new wxStaticText(this, CLASS_TODO_FEATURE_LABEL_ID, wxT("Unset Class Features:"));
  wxStaticText* selectedFeatureLabel = new wxStaticText(this, CLASS_TODO_FEATURE_LABEL_ID, wxT("Selected Class Features:"));

  /* Create a horizontal sizer to contain the two lists of class features: class features yet to choose and class features already selected */
  wxBoxSizer* hbox_features = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* vbox_todo_features = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* vbox_selected_features = new wxBoxSizer(wxVERTICAL);

  vbox_todo_features->Add(todoFeatureLabel, 0, wxUP | wxDOWN, 10);
  vbox_selected_features->Add(selectedFeatureLabel, 0, wxUP | wxDOWN, 10);

  wxListBox* todoFeatList = new wxListBox(this, CLASS_TODO_FEATURE_LIST_ID);
  wxListBox* selectedFeatList = new wxListBox(this, CLASS_SELECTED_FEATURE_LIST_ID);

  todoFeatList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnUnselectedFeatureSelected, this);
  selectedFeatList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnFinishedFeatureSelected, this);

  todoFeatList->Disable();
  selectedFeatList->Disable();
  vbox_todo_features->Add(todoFeatList, 1, wxEXPAND | wxRIGHT, 10);
  vbox_selected_features->Add(selectedFeatList, 1, wxEXPAND | wxLEFT, 10);

  hbox_features->Add(vbox_todo_features, 1, wxEXPAND | wxRIGHT, 10);
  hbox_features->Add(vbox_selected_features, 1, wxEXPAND | wxRIGHT, 10);
  vbox1->Add(hbox_features, 2, wxEXPAND | wxRIGHT | wxLEFT, 10);

  wxStaticText* feature_description = new wxStaticText(this, CLASS_FEATURE_DESCRIPTION_ID, wxT("Feature Description:"));
  vbox1->Add(feature_description, 0, wxRIGHT, 10);

  wxButton* addFeatureBtn = new wxButton(this, CLASS_FEATURE_BUTTON_ID, wxT("Choose Selected Feature"));
  addFeatureBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ClassPage::MakeFeatureChoice, this);
  addFeatureBtn->Disable();
  addFeatureBtn->Hide();

  vbox1->Add(addFeatureBtn, 0, wxRIGHT, 10);
  /* Now fill out the right side of the page */
  /* label the class abilities list */
  wxStaticText* abilitiesLabel = new wxStaticText(this, CLASS_ABILITIES_LABEL_ID, wxT("Class Abilities:"));
  vbox2->Add(abilitiesLabel, 0, wxEXPAND | wxUP | wxDOWN, 10);

  wxListBox* abilitiesList = new wxListBox(this, CLASS_ABILITIES_LIST_ID);
  abilitiesList->Disable();
  vbox2->Add(abilitiesList, 2, wxEXPAND | wxUP | wxDOWN, 10);

  abilitiesList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnAbilitySelected, this);

  wxStaticText* abilitiesDescription = new wxStaticText(this, CLASS_ABILITIES_DESCRIPTION_ID, wxT("Ability Description:"));
  abilitiesDescription->SetBackgroundColour(*wxWHITE);
  vbox2->Add(abilitiesDescription, 1, wxEXPAND | wxUP | wxDOWN, 10);

  this->SetSizerAndFit(hbox1);
}

void ClassPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  /* class dropdown list */
  wxWindow::FindWindowById(CLASS_DROPDOWN_ID)->Show();
  wxWindow::FindWindowById(CLASS_DROPDOWN_ID)->Enable();

  /* level up button */
  wxWindow::FindWindowById(CLASS_LEVELUP_BUTTON_ID)->Hide();
  wxWindow::FindWindowById(CLASS_LEVELUP_BUTTON_ID)->Disable();

  /* favored class button */
  wxWindow::FindWindowById(CLASS_FAVORED_CLASS_BUTTON_ID)->Show();
  wxWindow::FindWindowById(CLASS_FAVORED_CLASS_BUTTON_ID)->Enable();

  /* class description */
  wxWindow::FindWindowById(CLASS_DESCRIPTION_ID)->Show();

  /* todo features list */
  wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID)->Enable();
  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID))->Clear();
  this->todoFeatures_.clear();

  /* selected features list */
  wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID)->Enable();
  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID))->Clear();
  this->featureNames_.clear();
  this->featureDescriptions_.clear();
  this->choicesMade_.clear();

  /* feature description */
  wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID)->Show();

  /* add feature button */
  wxWindow::FindWindowById(CLASS_FEATURE_BUTTON_ID)->Show();
  //wxWindow::FindWindowById(CLASS_FEATURE_BUTTON_ID)->Enable();

  /* class abilities list */
  wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID)->Enable();
  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID))->Clear();
  this->abilities_.clear();

  /* class abilities description */
  wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID)->Show();

  this->Layout();
}

void ClassPage::OnFavoredClassAdded(wxCommandEvent& evt)
{

  int classIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection();

  if (classIdx < 0)
  {
    wxMessageBox("Select a class from the dropdown menu.");
    return;
  }
  else if (charPtr_->race().id() == -1)// if Race hasn't been set yet
  {
    wxMessageBox("You must finalize your race selection before selecting a favored class.");
    return;
  }

  charPtr_->addFavoredClass(static_cast<Pathfinder::classMarker>(classIdx));

  if (charPtr_->numFavoredClassLeft() == 0)
  {
    wxWindow::FindWindowById(CLASS_FAVORED_CLASS_BUTTON_ID)->Hide();
    wxWindow::FindWindowById(CLASS_FAVORED_CLASS_BUTTON_ID)->Disable();
    wxWindow::FindWindowById(CLASS_LEVELUP_BUTTON_ID)->Show();
    wxWindow::FindWindowById(CLASS_LEVELUP_BUTTON_ID)->Enable();

    this->Layout();
  }

  evt.Skip();
}

void ClassPage::OnLevelAdded(wxCommandEvent& evt)
{

  int classIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection();

  /* verify that the necessary other information has been decided */
  if (classIdx < 0)
  {
    wxMessageBox("First select a class from the dropdown menu.");
    return;
  }
  else if (charPtr_->race().id() == -1)// if Race hasn't been set yet
  {
    wxMessageBox("You must finalize your race selection before adding a class level.");
    return;
  }
  else if (!charPtr_->abilityScoresSet()) // or if the ability scores haven't been set yet
  {
    wxMessageBox("You must finalize your ability scores before adding a class level.");
    return;
  }
  else if (charPtr_->numFavoredClassLeft() > 0)
  {
    wxMessageBox("You must finish selecting your favored classes before adding a class level.");
    return;
  }
  //else if (other things to do) //FIXME
  //{
  //  wxMessageBox("You must finish levelling up before adding a new level?");
  //  return;
  //}

  /* increment the level */
  int classLevel = charPtr_->incrementClassLevel(classIdx);

  /* roll for new hit points */
  int hitPointsAdded = charPtr_->abilityModifier(Pathfinder::CONSTITUTION);
  if (charPtr_->getCharacterLevel() == 1)
  {
    hitPointsAdded += Pathfinder::PFTable::get_class(classIdx).hitDie();
  }
  else
  {
    hitPointsAdded += Pathfinder::rollDn(Pathfinder::PFTable::get_class(classIdx).hitDie());
  }

  hitPointsAdded = (hitPointsAdded < 1 ? 1 : hitPointsAdded);
  charPtr_->incrementHitPoints(hitPointsAdded);

  /* Add the new features, split into features that are fixed and features with a choice element */
  std::vector<Pathfinder::ClassFeature> newFeatures = Pathfinder::PFTable::get_class(classIdx).getFeatureVec(classLevel);

  wxListBox* todoFeatList = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID));
  wxListBox* doneFeatList = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID));

  todoFeatures_.reserve(todoFeatures_.size() + newFeatures.size());
  featureNames_.reserve(featureNames_.size() + newFeatures.size());
  featureDescriptions_.reserve(featureDescriptions_.size() + newFeatures.size());
  for (auto featIter = newFeatures.begin(); featIter != newFeatures.end(); ++featIter)
  {
    if (featIter->numChoices() > 0)
    {
      todoFeatures_.push_back(*featIter);
      todoFeatList->AppendString(featIter->name());
    }
    else
    {
      featureNames_.push_back(featIter->name());
      featureDescriptions_.push_back(featIter->desc());
      doneFeatList->AppendString(featIter->name());
    }
  }

  if (!todoFeatures_.empty())
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(CLASS_FEATURE_BUTTON_ID))->Enable();
  }
  /* add the new class abilities */
  std::vector<Pathfinder::ClassAbility> newAbilities = Pathfinder::PFTable::get_class(classIdx).getAbilityVec(classLevel);
  abilities_.reserve(abilities_.size() + newAbilities.size());
  for (auto abilityIter = newAbilities.begin(); abilityIter != newAbilities.end(); ++abilityIter)
  {
    //wxMessageBox("class feature [" + featIter->name() + "] has [" + wxString::Format(wxT("%d"), featIter->numChoices()) + "] choices");
    abilities_.push_back(*abilityIter);
    static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID))->AppendString(abilityIter->name());
  }

  /* if this is the first level, mark the new favored skills */
  // if (classLevel == 1)
  // {
  //   /* update the favored class skills in the gui*/
  // }

  /* If this is a favored class, decide whether to add a bonus hitpoint or bonus skill rank */
  if (charPtr_->isFavoredClass(static_cast<Pathfinder::classMarker>(classIdx)))
  {
    /* Do favored class logic here */
    wxMessageDialog choiceWindow(this, "You have added a level to a favored class, select either +1 hit point or +1 skill rank.", "caption string", wxYES_NO | wxCENTRE | wxSTAY_ON_TOP);
    choiceWindow.SetYesNoLabels("Bonus Hit Point", "Bonus Skill Rank");
    int bonus_selection = choiceWindow.ShowModal();
    if (bonus_selection == wxID_YES)
    {
      charPtr_->incrementHitPoints(1);
    }
    else if (bonus_selection == wxID_NO)
    {
      /* bonus skill rank */
      //charPtr_->
    }
  }

  //Pathfinder::PFTable::get_class(classIdx).levelItem(classLevel, Pathfinder::);
}

void ClassPage::OnClassSelected(wxCommandEvent& evt)
{
  int classIdx = evt.GetSelection();

  Pathfinder::ClassBase chosenClass = Pathfinder::PFTable::get_class(classIdx);

  wxString classText;
  if (!chosenClass.desc().empty())
  {
    classText = chosenClass.desc() + "\n";
  }
  classText += "hit die = d" + wxString::Format(wxT("%d"), chosenClass.hitDie());
  classText += ", alignment = " + chosenClass.alignmentReq();

  wxStaticText* classDescBox = static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_DESCRIPTION_ID));
  classDescBox->SetLabel(classText);
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());


  wxWindow::FindWindowById(CLASS_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnAbilitySelected(wxCommandEvent& evt)
{
  int abilityIdx = evt.GetSelection();

  wxString abilityDesc = "Ability Description:\n" + abilities_[abilityIdx].desc();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID))->SetLabel(abilityDesc);

  wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnUnselectedFeatureSelected(wxCommandEvent& evt)
{
  int featIdx = evt.GetSelection();

  wxString featDesc = "Feature Description:\n" + todoFeatures_[featIdx].desc();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID))->SetLabel(featDesc);

  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID))->SetSelection(wxNOT_FOUND);
  wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnFinishedFeatureSelected(wxCommandEvent& evt)
{
  int featIdx = evt.GetSelection();

  wxString featDesc = "Feature Description:\n" + featureDescriptions_[featIdx];
  static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID))->SetLabel(featDesc);

  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID))->SetSelection(wxNOT_FOUND);
  wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::MakeFeatureChoice(wxCommandEvent& evt)
{
  unsigned int featIdx = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID))->GetSelection();
  int classIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection();
  int classLvl = charPtr_->getClassLevel(classIdx);
  int numChoices = todoFeatures_[featIdx].numChoices();
  wxListBox* featListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID));

  /* make sure we didn't fuck up somewhere */
  if (featIdx == wxNOT_FOUND)
  {
    wxMessageBox("Select a feature first, dumbass!");
    return;
  }
  else if (featIdx > featListBox->GetCount() || featIdx > todoFeatures_.size())
  {
    wxMessageBox("The fuck is this? you giving me [" + wxString::Format(wxT("%d"), featIdx) + "] as a selection with only [" +
      wxString::Format(wxT("%d"), featListBox->GetCount()) + "] or [" +
      wxString::Format(wxT("%d"), todoFeatures_.size()) + "] items in the list?!");
    return;
  }

  //Create the popup window to offer the selection
  std::vector<Pathfinder::ClassChoice> choiceVec = Pathfinder::PFTable::get_class(classIdx).getChoiceVec(todoFeatures_[featIdx].categoryId());
  wxArrayString choiceStrings;
  for (auto choiceIter = choiceVec.begin(); choiceIter != choiceVec.end(); )
  {
    if(choiceIter->lvlReq() <= classLvl && choicesMade_.count(choiceIter->id()) < choiceIter->maxNumSelections())
    {
      choiceStrings.Add(choiceIter->name());
      ++choiceIter;
    }
    else
    {
      choiceIter = choiceVec.erase(choiceIter);
    }
  }

  int numChoicesMade = 0;
  while (numChoicesMade < numChoices)
  {
    wxSingleChoiceDialog* choiceDialog = new wxSingleChoiceDialog(this,
      "Make selection " + wxString::Format(wxT("%d"), numChoicesMade + 1) + " out of " + wxString::Format(wxT("%d"), numChoices),
      "caption string", choiceStrings, NULL, wxOK);
    int choiceReturn = choiceDialog->ShowModal();
    int choiceIdx = choiceDialog->GetSelection();

    if (choiceReturn == wxID_OK)
    {
      wxString featName = todoFeatures_[featIdx].name() + ": " + choiceVec[choiceIdx].name();
      featureNames_.push_back(featName);
      featureDescriptions_.push_back(choiceVec[choiceIdx].desc());
      choicesMade_.insert(choiceVec[choiceIdx].id());
      static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID))->AppendString(featName);

      if (choicesMade_.count(choiceVec[choiceIdx].id()) == choiceVec[choiceIdx].maxNumSelections())
      {
        choiceVec.erase(choiceVec.begin() + choiceIdx);
        choiceStrings.RemoveAt(choiceIdx);
      }
      numChoicesMade++;
    }
  }

  /* remove this item from the list of features */
  todoFeatures_.erase(todoFeatures_.begin() + featIdx);

  wxArrayString tmpList = featListBox->GetStrings();
  tmpList.RemoveAt(featIdx);
  featListBox->Clear();
  featListBox->InsertItems(tmpList, 0);

  /* if there aren't any features left to choose, turn off the button for now */
  if (todoFeatures_.empty())
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(CLASS_FEATURE_BUTTON_ID))->Disable();
  }

  featListBox->GetParent()->Layout();
}