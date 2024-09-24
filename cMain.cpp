#include "cMain.h"
#include "RacePage.h"

#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <filesystem>

#include <wx/window.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/filedlg.h>

#include <pf_include/PFTable.h>
#include <pf_include/Race.h>


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "PFMaker", wxPoint(30, 30), wxSize(DEFAULT_WINDOW_SIZE_X, DEFAULT_WINDOW_SIZE_Y))
{
  /* create a row of boxes along the top of the window, to switch between tabs */
  /* TODO: look into wxNotebook*/

  menubar_ = new wxMenuBar;
  file_ = new wxMenu;

  menubar_->Append(file_, wxT("&File"));
  file_->Append(FILE_IMPORT_ID, wxString("Import Character"), wxString("Import a complete character from a file"));
  file_->Append(FILE_EXPORT_ID, wxString("Export Character"), wxString("Export a completed character to a file"));
  file_->Append(FILE_RESET_ID, wxString("New Character"), wxString("Discard the current character and start a new sheet"));
  file_->Append(FILE_EXIT_ID, wxString("Exit"), wxString("Close the window"));

  help_ = new wxMenu;
  menubar_->Append(help_, wxT("&Help"));
  help_->Append(HELP_DESCRIBE_PAGE_ID, wxString("Describe Page"), wxString("Give a description of the currently selected page"));
  help_->Append(HELP_VERSION_ID, wxString("Version"), wxString("Get Current Version Number"));

  SetMenuBar(menubar_);
  menubar_->Bind(wxEVT_COMMAND_MENU_SELECTED, &cMain::menuCallback, this);

  wxPanel* panel1 = new wxPanel(this, wxID_ANY);
  wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
  notebook_ = new wxNotebook(panel1, wxID_ANY);

  InitializeNotebook();
  Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonPressed, this);

  box1->Add(notebook_, 1, wxEXPAND, 0);
  panel1->SetSizer(box1);

  Centre();
}

void cMain::importCharacter(void)
{
  std::string filename = "";
  wxFileDialog choiceWindow(this, "Select a .pfr File", "", "", "Pathfinder files(*.pfr) | *.pfr", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (choiceWindow.ShowModal() != wxID_CANCEL)
  {
    filename = choiceWindow.GetPath();
  }

  if (!filename.empty())
  {
    if (currChar_ != NULL)
    {
      delete currChar_;
    }
    currChar_ = new Pathfinder::Character;

    int raceId = currChar_->importFromFile(filename);
    Pathfinder::Race loadedRace = Pathfinder::PFTable::get_race(raceId);
    currChar_->race(loadedRace);
    ResetNotebook();
  }
  return;
}

void cMain::exportCharacter(void)
{
  std::string errMsg;
  if (currChar_ == NULL)
  {
    wxMessageBox("A character has not been created yet");
    return;
  }
  else if (!classPage_->IsReadyForLevel(-10, errMsg))
  {
    wxMessageBox("Not ready to save the character: " + errMsg);
    return;
  }

  std::string filename = "";
  wxFileDialog choiceWindow(this, _("Save to File"), "", currChar_->name() + "_" + std::to_string(currChar_->getCharacterLevel()) + ".pfr",
    "Pathfinder files (*.pfr)|*.pfr", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (choiceWindow.ShowModal() != wxID_CANCEL)
  {
    filename = choiceWindow.GetPath();
  }
  
  if (!filename.empty())
  {
    currChar_->exportToFile(filename);
  }
  return;
}

void cMain::showPageHelp(void)
{
  int pageId = notebook_->GetSelection();

  std::string helpMessage;
  switch (pageId)
  {
    case NOTEBOOK_SUMMARY_PAGE_INDEX:
      helpMessage = "This page is where you will populate your character's biographical information. As you make more choices on other pages, an overall summary of that information will be shown here as well.";
      break;
    case NOTEBOOK_RACE_PAGE_INDEX:
      helpMessage = "This page is where you will choose your character's race, and also which extra languages you will know at start of play, if your intelligence modifier is positive. You can select a racial ability from the list to see a description in the text box below.";
      break;
    case NOTEBOOK_ABSCR_PAGE_INDEX:
      helpMessage = "This page is where you will set your character's ability score values, and choose which score to increase as you gain points from class levels. There are multiple ways to set your initial values, which you can select from the dropdown menu. You can switch methods partway through or restart the same method by clicking the \"Select\" button again. Once you are finished make sure you click the \"Lock Scores\" button on the bottom right of the page.";
      break;
    case NOTEBOOK_CLASS_PAGE_INDEX:
      helpMessage = "This page is where you will select your favored class(es), add class levels, and make class specific choices as you level up. The \"Class Choices\" list will indicate when you have choices to make - clicking an item in this list and then clicking the \"Make Choice\" button will cause a window to popup which will off you the options to choose. Tooltips will popup as you mouse over the options to give you more information, but if you want to back out and look around before finalizing your choice you can cancel to return to the main window, even partway through a choice with multiple steps. As you make those choices they will be recorded in the \"Class Features\" list, along with other features that don't involve a choice.";
      break;
    case NOTEBOOK_SKILL_PAGE_INDEX:
      helpMessage = "This page is where you will add skill ranks. You can only subtract ranks to undo additions from your current level that haven't been finalized yet - once you finalize your choices you can't back them out.";
      break;
    case NOTEBOOK_SPELL_PAGE_INDEX:
      helpMessage = "This page will show you the spells you know, and allow you to choose which spells to learn if that is relevant for your class. Spell lists are class specific in Pathfinder; for example if you are a level 1 bard and a level 1 sorcerer, you can only use your bard spell slots for spells in your bard list and vice-versa for your sorcerer spell slots.";
      break;
    case NOTEBOOK_FEAT_PAGE_INDEX:
      helpMessage = "This page is where you will select which feats to take. A feat's name in red means you are missing some pre-requisites - you can mouse over to see a tooltip telling you which pre-requisites you are missing. A feat's name in grey means it is a weapon proficiency that is redundant with proficiencies granted by your class.";
      break;
    case NOTEBOOK_EQUIPMENT_PAGE_INDEX:
      helpMessage = "This page is where you will buy and sell equipment. Your starting wealth will be randomly generated when you add your first level to your first class, as per table 6-1 in the core rulebook. Currency exchange values are as follows : 1 pp = 10 gp = 100 sp = 1000 cp. You can filter items by category from the dropdown menu and/or search for items by typing into the search bar - the text search will be applied only after you hit Enter, and likewise to remove the text search filter you must clear the search bar and hit Enter again. An item's name in red means you cannot afford it, an item's name in grey means you lack the relevant proficiency to use that item without a penalty, and an item's name in blue means you cannot afford that item *and* you also lack the proficiency. The \"Master Work\" checkbox is how to specify you wish to purchase a master work weapon or armor, and the price will be increased accordingly. The \"Add Money\" button will launch a popup window asking how much money you wish to add to your character, as a way to load a character later and add money earned while adventuring.";
      break;
    default:
      helpMessage = "ERROR: Unknown page index";
      break;
  }

  wxMessageBox(helpMessage, wxMessageBoxCaptionStr, wxOK | wxICON_INFORMATION | wxCENTRE | wxSTAY_ON_TOP, this);

}

void cMain::menuCallback(wxCommandEvent& evt)
{
  int origId = evt.GetId();

  switch (origId)
  {
  case FILE_IMPORT_ID :
    importCharacter();
    break;
  case FILE_EXPORT_ID :
    exportCharacter();
    break;
  case FILE_RESET_ID:
    if (currChar_ != NULL)
    {
      delete currChar_;
    }
    currChar_ = new Pathfinder::Character;
    ResetNotebook();
    break;
  case FILE_EXIT_ID:
    delete this;
    break;
  case HELP_VERSION_ID:
    wxMessageBox("Current Version 0.2");
    break;
  case HELP_DESCRIBE_PAGE_ID:
    showPageHelp();
    break;
  default:
    wxMessageBox("Unknown file selection : " + std::to_string(origId));
    break;
  }
}

void cMain::InitializeNotebook()
{
  summaryPage_ = new SummaryPage(notebook_, currChar_);
  notebook_->AddPage(summaryPage_, L"Summary");
  racePage_ = new RacePage(notebook_, currChar_);
  notebook_->AddPage(racePage_, L"Race");
  abilityScorePage_ = new AbilityScorePage(notebook_, currChar_);
  notebook_->AddPage(abilityScorePage_, L"Ability Scores");
  classPage_ = new ClassPage(notebook_, currChar_);
  notebook_->AddPage(classPage_, L"Class");
  skillPage_ = new SkillPage(notebook_, currChar_);
  notebook_->AddPage(skillPage_, L"Skills");
  spellPage_ = new SpellPage(notebook_, currChar_);
  notebook_->AddPage(spellPage_, L"Spells");
  featPage_ = new FeatPage(notebook_, currChar_);
  notebook_->AddPage(featPage_, L"Feats");
  equipmentPage_ = new EquipmentPage(notebook_, currChar_);
  notebook_->AddPage(equipmentPage_, L"Equipment");
  InitializeFeatsPage();
}

void cMain::OnButtonPressed(wxCommandEvent& evt)
{

  switch(evt.GetId())
  {
  case RACE_SELECT_BTN_ID:
    /* Update racial bonuses on the ability scores page */
    abilityScorePage_->ApplyRacialBonuses();
    equipmentPage_->UpdateEquipmentPage();

    /* Propagate race information to the summary page */
    summaryPage_->PopulateRaceData();

    break;
  case RACE_BONUS_LANGUAGE_BTN_ID:
    /* Propagate race information to the summary page */
    summaryPage_->PopulateRaceData();

    break;
  case ABSCR_ATTRIBUTE_LOCK_BUTTON:
    /* Propagate Ability Scores to the summary page */
    summaryPage_->PopulateAbilityScoreData();
    if (currChar_->remainingBonusLanguages() > 0)
    {
      /* Tell the race page to open up language selection*/
      racePage_->SetupBonusLanguages();
    }
    featPage_->UpdateFeatPage(wxNOT_FOUND);
    equipmentPage_->UpdateEquipmentPage();
    break;
  case CLASS_FAVORED_CLASS_BUTTON_ID:
    /* Propagate favored classes to the summary page */
    summaryPage_->PopulateFavoredClassData();
    break;
  case CLASS_LEVELUP_BUTTON_ID:
    /* Propagate class levels to the summary page */
    abilityScorePage_->UpdateAbilityScorePage(static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection());
    skillPage_->UpdateSkillPage();
    classPage_->spellsLeft_ = spellPage_->UpdateSpellPage(static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection());
    summaryPage_->PopulateClassLevelData();
    if (classPage_->grantedSpells_)
    {
      spellPage_->GrantSpells(static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection());
      classPage_->grantedSpells_ = false;
    }
    featPage_->UpdateFeatPage(static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection());
    if (classPage_->grantedFeats_)
    {
      featPage_->GrantFeats();
      classPage_->grantedFeats_ = false;
    }
    if (classPage_->languageChange_)
    {
      racePage_->SetupBonusLanguages();
      summaryPage_->PopulateRaceData();
    }
    equipmentPage_->UpdateEquipmentPage();
    summaryPage_->PopulateSpellData();
    summaryPage_->PopulateFeatData();
    summaryPage_->PopulateAbilityScoreData();
    break;
  case CLASS_FEATURE_BUTTON_ID:
    if (classPage_->grantedSpells_)
    {
      spellPage_->GrantSpells(static_cast<wxChoice*>(wxWindow::FindWindowById(CLASS_DROPDOWN_ID))->GetSelection());
      summaryPage_->PopulateSpellData();
      classPage_->grantedSpells_ = false;
    }
    if (classPage_->grantedFeats_)
    {
      featPage_->GrantFeats();
      summaryPage_->PopulateFeatData();
      classPage_->grantedFeats_ = false;
    }
    if (classPage_->skillsChanged_)
    {
      skillPage_->UpdateSkillPage();
      summaryPage_->PopulateSkillData();
      classPage_->skillsChanged_ = false;
    }
    break;
  case SKILL_LOCK_BUTTON_ID:
    /* Propagate skills to the summary page */
    summaryPage_->PopulateSkillData();
    featPage_->UpdateFeatPage(wxNOT_FOUND);
    classPage_->skillsLocked_ = true;
    break;
  case SPELL_LEARN_BUTTON_ID:
    /* Propagate spells to the summary page */
    summaryPage_->PopulateSpellData();
    classPage_->spellsLeft_ = false;
    break;
  case FEAT_SELECT_BUTTON_ID:
    /* Propagate feats to the summary page */
    summaryPage_->PopulateFeatData();
    classPage_->featsLeft_ = false;
    break;
  case EQUIPMENT_PURCHASE_BUTTON_ID:
  case EQUIPMENT_SELL_BUTTON_ID:
    abilityScorePage_->UpdateFields();
    summaryPage_->PopulateAbilityScoreData();
    break;
  default:
    wxMessageBox("Unknown button ID passed up to cMain [" + std::to_string(evt.GetId()) + "]");
    break;
  }
}
//A test to get borders working - does not work at all, breaks sizers for all other pages when used.
void cMain::InitializeBorderPage()
{
  wxColour col1, col2;
  col1.Set(wxT("#4f5049"));
  col1.Set(wxT("#ededed"));

  wxPanel* page = new wxPanel(this, wxID_ANY);
  notebook_->AddPage(page, L"Border Test");
  wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
  wxPanel* panel = new wxPanel(page, wxID_ANY);
  panel->SetBackgroundColour(col1);
  wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

  wxPanel* midPan = new wxPanel(panel, wxID_ANY);
  midPan->SetBackgroundColour(col2);

  vbox->Add(midPan, 1, wxEXPAND | wxALL, 20);
  panel->SetSizer(vbox);

  page->SetSizer(hbox);
  Centre();
}

void cMain::ResetNotebook()
{
  summaryPage_->ResetPage(currChar_);
  abilityScorePage_->ResetPage(currChar_);
  racePage_->ResetPage(currChar_);
  classPage_->ResetPage(currChar_);
  skillPage_->ResetPage(currChar_);
  spellPage_->ResetPage(currChar_);
  featPage_->ResetPage(currChar_);
  equipmentPage_->ResetPage(currChar_);
}



void cMain::InitializeFeatsPage()
{
  wxPanel* panel = new wxPanel(notebook_);
  panel->SetBackgroundColour(BACKGROUND_COLOR);

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL); /* will contain the various vertical sizers */

  panel->SetSizerAndFit(hbox1);
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
  //delete[]btn;
}

//void cMain::OnButtonClicked(wxCommandEvent& evt)
//{
//  /* append the string in the text box to the list box */
//  //m_list1->AppendString(m_txt1->GetValue());
//  /* now tell wxWidgets that the event has been handled */
//
//  /* figure out coordinates of this button*/
//  int x = (evt.GetId() - 10000) % nFieldWidth;
//  int y = (evt.GetId() - 10000) / nFieldWidth;
//
//  /* populate mines on first click*/
//  if (bFirstClick)
//  {
//    int mines = 30;
//
//    while (mines)
//    {
//      int rx = rand() % nFieldWidth;
//      int ry = rand() % nFieldHeight;
//
//      /* if there isn't already a mine here, and this isn't the first location clicked, create a mine*/
//      if (nField[ry * nFieldWidth + rx] == 0 && (rx != x || ry != y))
//      {
//        nField[ry * nFieldWidth + rx] = -1;
//        mines--;
//      }
//    }
//
//    bFirstClick = false;
//  }
//
//  // Disable this button, so it can't be pressed again
//  btn[y * nFieldWidth + x]->Enable(false);
//
//  //Check if player hit a mine
//  if (nField[y * nFieldWidth + x] == -1)
//  {
//    wxMessageBox("BOOOM YOU FUCKIN NERRRRRD!");
//
//    //Reset the game
//    bFirstClick = true;
//    for (int x = 0; x < nFieldWidth; x++)
//    {
//      for (int y = 0; y < nFieldHeight; y++)
//      {
//        nField[y * nFieldWidth + x] = 0;
//        btn[y * nFieldWidth + x]->SetLabel("");
//        btn[y * nFieldWidth + x]->Enable(true);
//      }
//    }
//  }
//  else
//  {
//    /* figure out how many neighboring mines there are*/
//    int mine_count = 0;
//    for (int i = -1; i < 2; i++)
//    {
//      for (int j = -1; j < 2; j++)
//      {
//        if (x + i >= 0 && x + i < nFieldWidth && y + j >= 0 && y + j < nFieldHeight)
//        {
//          if (nField[(y + j) * nFieldWidth + (x + i)] == -1)
//            mine_count++;
//        }
//      }
//    }
//
//    btn[y * nFieldWidth + x]->SetLabel(std::to_string(mine_count));
//  }
//}