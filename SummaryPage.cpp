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
  wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL); /* class dropdown and label */
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

  /* Languages */
  wxBoxSizer* hbox_languages = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* languagesLabel = new wxStaticText(this, SUMMARY_LANGUAGE_LABEL_ID, wxT("Languages:"));
  hbox_languages->Add(languagesLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_languages);

  /* Favored Class(es) */
  wxBoxSizer* hbox_favClass = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* favClassLabel = new wxStaticText(this, SUMMARY_FAV_CLASS_LABEL_ID, wxT("Favored Class:"));
  hbox_favClass->Add(favClassLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_favClass);

  /* Class label summary*/
  wxBoxSizer* hbox_class_and_button = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* classLevelLabel = new wxStaticText(this, wxID_ANY, wxT("Class Levels:"));
  hbox_class_and_button->Add(classLevelLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  /* Add a spacer to break it up*/
  hbox_class_and_button->AddStretchSpacer(1);
  /* Lock info button */
  wxButton* summaryBtn = new wxButton(this, SUMMARY_BUTTON_ID, wxT("Lock Info"));
  summaryBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SummaryPage::OnCharLocked, this);
  summaryBtn->Disable();
  summaryBtn->Hide();
  hbox_class_and_button->Add(summaryBtn, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_class_and_button, 1, wxEXPAND);

  /* Class Levels */
  wxString* dummyStr = NULL;
  wxListBox* classLevelList = new wxListBox(this, SUMMARY_CLASS_LEVEL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  vbox1->Add(classLevelList, 10, wxEXPAND, 0);

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

  /* Class Selection Drop Down */
  wxStaticText* classDropdownLabel = new wxStaticText(this, wxID_ANY, wxT("Class:"));
  hbox2->Add(classDropdownLabel, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 5);
  wxChoice* classDropDown = new wxChoice(this, SUMMARY_CLASS_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);
  classDropDown->Bind(wxEVT_CHOICE, &SummaryPage::OnClassSelected, this);
  hbox2->Add(classDropDown, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 5);
  vbox4->Add(hbox2, 0, wxALIGN_CENTER, 5);
  /* Spell Slot List */
  wxStaticText* spellSlotLabel = new wxStaticText(this, wxID_ANY, wxT("Spell Slots:"));
  vbox4->Add(spellSlotLabel, 0, wxBOTTOM, 5);
  wxListCtrl* spellSlotList = new wxListCtrl(this, SUMMARY_SPELL_SLOT_LIST_ID, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
  wxClientDC dc(spellSlotList);
  dc.SetFont(spellSlotList->GetFont());
  wxCoord textWidth;
  dc.GetTextExtent("Spells Known", &textWidth, nullptr);
  spellSlotList->InsertColumn(0, "Spells Known", wxLIST_FORMAT_CENTER, textWidth);
  dc.GetTextExtent("Spell Save DC", &textWidth, nullptr);
  spellSlotList->InsertColumn(1, "Spell Save DC", wxLIST_FORMAT_CENTER, textWidth);
  dc.GetTextExtent("Level", &textWidth, nullptr);
  spellSlotList->InsertColumn(2, "Level", wxLIST_FORMAT_CENTER, textWidth);
  dc.GetTextExtent("Spells Per Day", &textWidth, nullptr);
  spellSlotList->InsertColumn(3, "Spells Per Day", wxLIST_FORMAT_CENTER, textWidth);
  dc.GetTextExtent("Bonus Spells", &textWidth, nullptr);
  spellSlotList->InsertColumn(4, "Bonus Spells", wxLIST_FORMAT_CENTER, textWidth);
  vbox4->Add(spellSlotList, 1, wxEXPAND, 0);

  /* Spell List */
  wxStaticText* spellListLabel = new wxStaticText(this, wxID_ANY, wxT("Spells Known:"));
  vbox4->Add(spellListLabel, 0, wxBOTTOM, 5);
  wxListBox* spellList = new wxListBox(this, SUMMARY_SPELL_LIST_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_SORT | wxLB_NEEDED_SB);
  vbox4->Add(spellList, 3, wxEXPAND, 0);

  hbox1->Add(vbox4, 1, wxEXPAND, 0);

  this->SetSizer(hbox1);
}

void SummaryPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  knownSpellsTable_.clear();
  classList_.clear();

  if (languageTextWrapper_ != NULL)
  {
    delete languageTextWrapper_;
  }
  int maxWidth = 0;
  wxWindow::FindWindowById(SUMMARY_CLASS_LEVEL_LIST_ID)->GetSize(&maxWidth, NULL);
  languageTextWrapper_ = new HardBreakWrapper(wxWindow::FindWindowById(SUMMARY_LANGUAGE_LABEL_ID), "", maxWidth);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_LANGUAGE_LABEL_ID))->SetLabel(languageTextWrapper_->UpdateText("Languages: "));

  if (charPtr_->getCharacterLevel() > 0)
  {
    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_NAME_LABEL_ID))->SetLabel(wxT("Character Name: " + charPtr_->name()));
    wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_NAME_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_PLAYER_LABEL_ID))->SetLabel(wxT("Player Name: " + charPtr_->player()));
    wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_PLAYER_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_ALIGNMENT_LABEL_ID))->SetLabel(wxT("Alignment: " + charPtr_->alignment()));
    wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_ALIGNMENT_DROPDOWN_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_HEIGHT_LABEL_ID))->SetLabel(wxT("Height: " + charPtr_->height()));
    wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_HEIGHT_INPUT_ID)->Hide();

    if((static_cast<int>(charPtr_->weight()) - charPtr_->weight()) < DBL_EPSILON)
    {
      static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_WEIGHT_LABEL_ID))->SetLabel(wxT("Weight: " + wxString::Format("%d lb", static_cast<int>(charPtr_->weight()))));
    }
    else
    {
      static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_WEIGHT_LABEL_ID))->SetLabel(wxT("Weight: " + wxString::Format("%.2f lb", charPtr_->weight())));
    }

    wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_WEIGHT_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_HAIR_LABEL_ID))->SetLabel(wxT("Hair: " + charPtr_->hair()));
    wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_HAIR_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_EYES_LABEL_ID))->SetLabel(wxT("Eyes: " + charPtr_->eyes()));
    wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_EYES_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_DEITY_LABEL_ID))->SetLabel(wxT("Deity: " + charPtr_->deity()));
    wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_DEITY_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_HOMELAND_LABEL_ID))->SetLabel(wxT("Homeland: " + charPtr_->homeland()));
    wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_HOMELAND_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_GENDER_LABEL_ID))->SetLabel(wxT("Gender: " + charPtr_->gender()));
    wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_GENDER_INPUT_ID)->Hide();

    static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_AGE_LABEL_ID))->SetLabel(wxT("Age: " + charPtr_->age()));
    wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID)->Hide();


    wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Disable();
    wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Hide();

    this->PopulateSkillData();
    this->PopulateSpellData();
    this->PopulateAbilityScoreData();
    this->PopulateFavoredClassData();
    this->PopulateRaceData();
  }
  else
  {
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

    static_cast<wxChoice*>(wxWindow::FindWindowById(SUMMARY_CLASS_DROPDOWN_ID))->Clear();
  }
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
  wxString age = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(SUMMARY_AGE_INPUT_ID))->GetValue();

  /* Verify that every box has been filled */
  if (name.empty() || playerName.empty() || alignmentIdx == wxNOT_FOUND || height.empty() || weight.empty() ||
    hair.empty() || eyes.empty() || deity.empty() || homeland.empty() || gender.empty() || age.empty())
  {
    wxMessageBox("Finish filling out the character summary first.");
    return;
  }
  double weight_val = 0.0;
  try
  {
    weight_val = std::stod(std::string(weight.mb_str()));
  }
  catch (const std::invalid_argument& e)
  {
    wxMessageBox("Weight must only be a numerical value (in pounds)");
    return;
  }

  /* Convert the non-string items */
  Pathfinder::GE_Alignment ge = static_cast<Pathfinder::GE_Alignment>(alignmentIdx % 3);
  Pathfinder::LC_Alignment lc = static_cast<Pathfinder::LC_Alignment>(floor(alignmentIdx / 3));

  /* Save the values to the character class */
  charPtr_->name(std::string(name.mb_str()));
  charPtr_->player(std::string(playerName.mb_str()));
  charPtr_->setAlignment(ge, lc);
  charPtr_->height(std::string(height.mb_str()));
  charPtr_->weight(weight_val);
  charPtr_->hair(std::string(hair.mb_str()));
  charPtr_->eyes(std::string(eyes.mb_str()));
  charPtr_->deity(std::string(deity.mb_str()));
  charPtr_->homeland(std::string(homeland.mb_str()));
  charPtr_->gender(std::string(gender.mb_str()));
  charPtr_->age(std::string(age.mb_str()));

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
  wxWindow::FindWindowById(SUMMARY_AGE_LABEL_ID)->SetLabel("Age: " + age);

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
  wxListBox* classAbilityList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_ABILITY_LIST_ID));

  classLevelList->Clear();
  for (int classId = 0; classId < Pathfinder::NUMBER_CLASSES; classId++)
  {
    int classLvl = charPtr_->getClassLevel(classId);
    if (classLvl > 0)
    {
      classLevelList->AppendString(Pathfinder::PFTable::get_class(classId).name() + " : Level " + std::to_string(classLvl));
    }
  }

  classAbilityList->Clear();
  std::vector<int> abilityList = charPtr_->getClassAbilities();
  for (std::vector<int>::iterator abilityIter = abilityList.begin(); abilityIter != abilityList.end(); ++abilityIter)
  {
    Pathfinder::ClassAbility ability = Pathfinder::PFTable::get_class_ability(*abilityIter);
    if (ability.spellId() == -1 && ability.featId() == -1 && ability.name().find("Class Skill") == std::string::npos)
    {
      classAbilityList->AppendString(ability.name());
    }
  }
}

void SummaryPage::PopulateRaceData(void)
{
  wxWindow::FindWindowById(SUMMARY_RACE_LABEL_ID)->SetLabel("Race: " + charPtr_->race().raceName());
  wxWindow::FindWindowById(SUMMARY_SIZE_LABEL_ID)->SetLabel("Size: " + std::string(Pathfinder::CHARACTER_SIZE_NAMES[charPtr_->race().charSize()]));
  wxWindow::FindWindowById(SUMMARY_SPEED_LABEL_ID)->SetLabel("Speed: " + std::to_string(charPtr_->race().speed()));

  std::set<int> languageList = charPtr_->getKnownLanguages();

  std::string languageString = "Languages: " + charPtr_->getKnownLanguageString();

  wxWindow::FindWindowById(SUMMARY_LANGUAGE_LABEL_ID)->SetLabel(languageTextWrapper_->UpdateText(languageString));
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

  abilityScoreList->AppendString("Hit Points : " + std::to_string(charPtr_->hitpoints()));
  abilityScoreList->AppendString("Armor Class : " + std::to_string(charPtr_->getArmorClass()));
  abilityScoreList->AppendString("Touch AC : " + std::to_string(charPtr_->getTouchArmorClass()));
  abilityScoreList->AppendString("Flat-Footed AC : " + std::to_string(charPtr_->getFlatFootedArmorClass()));
  abilityScoreList->AppendString("Initiative Mod : " + std::to_string(charPtr_->abilityModifier(Pathfinder::DEXTERITY)));
  abilityScoreList->AppendString("Fortitude Save : " + std::to_string(charPtr_->getFortitudeSave()));
  abilityScoreList->AppendString("Reflex Save : " + std::to_string(charPtr_->getReflexSave()));
  abilityScoreList->AppendString("Will Save : " + std::to_string(charPtr_->getWillSave()));
  abilityScoreList->AppendString("Base Attack Bonus : " + std::to_string(charPtr_->getBaseAttackBonus()));
  abilityScoreList->AppendString("CMB : " + std::to_string(charPtr_->getCombatManeuverBonus()));
  abilityScoreList->AppendString("CMD : " + std::to_string(charPtr_->getCombatManeuverDefense()));
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
  std::vector<Pathfinder::Feat> feat_vec = charPtr_->getSelectedFeats();

  for (std::vector<Pathfinder::Feat>::iterator featIter = feat_vec.begin(); featIter != feat_vec.end(); ++featIter)
  {
    featList->AppendString(featIter->fullName());
  }
}

void SummaryPage::OnClassSelected(wxCommandEvent& evt)
{
  this->PopulateSpellData();
}

void SummaryPage::PopulateSpellData(void)
{
  wxListBox* spellList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_SPELL_LIST_ID));
  wxListCtrl* spellSlotList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(SUMMARY_SPELL_SLOT_LIST_ID));
  wxChoice* classDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(SUMMARY_CLASS_DROPDOWN_ID));
  spellList->Clear();
  spellSlotList->DeleteAllItems();
  knownSpellsTable_.clear();
  int classChoice = classDropDown->GetSelection();

  for(int classId = 0; classId < Pathfinder::NUMBER_CLASSES; classId++)
  {
    std::vector<int> spell_vec = charPtr_->getKnownSpells(classId);
    if (!spell_vec.empty())
    {
      if(std::find(classList_.begin(), classList_.end(), classId) == classList_.end())
      {
        /* First time this class has spells, add it to the list */
        classList_.push_back(classId);
        classDropDown->AppendString(wxString(Pathfinder::CLASS_NAMES[classId]));
      }
      else if (classChoice != wxNOT_FOUND && classList_[classChoice] == classId)
      {
        /* We have this class selected - update the spell list */
        int classLevel = charPtr_->getClassLevel(classId);
        for (int spellLevel = 0; spellLevel <= 9; spellLevel++)
        {
          int slotCount = charPtr_->getNumSpellSlots(classId, spellLevel);
          int spellsKnown = Pathfinder::PFTable::get_class(classId).levelItem(classLevel, static_cast<Pathfinder::lvlUpMarker>(static_cast<int>(Pathfinder::SPELLS_KNOWN_0) + spellLevel));
          int spellDC = 10 + spellLevel + charPtr_->abilityModifier(Pathfinder::PFTable::get_class(classId).casterAbility());
          int bonusSlots = charPtr_->getNumBonusSpellSlots(classId, spellLevel);
          if (slotCount + bonusSlots > 0 && slotCount >= 0)
          {
            wxListItem item;
            item.SetId(spellSlotList->GetItemCount());
            item.SetColumn(0);
            item.SetText((spellsKnown > 0 ? std::to_string(spellsKnown) : "-"));
            spellSlotList->InsertItem(item);

            item.SetColumn(1);
            item.SetText(std::to_string(spellDC));
            spellSlotList->SetItem(item);

            item.SetColumn(2);
            item.SetText(std::to_string(spellLevel));
            spellSlotList->SetItem(item);

            item.SetColumn(3);
            item.SetText(std::to_string(slotCount));
            spellSlotList->SetItem(item);

            item.SetColumn(4);
            item.SetText((bonusSlots > 0 ? std::to_string(bonusSlots) : "-"));
            spellSlotList->SetItem(item);
          }
        }

        for (std::vector<int>::iterator spellIter = spell_vec.begin(); spellIter != spell_vec.end(); ++spellIter)
        {
          wxString spellName = Pathfinder::PFTable::get_spell(*spellIter).name(classId);
          spellList->AppendString(spellName);
          knownSpellsTable_.emplace(spellName, *spellIter);
        }
      }
    }
  }
}