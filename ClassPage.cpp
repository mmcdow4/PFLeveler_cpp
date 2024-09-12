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
#include <wx/tooltip.h>

/* Pathfinder includes */
#include <pf_include/PFTable.h>
#include <pf_include/Character.h>
#include <pf_include/Class.h>

#define LANGUAGE_KNOWN_FOUND 1
#define LANGUAGE_AVAIL_FOUND 2
#define LANGUAGE_NOT_FOUND -1

ClassPage::ClassPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar), skillsLocked_(true), spellsLeft_(false), featsLeft_(false), grantedSpells_(false), grantedFeats_(false), skillsChanged_(false), languageChange_(false)
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
  wxTextCtrl* classDescription = new wxTextCtrl(this, CLASS_DESCRIPTION_ID, wxT("Class Description:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  classDescription->SetBackgroundColour(*wxWHITE);
  classDescription->Bind(wxEVT_SIZE, &ClassPage::ResizeCallback, this);
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
  wxListBox* todoFeatList = new wxListBox(this, CLASS_TODO_FEATURE_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB | wxLB_HSCROLL);
  wxListBox* selectedFeatList = new wxListBox(this, CLASS_SELECTED_FEATURE_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB | wxLB_HSCROLL);

  todoFeatList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnUnselectedFeatureSelected, this);
  selectedFeatList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnFinishedFeatureSelected, this);

  todoFeatList->Disable();
  selectedFeatList->Disable();
  vbox_todo_features->Add(todoFeatList, 1, wxEXPAND | wxRIGHT, 10);
  vbox_selected_features->Add(selectedFeatList, 1, wxEXPAND | wxLEFT, 10);

  hbox_features->Add(vbox_todo_features, 1, wxEXPAND | wxRIGHT, 10);
  hbox_features->Add(vbox_selected_features, 1, wxEXPAND | wxRIGHT, 10);
  vbox1->Add(hbox_features, 3, wxEXPAND | wxRIGHT | wxLEFT, 10);

  wxTextCtrl* feature_description = new wxTextCtrl(this, CLASS_FEATURE_DESCRIPTION_ID, wxT("Feature Description:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  feature_description->SetBackgroundColour(*wxWHITE);
  feature_description->Bind(wxEVT_SIZE, &ClassPage::ResizeCallback, this);
  vbox1->Add(feature_description, 1, wxALL | wxEXPAND, 10);

  wxButton* addFeatureBtn = new wxButton(this, CLASS_FEATURE_BUTTON_ID, wxT("Choose Selected Feature"));
  addFeatureBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &ClassPage::SelectFeatureButtonPress, this);
  addFeatureBtn->Disable();
  addFeatureBtn->Hide();

  vbox1->Add(addFeatureBtn, 0, wxRIGHT, 10);
  /* Now fill out the right side of the page */
  /* label the class abilities list */
  wxStaticText* abilitiesLabel = new wxStaticText(this, CLASS_ABILITIES_LABEL_ID, wxT("Class Abilities:"));
  vbox2->Add(abilitiesLabel, 0, wxEXPAND | wxUP | wxDOWN, 10);

  wxListBox* abilitiesList = new wxListBox(this, CLASS_ABILITIES_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB | wxLB_HSCROLL);
  abilitiesList->Disable();
  vbox2->Add(abilitiesList, 2, wxEXPAND | wxUP | wxDOWN, 10);

  abilitiesList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &ClassPage::OnAbilitySelected, this);

  wxTextCtrl* abilitiesDescription = new wxTextCtrl(this, CLASS_ABILITIES_DESCRIPTION_ID, wxT("Ability Description:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  abilitiesDescription->SetBackgroundColour(*wxWHITE);
  int windowWidth = GetClientSize().GetWidth();
  int minWidth = (windowWidth / 2) - 20;
  abilitiesDescription->SetMinSize(wxSize(minWidth, -1));
  abilitiesDescription->Bind(wxEVT_SIZE, &ClassPage::ResizeCallback, this);
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
  skillsChanged_ = false;

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
  //this->toolTip_ = NULL;

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

  if (charPtr_->getCharacterLevel() > 0)
  {
    /* Imported character has some class levels already, import that data */

    /* level up button */
    wxWindow::FindWindowById(CLASS_LEVELUP_BUTTON_ID)->Show();
    wxWindow::FindWindowById(CLASS_LEVELUP_BUTTON_ID)->Enable();

    /* favored class button */
    wxWindow::FindWindowById(CLASS_FAVORED_CLASS_BUTTON_ID)->Hide();
    wxWindow::FindWindowById(CLASS_FAVORED_CLASS_BUTTON_ID)->Disable();

    std::vector<int> imported_abilities = charPtr_->getClassAbilities();
    std::vector<int> imported_features = charPtr_->getClassFeatures();
    std::vector<int> imported_choices = charPtr_->getClassChoices();
    /* Go through the imported character's abilities and add them */
    for (std::vector<int>::iterator iter = imported_abilities.begin(); iter != imported_abilities.end(); ++iter)
    {
      Pathfinder::ClassAbility ability = Pathfinder::PFTable::get_class_ability(*iter);
      abilities_.push_back(ability);
      static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID))->AppendString(ability.name());
    }
    
    /* Go through the imported character's class features and add them */
    wxListBox* doneFeatList = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID));
    for (std::vector<int>::iterator iter = imported_features.begin(); iter != imported_features.end(); ++iter)
    {
      Pathfinder::ClassFeature feature = Pathfinder::PFTable::get_class_feature(*iter);
      
      featureNames_.push_back(feature.name());
      featureDescriptions_.push_back(feature.desc());
      doneFeatList->AppendString(feature.name());
    }

    /* Go through the imported character's class choices and add them */
    for (std::vector<int>::iterator iter = imported_choices.begin(); iter != imported_choices.end(); ++iter)
    {
      Pathfinder::ClassChoice choice = Pathfinder::PFTable::get_class_choice(*iter);
      
      wxString featName = choice.name();
      
      featureNames_.push_back(featName);
      featureDescriptions_.push_back(choice.desc());
      doneFeatList->AppendString(featName);
      
      //Record this choice
      choicesMade_.insert(choice.id());
    }
  }

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

bool ClassPage::IsReadyForLevel(int classIdx, std::string &errMsg)
{
  if (classIdx == wxNOT_FOUND)
  {
    errMsg = "First select a class from the dropdown menu.";
    return false;
  }
  else if (charPtr_->race().id() == -1)// if Race hasn't been set yet
  {
    errMsg ="Race selection is not finalized yet.";
    return false;
  }
  else if (!charPtr_->abilityScoresSet()) // or if the ability scores haven't been set yet
  {
    errMsg ="Ability scores are not finalized yet.";
    return false;
  }
  else if (classIdx == -10 && charPtr_->remainingBonusLanguages() > 0)
  {
    /* Only care about this for export, hence the classIdx == -10 */
    errMsg = "You must finish learning bonus starting languages.";
    return false;
  }
  else if (charPtr_->geAlignment() == Pathfinder::GE_ANY)
  {
    errMsg = "You must finish inputting your character's biographical information.";
    return false;
  }
  else if (charPtr_->numFavoredClassLeft() > 0)
  {
    errMsg ="Favored class(es) are not finalized yet.";
    return false;
  }
  else if (!skillsLocked_)
  {
    errMsg ="Skill points have not been assigned yet.";
    return false;
  }
  else if (spellsLeft_)
  {
    errMsg ="You have not finished learning spells yet.";
    return false;
  }
  else if (featsLeft_)
  {
    errMsg ="You have not finished selecting feats yet.";
    return false;
  }
  else if (!todoFeatures_.empty())
  {
    errMsg ="You have not finished making choices for your class features yet.";
    return false;
  }

  return true;
}
void ClassPage::OnLevelAdded(wxCommandEvent& evt)
{

  int classIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection();

  /* verify that the necessary other information has been decided */
  std::string errMsg = "";
  if (!this->IsReadyForLevel(classIdx, errMsg))
  {
    wxMessageBox("Unable to level up yet: " + errMsg);
    return;
  }

  if (!charPtr_->classAlignmentMatch(classIdx))
  {
    wxMessageBox(wxString::Format(wxT("You cannot add a %s level due to alignment requirements"), Pathfinder::CLASS_NAMES[classIdx]));
    return;
  }

  /* increment the level */
  int classLevel = charPtr_->incrementClassLevel(classIdx);

  if (charPtr_->getCharacterLevel() == 1)
  {
    /* First level added, roll for starting wealth */
    charPtr_->rollWealth(classIdx);
  }

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
      charPtr_->addClassFeature(featIter->id());
      doneFeatList->AppendString(featIter->name());
      int langIdx = -1;
      int langCode = ParseNameForLanguage(featIter->name(), &langIdx);
      if (langCode == LANGUAGE_KNOWN_FOUND)
      {
        charPtr_->grantLanguage(langIdx);
        languageChange_ = true;
      }
      else if (langCode == LANGUAGE_AVAIL_FOUND)
      {
        charPtr_->addBonusLanguage(langIdx);
        languageChange_ = true;
      }
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
      int skillIdx = ParseNameForSkill(abilityIter->name());
      if (skillIdx > -1)
      {
        skillsChanged_ = true;
        charPtr_->addClassSkill(static_cast<Pathfinder::skillMarker>(skillIdx));
      }
      charPtr_->addClassAbility(abilityIter->id());
      static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID))->AppendString(abilityIter->name());
      if (abilityIter->spellId() >= 0) {
        charPtr_->learnSpell(classIdx, abilityIter->spellId());
        grantedSpells_ = true;
      }
      else if (abilityIter->featId() >= 0) {
        charPtr_->selectFeat(abilityIter->featId());
        grantedFeats_ = true;
      }
    }
  }

  /* Add skill points */
  int numPoints = std::max(1, Pathfinder::PFTable::get_class(classIdx).skillsPerLvl() + charPtr_->abilityModifier(Pathfinder::INTELLIGENCE) + charPtr_->race().bonusSkill());
  
  charPtr_->addSkillPoints(numPoints);
  skillsLocked_ = false;

  /* If this is a favored class, decide whether to add a bonus hitpoint or bonus skill rank */
  if (charPtr_->isFavoredClass(static_cast<Pathfinder::classMarker>(classIdx)))
  {
    /* Do favored class logic here */
    wxMessageDialog choiceWindow(this, wxString::Format(wxT("You have added a level to a favored class, select either +1 hit point (in addition to %d) or +1 skill rank (in addition to %d)."), hitPointsAdded, numPoints), "caption string", wxYES_NO | wxCENTRE | wxSTAY_ON_TOP);
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
  classText += ", starting wealth = " + std::to_string(chosenClass.startingWealthD6()) + "d6 x 10 gp";

  wxTextCtrl* classDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(CLASS_DESCRIPTION_ID));
  classDescBox->Clear();
  *classDescBox << classDescWrapper_->UpdateText(classText);
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());


  wxWindow::FindWindowById(CLASS_DESCRIPTION_ID)->GetParent()->Layout();
}

void ClassPage::OnAbilitySelected(wxCommandEvent& evt)
{
  int abilityIdx = evt.GetSelection();

  wxString abilityDesc = "Ability Description:\n" + abilities_[abilityIdx].desc();
  wxTextCtrl* abilityDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID));
  abilityDescBox->Clear();
  *abilityDescBox << abilityDescWrapper_->UpdateText(abilityDesc);

  abilityDescBox->GetParent()->Layout();
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
  wxTextCtrl* featureDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID));
  featureDescBox->Clear();
  *featureDescBox << featureDescWrapper_->UpdateText(featDesc);

  static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_TODO_FEATURE_LIST_ID))->SetSelection(wxNOT_FOUND);
  featureDescBox->GetParent()->Layout();
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

  if(this->MakeFeatureChoice(classIdx, classLvl, numChoices, choiceVec))
  {
    /* A choice was successfully made: remove this item from the list of features */
    todoFeatures_.erase(todoFeatures_.begin() + featIdx);

    featListBox->Delete(featIdx);
  }

  /* if there aren't any features left to choose, turn off the button for now */
  if (todoFeatures_.empty())
  {
    static_cast<wxButton*>(wxWindow::FindWindowById(CLASS_FEATURE_BUTTON_ID))->Disable();
  }

  featListBox->GetParent()->Layout();
  if (grantedSpells_ || grantedFeats_ || skillsChanged_)
  {
    evt.Skip();
  }
}

bool ClassPage::MakeFeatureChoice(int classIdx, int classLvl, int numChoices, std::vector<Pathfinder::ClassChoice> &choiceVec)
{
  std::vector<wxString> choiceDescriptions;
  bool status = true; // Indicate on return whether choice was successfully made or not
  wxListBox* featListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_SELECTED_FEATURE_LIST_ID));
  wxListBox* abilityListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(CLASS_ABILITIES_LIST_ID));

  //Create the popup window to offer the selection
  wxArrayString choiceStrings;
  for (auto choiceIter = choiceVec.begin(); choiceIter != choiceVec.end(); )
  {
    std::string dummy;
    if(choiceIter->lvlReq() <= classLvl && choicesMade_.count(choiceIter->id()) < choiceIter->maxNumSelections() &&
      !charPtr_->isFeatSelected(choiceIter->featId()) && charPtr_->checkFeatPrereqs(choiceIter->featId(), dummy) &&
      (choiceIter->prereqId() == -1 || charPtr_->checkForChoice(choiceIter->prereqId())))
    {
      /* If you meet the level requirement, haven't chosen this too many times already, and it doesn't grant a feat you've already taken, and either has no prereq or you made the prereq choice */
      choiceStrings.Add(choiceIter->name());
      if(choiceIter->featId() == -1)
      {
        choiceDescriptions.push_back(choiceIter->desc());
      }
      else
      {
        choiceDescriptions.push_back(Pathfinder::PFTable::get_feat(choiceIter->featId()).description());
      }
      ++choiceIter;
    }
    else
    {
      choiceIter = choiceVec.erase(choiceIter);
    }
  }

  int numChoicesMade = 0;

  /* Record all local choices */
  std::vector<int> currentChoicesMade;
  while (numChoicesMade < numChoices)
  {
    myDialog* choiceDialog = new myDialog(this,
      wxString::Format(wxT("Make selection %d out of %d"), numChoicesMade + 1, numChoices),
      "caption string", choiceStrings, NULL, wxOK | wxCANCEL, wxDefaultPosition, choiceDescriptions);
    int choiceReturn = choiceDialog->ShowModal();
    int choiceIdx = choiceDialog->GetSelection();
    if (choiceReturn == wxID_OK)
    {
      if (choiceVec[choiceIdx].numSubsequentChoices() > 0)
      {
        std::vector<Pathfinder::ClassChoice> subsequentChoices = Pathfinder::PFTable::get_class(classIdx).getChoiceVec(choiceVec[choiceIdx].subsequentChoiceCategory());
        status = this->MakeFeatureChoice(classIdx, classLvl, choiceVec[choiceIdx].numSubsequentChoices(), subsequentChoices);
        if (!status)
        {
          /* Subsequent choice failed, just back out and retry the previous level choice */
          continue;
        }
      }

      //Record this choice
      choicesMade_.insert(choiceVec[choiceIdx].id());
      currentChoicesMade.push_back(choiceVec[choiceIdx].id());

      //Record the choice within the character class
      charPtr_->makeClassChoice(choiceVec[choiceIdx].id());
      if (choicesMade_.count(choiceVec[choiceIdx].id()) == choiceVec[choiceIdx].maxNumSelections())
      {
        choiceVec.erase(choiceVec.begin() + choiceIdx);
        choiceStrings.RemoveAt(choiceIdx);
      }
      numChoicesMade++;
    }
    else
    {
      /* Choice cancelled */
      status = false;
      break;
    }
  }// While more choices left

  for (std::vector<int>::iterator iter = currentChoicesMade.begin(); iter != currentChoicesMade.end(); ++iter)
  {
    if(status)
    {
      // All choices were successfully made, finalize those decisions
      Pathfinder::ClassChoice choice = Pathfinder::PFTable::get_class_choice(*iter);
      int skillIdx = ParseNameForSkill(choice.name());
      int spellSchoolIdx = ParseNameForSpellSchool(choice.name());
      if (spellSchoolIdx > -1)
      {
        /* If choosing an opposition school then remove all spells of that school */
        charPtr_->loseSpellSchool(Pathfinder::WIZARD, static_cast<Pathfinder::SpellSchoolMarker>(spellSchoolIdx));
        grantedSpells_ = true;
      }
      else if (skillIdx > -1)
      {
        skillsChanged_ = true;
        charPtr_->addClassSkill(static_cast<Pathfinder::skillMarker>(skillIdx));
      }

      if (choice.featId() >= 0)
      {
        grantedFeats_ = true;
        charPtr_->selectFeat(choice.featId());
      }
      featureNames_.push_back(choice.name());
      featureDescriptions_.push_back(choice.desc());
      featListBox->AppendString(choice.name());
      //Go add any class abilities that had this choice as a prerequisite
      for (int classLevel = 0; classLevel <= charPtr_->getClassLevel(classIdx); classLevel++) {
        std::vector<Pathfinder::ClassAbility> abilityVec = Pathfinder::PFTable::get_class(classIdx).getAbilityVec(classLevel);
        for (unsigned int abilityIdx = 0; abilityIdx < abilityVec.size(); abilityIdx++) {
          if (abilityVec[abilityIdx].choicePrereqId() == choice.id()) {
            abilities_.push_back(abilityVec[abilityIdx]);
            int skillIdx = ParseNameForSkill(abilityVec[abilityIdx].name());
            if (skillIdx > -1)
            {
              skillsChanged_ = true;
              charPtr_->addClassSkill(static_cast<Pathfinder::skillMarker>(skillIdx));
            }
            charPtr_->addClassAbility(abilityVec[abilityIdx].id());
            abilityListBox->AppendString(abilityVec[abilityIdx].name());
            if (abilityVec[abilityIdx].spellId() >= 0) {
              charPtr_->learnSpell(classIdx, abilityVec[abilityIdx].spellId());
              grantedSpells_ = true;
            }
            else if (abilityVec[abilityIdx].featId() >= 0) {
              charPtr_->selectFeat(abilityVec[abilityIdx].featId());
              grantedFeats_ = true;
            }
          }//If this choice was a prereq for this ability
        }//For each ability
      }//For each class level
    }//If the choice was successful
    else
    {
      /* Unmake all previous choices */
      choicesMade_.erase(*iter);
      charPtr_->removeClassChoice(*iter);
    }
  }// For each choice in the process of being made

  return status;
}

void ClassPage::ResizeCallback(wxSizeEvent& evt)
{
  if (classDescWrapper_ != NULL)
  {
    //int windowWidth = GetClientSize().GetWidth();
    //int minWidth = (windowWidth / 2) - 20;
    //int maxWidth = (windowWidth / 2) - 25;
    int maxWidth = 0;
    wxTextCtrl* textBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(CLASS_DESCRIPTION_ID));
    //textBox->SetMinSize(wxSize(minWidth, -1));
    textBox->GetSize(&maxWidth, NULL);
    textBox->Clear();
    *textBox << classDescWrapper_->UpdateWidth(maxWidth-20);

    textBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(CLASS_FEATURE_DESCRIPTION_ID));
    //textBox->SetMinSize(wxSize(minWidth, -1));
    textBox->GetSize(&maxWidth, NULL);
    textBox->Clear();
    *textBox << featureDescWrapper_->UpdateWidth(maxWidth-20);

    textBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(CLASS_ABILITIES_DESCRIPTION_ID));
    //textBox->SetMinSize(wxSize(minWidth, -1));
    textBox->GetSize(&maxWidth, NULL);
    textBox->Clear();
    *textBox << abilityDescWrapper_->UpdateWidth(maxWidth-20);
  }
  evt.Skip();
}

int ClassPage::ParseNameForLanguage(std::string name, int* languageIdx)
{
  bool knownLanguage = (name.find("Known Language: ") != std::string::npos);
  bool availLanguage = (name.find("Bonus Language: ") != std::string::npos);
  if (knownLanguage || availLanguage)
  {
    std::string temp_string = name.substr(16);
    int lang = 0;
    for (; lang < Pathfinder::PFTable::get_num_languages(); lang++)
    {
      if (temp_string.compare(std::string(Pathfinder::PFTable::get_language(lang))) == 0)
      {
        break;
      }
    }
    *languageIdx = lang;
    if (knownLanguage)
    {
      return LANGUAGE_KNOWN_FOUND;
    }
    else if (availLanguage)
    {
      return LANGUAGE_AVAIL_FOUND;
    }
  }
  return LANGUAGE_NOT_FOUND;
}

int ClassPage::ParseNameForSkill(std::string name)
{
  if (name.find("Class Skill: ") != std::string::npos)
  {
    std::string temp_string = name.substr(13);
    int skill = 0;
    for (; skill < Pathfinder::NUMBER_SKILLS; skill++)
    {
      if (temp_string.compare(std::string(Pathfinder::skillStrings[skill])) == 0)
      {
        break;
      }
    }
    return skill;
  }
  return -1;
}

int ClassPage::ParseNameForSpellSchool(std::string name)
{
  if (name.find("Opposition School:") != std::string::npos)
  {
    /* Really ugly but its a one-off : if choosing an opposition school then remove all spells of that school */
    std::string temp_string = name.substr(19);
    int spellSchool = 0;
    for (; spellSchool < Pathfinder::NUMBER_SPELL_SCHOOLS; spellSchool++)
    {
      if (temp_string.compare(std::string(Pathfinder::SPELL_SCHOOL_NAMES[spellSchool])) == 0)
      {
        break;
      }
    }
    return spellSchool;
  }
  return -1;
}