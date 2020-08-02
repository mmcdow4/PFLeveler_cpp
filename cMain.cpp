#include "cMain.h"

#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <map>
#include <wx/window.h>
#include <wx/stattext.h>
#include <wx/notebook.h>

#include <pf_include/PFTable.h>
#include <pf_include/Race.h>

/* create a table to handle events for class cMain based on class wxFrame */
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(10001, OnButtonClicked)

wxEND_EVENT_TABLE()


wxString populateRaceText(Pathfinder::Race raceObj);
void populateRacialTable(wxListBox* racialAbilityList, Pathfinder::Race raceObj);

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "char_generator", wxPoint(30, 30), wxSize(DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y))
{
  /* create a row of boxes along the top of the window, to switch between tabs */
  /* TODO: look into wxNotebook*/

  menubar_ = new wxMenuBar;
  file_ = new wxMenu;

  menubar_->Append(file_, wxT("&File"));
  file_->Append(FILE_IMPORT_ID, wxString("Import Character"), wxString("Import a complete character PDF for levelling up"));
  file_->Append(FILE_EXPORT_ID, wxString("Export Character"), wxString("Export a completed character to a PDF character sheet"));
  file_->Append(FILE_RESET_ID, wxString("New Character"), wxString("Discard the current character and start a new sheet"));
  file_->Append(FILE_EXIT_ID, wxString("Exit"), wxString("Close the window"));

  SetMenuBar(menubar_);
  menubar_->Bind(wxEVT_COMMAND_MENU_SELECTED, &cMain::menuCallback, this);

  wxPanel* panel1 = new wxPanel(this, wxID_ANY);
  wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
  notebook_ = new wxNotebook(panel1, wxID_ANY);


  InitializeNotebook();

  //SetupSummaryPage();

  //SetupRacePage();

  box1->Add(notebook_, 1, wxEXPAND, 0);
  panel1->SetSizer(box1);

  Centre();
}

void cMain::menuCallback(wxCommandEvent& evt)
{
  int origId = evt.GetId();

  switch (origId)
  {
  case FILE_IMPORT_ID :
    wxMessageBox("Import is not implemented yet, come back in rev 2.0");
    break;
  case FILE_EXPORT_ID :
    wxMessageBox("Export is not implemented yet, come back in rev 2.0");
    break;
  case FILE_RESET_ID:
    setupNotebook();
    break;
  case FILE_EXIT_ID:
    evt.Skip();
    delete this;
    break;
  default:
    wxMessageBox("Unknown file selection : " + std::to_string(origId));
    break;
  }
  evt.Skip();
}
void cMain::InitializeNotebook()
{
  InitializeSummaryPage();
  InitializeAbilityScoresPage();
  InitializeRacePage();
  InitializeClassPage();
  InitializeSkillsPage();
  InitializeSpellsPage();
  InitializeFeatsPage();
}

void cMain::setupNotebook()
{
  SetupSummaryPage();
  //setupAbilityScoresPage();
  SetupRacePage();
  //setupClassPage();
  //setupSkillsPage();
  //setupSpellsPage();
  //setupFeatsPage();
}
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

void cMain::InitializeAbilityScoresPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Ability Scores");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */

  panel->SetSizerAndFit(hbox1);
}

void cMain::InitializeRacePage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Race");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);/* will contain the veritcal boxes below */
  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* will hold race dropdown and stats */
  wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL); /* will hold the list of racials */

  /* two vertical box sizers side by side in a horizontal box sizer:
   * The left box will contain the race drop down menu, select button, and overall description,
   * The right box will contain the list box of racial bonuses and a text box below describing
   * a selected racial bonus */
  hbox1->Add(vbox1, 1, wxEXPAND | wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  hbox1->Add(vbox2, 1, wxEXPAND | wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);

  /* A horizontal sizer to contain the "Race" label, drop down box, and select button */
  wxBoxSizer* hbox_name = new wxBoxSizer(wxHORIZONTAL);
  vbox1->Add(hbox_name);


  /* create label, drop down menu, and button for race selection: */
  wxStaticText* nameLabel = new wxStaticText(panel, RACE_NAME_LABEL_ID, wxT("Race:"));
  hbox_name->Add(nameLabel, 0, wxRIGHT, 8);
  wxChoice* raceDropDown = new wxChoice(panel, RACE_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);

  raceDropDown->Bind(wxEVT_CHOICE, &cMain::OnRaceSelected, this);
  /* Populate the race dropdown menu */
  int Nraces = Pathfinder::PFTable::get_num_races();
  for (int raceIdx = 0; raceIdx < Nraces; raceIdx++)
  {
    try
    {
      raceDropDown->Append(Pathfinder::PFTable::get_race(raceIdx).raceName());
    }
    catch (const std::invalid_argument& e)
    {
      wxMessageBox(e.what());
    }
  }

  raceDropDown->Disable();
  raceDropDown->Hide();
  hbox_name->Add(raceDropDown, 0, wxLEFT | wxRIGHT, 30);

  /* add a select button */
  /* first add a spacer so it will appear on the far right */
  hbox_name->Add(new wxPanel(panel, wxID_ANY), 1, wxEXPAND);

  wxButton* selectBtn = new wxButton(panel, RACE_SELECT_BTN_ID, wxT("Lock Race Selection"));
  selectBtn->Hide();
  hbox_name->Add(selectBtn, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

  /* Create a text box containing the race details: size, attribute bonuses, etc. */
  wxString raceText = "Size: \nSpeed: \nAbilty Score Offsets: \nLangauges known: \nLangauges Available: ";

  wxStaticText* raceTextbox = new wxStaticText(panel, RACE_TEXT_ID, raceText);
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());
  vbox1->Add(raceTextbox, 1, wxEXPAND, 0);


  /* On the right of the screen show a list of all abilities above and description */

  wxStaticText* raceAbilitiesListLabel = new wxStaticText(panel, wxID_ANY, wxT("Racial Bonuses:"));
  vbox2->Add(raceAbilitiesListLabel, 0, 0, 0);

  /* fill out the race ability list here */
  wxBoxSizer* hbox_racials = new wxBoxSizer(wxHORIZONTAL);
  wxListBox* raceAbilityList = new wxListBox(panel, RACE_RACIAL_BONUS_TABLE_ID);

  hbox_racials->Add(raceAbilityList, 1, wxEXPAND | wxFIXED_MINSIZE, 0);
  vbox2->Add(hbox_racials, 2, wxEXPAND | wxBOTTOM | wxFIXED_MINSIZE, 5);
  //vbox2->Add(raceAbilityList, 2, wxEXPAND, 0);

  wxString racialDesc = "Description:";
  wxStaticText* raceAbilitiesDesc = new wxStaticText(panel, RACE_RACIAL_BONUS_DESC_ID, wxT("Description:"));
  vbox2->Add(raceAbilitiesDesc, 1, wxEXPAND, 0);

  //raceAbilityDesc->Wrap(raceAbilityDesc->GetClientSize().GetWidth());

  panel->SetSizerAndFit(hbox1);
}

void cMain::InitializeClassPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Class");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */

  panel->SetSizerAndFit(hbox1);
}

void cMain::InitializeSkillsPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Skills");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */

  panel->SetSizerAndFit(hbox1);
}

void cMain::InitializeSpellsPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Spells");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */

  panel->SetSizerAndFit(hbox1);
}

void cMain::InitializeFeatsPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  notebook_->AddPage(panel, L"Feats");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */

  panel->SetSizerAndFit(hbox1);
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

void cMain::SetupRacePage()
{
  wxPanel* panel = static_cast<wxPanel*>(wxWindow::FindWindowById(RACE_NAME_LABEL_ID)->GetParent());

  /* Turn on dropdown menu and lock button */
  wxChoice* raceDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(RACE_DROPDOWN_ID));
  raceDropDown->Show();
  raceDropDown->Enable();
  wxWindow::FindWindowById(RACE_SELECT_BTN_ID)->Show();
  wxWindow::FindWindowById(RACE_SELECT_BTN_ID)->Enable();

  /* This triggers the selection callback function, which populates the race text and racials table for us*/
  raceDropDown->SetSelection(0);

  Pathfinder::Race chosenRace = Pathfinder::PFTable::get_race(raceDropDown->GetSelection());

  /* Create a text box containing the race details: size, attribute bonuses, etc. */
  wxString raceText = populateRaceText(chosenRace);

  static_cast<wxStaticText*>(wxWindow::FindWindowById(RACE_TEXT_ID))->SetLabel(raceText);

  /* On the right of the screen show a list of all abilities above and description */

  /* fill out the race ability list here */
  wxListBox* raceAbilityList = static_cast<wxListBox*>(wxWindow::FindWindowById(RACE_RACIAL_BONUS_TABLE_ID));
  populateRacialTable(raceAbilityList, chosenRace);

  panel->Layout();
}
//{
//  /* create a button, text box, and list box */
//  //m_btn1 = new wxButton(this, 10001, "Click Me", wxPoint(10, 10), wxSize(150, 50));
//  //m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
//  //m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
//
//  /* create an nFieldWidth x nFieldHeight array of buttons, dynamically sized relative to parent window */
//  btn = new wxButton * [nFieldWidth * nFieldHeight];
//
//  /* create the array of mines */
//  nField = new int[nFieldWidth * nFieldHeight];
//
//  /* create a grid sizer to do the sizing for me*/
//  wxGridSizer* grid = new wxGridSizer(nFieldWidth, nFieldHeight, /*vertical padding*/ 0, /*horizontal padding*/ 0);
//
//  /* define font for button text*/
//  wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
//
//  for (int x = 0; x < nFieldWidth; x++)
//  {
//    for (int y = 0; y < nFieldHeight; y++)
//    {
//      /* Create a button and assign it to the grid sizer */
//      btn[y * nFieldWidth + x] = new wxButton(this, 10000 + (y * nFieldWidth + x));
//      btn[y * nFieldWidth + x]->SetFont(font);
//      grid->Add(btn[y * nFieldWidth + x], 1, wxEXPAND | wxALL);
//
//      /* Rather than create 100 entries in our event table above, we can dynamically bind these buttons to a single event handler */
//      btn[y * nFieldWidth + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
//      nField[y * nFieldWidth + x] = 0; /* no mine here!*/
//    }
//  }
//
//  this->SetSizer(grid);
//  grid->Layout();
//
//}

cMain::~cMain()
{
  /* NOTE: due to the unpredictable nature of GUI's, wxWidgets prefers to handle the deletes itself */

  /* HOWEVER, the array of buttons does need to be deleted */
  delete[]btn;
}

void cMain::OnRaceSelected(wxCommandEvent& evt)
{
  int raceIdx = evt.GetSelection();

  Pathfinder::Race chosenRace = Pathfinder::PFTable::get_race(raceIdx);

  wxString raceText = populateRaceText(chosenRace);
  wxStaticText* raceTextbox = static_cast<wxStaticText*>(wxWindow::FindWindowById(RACE_TEXT_ID));
  raceTextbox->SetLabel(raceText);
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());

  wxListBox* racialListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(RACE_RACIAL_BONUS_TABLE_ID));
  populateRacialTable(racialListBox, chosenRace);

  evt.Skip();
}

wxString populateRaceText(Pathfinder::Race chosenRace)
{
  wxString raceText;
  raceText += "Size: " + chosenRace.charSize();
  raceText += "\nSpeed: " + std::to_string(chosenRace.speed());
  raceText += "\nAbilty Score Offsets:";
  for (int ii = 0; ii < Pathfinder::NUMBER_ABILITY_SCORES + 1; ii++)
  {
    char buff[128];
    int offset = chosenRace.abilityOffset(static_cast<Pathfinder::abilityScoreMarker>(ii));

    if (offset != 0)
    {
      sprintf(buff, "\n  %+d to %s", offset, Pathfinder::ABILITY_SCORE_NAMES[ii]);
      raceText += std::string(buff);
    }
  }


  raceText += "\nLangauges known: ";
  for (int langIdx = 0; langIdx < chosenRace.numLanguages(); langIdx++)
  {
    std::string langName;
    try
    {
      langName = Pathfinder::PFTable::get_language(chosenRace.getLang(langIdx));
    }
    catch (const std::invalid_argument& e)
    {
      wxMessageBox(e.what());
      continue;
    }
    if (langIdx > 0)
    {
      raceText += ", ";
    }
    raceText += langName;
  }

  raceText += "\nLangauges Available: ";
  for (int langIdx = 0; langIdx < chosenRace.numAvailLanguages(); langIdx++)
  {
    std::string langName;
    try
    {
      langName = Pathfinder::PFTable::get_language(chosenRace.getAvailLang(langIdx));
    }
    catch (const std::invalid_argument& e)
    {
      wxMessageBox(e.what());
      continue;
    }
    if (langIdx > 0)
    {
      raceText += ", ";
    }
    raceText += langName;
  }

  return raceText;
}

void populateRacialTable(wxListBox* raceAbilityList, Pathfinder::Race chosenRace)
{
  raceAbilityList->Clear();
  for (int racialIdx = 0; racialIdx < chosenRace.numRacials(); racialIdx++)
  {
    try
    {
      Pathfinder::RacialAbility racial = chosenRace.getRacial(racialIdx);
      raceAbilityList->AppendString(racial.name());
    }
    catch (const std::invalid_argument& e)
    {
      wxMessageBox(e.what());
      continue;
    }
  }
}
void cMain::OnButtonClicked(wxCommandEvent& evt)
{
  /* append the string in the text box to the list box */
  //m_list1->AppendString(m_txt1->GetValue());
  /* now tell wxWidgets that the event has been handled */

  /* figure out coordinates of this button*/
  int x = (evt.GetId() - 10000) % nFieldWidth;
  int y = (evt.GetId() - 10000) / nFieldWidth;

  /* populate mines on first click*/
  if (bFirstClick)
  {
    int mines = 30;

    while (mines)
    {
      int rx = rand() % nFieldWidth;
      int ry = rand() % nFieldHeight;

      /* if there isn't already a mine here, and this isn't the first location clicked, create a mine*/
      if (nField[ry * nFieldWidth + rx] == 0 && (rx != x || ry != y))
      {
        nField[ry * nFieldWidth + rx] = -1;
        mines--;
      }
    }

    bFirstClick = false;
  }

  // Disable this button, so it can't be pressed again
  btn[y * nFieldWidth + x]->Enable(false);

  //Check if player hit a mine
  if (nField[y * nFieldWidth + x] == -1)
  {
    wxMessageBox("BOOOM YOU FUCKIN NERRRRRD!");

    //Reset the game
    bFirstClick = true;
    for (int x = 0; x < nFieldWidth; x++)
    {
      for (int y = 0; y < nFieldHeight; y++)
      {
        nField[y * nFieldWidth + x] = 0;
        btn[y * nFieldWidth + x]->SetLabel("");
        btn[y * nFieldWidth + x]->Enable(true);
      }
    }
  }
  else
  {
    /* figure out how many neighboring mines there are*/
    int mine_count = 0;
    for (int i = -1; i < 2; i++)
    {
      for (int j = -1; j < 2; j++)
      {
        if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
        {
          if (nField[(y + j) * nFieldWidth + (x + i)] == -1)
            mine_count++;
        }
      }
    }

    btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_count));
  }
  evt.Skip();
}