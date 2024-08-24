/* local includes*/
#include "AbilityScorePage.h"

/* stdlib includes*/
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <string>
#include <time.h>

/* wxWidgets includes */
#include <wx/window.h>
#include <wx/stattext.h>
#include <wx/notebook.h>

/* Pathfinder includes */
#include <pf_include/PFTable.h>
#include <pf_include/Character.h>
#include <pf_include/Attributes.h>
#include <pf_include/Race.h>


AbilityScorePage::AbilityScorePage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hboxOverview = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText* methodDropdownLabel = new wxStaticText(this, ABSCR_METHOD_DROPDOWN_LABEL_ID, wxT("Choose a Method: "));
  methodDropdownLabel->Hide();
  hboxOverview->Add(methodDropdownLabel);

  wxChoice* methodDropdown = new wxChoice(this, ABSCR_METHOD_DROPDOWN_ID);
  methodDropdown->Append("Standard");
  methodDropdown->Append("Classic");
  methodDropdown->Append("Heroic");
  methodDropdown->Append("Dice Pool");
  methodDropdown->Append("Purchase");
  methodDropdown->Append("Direct Input");

  methodDropdown->Disable();
  methodDropdown->Hide();
  hboxOverview->Add(methodDropdown);

  wxButton* methodSelectBtn = new wxButton(this, ABSCR_METHOD_BTN_ID, wxT("Select"));
  methodSelectBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AbilityScorePage::OnAttributeModeSelected, this);
  methodSelectBtn->Disable();
  methodSelectBtn->Hide();
  hboxOverview->Add(methodSelectBtn);

  wxStaticText* resourcePoolTxt = new wxStaticText(this, ABSCR_SCORES_REMAINING_TEXT_ID, wxT(" "));
  hboxOverview->Add(resourcePoolTxt);

  vbox1->Add(hboxOverview, 1, wxEXPAND);

  wxFlexGridSizer* scoresGrid = new wxFlexGridSizer(7, 7, 1, 1);
  /* add column labels */
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Attribute Name")), 0, wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" ")), 0, wxALIGN_CENTER); /* minus buttons */
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Raw Value")), 0, wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" ")), 0, wxALIGN_CENTER); /* plus buttons */
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Racial Bonus")), 0, wxALIGN_CENTER);
  //scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" ")), 0, wxALIGN_CENTER); /* flexible bonus */
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Total")), 0, wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Modifier")), 0, wxALIGN_CENTER);

  scoresGrid->AddGrowableCol(0, 1);
  scoresGrid->AddGrowableCol(1, 0);
  scoresGrid->AddGrowableCol(2, 1);
  scoresGrid->AddGrowableCol(3, 0);
  scoresGrid->AddGrowableCol(4, 1);
  scoresGrid->AddGrowableCol(5, 1);
  scoresGrid->AddGrowableCol(6, 1);
  for (int abilityIdx = 0; abilityIdx < static_cast<int>(Pathfinder::NUMBER_ABILITY_SCORES); abilityIdx++)
  {
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_LABELS + abilityIdx, (std::string(Pathfinder::ABILITY_SCORE_NAMES[abilityIdx]))), 0, wxALIGN_CENTER);
    scoresGrid->Add(new wxButton(this, ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx, "-"), 0, wxALIGN_CENTER);

    wxPanel* valuePanel = new wxPanel(this);
    wxBoxSizer* hboxValue = new wxBoxSizer(wxHORIZONTAL);
    hboxValue->Add(new wxChoice(valuePanel, ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx), 0, wxALIGN_CENTER);
    hboxValue->Add(new wxStaticText(valuePanel, ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx, " 0 "), 0, wxALIGN_CENTER);
    hboxValue->Add(new wxTextCtrl(valuePanel, ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_CENTRE),
      0, wxALIGN_CENTER);
    valuePanel->SetSizer(hboxValue);
    scoresGrid->Add(valuePanel, 1, wxALIGN_CENTRE);

    scoresGrid->Add(new wxButton(this, ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx, "+"), 0, wxALIGN_CENTER);

    wxPanel* racialPanel = new wxPanel(this);
    wxBoxSizer* hboxRacial = new wxBoxSizer(wxHORIZONTAL);
    hboxRacial->Add(new wxStaticText(racialPanel, ABSCR_ATTRIBUTE_RACIALS + abilityIdx, wxT(" 0 ")), 0, wxEXPAND);
    hboxRacial->Add(new wxRadioButton(racialPanel, ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx, wxT("")), 0, wxEXPAND);
    racialPanel->SetSizer(hboxRacial);
    scoresGrid->Add(racialPanel, 1, wxALIGN_CENTER);
    
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_TOTALS + abilityIdx, wxT(" 0 ")), 1, wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx, wxT(" 0 ")), 1, wxALIGN_CENTER);

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Hide();

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->SetBackgroundColour(*wxWHITE);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIALS + abilityIdx)->SetBackgroundColour(*wxWHITE);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_TOTALS + abilityIdx)->SetBackgroundColour(*wxWHITE);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx)->SetBackgroundColour(*wxWHITE);

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Bind(wxEVT_CHOICE, &AbilityScorePage::OnAbilityScoreSelected, this);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx)->Bind(wxEVT_TEXT, &AbilityScorePage::OnTextInput, this);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Bind(wxEVT_RADIOBUTTON, &AbilityScorePage::OnRacialRadioSelected, this);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AbilityScorePage::OnPlusButtonPress, this);
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AbilityScorePage::OnMinusButtonPress, this);
  }
  vbox1->Add(scoresGrid, 1, wxEXPAND);

  wxFlexGridSizer* ACGrid = new wxFlexGridSizer(2, 18, 1, 1);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Armor Class: "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_TOTAL, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "="), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " 10 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_ARMOR_VAL, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_SHIELD_VAL, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_DEX_MOD, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_SIZE_MOD, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_NATURAL_ARMOR, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_DEFLECT_MOD, " 0 "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_MISC_BONUS, " 0 "), 0, wxALIGN_CENTER);


  wxWindow::FindWindowById(ABSCR_AC_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_ARMOR_VAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_SHIELD_VAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_DEX_MOD)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_SIZE_MOD)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_NATURAL_ARMOR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_DEFLECT_MOD)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_AC_MISC_BONUS)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  for (int ii = 0; ii < 18; ii++)
  {
    ACGrid->AddGrowableCol(ii, 1);
  }

  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Base"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Armor"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Shield"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Dex Mod"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Size Mod"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Natural Armor"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Deflection Mod"), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Misc."), 0, wxALIGN_CENTER);

  vbox1->Add(ACGrid, 1, wxEXPAND);

  wxFlexGridSizer* MiscGrid = new wxFlexGridSizer(1, 8, 1, 1);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "Touch AC = "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_TOUCH_VALUE, " 0 "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "Flat-Footed AC = "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_FLATFOOT_VALUE, " 0 "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "Initiative Modifier = "), 0, wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_INITMOD_VALUE, " 0 "), 0, wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_MISC_TOUCH_VALUE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_MISC_FLATFOOT_VALUE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_MISC_INITMOD_VALUE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););

  for (int ii = 0; ii < 8; ii++)
  {
    MiscGrid->AddGrowableCol(ii, 1);
  }

  vbox1->Add(MiscGrid, 1, wxEXPAND);

  wxFlexGridSizer* SavesGrid = new wxFlexGridSizer(4, 12, 1, 1);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Saving Throws"), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Base"), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Ability Mod"), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Magic Mod"), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Misc Mod"), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Temp Mod"), 0, wxALIGN_CENTER);

  for (int ii = 0; ii < 12; ii++)
  {
    SavesGrid->AddGrowableCol(ii, 1);
  }

  for (int saveIdx = 0; saveIdx < 3; saveIdx++)
  {
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_NAME + saveIdx, Pathfinder::SAVING_THROW_NAMES[saveIdx]), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_TOTAL + saveIdx, " 0 "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " = "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_BASE + saveIdx, " 0 "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_ABILITY_MOD + saveIdx, " 0 "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_MAGIC_MOD + saveIdx, " 0 "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_MISC_MOD + saveIdx, " 0 "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_TEMP_MOD + saveIdx, " 0 "), 0, wxALIGN_CENTER);


    wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_BASE + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_MAGIC_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_MISC_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_TEMP_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  }

  vbox1->Add(SavesGrid, 1, wxEXPAND);

  wxFlexGridSizer* CombatGrid = new wxFlexGridSizer(4, 10, 1, 1);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_LABEL, "Combat Maneuver Bonus"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_TOTAL, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " = "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_BAB, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_STR, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_SIZE, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_BAB)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_STR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_SIZE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  for (int ii = 0; ii < 10; ii++)
  {
    CombatGrid->AddGrowableCol(ii, 1);
  }

  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Base Attack Bonus"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "STR Mod"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Size Mod"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);

  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_LABEL, "Combat Maneuver Defense"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_TOTAL, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " = "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_BAB, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_STR, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_DEX, " 0 "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_SIZE, " 0 "), 0, wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_BAB)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_STR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_DEX)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_SIZE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););

  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Base Attack Bonus"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "STR Mod"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "DEX Mod"), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Size Mod"), 0, wxALIGN_CENTER);

  vbox1->Add(CombatGrid, 1, wxEXPAND);

  vbox1->Add(new wxButton(this, ABSCR_ATTRIBUTE_LOCK_BUTTON, "Lock Attributes"), 0, wxALIGN_RIGHT);
  wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AbilityScorePage::OnAttributesLocked, this);
  this->SetSizerAndFit(vbox1);
}

void AbilityScorePage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  if (charPtr_->getCharacterLevel() > 0)
  {
    flexibleApplied_ = true;
    for (int abilityIdx = 0; abilityIdx < Pathfinder::NUMBER_ABILITY_SCORES; abilityIdx++)
    {
      wxChoice* currDropdown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx));
      wxTextCtrl* currTextInput = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx));

      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->Show();
      Pathfinder::abilityScoreMarker ability_marker = static_cast<Pathfinder::abilityScoreMarker>(abilityIdx);
      int raw_score_val = charPtr_->getAbilityScore(ability_marker) - charPtr_->racialAbilityScoreBonus(ability_marker);
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->SetLabel(wxString::Format(wxT(" %d "), raw_score_val));
      currDropdown->Disable();
      currDropdown->Hide();
      prevSelections_[abilityIdx] = -1;

      currTextInput->Disable();
      currTextInput->Hide();
      static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->Disable();
      static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->Hide();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Disable();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Hide();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Disable();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Hide();
    }

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Hide();

    wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID)->Hide();
    wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Disable();
    wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Hide();
    wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Disable();
    wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Hide();
    wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID)->Hide();
  }
  else
  {
    currChar->abilityScoresSet(false);
    flexibleApplied_ = false;
    newPoint_ = false;
    newPointUsed_ = false;

    /* turn on the method dropdown, select button, and label text*/
    wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID)->Show();
    wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Enable();
    wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Show();
    wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Enable();
    wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Show();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Show();

    /* reset all of the ability score page text fields*/
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->SetLabel(" ");
    for (int abilityIdx = 0; abilityIdx < 6; abilityIdx++)
    {
      prevSelections_[abilityIdx] = 0;
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Disable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Hide();
      static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx))->Clear();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Disable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Hide();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->Show();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->SetLabel(" 0 ");
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Disable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Hide();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Disable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Hide();
    }
  }
  /* fix all of the text fields */
  UpdateFields();

  /* fix the layout */
  Layout();
}

void AbilityScorePage::OnTextInput(wxCommandEvent& evt)
{
  int abilityIdx = evt.GetId() - ABSCR_ATTRIBUTE_VALUE_INPUT;

  wxTextCtrl* textBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(evt.GetId()));
  wxString inputStr = textBox->GetValue();
  long value;
  if (!inputStr.ToLong(&value) || value <= 0)
  {
    if (prevSelections_[abilityIdx] > 0)
    {
      textBox->SetValue(wxString::Format(wxT("%d"), prevSelections_[abilityIdx]));
    }
    else
    {
      textBox->SetValue("");
    }
    return;
  }
  else
  {
    charPtr_->decrementAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(abilityIdx), prevSelections_[abilityIdx]);
    prevSelections_[abilityIdx] = value;
    charPtr_->incrementAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(abilityIdx), prevSelections_[abilityIdx]);
    //charPtr_->setAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(abilityIdx), value);
    UpdateFields();
  }
}

void AbilityScorePage::OnAttributeModeSelected(wxCommandEvent& evt)
{
  int modeIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID))->GetCurrentSelection();

  for (int ii = 0; ii < 6; ii++)
  {
    Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(ii);
    int selectedVal = wxAtoi(static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->GetStringSelection());
    charPtr_->decrementAbilityScore(marker, selectedVal);
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->Clear();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->AppendString(" ");
    prevSelections_[ii] = 0;
  }
  switch (modeIdx)
  {
  case ABSCR_MODE_STANDARD:/* roll 4d6 and discard lowest die roll, repeat 6 times, assign scores*/
  case ABSCR_MODE_CLASSIC:/* roll 3d6, repeat 6 times, assign scores */
  case ABSCR_MODE_HEROIC:/* roll 2d6 and add 6, repeat 6 times, and assign scores*/
    populateScorePool(modeIdx);
    for (int ii = 0; ii < 6; ii++)
    {
      wxChoice* currDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii));
      currDropDown->Enable();
      currDropDown->Show();
      currDropDown->SetSelection(0);
      prevSelections_[ii] = 0;
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + ii)->Hide();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + ii)->Disable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + ii)->Hide();
    }

    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID))->GetParent()->Layout();
    break;
  case ABSCR_MODE_DICEPOOL:/*dice pool*/
    /* pool of 24d6 available. For each ability score, select a number of dice to roll (minimum 3).
    Once the pool is assigned, roll the dice for each ability score, record the sum the 3 highest values*/
    wxMessageBox("Dice pool ability score generation is not implemented yet.");
    break;
  case ABSCR_MODE_PURCHASE:/*purchase*/
    /* pool of points available and each ability score starts at 10. Spend points to raise ability score values or lower ability scores to get more points.
    Cannot lower ability scores below 7 or raise them above 18*/
    wxMessageBox("Purchase ability score generation is not implemented yet.");
    break;
  case ABSCR_MODE_DIRECT_INPUT:/*direct input*/
    /* directly type in desired numerical values for each ability score */
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID))->Hide();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID))->Hide();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID))->Hide();
    for (int abilityIdx = 0; abilityIdx < Pathfinder::NUMBER_ABILITY_SCORES; abilityIdx++)
    {
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Disable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Hide();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->Hide();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx)->Enable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx)->Show();
    }

    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON))->Enable();
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON))->Show();
    Layout();
    break;
  default:
    wxMessageBox("You must select a method for generating ability scores first");
    break;
  }

  UpdateFields();
}


void AbilityScorePage::OnAbilityScoreSelected(wxCommandEvent& evt)
{
  /* identify which ability score this corresponds to */
  int abilityIdx = evt.GetId() - ABSCR_ATTRIBUTE_VALUE_DROPDOWN;
  Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(abilityIdx);
  int scoreIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(evt.GetId()))->GetSelection();
  int prevVal = (prevSelections_[abilityIdx] > 0 ?
    stoi(std::string(static_cast<wxChoice*>(wxWindow::FindWindowById(evt.GetId()))->GetString(prevSelections_[abilityIdx]))) : 0);
  charPtr_->decrementAbilityScore(marker, prevVal);
  if (/*scoreIdx != prevSelections_[abilityIdx] &&*/ scoreIdx > 0)
  {
    /* get the selected value */
    
    int scoreVal = stoi(std::string(evt.GetString()));

    /* save the chosen value */
    charPtr_->incrementAbilityScore(marker, scoreVal);

    /* If something else had this selected, swap the selected values */
    for (int tmpIdx = 0; tmpIdx < 6; tmpIdx++)
    {
      if (tmpIdx == abilityIdx)
      {
        continue;
      }
      else if (scoreIdx == static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + tmpIdx))->GetSelection())
      {
        /* This choice conflicts with another choice already made - swap out the scores */
        static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + tmpIdx))->SetSelection(prevSelections_[abilityIdx]);

        charPtr_->decrementAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(tmpIdx), scoreVal);
        charPtr_->incrementAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(tmpIdx), prevVal);
        prevSelections_[tmpIdx] = prevSelections_[abilityIdx];
      }
    }
  }

  prevSelections_[abilityIdx] = scoreIdx;
  /* update all relevant text fields */
  UpdateFields();
}

void AbilityScorePage::ApplyRacialBonuses()
{
  bool hasFlexibleBonus = charPtr_->race().abilityOffset(Pathfinder::NUMBER_ABILITY_SCORES) > 0;
  for (int abilityIdx = 0; abilityIdx < 6; abilityIdx++)
  {
    Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(abilityIdx);
    int newTot = charPtr_->incrementAbilityScore(marker, charPtr_->race().abilityOffset(marker));
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIALS + abilityIdx))->SetLabel(
      " " + wxString::Format(wxT("%+i"), charPtr_->racialAbilityScoreBonus(marker)) + " ");
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_TOTALS + abilityIdx))->SetLabel(
      " " + wxString::Format(wxT("%i"), newTot) + " ");
    if (hasFlexibleBonus)
    {
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Enable();
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx)->Show();
      static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->SetValue(false);
    }
  }

  /* update all of the fields */
  UpdateFields();
}

void AbilityScorePage::OnRacialRadioSelected(wxCommandEvent& evt)
{
  Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(evt.GetId() - ABSCR_ATTRIBUTE_RACIAL_RADIO);

  if (evt.IsChecked())
  {
    /* apply the flexible racial. This removes the racial from previous applied abilities, if necessary */
    charPtr_->applyFlexibleRacial(marker);

    /* uncheck all of the other checkboxes*/
    for (int abilityIdx = 0; abilityIdx < Pathfinder::NUMBER_ABILITY_SCORES; abilityIdx++)
    {
      if (abilityIdx != evt.GetId() - ABSCR_ATTRIBUTE_RACIAL_RADIO &&
        static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->GetValue())
      {
        static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->SetValue(false);
      }
    }
    flexibleApplied_ = true;
  }

  /* update the entire page accordingly */
  UpdateFields();
}

void AbilityScorePage::OnAttributesLocked(wxCommandEvent& evt)
{
  if (charPtr_->race().id() == -1) /* if race is unassigned */
  {
    wxMessageBox("You must select a race before locking ability scores.");
    return;
  }
  //else if (wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN)->IsEnabled() == false && wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT)->IsEnabled() == false)
  //{
  //  wxMessageBox("You must assign ability scores.");
  //  return;
  //}
  else if ((prevSelections_[0] == 0) || (prevSelections_[1] == 0) || (prevSelections_[2] == 0) ||
    (prevSelections_[3] == 0) || (prevSelections_[4] == 0) || (prevSelections_[5] == 0))/* if any ability scores are unassigned */
  {
    wxMessageBox("You must set a base value for every ability score.");
    return;
  }
  else if (charPtr_->race().abilityOffset(Pathfinder::FLEXIBLE_ABILITY_SCORE_BONUS) > 0 && flexibleApplied_ == false)
  {
    wxMessageBox("You must apply your flexible ability score bonus before locking ability scores.");
    return;
  }
  else if (newPoint_ && !newPointUsed_)
  {
    wxMessageBox("You must apply your new ability score point");
    return;
  }

  if (charPtr_->abilityScoresSet() == false && charPtr_->abilityModifier(Pathfinder::INTELLIGENCE) > 0)
  {
    /* We are setting the initial ability scores, so indicate that there may be more starting languages to learn */
    charPtr_->remainingBonusLanguages(charPtr_->abilityModifier(Pathfinder::INTELLIGENCE));
  }
  charPtr_->abilityScoresSet(true);

  for (int abilityIdx = 0; abilityIdx < Pathfinder::NUMBER_ABILITY_SCORES; abilityIdx++)
  {
    wxChoice* currDropdown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx));
    wxTextCtrl* currTextInput = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_INPUT + abilityIdx));

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->Show();
    if (currDropdown->IsEnabled())
    {
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->SetLabel(" " + currDropdown->GetStringSelection() + " ");
    }
    else if (currTextInput->IsEnabled())
    {
      wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx)->SetLabel(" " + currTextInput->GetValue() + " ");
    }

    currDropdown->Disable();
    currDropdown->Hide();

    currTextInput->Disable();
    currTextInput->Hide();
    static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->Disable();
    static_cast<wxRadioButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIAL_RADIO + abilityIdx))->Hide();
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Disable();
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Hide();
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Disable();
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Hide();
  }

  wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Disable();
  wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON)->Hide();

  wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID)->Hide();
  wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Disable();
  wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Hide();
  wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Disable();
  wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Hide();
  wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID)->Hide();

  evt.Skip();
}

void AbilityScorePage::UpdateFields()
{

  /* Fill out ability score table: */
  for (int abilityIdx = 0; abilityIdx < 6; abilityIdx++)
  {
    Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(abilityIdx);
    int modValue = charPtr_->getAbilityScore(marker) >= 3 ? charPtr_->abilityModifier(marker) : 0;
    wxString modString = " " + wxString::Format(wxT("%+i"), modValue) + " ";
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_TEXT + abilityIdx))->SetLabel(
      " " + wxString::Format(wxT("%d"), charPtr_->getAbilityScore(marker) - charPtr_->racialAbilityScoreBonus(marker)) + " ");
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIALS + abilityIdx))->SetLabel(
      " " + wxString::Format(wxT("%+i"), charPtr_->racialAbilityScoreBonus(marker)) + " ");
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_TOTALS + abilityIdx))->SetLabel(
      " " + wxString::Format(wxT("%i"), charPtr_->getAbilityScore(marker)) + " ");
    static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx))->SetLabel(modString);
  }

  int strMod = (charPtr_->getAbilityScore(Pathfinder::STRENGTH) >= 3 ? charPtr_->abilityModifier(Pathfinder::STRENGTH) : 0);
  int conMod = (charPtr_->getAbilityScore(Pathfinder::CONSTITUTION) >= 3 ? charPtr_->abilityModifier(Pathfinder::CONSTITUTION) : 0);
  int dexMod = (charPtr_->getAbilityScore(Pathfinder::DEXTERITY) >= 3 ? charPtr_->abilityModifier(Pathfinder::DEXTERITY) : 0);
  int wisMod = (charPtr_->getAbilityScore(Pathfinder::WISDOM) >= 3 ? charPtr_->abilityModifier(Pathfinder::WISDOM) : 0);
  int sizeMod = ((charPtr_->race().charSize().compare("Small") == 0) ? 1 : 0);

  wxString strMod_str = " " + wxString::Format(wxT("%+i"), strMod) + " ";
  wxString conMod_str = " " + wxString::Format(wxT("%+i"), conMod) + " ";
  wxString dexMod_str = " " + wxString::Format(wxT("%+i"), dexMod) + " ";
  wxString wisMod_str = " " + wxString::Format(wxT("%+i"), wisMod) + " ";
  wxString sizeMod_str = " " + wxString::Format(wxT("%+i"), sizeMod) + " ";

  ///* TODO: From class retrieve BAB, Base fort/reflex/will saves */
  int bab = charPtr_->getBaseAttackBonus();
  int baseFort = charPtr_->getBaseFortitudeSave();
  int baseRef = charPtr_->getBaseReflexSave();
  int baseWill = charPtr_->getBaseWillSave();
  wxString baseFortStr = " " + wxString::Format(wxT("%+i"), charPtr_->getBaseFortitudeSave()) + " ";
  wxString baseRefStr = " " + wxString::Format(wxT("%+i"), charPtr_->getBaseReflexSave()) + " ";
  wxString baseWillStr = " " + wxString::Format(wxT("%+i"), charPtr_->getBaseWillSave()) + " ";
  /* AC Equations: */
  wxString ACTotStr = " " + wxString::Format(wxT("%i"), charPtr_->getArmorClass()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_AC_TOTAL))->SetLabel(ACTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_AC_SIZE_MOD))->SetLabel(sizeMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_AC_DEX_MOD))->SetLabel(dexMod_str);

  wxString touchACTotStr = " " + wxString::Format(wxT("%i"), charPtr_->getTouchArmorClass()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_TOUCH_VALUE))->SetLabel(touchACTotStr);

  wxString flatFoothACTotStr = " " + wxString::Format(wxT("%i"), charPtr_->getFlatFootedArmorClass()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_FLATFOOT_VALUE))->SetLabel(flatFoothACTotStr);

  wxString initModStr = " " + wxString::Format(wxT("%+i"), dexMod /* + misc */) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_INITMOD_VALUE))->SetLabel(initModStr);

  /* Saving throw equations: */
  /* Fortitude (Constitution) */
  wxString fortSaveTotStr = " " + wxString::Format(wxT("%+i"), charPtr_->getFortitudeSave()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_TOTAL))->SetLabel(fortSaveTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_BASE))->SetLabel(baseFortStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD))->SetLabel(conMod_str);

  /* Reflex (Dexterity) */
  wxString refSaveTotStr = " " + wxString::Format(wxT("%+i"), charPtr_->getReflexSave()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + 1))->SetLabel(refSaveTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_BASE + 1))->SetLabel(baseRefStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + 1))->SetLabel(dexMod_str);

  /* Will (Wisdom) */
  wxString willSaveTotStr = " " + wxString::Format(wxT("%+i"), charPtr_->getWillSave()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + 2))->SetLabel(willSaveTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_BASE + 2))->SetLabel(baseWillStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + 2))->SetLabel(wisMod_str);

  /* CMB Equation: */
  wxString CmbTotStr = " " + wxString::Format(wxT("%+i"), charPtr_->getCombatManeuverBonus()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_TOTAL))->SetLabel(CmbTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_STR))->SetLabel(strMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_SIZE))->SetLabel(sizeMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_BAB))->SetLabel(std::to_string(bab));

  /* CMD Equation: */
  wxString CmdTotStr = " " + wxString::Format(wxT("%+i"), charPtr_->getCombatManeuverDefense()) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_TOTAL))->SetLabel(CmdTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_STR))->SetLabel(strMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_DEX))->SetLabel(dexMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_SIZE))->SetLabel(sizeMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_BAB))->SetLabel(std::to_string(bab));

  this->Layout();
}

void AbilityScorePage::populateScorePool(int modeIdx)
{
  srand(time(NULL));
  std::vector<int> diceRolls;
  std::vector<int> rolledScores;
  for (int rollIdx = 0; rollIdx < 6; rollIdx++)
  {
    std::vector<int> diceRolls;
    diceRolls.push_back(Pathfinder::rollD6());
    diceRolls.push_back(Pathfinder::rollD6());
    diceRolls.push_back(modeIdx == ABSCR_MODE_HEROIC ? 6 : Pathfinder::rollD6()); /* if heroic, give the guaranteed 6 roll; otherwise do another roll for classic or standard*/
    diceRolls.push_back(modeIdx == ABSCR_MODE_STANDARD ? Pathfinder::rollD6() : 0); /* if standard, do the fourth dice rool; otherwise keep a 0*/

    /* sort into ascending order */
    std::sort(diceRolls.rbegin(), diceRolls.rend());
    int scoreValue = 0;
    for (int ii = 0; ii < 3; ii++)
    {
      scoreValue += diceRolls[ii];
    }
    rolledScores.push_back(scoreValue);
  }
  std::sort(rolledScores.begin(), rolledScores.end());
  for (int ii = 0; ii < 6; ii++)
  {
    wxChoice* scoreDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii));
    for (auto scoreIter = rolledScores.begin(); scoreIter != rolledScores.end(); ++scoreIter)
    {
      scoreDropDown->Append(std::to_string(*scoreIter));
    }
  }
  wxString scoreText;

  scoreText += "Remaining ability scores to assign: ";
  for (auto scoreIter = rolledScores.begin(); scoreIter != rolledScores.end(); ++scoreIter)
  {
    if (scoreIter != rolledScores.begin())
    {
      scoreText += ", ";
    }
    scoreText += std::to_string(*scoreIter);
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->SetLabel(scoreText);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->Show();

  static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON))->Enable();
  static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON))->Show();
}


void AbilityScorePage::UpdateAbilityScorePage(int classId)
{
  int classLvl = charPtr_->getClassLevel(classId);
  if (Pathfinder::PFTable::get_class(classId).levelItem(classLvl, Pathfinder::ABILITY_SCORE_POINT))
  {
    charPtr_->abilityScoresSet(false);
    newPoint_ = true;
    newPointUsed_ = false;
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON))->Enable();
    static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_LOCK_BUTTON))->Show();
    /* Show the plus and minus buttons, unlock the plus, add 1 to number of abiliity points left */
    for (size_t abilityIdx = 0; abilityIdx < static_cast<size_t>(Pathfinder::NUMBER_ABILITY_SCORES); abilityIdx++)
    {
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Enable();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Show();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Disable();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Show();
    }
    /* Add callbacks for plus and minus functions, you know what to do*/
  }
  UpdateFields();
}

void AbilityScorePage::OnPlusButtonPress(wxCommandEvent& evt)
{
  Pathfinder::abilityScoreMarker abilityIdx = static_cast<Pathfinder::abilityScoreMarker>(evt.GetId() - ABSCR_ATTRIBUTE_PLUS_BTN);
  
  if (newPoint_ && !newPointUsed_)
  {
    charPtr_->incrementAbilityScore(abilityIdx);
    newPointUsed_ = true;

    for (size_t abilityIdx = 0; abilityIdx < static_cast<size_t>(Pathfinder::NUMBER_ABILITY_SCORES); abilityIdx++)
    {
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Disable();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Enable();
    }

    UpdateFields();
  }
}

void AbilityScorePage::OnMinusButtonPress(wxCommandEvent& evt)
{
  Pathfinder::abilityScoreMarker abilityIdx = static_cast<Pathfinder::abilityScoreMarker>(evt.GetId() - ABSCR_ATTRIBUTE_MINUS_BTN);

  if (newPoint_ && newPointUsed_)
  {
    charPtr_->decrementAbilityScore(abilityIdx);
    newPointUsed_ = false;

    for (size_t abilityIdx = 0; abilityIdx < static_cast<size_t>(Pathfinder::NUMBER_ABILITY_SCORES); abilityIdx++)
    {
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx))->Enable();
      static_cast<wxButton*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx))->Disable();
    }

    UpdateFields();
  }
}