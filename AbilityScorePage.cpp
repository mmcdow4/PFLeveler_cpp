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
  methodDropdown->Append("Alternate");

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
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Attribute Name")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" ")), 0, wxEXPAND | wxALIGN_CENTER); /* minus buttons */
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Raw Value")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT(" ")), 0, wxEXPAND | wxALIGN_CENTER); /* plus buttons */
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Racial Bonus")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Total")), 0, wxEXPAND | wxALIGN_CENTER);
  scoresGrid->Add(new wxStaticText(this, wxID_ANY, wxT("Modifier")), 0, wxEXPAND | wxALIGN_CENTER);

  scoresGrid->AddGrowableCol(0, 1);
  scoresGrid->AddGrowableCol(1, 0);
  scoresGrid->AddGrowableCol(2, 1);
  scoresGrid->AddGrowableCol(3, 0);
  scoresGrid->AddGrowableCol(4, 1);
  scoresGrid->AddGrowableCol(5, 1);
  scoresGrid->AddGrowableCol(6, 1);
  for (int abilityIdx = 0; abilityIdx < static_cast<int>(Pathfinder::NUMBER_ABILITY_SCORES); abilityIdx++)
  {
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_LABELS + abilityIdx, (std::string(Pathfinder::ABILITY_SCORE_NAMES[abilityIdx]))), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxButton(this, ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx, "-"), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxChoice(this, ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxButton(this, ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx, "+"), 0, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_RACIALS + abilityIdx, wxT(" 0 ")), 1, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_TOTALS + abilityIdx, wxT(" 0 ")), 1, wxEXPAND | wxALIGN_CENTER);
    scoresGrid->Add(new wxStaticText(this, ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx, wxT(" 0 ")), 1, wxEXPAND | wxALIGN_CENTER);

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MINUS_BTN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Hide();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Disable();
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_PLUS_BTN + abilityIdx)->Hide();

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIALS + abilityIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_TOTALS + abilityIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_MODIFIERS + abilityIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););

    wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + abilityIdx)->Bind(wxEVT_CHOICE, &AbilityScorePage::OnAbilityScoreSelected, this);
  }
  vbox1->Add(scoresGrid, 1, wxEXPAND);
  
  wxFlexGridSizer* ACGrid = new wxFlexGridSizer(2, 18, 1, 1);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Armor Class: "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_TOTAL, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "="), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " 10 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_ARMOR_VAL, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_SHIELD_VAL, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_DEX_MOD, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_SIZE_MOD, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_NATURAL_ARMOR, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_DEFLECT_MOD, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, ABSCR_AC_MISC_BONUS, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);


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

  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Base"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Armor"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Shield"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Dex Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Size Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Natural Armor"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Deflection Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  ACGrid->Add(new wxStaticText(this, wxID_ANY, "Misc."), 0, wxEXPAND | wxALIGN_CENTER);

  vbox1->Add(ACGrid, 1, wxEXPAND);

  wxFlexGridSizer* MiscGrid = new wxFlexGridSizer(1, 12, 1, 1);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "Touch AC = "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_TOUCH_VALUE, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "Flat-Footed AC = "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_FLATFOOT_VALUE, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "Initiative Modifier"), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_INITMOD_VALUE, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "="), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_DEX_MOD, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, wxID_ANY, "+"), 0, wxEXPAND | wxALIGN_CENTER);
  MiscGrid->Add(new wxStaticText(this, ABSCR_MISC_MISC_MOD, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);

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
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Saving Throws"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Base"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Ability Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Magic Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Misc Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  SavesGrid->Add(new wxStaticText(this, wxID_ANY, "Temp Mod"), 0, wxEXPAND | wxALIGN_CENTER);

  for (int ii = 0; ii < 12; ii++)
  {
    SavesGrid->AddGrowableCol(ii, 1);
  }

  for (int saveIdx = 0; saveIdx < 3; saveIdx++)
  {
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_NAME + saveIdx, Pathfinder::SAVING_THROW_NAMES[saveIdx]), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_TOTAL + saveIdx, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " = "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_BASE + saveIdx, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_ABILITY_MOD + saveIdx, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_MAGIC_MOD + saveIdx, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_MISC_MOD + saveIdx, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
    SavesGrid->Add(new wxStaticText(this, ABSCR_SAVES_TEMP_MOD + saveIdx, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);


    wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_BASE + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_MAGIC_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_MISC_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
    wxWindow::FindWindowById(ABSCR_SAVES_TEMP_MOD + saveIdx)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  }

  vbox1->Add(SavesGrid, 1, wxEXPAND);

  wxFlexGridSizer* CombatGrid = new wxFlexGridSizer(4, 10, 1, 1);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_LABEL, "Combat Maneuver Bonus"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_TOTAL, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " = "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_BAB, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_STR, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMB_SIZE, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_BAB)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_STR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMB_SIZE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  for (int ii = 0; ii < 10; ii++)
  {
    CombatGrid->AddGrowableCol(ii, 1);
  }

  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Base Attack Bonus"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "STR Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Size Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);

  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_LABEL, "Combat Maneuver Defense"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_TOTAL, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " = "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_BAB, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_STR, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_DEX, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " + "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, ABSCR_COMBAT_CMD_SIZE, " 0 "), 0, wxEXPAND | wxALIGN_CENTER);

  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_TOTAL)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_BAB)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_STR)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_DEX)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););
  wxWindow::FindWindowById(ABSCR_COMBAT_CMD_SIZE)->SetBackgroundColour(*wxWHITE);//wxLIGHT_GREY););

  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Total"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Base Attack Bonus"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "STR Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "DEX Mod"), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, " "), 0, wxEXPAND | wxALIGN_CENTER);
  CombatGrid->Add(new wxStaticText(this, wxID_ANY, "Size Mod"), 0, wxEXPAND | wxALIGN_CENTER);

  vbox1->Add(CombatGrid, 1, wxEXPAND);

  this->SetSizerAndFit(vbox1);
}

void AbilityScorePage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

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

void AbilityScorePage::OnAttributeModeSelected(wxCommandEvent& evt)
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
    populateScorePool(modeIdx);
    for (int ii = 0; ii < 6; ii++)
    {
      wxChoice* currDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii));
      currDropDown->Enable();
      currDropDown->Show();
      currDropDown->SetSelection(0);
      prevSelections_[ii] = 0;
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
  if (scoreIdx != prevSelections_[abilityIdx])
  {
    /* get the selected value */
    int scoreVal = stoi(std::string(static_cast<wxChoice*>(wxWindow::FindWindowById(evt.GetId()))->GetStringSelection()));

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
        //charPtr_->setAbilityScore(static_cast<Pathfinder::abilityScoreMarker>(tmpIdx), prevSelections_[abilityIdx]);
      }
    }
  }

  prevSelections_[abilityIdx] = scoreIdx;
  /* update all relevant text fields */
  UpdateFields();
}

void AbilityScorePage::ApplyRacialBonuses()
{
  if (charPtr_->race().abilityOffset(Pathfinder::NUMBER_ABILITY_SCORES) == 0)
  {
    /* if the racial bonus isn't flexible, apply the fixed bonuses*/
    for (int abilityIdx = 0; abilityIdx < 6; abilityIdx++)
    {
      Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(abilityIdx);
      int newTot = charPtr_->incrementAbilityScore(marker, charPtr_->race().abilityOffset(marker));
      static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_RACIALS + abilityIdx))->SetLabel(
        " " + wxString::Format(wxT("%+i"), charPtr_->race().abilityOffset(marker)) + " ");
      static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_TOTALS + abilityIdx))->SetLabel(
        " " + wxString::Format(wxT("%i"), newTot) + " ");
    }
  }
  /* TODO else: open the checkboxes, or apply that value if they are already active*/
}

void AbilityScorePage::UpdateFields()
{
  
  /* Fill out ability score table: */
  for (int abilityIdx = 0; abilityIdx < 6; abilityIdx++)
  {
    Pathfinder::abilityScoreMarker marker = static_cast<Pathfinder::abilityScoreMarker>(abilityIdx);
    int modValue = charPtr_->getAbilityScore(marker) >= 3 ? charPtr_->abilityModifier(marker) : 0;
    wxString modString = " " + wxString::Format(wxT("%+i"), modValue) + " ";
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

  /* TODO: From class retrieve BAB, Base fort/reflex/will saves */

  /* AC Equations: */
  wxString ACTotStr = " " + wxString::Format(wxT("%i"), 10 + dexMod + sizeMod /* + dodgeMod + armorVal + shieldVal + natArmor + misc + deflect*/) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_AC_TOTAL))->SetLabel(ACTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_AC_SIZE_MOD))->SetLabel(sizeMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_AC_DEX_MOD))->SetLabel(dexMod_str);

  wxString touchACTotStr = " " + wxString::Format(wxT("%i"), 10 + dexMod + sizeMod /* + dodgeMod + misc + deflect*/) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_TOUCH_VALUE))->SetLabel(touchACTotStr);

  wxString flatFoothACTotStr = " " + wxString::Format(wxT("%i"), 10 + sizeMod /* + armorVal + shieldVal + natArmor + misc + deflect*/) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_FLATFOOT_VALUE))->SetLabel(flatFoothACTotStr);

  wxString initModStr = " " + wxString::Format(wxT("%+i"), dexMod /* + misc */) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_INITMOD_VALUE))->SetLabel(initModStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_MISC_DEX_MOD))->SetLabel(dexMod_str);

  /* Saving throw equations: */
  /* Fortitude (Constitution) */
  wxString fortSaveTotStr = " " + wxString::Format(wxT("%+i"), /* fortSaveBase + magicMod + tempMod + miscMod +*/ conMod) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_TOTAL))->SetLabel(fortSaveTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD))->SetLabel(conMod_str);

  /* Reflex (Dexterity) */
  wxString refSaveTotStr = " " + wxString::Format(wxT("%+i"), /* refSaveBase + magicMod + tempMod + miscMod +*/ dexMod) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + 1))->SetLabel(refSaveTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + 1))->SetLabel(dexMod_str);

  /* Will (Wisdom) */
  wxString willSaveTotStr = " " + wxString::Format(wxT("%+i"), /* willSaveBase + magicMod + tempMod + miscMod +*/ wisMod) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_TOTAL + 2))->SetLabel(willSaveTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SAVES_ABILITY_MOD + 2))->SetLabel(wisMod_str);

  /* CMB Equation: */
  wxString CmbTotStr = " " + wxString::Format(wxT("%+i"), /* bab +*/ strMod + sizeMod) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_TOTAL))->SetLabel(CmbTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_STR))->SetLabel(strMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMB_SIZE))->SetLabel(sizeMod_str);

  /* CMD Equation: */
  wxString CmdTotStr = " " + wxString::Format(wxT("%+i"), /* bab +*/ strMod + dexMod + sizeMod) + " ";
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_TOTAL))->SetLabel(CmdTotStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_STR))->SetLabel(strMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_DEX))->SetLabel(dexMod_str);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_COMBAT_CMD_SIZE))->SetLabel(sizeMod_str);
}
void AbilityScorePage::populateScorePool(int modeIdx)
{
  srand(time(NULL));
  std::vector<int> diceRolls;
  rolledScores_.clear();
  for(int rollIdx = 0; rollIdx < 6; rollIdx++)
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
    rolledScores_.push_back(scoreValue);
  }
  std::sort(rolledScores_.begin(), rolledScores_.end());
  for (int ii = 0; ii < 6; ii++)
  {
    wxChoice* scoreDropDown = static_cast<wxChoice*>(wxWindow::FindWindowById(ABSCR_ATTRIBUTE_VALUE_DROPDOWN + ii));
    for (auto scoreIter = rolledScores_.begin(); scoreIter != rolledScores_.end(); ++scoreIter)
    {
      scoreDropDown->Append(std::to_string(*scoreIter));
    }
  }
  wxString scoreText;

  scoreText += "Remaining ability scores to assign: ";
  for (auto scoreIter = rolledScores_.begin(); scoreIter != rolledScores_.end(); ++scoreIter)
  {
    if (scoreIter != rolledScores_.begin())
    {
      scoreText += ", ";
    }
    scoreText += std::to_string(*scoreIter);
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->SetLabel(scoreText);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(ABSCR_SCORES_REMAINING_TEXT_ID))->Show();
}