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
#include <pf_include/Attributes.h>
#include <pf_include/Race.h>

void populate_score_pool(int modeIdx);

void cMain::InitializeAbilityScoresPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  panel->SetBackgroundColour(BACKGROUND_COLOR);
  notebook_->AddPage(panel, L"Ability Scores");

  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */
  wxBoxSizer* hboxOverview = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText* methodDropdownLabel = new wxStaticText(panel, ABSCR_METHOD_DROPDOWN_LABEL_ID, wxT("Choose a Method: "));
  methodDropdownLabel->Hide();
  hboxOverview->Add(methodDropdownLabel);

  wxChoice* methodDropdown = new wxChoice(panel, ABSCR_METHOD_DROPDOWN_ID);
  methodDropdown->Append("Standard");
  methodDropdown->Append("Classic");
  methodDropdown->Append("Heroic");
  methodDropdown->Append("Dice Pool");
  methodDropdown->Append("Purchase");
  methodDropdown->Append("Alternate");

  methodDropdown->Disable();
  methodDropdown->Hide();
  hboxOverview->Add(methodDropdown);

  wxButton* methodSelectBtn = new wxButton(panel, ABSCR_METHOD_BTN_ID, wxT("Select"));
  methodSelectBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnAttributeModeSelected, this);
  methodSelectBtn->Disable();
  methodSelectBtn->Hide();
  hboxOverview->Add(methodSelectBtn);

  wxStaticText* resourcePoolTxt = new wxStaticText(panel, ABSCR_SCORES_REMAINING_TEXT_ID, wxT(" "));
  hboxOverview->Add(resourcePoolTxt);
  
  vbox1->Add(hboxOverview, 1, wxEXPAND);

  wxFlexGridSizer* scoresGrid = new wxFlexGridSizer(7, 7, 1, 1);
  /* add column labels */
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Attribute Name")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT(" ")), 0, wxEXPAND | wxALIGN_CENTER); /* minus buttons */
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Raw Value")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT(" ")), 0, wxEXPAND | wxALIGN_CENTER); /* plus buttons */
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Racial Bonus")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Total")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Modifier")), 0, wxEXPAND | wxALIGN_CENTER);

  scoresGrid->AddGrowableCol(0, 1);
  scoresGrid->AddGrowableCol(1, 0);
  scoresGrid->AddGrowableCol(2, 1);
  scoresGrid->AddGrowableCol(3, 0);
  scoresGrid->AddGrowableCol(4, 1);
  scoresGrid->AddGrowableCol(5, 1);
  scoresGrid->AddGrowableCol(6, 1);
  for (int abilityIdx = 0; abilityIdx < static_cast<int>(Pathfinder::NUMBER_ABILITY_SCORES); abilityIdx++)
  {
    scoresGrid->Add(new wxStaticText(panel, ABSCR_ATTRIBUTE_LABELS + abilityIdx, (std::string(Pathfinder::ABILITY_SCORE_NAMES[abilityIdx]))), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxButton(panel, ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx, "-"), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxChoice(panel, ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxButton(panel, ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx, "+"), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(panel, ABSCR_ATTRIBUTE_RACIALS + abilityIdx, wxT(" _ ")), 1, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(panel, ABSCR_ATTRIBUTE_TOTALS + abilityIdx, wxT(" _ ")), 1, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(panel, ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx, wxT(" _ ")), 1, wxEXPAND | wxALIGN_CENTER);

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Hide();

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIALS + abilityIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_TOTALS + abilityIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  }
  vbox1->Add(scoresGrid, 1, wxEXPAND);
  
  wxFlexGridSizer* ACGrid = new wxFlexGridSizer(2, 18, 1, 1);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Armor Class: "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_TOTAL, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "="), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " 10 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_ARMOR_VAL, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_SHIELD_VAL, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_DEX_MOD, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_SIZE_MOD, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_NATURAL_ARMOR, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_DEFLECT_MOD, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, ABSCR_AC_MISC_BONUS, " _ "), 0, wxEXPAND | wxALIGN_CENTER);


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

  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Base"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Armor"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Shield"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Dex Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Size Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Natural Armor"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Deflection Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(panel, wxID_ANY, "Misc."), 0, wxEXPAND | wxALIGN_CENTER);

  vbox1->Add(ACGrid, 1, wxEXPAND);

  wxFlexGridSizer* MiscGrid = new wxFlexGridSizer(1, 12, 1, 1);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, "Touch AC = "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, ABSCR_MISC_TOUCH_VALUE, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, "Flat-Footed AC = "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, ABSCR_MISC_FLATFOOT_VALUE, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, "Initiative Modifier"), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, ABSCR_MISC_INITMOD_VALUE, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, "="), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, ABSCR_MISC_DEX_MOD, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(panel, ABSCR_MISC_MISC_MOD, " _ "), 0, wxEXPAND | wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_MISC_TOUCH_VALUE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_MISC_FLATFOOT_VALUE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_MISC_INITMOD_VALUE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_MISC_DEX_MOD)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_MISC_MISC_MOD)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););

  for (int ii = 0; ii < 12; ii++)
  {
    MiscGrid->AddGrowableCol(ii, 1);
  }

  vbox1->Add(MiscGrid, 1, wxEXPAND);

  wxFlexGridSizer* SavesGrid = new wxFlexGridSizer(4, 12, 1, 1);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Saving Throws"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Base"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Ability Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Magic Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Misc Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(panel, wxID_ANY, "Temp Mod"), 0, wxEXPAND | wxALIGN_CENTER);

  for (int ii = 0; ii < 12; ii++)
  {
    SavesGrid->AddGrowableCol(ii, 1);
  }

  for (int saveIdx = 0; saveIdx < 3; saveIdx++)
  {
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_NAME + saveIdx, Pathfinder::SAVING_THROW_NAMES[saveIdx]), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_TOTAL + saveIdx, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " = "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_BASE + saveIdx, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_ABILITY_MOD + saveIdx, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_MAGIC_MOD + saveIdx, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_MISC_MOD + saveIdx, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(panel, ABSCR_SAVES_TEMP_MOD + saveIdx, " _ "), 0, wxEXPAND | wxALIGN_CENTER);


    wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_BASE + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_MAGIC_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_MISC_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_TEMP_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  }

  vbox1->Add(SavesGrid, 1, wxEXPAND);

  wxFlexGridSizer* CombatGrid = new wxFlexGridSizer(4, 10, 1, 1);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMB_LABEL, "Combat Maneuver Bonus"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMB_TOTAL, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " = "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMB_BAB, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMB_STR, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMB_SIZE, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_BAB)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_STR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_SIZE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  for (int ii = 0; ii < 10; ii++)
  {
    CombatGrid->AddGrowableCol(ii, 1);
  }

  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "Base Attack Bonus"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "STR Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "Size Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);

  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMD_LABEL, "Combat Maneuver Defense"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMD_TOTAL, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " = "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMD_BAB, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMD_STR, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMD_DEX, " _ "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, ABSCR_COMBAT_CMD_SIZE, " _ "), 0, wxEXPAND | wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_BAB)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_STR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_DEX)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_SIZE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););

  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "Base Attack Bonus"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "STR Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "DEX Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(panel, wxID_ANY, "Size Mod"), 0, wxEXPAND | wxALIGN_CENTER);

  vbox1->Add(CombatGrid, 1, wxEXPAND);

  panel->SetSizerAndFit(vbox1);
}

void cMain::SetupAbilityScoresPage()
{
  /* turn on the method dropdown, select button, and label text*/
  wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID)->Show();
  wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Enable();
  wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID)->Show();
  wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Enable();
  wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID)->Show();

  /* reset all of the ability score page text fields*/
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->SetLabel(" ");
  for (int abilityIdx = 0; abilityIdx < 6; abilityIdx++)
  {
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Hide();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx))->Clear();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Hide();
  }

  /* fix the layout */
  static_cast<wxPanel*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID)->GetParent())->Layout();
}

void cMain::OnAttributeModeSelected(wxCommandEvent& evt)
{
  int modeIdx = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID))->GetCurrentSelection();

  for (int ii = 0; ii < 6; ii++)
  {
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->Clear();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->AppendString(" ");
  }
  switch (modeIdx)
  {
  //case 0:/*standard*/
  //  /* roll 4d6 discarding lowest die roll, repeat 6 times, assign scores*/
  //  wxMessageBox("Standard ability score generation is not implemented yet, come back in rev 2.0");
  //  break;
  //case 1:/*classic*/
  //  /* roll 3d6, repeat 6 times, assign scores */
  //  wxMessageBox("Classic ability score generation is not implemented yet, come back in rev 2.0");
  //  break;
  //case 2:/*heroic*/
  //  /* roll 2d6 and add 6, repeat 6 times, and assign scores*/
  //  wxMessageBox("Heroic ability score generation is not implemented yet, come back in rev 2.0");
  //  break;
  case ABSCR_MODE_STANDARD:
  case ABSCR_MODE_CLASSIC:
  case ABSCR_MODE_HEROIC:
    populate_score_pool(modeIdx);
    for (int ii = 0; ii < 6; ii++)
    {
      static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->Enable();
      static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii))->Show();
    }
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID))->Hide();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID))->Disable();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_ID))->Hide();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID))->Disable();
    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_BTN_ID))->Hide();

    static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_METHOD_DROPDOWN_LABEL_ID))->GetParent()->Layout();
    break;
  case ABSCR_MODE_DICEPOOL:/*dice pool*/
    /* pool of 24d6 available. For each ability score, select a number of dice to roll (minimum 3).
    Once the pool is assigned, roll the dice for each ability score, record the sum the 3 highest values*/
    wxMessageBox("Dice pool ability score generation is not implemented yet, come back in rev 2.0");
    break;
  case ABSCR_MODE_PURCHASE:/*purchase*/
    /* pool of points available and each ability score starts at 10. Spend points to raise ability score values or lower ability scores to get more points.
    Cannot lower ability scores below 7 or raise them above 18*/
    wxMessageBox("Purchase ability score generation is not implemented yet, come back in rev 2.0");
    break;
  default:
    wxMessageBox("You must select a method for generating ability scores first");
    break;
  }
  evt.Skip();
}

void populate_score_pool(int modeIdx)
{
  std::vector<int> diceRolls;
  std::vector<int> abilityScores;
  for(int rollIdx = 0; rollIdx < 6; rollIdx++)
  {
    std::vector<int> diceRolls;
    diceRolls.push_back(Pathfinder::rollD6());
    diceRolls.push_back(Pathfinder::rollD6());
    diceRolls.push_back(modeIdx == ABSCR_MODE_HEROIC ? 6 : Pathfinder::rollD6()); /* if heroic, give the guaranteed 6 roll; otherwise do another roll for classic or standard*/
    diceRolls.push_back(modeIdx == ABSCR_MODE_STANDARD ? Pathfinder::rollD6() : 0); /* if standard, do the fourth dice rool; otherwise keep a 0*/

    std::sort(diceRolls.rbegin(), diceRolls.rend());
    int scoreValue = 0;
    for (int ii = 0; ii < 3; ii++)
    {
      scoreValue += diceRolls[ii];
    }
    abilityScores.push_back(scoreValue);
  }
  std::sort(abilityScores.begin(), abilityScores.end());
  for (int ii = 0; ii < 6; ii++)
  {
    wxChoice* scoreDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii));
    for (auto scoreIter = abilityScores.begin(); scoreIter != abilityScores.end(); ++scoreIter)
    {
      scoreDropDown->Append(std::to_string(*scoreIter));
    }
  }
  wxString scoreText;

  scoreText += "Remaining ability scores to assign: ";
  for (auto scoreIter = abilityScores.begin(); scoreIter != abilityScores.end(); ++scoreIter)
  {
    if (scoreIter != abilityScores.begin())
    {
      scoreText += ", ";
    }
    scoreText += std::to_string(*scoreIter);
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->SetLabel(scoreText);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->Show();
}