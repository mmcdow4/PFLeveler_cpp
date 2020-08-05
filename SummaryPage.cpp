/* local includes*/
#include "cMain.h"

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


void cMain::InitializeSummaryPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Summary");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */
  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* character summary and todo list */
  wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL); /* spell list */
  wxBoxSizer* vbox3 = new wxBoxSizer(wxVERTICAL); /* feat list */
  wxBoxSizer* vbox4 = new wxBoxSizer(wxVERTICAL); /* skill list */

  int summarySpacing = 1;
  /* Character Name */
  wxBoxSizer* hbox_char_name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* charNameLabel = new wxStaticText(panel, SUMMARY_NAME_LABEL_ID, wxT("Character Name:"));
  hbox_char_name->Add(charNameLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* charNameTxt = new wxTextCtrl(panel, SUMMARY_NAME_INPUT_ID);
  charNameTxt->Disable();
  charNameTxt->Hide();
  hbox_char_name->Add(charNameTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_char_name);

  /* Player Name */
  wxBoxSizer* hbox_player_name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* playerNameLabel = new wxStaticText(panel, SUMMARY_PLAYER_LABEL_ID, wxT("Player Name:"));
  hbox_player_name->Add(playerNameLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* playerNameTxt = new wxTextCtrl(panel, SUMMARY_PLAYER_INPUT_ID);
  playerNameTxt->Disable();
  playerNameTxt->Hide();
  hbox_player_name->Add(playerNameTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_player_name);

  /* Alignment */
  wxBoxSizer* hbox_alignment = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* alignmentLabel = new wxStaticText(panel, SUMMARY_ALIGNMENT_LABEL_ID, wxT("Alignment:"));
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

  wxChoice* alignmentDropdown = new wxChoice(panel, SUMMARY_ALIGNMENT_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, alignmentStrings);
  alignmentDropdown->Disable();
  alignmentDropdown->Hide();
  hbox_alignment->Add(alignmentDropdown, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_alignment);

  /* Height */
  wxBoxSizer* hbox_height = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* heightLabel = new wxStaticText(panel, SUMMARY_HEIGHT_LABEL_ID, wxT("Height:"));
  hbox_height->Add(heightLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* heightTxt = new wxTextCtrl(panel, SUMMARY_HEIGHT_INPUT_ID);
  heightTxt->Disable();
  heightTxt->Hide();
  hbox_height->Add(heightTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_height);

  /* Weight */
  wxBoxSizer* hbox_weight = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* weightLabel = new wxStaticText(panel, SUMMARY_WEIGHT_LABEL_ID, wxT("Weight:"));
  hbox_weight->Add(weightLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* weightTxt = new wxTextCtrl(panel, SUMMARY_WEIGHT_INPUT_ID);
  weightTxt->Disable();
  weightTxt->Hide();
  hbox_weight->Add(weightTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_weight);

  /* Hair color */
  wxBoxSizer* hbox_hair = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* hairLabel = new wxStaticText(panel, SUMMARY_HAIR_LABEL_ID, wxT("Hair:"));
  hbox_hair->Add(hairLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* hairTxt = new wxTextCtrl(panel, SUMMARY_HAIR_INPUT_ID);
  hairTxt->Disable();
  hairTxt->Hide();
  hbox_hair->Add(hairTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_hair);

  /* Eyes */
  wxBoxSizer* hbox_eyes = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* eyesLabel = new wxStaticText(panel, SUMMARY_EYES_LABEL_ID, wxT("Eyes:"));
  hbox_eyes->Add(eyesLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* eyesTxt = new wxTextCtrl(panel, SUMMARY_EYES_INPUT_ID);
  eyesTxt->Disable();
  eyesTxt->Hide();
  hbox_eyes->Add(eyesTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_eyes);

  /* Deity */
  wxBoxSizer* hbox_deity = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* deityLabel = new wxStaticText(panel, SUMMARY_DEITY_LABEL_ID, wxT("Deity:"));
  hbox_deity->Add(deityLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* deityTxt = new wxTextCtrl(panel, SUMMARY_DEITY_INPUT_ID);
  deityTxt->Disable();
  deityTxt->Hide();
  hbox_deity->Add(deityTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_deity);

  /* Homeland */
  wxBoxSizer* hbox_homeland = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* homelandLabel = new wxStaticText(panel, SUMMARY_HOMELAND_LABEL_ID, wxT("Homeland:"));
  hbox_homeland->Add(homelandLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* homeTxt = new wxTextCtrl(panel, SUMMARY_HOMELAND_INPUT_ID);
  homeTxt->Disable();
  homeTxt->Hide();
  hbox_homeland->Add(homeTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_homeland);

  /* Gender */
  wxBoxSizer* hbox_gender = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* genderLabel = new wxStaticText(panel, SUMMARY_GENDER_LABEL_ID, wxT("Gender:"));
  hbox_gender->Add(genderLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* genderTxt = new wxTextCtrl(panel, SUMMARY_GENDER_INPUT_ID);
  genderTxt->Disable();
  genderTxt->Hide();
  hbox_gender->Add(genderTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_gender);

  /* Age */
  wxBoxSizer* hbox_age = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* ageLabel = new wxStaticText(panel, SUMMARY_AGE_LABEL_ID, wxT("Age:"));
  hbox_age->Add(ageLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  wxTextCtrl* ageTxt = new wxTextCtrl(panel, SUMMARY_AGE_INPUT_ID);
  ageTxt->Disable();
  ageTxt->Hide();
  hbox_age->Add(ageTxt, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_age);

  /* Race */
  wxBoxSizer* hbox_race = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* raceLabel = new wxStaticText(panel, SUMMARY_RACE_LABEL_ID, wxT("Race:"));
  hbox_race->Add(raceLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_race);

  /* Favored Class(es) */
  wxBoxSizer* hbox_favClass = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* favClassLabel = new wxStaticText(panel, SUMMARY_FAV_CLASS_LABEL_ID, wxT("Favored Class:"));
  hbox_favClass->Add(favClassLabel, 0, wxRIGHT | wxTOP | wxBOTTOM, summarySpacing);
  vbox1->Add(hbox_favClass);

  wxButton* summaryBtn = new wxButton(panel, SUMMARY_BUTTON_ID, wxT("Lock Info"));
  summaryBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnCharLocked, this);
  summaryBtn->Disable();
  summaryBtn->Hide();
  vbox1->Add(summaryBtn, 0, wxALIGN_RIGHT, 0);

  /* To Do List */
  wxStaticText* todoLabel = new wxStaticText(panel, SUMMARY_TODO_LABEL_ID, wxT("To Do List:"));
  todoLabel->Hide();
  vbox1->Add(todoLabel, 0, 0, 0);

  wxListBox* todoList = new wxListBox(panel, SUMMARY_TODO_LIST_ID);
  todoList->Hide();
  vbox1->Add(todoList, 1, wxEXPAND, 0);

  /* Summary Box Finished */
  hbox1->Add(vbox1, 1, wxEXPAND | wxRIGHT, 10);

  /* Spell List */
  wxStaticText* spellLabel = new wxStaticText(panel, wxID_ANY, wxT("Known Spells:"));
  vbox2->Add(spellLabel, 0, wxBOTTOM, 5);
  wxListBox* spellList = new wxListBox(panel, SUMMARY_SPELL_LIST_ID);
  vbox2->Add(spellList, 1, wxEXPAND, 0);

  hbox1->Add(vbox2, 1, wxEXPAND | wxRIGHT, 10);

  /* Feat List */
  wxStaticText* featLabel = new wxStaticText(panel, wxID_ANY, wxT("Your Feats:"));
  vbox3->Add(featLabel, 0, wxBOTTOM, 5);
  wxListBox* featList = new wxListBox(panel, SUMMARY_FEAT_LIST_ID);
  vbox3->Add(featList, 1, wxEXPAND, 0);

  hbox1->Add(vbox3, 1, wxEXPAND | wxRIGHT, 10);

  /* Skill List */
  wxStaticText* skillLabel = new wxStaticText(panel, wxID_ANY, wxT("Skills:"));
  vbox4->Add(skillLabel, 0, wxBOTTOM, 5);
  wxListBox* skillList = new wxListBox(panel, SUMMARY_SKILL_LIST_ID);
  //skillList->AppendString(wxString("Acrobatics: 4"));
  vbox4->Add(skillList, 1, wxEXPAND, 0);

  hbox1->Add(vbox4, 1, wxEXPAND, 0);

  panel->SetSizer(hbox1);
}

void cMain::SetupSummaryPage()
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

  static_cast<wxStaticText*>(wxWindow::FindWindowById(SUMMARY_FAV_CLASS_LABEL_ID))->SetLabel(wxT("Favored Class:"));

  wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Enable();
  wxWindow::FindWindowById(SUMMARY_BUTTON_ID)->Show();

  wxWindow::FindWindowById(SUMMARY_TODO_LABEL_ID)->Show();
  wxListBox* todoList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_TODO_LIST_ID));
  todoList->Clear();
  todoList->Show();
  todoList->AppendString(wxString("Fill Out Character Info"));
  todoList->AppendString(wxString("Generate Ability Scores"));
  todoList->AppendString(wxString("Pick Race"));
  todoList->AppendString(wxString("Pick Class"));
  todoList->AppendString(wxString("Pick Skills"));
  todoList->AppendString(wxString("Pick Feats"));

  wxWindow::FindWindowById(SUMMARY_NAME_LABEL_ID)->GetParent()->GetSizer()->Layout();
}

void cMain::OnCharLocked(wxCommandEvent& evt)
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
    evt.Skip();
    return;
  }

  /* Convert the non-string items */
  Pathfinder::GE_Alignment ge = static_cast<Pathfinder::GE_Alignment>(alignmentIdx % 3);
  Pathfinder::LC_Alignment lc = static_cast<Pathfinder::LC_Alignment>(floor(alignmentIdx / 3));
  long age;
  ageStr.ToLong(&age);

  /* Save the values to the character class */
  currChar_->name(std::string(name.mb_str()));
  currChar_->player(std::string(playerName.mb_str()));
  currChar_->geAlignment(ge);
  currChar_->lcAlignment(lc);
  currChar_->height(std::string(height.mb_str()));
  currChar_->weight(std::string(weight.mb_str()));
  currChar_->hair(std::string(hair.mb_str()));
  currChar_->hair(std::string(eyes.mb_str()));
  currChar_->hair(std::string(deity.mb_str()));
  currChar_->hair(std::string(homeland.mb_str()));
  currChar_->hair(std::string(gender.mb_str()));
  currChar_->age(age);

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

  /* Remove item from the to do list*/
  wxListBox* todoList = static_cast<wxListBox*>(wxWindow::FindWindowById(SUMMARY_TODO_LIST_ID));
  int todoIdx = todoList->FindString("Fill Out Character Info");
  if (todoIdx != wxNOT_FOUND)
  {
    todoList->Delete(todoIdx);
  }

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

  evt.Skip();
}