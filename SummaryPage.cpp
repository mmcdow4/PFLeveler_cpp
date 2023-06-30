/* local includes*/
#include "SummaryPage.h"

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


SummaryPage::SummaryPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */
  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* character summary and class levels */
  wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL); /* Ability scores and skill bonuses */
  wxBoxSizer* vbox3 = new wxBoxSizer(wxVERTICAL); /* feats and abilities */
  wxBoxSizer* vbox4 = new wxBoxSizer(wxVERTICAL); /* spell slots and spells known */

  int summarySpacing = 1;
  /* Character Name */
  wxBoxSizer* hbox_char_name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* charNameLabel = new wxStaticText(this, SUMMARY_NAME_LABEL_ID, wxT("Character Name:"));
  hbox_char_name->Add(charNameLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* charNameTxt = new wxTextCtrl(this, SUMMARY_NAME_INPUT_ID);
  charNameTxt->Disable();
  charNameTxt->Hide();
  hbox_char_name->Add(charNameTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_char_name);

  /* Player Name */
  wxBoxSizer* hbox_player_name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* playerNameLabel = new wxStaticText(this, SUMMARY_PLAYER_LABEL_ID, wxT("Player Name:"));
  hbox_player_name->Add(playerNameLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* playerNameTxt = new wxTextCtrl(this, SUMMARY_PLAYER_INPUT_ID);
  playerNameTxt->Disable();
  playerNameTxt->Hide();
  hbox_player_name->Add(playerNameTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_player_name);

  /* Alignment */
  wxBoxSizer* hbox_alignment = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* alignmentLabel = new wxStaticText(this, SUMMARY_ALIGNMENT_LABEL_ID, wxT("Alignment:"));
  hbox_alignment->Add(alignmentLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxArrayString alignmentStrings;
  alignmentStrings.Add("Lawful Good");
  alignmentStrings.Add("Lawful Neutral");
  alignmentStrings.Add("Lawful Evil");
  alignmentStrings.Add("Neutral Good");
  alignmentStrings.Add("True Neutral");
  alignmentStrings.Add("Neutral Evil");
  alignmentStrings.Add("Chaotic Good");
  alignmentStrings.Add("Chaotic Neutral");
  alignmentStrings.Add("Chaotic Evil");

  wxChoice* alignmentDropdown = new wxChoice(this, SUMMARY_ALIGNMENT_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, alignmentStrings);
  alignmentDropdown->Disable();
  alignmentDropdown->Hide();
  hbox_alignment->Add(alignmentDropdown, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_alignment);

  /* Height */
  wxBoxSizer* hbox_height = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* heightLabel = new wxStaticText(this, SUMMARY_HEIGHT_LABEL_ID, wxT("Height:"));
  hbox_height->Add(heightLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* heightTxt = new wxTextCtrl(this, SUMMARY_HEIGHT_INPUT_ID);
  heightTxt->Disable();
  heightTxt->Hide();
  hbox_height->Add(heightTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_height);

  /* Weight */
  wxBoxSizer* hbox_weight = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* weightLabel = new wxStaticText(this, SUMMARY_WEIGHT_LABEL_ID, wxT("Weight:"));
  hbox_weight->Add(weightLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* weightTxt = new wxTextCtrl(this, SUMMARY_WEIGHT_INPUT_ID);
  weightTxt->Disable();
  weightTxt->Hide();
  hbox_weight->Add(weightTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_weight);

  /* Hair color */
  wxBoxSizer* hbox_hair = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* hairLabel = new wxStaticText(this, SUMMARY_HAIR_LABEL_ID, wxT("Hair:"));
  hbox_hair->Add(hairLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* hairTxt = new wxTextCtrl(this, SUMMARY_HAIR_INPUT_ID);
  hairTxt->Disable();
  hairTxt->Hide();
  hbox_hair->Add(hairTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_hair);

  /* Eyes */
  wxBoxSizer* hbox_eyes = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* eyesLabel = new wxStaticText(this, SUMMARY_EYES_LABEL_ID, wxT("Eyes:"));
  hbox_eyes->Add(eyesLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* eyesTxt = new wxTextCtrl(this, SUMMARY_EYES_INPUT_ID);
  eyesTxt->Disable();
  eyesTxt->Hide();
  hbox_eyes->Add(eyesTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_eyes);

  /* Deity */
  wxBoxSizer* hbox_deity = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* deityLabel = new wxStaticText(this, SUMMARY_DEITY_LABEL_ID, wxT("Deity:"));
  hbox_deity->Add(deityLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* deityTxt = new wxTextCtrl(this, SUMMARY_DEITY_INPUT_ID);
  deityTxt->Disable();
  deityTxt->Hide();
  hbox_deity->Add(deityTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_deity);

  /* Homeland */
  wxBoxSizer* hbox_homeland = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* homelandLabel = new wxStaticText(this, SUMMARY_HOMELAND_LABEL_ID, wxT("Homeland:"));
  hbox_homeland->Add(homelandLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* homeTxt = new wxTextCtrl(this, SUMMARY_HOMELAND_INPUT_ID);
  homeTxt->Disable();
  homeTxt->Hide();
  hbox_homeland->Add(homeTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_homeland);

  /* Gender */
  wxBoxSizer* hbox_gender = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* genderLabel = new wxStaticText(this, SUMMARY_GENDER_LABEL_ID, wxT("Gender:"));
  hbox_gender->Add(genderLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* genderTxt = new wxTextCtrl(this, SUMMARY_GENDER_INPUT_ID);
  genderTxt->Disable();
  genderTxt->Hide();
  hbox_gender->Add(genderTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_gender);

  /* Age */
  wxBoxSizer* hbox_age = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* ageLabel = new wxStaticText(this, SUMMARY_AGE_LABEL_ID, wxT("Age:"));
  hbox_age->Add(ageLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* ageTxt = new wxTextCtrl(this, SUMMARY_AGE_INPUT_ID);
  ageTxt->Disable();
  ageTxt->Hide();
  hbox_age->Add(ageTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_age);

  /* Race */
  wxBoxSizer* hbox_race = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* raceLabel = new wxStaticText(this, SUMMARY_RACE_LABEL_ID, wxT("Race: "));
  hbox_race->Add(raceLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_race);

  /* Size */
  wxBoxSizer* hbox_size = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* sizeLabel = new wxStaticText(this, SUMMARY_SIZE_LABEL_ID, wxT("Size: "));
  hbox_size->Add(sizeLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_size);

  /* Speed */
  wxBoxSizer* hbox_speed = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* speedLabel = new wxStaticText(this, SUMMARY_SPEED_LABEL_ID, wxT("Speed: "));
  hbox_speed->Add(speedLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_speed);

  /* Favored Class(es) */
  wxBoxSizer* hbox_favClass = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* favClassLabel = new wxStaticText(this, SUMMARY_FAV_CLASS_LABEL_ID, wxT("Favored Class:"));
  hbox_favClass->Add(favClassLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_favClass);

  wxButton* summaryBtn = new wxButton(this, SUMMARY_BUTTON_ID, wxT("Lock Info"));
  summaryBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SummaryPage::OnCharLocked, this);
  summaryBtn->Disable();
  summaryBtn->Hide();
  vbox1->Add(summaryBtn, 0, wxALIGN_RIGHT, 0);

  /* Class Levels */
  wxString* dummyStr = NULL;
  wxListBox* classLevelList = new wxListBox(this, SUMMARY_CLASS_LEVEL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox1->Add(classLevelList, 1, wxEXPAND, 0);

  /* Summary Box Finished */
  hbox1->Add(vbox1, 1, wxEXPAND | wxRIGHT, 10);

  /* Ability Scores and etc. */
  wxStaticText* AbilityScoreLabel = new wxStaticText(this, wxID_ANY, wxT("Ability Scores:"));
  vbox2->Add(AbilityScoreLabel, 0, wxBOTTOM, 5);
  wxListBox* abilityScoreList = new wxListBox(this, SUMMARY_ABILITY_SCORES_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox2->Add(abilityScoreList, 1, wxEXPAND, 0);

  /* Skill List */
  wxStaticText* skillLabel = new wxStaticText(this, wxID_ANY, wxT("Skills:"));
  vbox2->Add(skillLabel, 0, wxBOTTOM, 5);
  wxListBox* skillList = new wxListBox(this, SUMMARY_SKILL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox2->Add(skillList, 1, wxEXPAND, 0);

  hbox1->Add(vbox2, 1, wxEXPAND | wxRIGHT, 10);

  /* Feat List */
  wxStaticText* featLabel = new wxStaticText(this, wxID_ANY, wxT("Your Feats:"));
  vbox3->Add(featLabel, 0, wxBOTTOM, 5);
  wxListBox* featList = new wxListBox(this, SUMMARY_FEAT_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox3->Add(featList, 1, wxEXPAND, 0);

  /* Special Abilities List */
  wxStaticText* abilityLabel = new wxStaticText(this, wxID_ANY, wxT("Special Abilities:"));
  vbox3->Add(abilityLabel, 0, wxBOTTOM, 5);
  wxListBox* abilityList = new wxListBox(this, SUMMARY_ABILITY_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox3->Add(abilityList, 1, wxEXPAND, 0);
  hbox1->Add(vbox3, 1, wxEXPAND | wxRIGHT, 10);

  /* Spell Slot List */
  wxStaticText* spellSlotLabel = new wxStaticText(this, wxID_ANY, wxT("Spell Slots:"));
  vbox4->Add(spellSlotLabel, 0, wxBOTTOM, 5);
  wxListBox* spellSlotList = new wxListBox(this, SUMMARY_SPELL_SLOT_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox4->Add(spellSlotList, 1, wxEXPAND, 0);

  wxStaticText* spellListLabel = new wxStaticText(this, wxID_ANY, wxT("Spells Known:"));
  vbox4->Add(spellListLabel, 0, wxBOTTOM, 5);
  wxListBox* spellList = new wxListBox(this, SUMMARY_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox4->Add(spellList, 3, wxEXPAND, 0);

  hbox1->Add(vbox4, 1, wxEXPAND, 0);

  this->SetSizer(hbox1);
}

void SummaryPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  /* Reset summary info text boxes, enable and show input boxes for these fields */
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_NAME_LABEL_ID))->SetLabel(wxT("Character Name:"));
  wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_PLAYER_LABEL_ID))->SetLabel(wxT("Player Name:"));
  wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_ALIGNMENT_LABEL_ID))->SetLabel(wxT("Alignment:"));
  wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_HEIGHT_LABEL_ID))->SetLabel(wxT("Height:"));
  wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_WEIGHT_LABEL_ID))->SetLabel(wxT("Weight:"));
  wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_HAIR_LABEL_ID))->SetLabel(wxT("Hair:"));
  wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_EYES_LABEL_ID))->SetLabel(wxT("Eyes:"));
  wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_DEITY_LABEL_ID))->SetLabel(wxT("Deity:"));
  wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_HOMELAND_LABEL_ID))->SetLabel(wxT("Homeland:"));
  wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_GENDER_LABEL_ID))->SetLabel(wxT("Gender:"));
  wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_AGE_LABEL_ID))->SetLabel(wxT("Age:"));
  wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID)->Show();

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_RACE_LABEL_ID))->SetLabel(wxT("Race:"));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_SIZE_LABEL_ID))->SetLabel(wxT("Size:"));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_SPEED_LABEL_ID))->SetLabel(wxT("Speed:"));

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_FAV_CLASS_LABEL_ID))->SetLabel(wxT("Favored Class:"));

  wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Show();

  static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_ABILITY_SCORES_LIST_ID))->Clear();
  static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_SKILL_LIST_ID))->Clear();
  static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_ABILITY_LIST_ID))->Clear();

  this->PopulateClassLevelData();
  this->PopulateFeatData();
  this->PopulateSpellData();

  wxWindow::FindWindowById(SUMMARY_NAME_LABEL_ID)->GetParent()->GetSizer()->Layout();
}

void SummaryPage::OnCharLocked(wxCommandEvent& evt)
{
  /* Pull the user input values from the various boxes */
  wxString name = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID))->GetValue();
  wxString playerName = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID))->GetValue();
  int alignmentIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID))->GetSelection();
  wxString height = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID))->GetValue();
  wxString weight = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID))->GetValue();
  wxString hair = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID))->GetValue();
  wxString eyes = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID))->GetValue();
  wxString deity = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID))->GetValue();
  wxString homeland = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID))->GetValue();
  wxString gender = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID))->GetValue();
  wxString ageStr = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID))->GetValue();

  /* Verify that every box has been filled */
  if (name.empty() || playerName.empty() || alignmentIdx == wxNOT_FOUND || height.empty() || weight.empty() ||
    hair.empty() || eyes.empty() || deity.empty() || homeland.empty() || gender.empty() || ageStr.empty())
  {
    wxMessageBox("Finish filling out the character summary first.");
    return;
  }

  /* Convert the non-string items */
  Pathfinder::GE_Alignment ge = static_cast<Pathfinder::GE_Alignment>(alignmentIdx % 3);
  Pathfinder::LC_Alignment lc = static_cast<Pathfinder::LC_Alignment>(floor(alignmentIdx / 3));
  long age;
  ageStr.ToLong(&age);

  /* Save the values to the character class */
  charPtr_->name(std::string(name.mb_str()));
  charPtr_->player(std::string(playerName.mb_str()));
  charPtr_->geAlignment(ge);
  charPtr_->lcAlignment(lc);
  charPtr_->height(std::string(height.mb_str()));
  charPtr_->weight(std::string(weight.mb_str()));
  charPtr_->hair(std::string(hair.mb_str()));
  charPtr_->hair(std::string(eyes.mb_str()));
  charPtr_->hair(std::string(deity.mb_str()));
  charPtr_->hair(std::string(homeland.mb_str()));
  charPtr_->hair(std::string(gender.mb_str()));
  charPtr_->age(age);

  /* Update the text boxes */
  wxWindow::FindWindowById(SUMMARY_NAME_LABEL_ID)->SetLabel("Character Name: " + name);
  wxWindow::FindWindowById(SUMMARY_PLAYER_LABEL_ID)->SetLabel("Player Name: " + playerName);
  wxWindow::FindWindowById(SUMMARY_ALIGNMENT_LABEL_ID)->SetLabel("Alignment: " + wxString(Pathfinder::alignmentStrings[alignmentIdx]));
  wxWindow::FindWindowById(SUMMARY_HEIGHT_LABEL_ID)->SetLabel("Height: " + height);
  wxWindow::FindWindowById(SUMMARY_WEIGHT_LABEL_ID)->SetLabel("Weight: " + weight);
  wxWindow::FindWindowById(SUMMARY_HAIR_LABEL_ID)->SetLabel("Hair: " + hair);
  wxWindow::FindWindowById(SUMMARY_EYES_LABEL_ID)->SetLabel("Eyes: " + eyes);
  wxWindow::FindWindowById(SUMMARY_DEITY_LABEL_ID)->SetLabel("Deity: " + deity);
  wxWindow::FindWindowById(SUMMARY_HOMELAND_LABEL_ID)->SetLabel("Homeland: " + homeland);
  wxWindow::FindWindowById(SUMMARY_GENDER_LABEL_ID)->SetLabel("Gender: " + gender);
  wxWindow::FindWindowById(SUMMARY_AGE_LABEL_ID)->SetLabel("Age: " + ageStr);

  /* disable and hide all of the input boxes */
  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID)->Disable();

  static_cast<wxChoice*>(wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID))->SetSelection(wxNOT_FOUND);
  wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID)->Disable();

  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID))->Clear();
  wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID)->Disable();

  wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Hide();
  wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Disable();

}

void SummaryPage::PopulateFavoredClassData(void)
{
  if (charPtr_->race().numFavoredClass() > 1)
  {
    wxWindow::FindWindowById(SUMMARY_FAV_CLASS_LABEL_ID)->SetLabel("Favored Classes: " + wxString(charPtr_->getFavoredClassList()));
  }
  else
  {
    wxWindow::FindWindowById(SUMMARY_FAV_CLASS_LABEL_ID)->SetLabel("Favored Class: " + wxString(charPtr_->getFavoredClassList()));
  }
}

void SummaryPage::PopulateClassLevelData(void)
{
  wxListBox* classLevelList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_CLASS_LEVEL_LIST_ID));

  classLevelList->Clear();
  for (int classId = 0; classId < Pathfinder::NUMBER_CLASSES; classId++)
  {
    int classLvl = charPtr_->getClassLevel(classId);
    if (classLvl > 0)
    {
      classLevelList->AppendString(Pathfinder::PFTable::get_class(classId).name() + " : Level " + std::to_string(classLvl));
    }
  }
}

void SummaryPage::PopulateRaceData(void)
{
  wxWindow::FindWindowById(SUMMARY_RACE_LABEL_ID)->SetLabel("Race: " + charPtr_->race().raceName());
  wxWindow::FindWindowById(SUMMARY_SIZE_LABEL_ID)->SetLabel("Size: " + charPtr_->race().charSize());
  wxWindow::FindWindowById(SUMMARY_SPEED_LABEL_ID)->SetLabel("Speed: " + std::to_string(charPtr_->race().speed()));

  if (charPtr_->race().numFavoredClass() > 1)
  {
    wxWindow::FindWindowById(SUMMARY_FAV_CLASS_LABEL_ID)->SetLabel("Favored Classes: ");
  }
}

void SummaryPage::PopulateAbilityScoreData(void)
{
  wxListBox* abilityScoreList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_ABILITY_SCORES_LIST_ID));

  abilityScoreList->Clear();
  for(int abilityScoreId = 0; abilityScoreId < Pathfinder::NUMBER_ABILITY_SCORES; abilityScoreId++)
  {
    Pathfinder::abilityScoreMarker ability = static_cast<Pathfinder::abilityScoreMarker>(abilityScoreId);
    abilityScoreList->AppendString(wxString(Pathfinder::ABILITY_SCORE_ABBREVIATIONS[abilityScoreId]) + " = " + std::to_string(charPtr_->getAbilityScore(ability)) + " (Mod. " + std::to_string(charPtr_->abilityModifier(ability)) + ")");
  }

  /* add armor class */
  /* add fortitude base save */
  /* add reflex base save */
  /* add will base save */
  /* add bab */
  /* add cmb */
  /* add cmd */
}

void SummaryPage::PopulateSkillData(void)
{
  wxListBox* skillList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_SKILL_LIST_ID));

  skillList->Clear();
  for (int skillId = 0; skillId < Pathfinder::NUMBER_SKILLS; skillId++)
  {
    skillList->AppendString(wxString(Pathfinder::skillStrings[skillId]) + " : " + std::to_string(charPtr_->effectiveSkillRank(static_cast<Pathfinder::skillMarker>(skillId))));
  }
}

void SummaryPage::PopulateFeatData(void)
{
  wxListBox* featList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_FEAT_LIST_ID));

  featList->Clear();
  std::vector<int> feat_vec = charPtr_->getSelectedFeats();

  for (std::vector<int>::iterator featIter = feat_vec.begin(); featIter != feat_vec.end(); ++featIter)
  {
    featList->AppendString(Pathfinder::PFTable::get_feat(*featIter).name());
  }
}

void SummaryPage::PopulateSpellData(void)
{
  wxListBox* spellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_SPELL_LIST_ID));

  spellList->Clear();
  std::vector<int> spell_vec = charPtr_->getKnownSpells();

  for (std::vector<int>::iterator spellIter = spell_vec.begin(); spellIter != spell_vec.end(); ++spellIter)
  {
    Pathfinder::Spell currSpell = Pathfinder::PFTable::get_spell(*spellIter);
    spellList->AppendString(wxString::Format(wxT("level %d spell: %s"), currSpell.SlaLvl(), currSpell.name()));
  }
}