/* local includes*/
#include "RacePage.h"

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
#include <pf_include/Race.h>


wxString populateRaceText(Pathfinder::Race raceObj);
void populateRacialTable(wxListBox* racialAbilityList, Pathfinder::Race raceObj);


RacePage::RacePage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

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
  wxStaticText* nameLabel = new wxStaticText(this, RACE_NAME_LABEL_ID, wxT("Race:"));
  hbox_name->Add(nameLabel, 0, wxRIGHT, 8);
  wxChoice* raceDropDown = new wxChoice(this, RACE_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);

  raceDropDown->Bind(wxEVT_CHOICE, &RacePage::OnRaceSelected, this);
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
  hbox_name->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND);

  wxButton* selectBtn = new wxButton(this, RACE_SELECT_BTN_ID, wxT("Lock Race Selection"));
  selectBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &RacePage::OnRaceLocked, this);
  selectBtn->Hide();
  hbox_name->Add(selectBtn, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

  /* Create a text box containing the race details: size, attribute bonuses, etc. */
  wxString raceText = "Size: \nSpeed: \nAbilty Score Offsets: \nLangauges known: \nLangauges Available: ";

  wxStaticText* raceTextbox = new wxStaticText(this, RACE_TEXT_ID, raceText);
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());
  vbox1->Add(raceTextbox, 1, wxEXPAND, 0);


  /* On the right of the screen show a list of all abilities above and description */

  wxStaticText* raceAbilitiesListLabel = new wxStaticText(this, wxID_ANY, wxT("Racial Bonuses:"));
  vbox2->Add(raceAbilitiesListLabel, 0, 0, 0);

  /* fill out the race ability list here */
  wxBoxSizer* hbox_racials = new wxBoxSizer(wxHORIZONTAL);
  wxString* dummyStr = NULL;
  wxListBox* raceAbilityList = new wxListBox(this, RACE_RACIAL_BONUS_TABLE_ID, wxDefaultPosition, wxDefaultSize, 0, dummyStr, wxLB_NEEDED_SB);
  raceAbilityList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &RacePage::OnRacialSelected, this);

  hbox_racials->Add(raceAbilityList, 1, wxEXPAND | wxFIXED_MINSIZE, 0);
  vbox2->Add(hbox_racials, 2, wxEXPAND | wxBOTTOM | wxFIXED_MINSIZE, 5);
  //vbox2->Add(raceAbilityList, 2, wxEXPAND, 0);

  wxString racialDesc = "Description:";
  wxStaticText* raceAbilitiesDesc = new wxStaticText(this, RACE_RACIAL_BONUS_DESC_ID, wxT("Description:"));
  raceAbilitiesDesc->SetBackgroundColour(*wxWHITE);
  vbox2->Add(raceAbilitiesDesc, 1, wxEXPAND, 0);

  //raceAbilityDesc->Wrap(raceAbilityDesc->GetClientSize().GetWidth());

  this->SetSizerAndFit(hbox1);
}

void RacePage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

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

  this->Layout();
}

void RacePage::OnRaceSelected(wxCommandEvent& evt)
{
  int raceIdx = evt.GetSelection();

  Pathfinder::Race chosenRace = Pathfinder::PFTable::get_race(raceIdx);

  wxString raceText = populateRaceText(chosenRace);
  wxStaticText* raceTextbox = static_cast<wxStaticText*>(wxWindow::FindWindowById(RACE_TEXT_ID));
  raceTextbox->SetLabel(raceText);
  //raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());

  wxListBox* racialListBox = static_cast<wxListBox*>(wxWindow::FindWindowById(RACE_RACIAL_BONUS_TABLE_ID));
  populateRacialTable(racialListBox, chosenRace);

  wxWindow::FindWindowById(RACE_RACIAL_BONUS_DESC_ID)->SetLabel("Description:");

  wxWindow::FindWindowById(RACE_DROPDOWN_ID)->GetParent()->Layout();
}

void RacePage::OnRaceLocked(wxCommandEvent& evt)
{
  /* Save the selected race */
  int raceIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(RACE_DROPDOWN_ID))->GetSelection();
  Pathfinder::Race currentRace = Pathfinder::PFTable::get_race(raceIdx);
  charPtr_->race(currentRace);

  /* Remove the drop down menu and button, update the race name text */
  wxWindow::FindWindowById(RACE_DROPDOWN_ID)->Disable();
  wxWindow::FindWindowById(RACE_DROPDOWN_ID)->Hide();
  wxWindow::FindWindowById(RACE_SELECT_BTN_ID)->Disable();
  wxWindow::FindWindowById(RACE_SELECT_BTN_ID)->Hide();
  wxWindow::FindWindowById(RACE_NAME_LABEL_ID)->SetLabel("Race: " + currentRace.raceName());

  evt.Skip();
}

void RacePage::OnRacialSelected(wxCommandEvent& evt)
{
  int raceId = static_cast<wxChoice*>(wxWindow::FindWindowById(RACE_DROPDOWN_ID))->GetSelection();
  int racialId = evt.GetSelection();
  wxString racialTxt = "Description:\n" + Pathfinder::PFTable::get_race(raceId).getRacial(racialId).description();
  wxWindow::FindWindowById(RACE_RACIAL_BONUS_DESC_ID)->SetLabel(racialTxt);

  wxWindow::FindWindowById(RACE_RACIAL_BONUS_DESC_ID)->GetParent()->Layout();
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
      sprintf_s(buff, "\n  %+d to %s", offset, Pathfinder::ABILITY_SCORE_NAMES[ii]);
      raceText += std::string(buff);
    }
  }

  if (chosenRace.bonusSkill()) {
    raceText += "\nBonus Skill Point Each Level";
  }
  if (chosenRace.bonusFeat()) {
    raceText += "\nBonus Feat At First Level";
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