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


ClassPage::ClassPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar), skillsLocked_(true), spellsLeft_(false), featsLeft_(false), grantedSpells_(false), grantedFeats_(false)
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
  classDescWrapper_ = NULL;
  wxStaticText* classDescription = new wxStaticText(this, CLASS_DESCRIPTION_ID, wxT("Class Description:"));
  classDescription->SetBackgroundColour(*wxWHITE);
  //classDescription->Bind(wxEVT_SIZE, &ClassPage::ResizeCallback, this);
  vbox1->Add(classDescription, 1, wxEXPAND | wxUP | wxDOWN, 10);

  wxStaticText* todoFeatureLabel = new wxStaticText(this, CLASS_TODO_FEATURE_LABEL_ID, wxT("Unset Class Features:"));
  wxStaticText* selectedFeatureLabel = new wxStaticText(this, CLASS_TODO_FEATURE_LABEL_ID, wxT("Selected Class Features:"));

  /* Create a horizontal sizer to contain the two lists of class features: class features yet to choose and class features already selected */
  wxBoxSizer* hbox_features = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* vbox_todo_features = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* vbox_selected_features = new wxBoxSizer(wxVERTICAL);

  vbox_todo_features->Add(todoFeatureLabel, 0, wxUP | wxDOWN, 10);
  vbox_selected_features->Add(selectedFeatureLabel, 0, wxUP | wxDOWN, 10);

  wxString* dummyStr = NULL;
  wxListBox* todoFeatList = new wxListBox(this, CLASS_TODO_FEATURE_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  wxListBox* selectedFeatList = new wxListBox(this, CLASS_SELECTED_FEATURE_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);

  todoFeatList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnUnselectedFeatureSelected, this);
  selectedFeatList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnFinishedFeatureSelected, this);

  todoFeatList->Disable();
  selectedFeatList->Disable();
  vbox_todo_features->Add(todoFeatList, 1, wxEXPAND | wxRIGHT, 10);
  vbox_selected_features->Add(selectedFeatList, 1, wxEXPAND | wxLEFT, 10);

  hbox_features->Add(vbox_todo_features, 1, wxEXPAND | wxRIGHT, 10);
  hbox_features->Add(vbox_selected_features, 1, wxEXPAND | wxRIGHT, 10);
  vbox1->Add(hbox_features, 3, wxEXPAND | wxRIGHT | wxLEFT, 10);

  wxStaticText* feature_description = new wxStaticText(this, CLASS_FEATURE_DESCRIPTION_ID, wxT("Feature Description:"));
  feature_description->SetBackgroundColour(*wxWHITE);
  //feature_description->Bind(wxEVT_SIZE, &ClassPage::ResizeCallback, this);
  vbox1->Add(feature_description, 1, wxALL, 10);

  wxButton* addFeatureBtn = new wxButton(this, CLASS_FEATURE_BUTTON_ID, wxT("Choose Selected Feature"));
  addFeatureBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ClassPage::SelectFeatureButtonPress, this);
  addFeatureBtn->Disable();
  addFeatureBtn->Hide();

  vbox1->Add(addFeatureBtn, 0, wxRIGHT, 10);
  /* Now fill out the right side of the page */
  /* label the class abilities list */
  wxStaticText* abilitiesLabel = new wxStaticText(this, CLASS_ABILITIES_LABEL_ID, wxT("Class Abilities:"));
  vbox2->Add(abilitiesLabel, 0, wxEXPAND | wxUP | wxDOWN, 10);

  wxListBox* abilitiesList = new wxListBox(this, CLASS_ABILITIES_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  abilitiesList->Disable();
  vbox2->Add(abilitiesList, 2, wxEXPAND | wxUP | wxDOWN, 10);

  abilitiesList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnAbilitySelected, this);

  wxStaticText* abilitiesDescription = new wxStaticText(this, CLASS_ABILITIES_DESCRIPTION_ID, wxT("Ability Description:"));
  abilitiesDescription->SetBackgroundColour(*wxWHITE);
  //abilitiesDescription->Bind(wxEVT_SIZE, &ClassPage::ResizeCallback, this);
  vbox2->Add(abilitiesDescription, 1, wxEXPAND | wxUP | wxDOWN, 10);

  this->SetSizerAndFit(hbox1);

}

void ClassPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;
  skillsLocked_ = true;
  spellsLeft_ = false;
  featsLeft_ = false;

  grantedSpells_ = false;
  grantedFeats_ = false;

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

  if (classDescWrapper_ != NULL)
  {
    delete classDescWrapper_;
    delete featureDescWrapper_;
    delete abilityDescWrapper_;
  }

  int maxWidth = 0;
  wxWindow::FindWindowById(CLASS_DESCRIPTION_ID)->GetSize(&maxWidth, NULL);
  classDescWrapper_ = new HardBreakWrapper(wxWindow::FindWindowById(CLASS_DESCRIPTION_ID), "", maxWidth);

  //wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID)->GetSize(&maxWidth, NULL);
  featureDescWrapper_ = new HardBreakWrapper(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID), "", maxWidth);
  
  //wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID)->GetSize(&maxWidth, NULL);
  abilityDescWrapper_ = new HardBreakWrapper(wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID), "", maxWidth);
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
    wxMessageBox("You need to finalize your race selection before adding a class level.");
    return;
  }
  else if (!charPtr_->abilityScoresSet()) // or if the ability scores haven't been set yet
  {
    wxMessageBox("You need to finalize your ability scores before adding a class level.");
    return;
  }
  else if (charPtr_->numFavoredClassLeft() > 0)
  {
    wxMessageBox("You need to finish selecting your favored class(es) before adding a class level.");
    return;
  }
  else if (!skillsLocked_)
  {
    wxMessageBox("You need to finish assigning skill points before adding a class level.");
    return;
  }
  else if (spellsLeft_)
  {
    wxMessageBox("You need to finish learning spells before adding a class level.");
    return;
  }
  else if (featsLeft_)
  {
    wxMessageBox("You need to finish selecting feats before adding a class level.");
    return;
  }
  else if (!todoFeatures_.empty())
  {
    wxMessageBox("You need to finish making choices for your class features before adding a class level.");
    return;
  }

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
    if(abilityIter->choicePrereqId() < 0 || charPtr_->checkForChoice(abilityIter->choicePrereqId())) {
      abilities_.push_back(*abilityIter);
      charPtr_->addClassAbility(abilityIter->id());
      static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID))->AppendString(abilityIter->name());
      if (abilityIter->spellId() >= 0) {
        charPtr_->learnSpell(abilityIter->spellId());
        grantedSpells_ = true;
      }
      else if (abilityIter->featId() >= 0) {
        charPtr_->selectFeat(abilityIter->featId());
        grantedFeats_ = true;
      }
    }
  }

  /* Add skill points */
  int numPoints = Pathfinder::PFTable::get_class(classIdx).skillsPerLvl() + charPtr_->abilityModifier(Pathfinder::INTELLIGENCE) + charPtr_->race().bonusSkill();
  if (numPoints > 0)
  {
    charPtr_->addSkillPoints(numPoints);
    skillsLocked_ = false;
  }

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
      charPtr_->addSkillPoints(1);
      skillsLocked_ = false;
    }
  }

  /* assume you need to handle spells, the spell page will immediately kick back and correct this if there are no spells to learn */
  spellsLeft_ = true;
  featsLeft_ = (Pathfinder::PFTable::get_class(classIdx).levelItem(classLevel, Pathfinder::NEW_FEAT) > 0);

  evt.Skip();
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
  classDescBox->SetLabel(classDescWrapper_->UpdateText(classText));
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());


  wxWindow::FindWindowById(CLASS_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnAbilitySelected(wxCommandEvent& evt)
{
  int abilityIdx = evt.GetSelection();

  wxString abilityDesc = "Ability Description:\n" + abilities_[abilityIdx].desc();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID))->SetLabel(abilityDescWrapper_->UpdateText(abilityDesc));

  wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnUnselectedFeatureSelected(wxCommandEvent& evt)
{
  int featIdx = evt.GetSelection();

  wxString featDesc = "Feature Description:\n" + todoFeatures_[featIdx].desc();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID))->SetLabel(featureDescWrapper_->UpdateText(featDesc));

  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID))->SetSelection(wxNOT_FOUND);
  wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnFinishedFeatureSelected(wxCommandEvent& evt)
{
  int featIdx = evt.GetSelection();

  wxString featDesc = "Feature Description:\n" + featureDescriptions_[featIdx];
  static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID))->SetLabel(featureDescWrapper_->UpdateText(featDesc));

  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID))->SetSelection(wxNOT_FOUND);
  wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::SelectFeatureButtonPress(wxCommandEvent& evt)
{
  unsigned int featIdx = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID))->GetSelection();
  int classIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection();
  int classLvl = charPtr_->getClassLevel(classIdx);
  int numChoices = todoFeatures_[featIdx].numChoices();
  wxListBox* featListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID));

  /* make sure we didn't fuck up somewhere */
  if (featIdx == wxNOT_FOUND)
  {
    wxMessageBox("Select a feature first!");
    return;
  }
  else if (featIdx > featListBox->GetCount() || featIdx > todoFeatures_.size())
  {
    wxMessageBox("The fuck is this? you giving me [" + wxString::Format(wxT("%d"), featIdx) + "] as a selection with only [" +
      wxString::Format(wxT("%d"), featListBox->GetCount()) + "] or [" +
      wxString::Format(wxT("%d"), todoFeatures_.size()) + "] items in the list?!");
    return;
  }

  std::vector<Pathfinder::ClassChoice> choiceVec = Pathfinder::PFTable::get_class(classIdx).getChoiceVec(todoFeatures_[featIdx].categoryId());

  this->MakeFeatureChoice(classIdx, classLvl, numChoices, choiceVec, todoFeatures_[featIdx].name());
  /* remove this item from the list of features */
  todoFeatures_.erase(todoFeatures_.begin() + featIdx);

  featListBox->Delete(featIdx);

  /* if there aren't any features left to choose, turn off the button for now */
  if (todoFeatures_.empty())
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(CLASS_FEATURE_BUTTON_ID))->Disable();
  }

  featListBox->GetParent()->Layout();
  if (grantedSpells_ || grantedFeats_)
  {
    evt.Skip();
  }
}

void ClassPage::MakeFeatureChoice(int classIdx, int classLvl, int numChoices, std::vector<Pathfinder::ClassChoice> &choiceVec, wxString catName)
{
  wxListBox* featListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID));
  wxListBox* abilityListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID));

  //Create the popup window to offer the selection
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
      wxString featName = catName + ": " + choiceVec[choiceIdx].name();
      if (choiceVec[choiceIdx].numSubsequentChoices() > 0)
      {
        std::vector<Pathfinder::ClassChoice> subsequentChoices = Pathfinder::PFTable::get_class(classIdx).getChoiceVec(choiceVec[choiceIdx].subsequentChoiceCategory());
        this->MakeFeatureChoice(classIdx, classLvl, choiceVec[choiceIdx].numSubsequentChoices(), subsequentChoices, featName);
      }
      else
      {
        featureNames_.push_back(featName);
        featureDescriptions_.push_back(choiceVec[choiceIdx].desc());
        featListBox->AppendString(featName);
      }

      //Record this choice
      choicesMade_.insert(choiceVec[choiceIdx].id());

      //Record the choice within the character class
      charPtr_->makeClassChoice(choiceVec[choiceIdx].id());
      if (choiceVec[choiceIdx].featId() >= 0) {
        grantedFeats_ = true;
        charPtr_->selectFeat(choiceVec[choiceIdx].featId());
      }
      //Go add any class abilities that had this choice as a prerequisite
      for (int classLevel = 0; classLevel <= charPtr_->getClassLevel(classIdx); classLevel++) {
        std::vector<Pathfinder::ClassAbility> abilityVec = Pathfinder::PFTable::get_class(classIdx).getAbilityVec(classLevel);
        for (unsigned int abilityIdx = 0; abilityIdx < abilityVec.size(); abilityIdx++) {
          if (abilityVec[abilityIdx].choicePrereqId() == choiceVec[choiceIdx].id()) {
            abilities_.push_back(abilityVec[abilityIdx]);
            charPtr_->addClassAbility(abilityVec[abilityIdx].id());
            abilityListBox->AppendString(abilityVec[abilityIdx].name());
            if (abilityVec[abilityIdx].spellId() >= 0) {
              charPtr_->learnSpell(abilityVec[abilityIdx].spellId());
              grantedSpells_ = true;
            }
            else if (abilityVec[abilityIdx].featId() >= 0) {
              charPtr_->selectFeat(abilityVec[abilityIdx].featId());
              grantedFeats_ = true;
            }
          }
        }
      }
      if (choicesMade_.count(choiceVec[choiceIdx].id()) == choiceVec[choiceIdx].maxNumSelections())
      {
        choiceVec.erase(choiceVec.begin() + choiceIdx);
        choiceStrings.RemoveAt(choiceIdx);
      }
      numChoicesMade++;
    }
  }
}

void ClassPage::ResizeCallback(wxSizeEvent& evt)
{
  if (classDescWrapper_ != NULL)
  {
    int maxWidth = 0;
    wxStaticText* classDescBox = static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_DESCRIPTION_ID));
    classDescBox->GetSize(&maxWidth, NULL);
    classDescBox->SetLabel(classDescWrapper_->UpdateWidth(maxWidth));

    classDescBox = static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID));
    classDescBox->GetSize(&maxWidth, NULL);
    classDescBox->SetLabel(featureDescWrapper_->UpdateWidth(maxWidth));

    classDescBox = static_cast<wxStaticText*>(wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID));
    classDescBox->GetSize(&maxWidth, NULL);
    classDescBox->SetLabel(abilityDescWrapper_->UpdateWidth(maxWidth));
  }
  evt.Skip();
}