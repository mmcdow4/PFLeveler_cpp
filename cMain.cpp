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

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "char_generator", wxPoint(30, 30), wxSize(600, 600))
{
  /* create a row of boxes along the top of the window, to switch between tabs */
  /* TODO: look into wxNotebook*/

  menubar = new wxMenuBar;
  file = new wxMenu;

  menubar->Append(file, wxT("&File"));
  file->Append(wxID_ANY, wxString("Import Character"), wxString("Import a complete character PDF for levelling up"));
  file->Append(wxID_ANY, wxString("Export Character"), wxString("Export a completed character to a PDF character sheet"));
  file->Append(wxID_ANY, wxString("Reset Character"), wxString("Reset to a blank character"));
  file->Append(wxID_ANY, wxString("Exit"), wxString("Close the window"));
  SetMenuBar(menubar);

  wxPanel* panel1 = new wxPanel(this, wxID_ANY);
  wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
  wxNotebook* notebook = new wxNotebook(panel1, wxID_ANY);


  SetupSummaryPage(notebook);

  notebook->AddPage(new wxNotebookPage(notebook, -1), L"Ability Scores");
  SetupRacePage(notebook);
  //notebook->AddPage(new wxNotebookPage(notebook, -1), L"Race");
  notebook->AddPage(new wxNotebookPage(notebook, -1), L"Class");
  notebook->AddPage(new wxNotebookPage(notebook, -1), L"Skills");
  notebook->AddPage(new wxNotebookPage(notebook, -1), L"Spells");
  notebook->AddPage(new wxNotebookPage(notebook, -1), L"Feats");
  notebook->AddPage(new wxNotebookPage(notebook, -1), L"Equipment");

  box1->Add(notebook, 1, wxEXPAND, 0);
  panel1->SetSizer(box1);

  Centre();
}

void SetupSummaryPage(wxNotebook* notebook)
{
  wxPanel* panel = new wxPanel(notebook);
  notebook->AddPage(panel, L"Summary");
  
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);/* will contain the veritcal boxes below */
  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* will hold character summary data and todo list */
  wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL); /* will hold the spell list */
  wxBoxSizer* vbox3 = new wxBoxSizer(wxVERTICAL); /* will hold the feat list */
  wxBoxSizer* vbox4 = new wxBoxSizer(wxVERTICAL); /* will hold the skill list */

  /* create entry boxes for character summary data: */
  wxBoxSizer* hbox_name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* nameLabel = new wxStaticText(panel, wxID_ANY, wxT("Character Name:"));
  hbox_name->Add(nameLabel, 0, wxRIGHT, 8);
  wxTextCtrl* nameTxt = new wxTextCtrl(panel, wxID_ANY);
  hbox_name->Add(nameTxt, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

  vbox1->Add(hbox_name);
  /* need to do the above for: alignment, height, weight, hair, eyes, deity, homeland, gender, age, favored class*/

  /* and also for the following, which won't be editable on this page: race, class, character level (with level up button?)*/

  wxStaticText* todoLabel = new wxStaticText(panel, wxID_ANY, wxT("To Do List:"));
  vbox1->Add(todoLabel, 0, 0, 0);
  
  wxListBox* todoList = new wxListBox(panel, wxID_ANY);
  todoList->AppendString(wxString("Fill Out Character Info"));
  todoList->AppendString(wxString("Generate Ability Scores"));
  todoList->AppendString(wxString("Pick Race"));
  todoList->AppendString(wxString("Pick Class"));
  todoList->AppendString(wxString("Pick Skills"));
  todoList->AppendString(wxString("Pick Feats"));
  vbox1->Add(todoList, 1, wxEXPAND, 0);

  hbox1->Add(vbox1, 1, wxEXPAND | wxRIGHT, 10);

  wxStaticText* spellLabel = new wxStaticText(panel, wxID_ANY, wxT("Known Spells:"));
  vbox2->Add(spellLabel, 0, wxBOTTOM, 5);
  wxListBox* spellList = new wxListBox(panel, wxID_ANY);
  vbox2->Add(spellList, 1, wxEXPAND, 0);

  hbox1->Add(vbox2, 1, wxEXPAND | wxRIGHT, 10);

  wxStaticText* featLabel = new wxStaticText(panel, wxID_ANY, wxT("Your Feats:"));
  vbox3->Add(featLabel, 0, wxBOTTOM, 5);
  wxListBox* featList = new wxListBox(panel, wxID_ANY);
  vbox3->Add(featList, 1, wxEXPAND, 0);

  hbox1->Add(vbox3, 1, wxEXPAND | wxRIGHT, 10);

  wxStaticText* skillLabel = new wxStaticText(panel, wxID_ANY, wxT("Skills:"));
  vbox4->Add(skillLabel, 0, wxBOTTOM, 5);
  wxListBox* skillList = new wxListBox(panel, wxID_ANY);
  skillList->AppendString(wxString("Acrobatics: 4"));
  vbox4->Add(skillList, 1, wxEXPAND, 0);

  hbox1->Add(vbox4, 1, wxEXPAND, 0);

  panel->SetSizer(hbox1);
}

void SetupRacePage(wxNotebook* notebook)
{
  wxPanel* panel = new wxPanel(notebook);
  notebook->AddPage(panel, L"Race");

  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);/* will contain the veritcal boxes below */
  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* will hold race dropdown and stats */
  wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL); /* will hold the list of racials */

  /* create entry boxes for race attributes: */
  wxBoxSizer* hbox_name = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText* nameLabel = new wxStaticText(panel, wxID_ANY, wxT("Race:"));
  hbox_name->Add(nameLabel, 0, wxRIGHT, 8);
  int Nraces = Pathfinder::PFTable::get_num_races();
  wxChoice* raceDropDown = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
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
  raceDropDown->SetSelection(0);
  hbox_name->Add(raceDropDown, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

  Pathfinder::Race chosenRace = Pathfinder::PFTable::get_race(raceDropDown->GetSelection());
  /* TODO: add a select button */

  vbox1->Add(hbox_name);

  wxBoxSizer* hbox_text = new wxBoxSizer(wxHORIZONTAL);
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

  wxStaticText* raceTextbox = new wxStaticText(panel, wxID_ANY, raceText);
  raceTextbox->Wrap(raceTextbox->GetClientSize().GetWidth());
  hbox_text->Add(raceTextbox, 0, wxEXPAND, 0);

  vbox1->Add(hbox_text);

  wxBoxSizer* vbox_blank = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer* hbox_btn = new wxBoxSizer(wxHORIZONTAL);
  wxButton* selectBtn = new wxButton(panel, wxID_ANY, wxT("Lock Race Selection"));
  hbox_btn->Add(selectBtn, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

  vbox_blank->Add(new wxPanel(panel, wxID_ANY));
  vbox1->Add(vbox_blank, 1, wxEXPAND);
  vbox1->Add(hbox_btn, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT);

  hbox1->Add(vbox1, 1, wxEXPAND | wxRIGHT, 10);
  /* On the right of the screen show a list of all abilities above and description */

  wxStaticText* raceAbilitiesListLabel = new wxStaticText(panel, wxID_ANY, wxT("Racial Bonuses:"));
  vbox2->Add(raceAbilitiesListLabel, 0, 0, 0);

  /* fill out the race ability list here */
  wxListBox* raceAbilityList = new wxListBox(panel, wxID_ANY);
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
  
  vbox2->Add(raceAbilityList, 2, wxEXPAND, 0);


  wxStaticText* raceAbilitiesDescLabel = new wxStaticText(panel, wxID_ANY, wxT("Description:"));
  vbox2->Add(raceAbilitiesDescLabel, 0, 0, 0);

  wxStaticText* raceAbilityDesc = new wxStaticText(panel, wxID_ANY, wxT("And here's where the description would be... IF I HAD ONE!"));
  raceAbilityDesc->Wrap(raceAbilityDesc->GetClientSize().GetWidth());
  
  vbox2->Add(raceAbilityDesc, 0, 0, 0);

  hbox1->Add(vbox2, 1, wxEXPAND | wxLEFT, 10);

  panel->SetSizer(hbox1);
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