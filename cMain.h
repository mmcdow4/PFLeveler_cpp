#pragma once

#ifndef CMAIN_H
#define CMAIN_H

/* wxWidgets includes */
#include <wx/wx.h>
#include <wx/notebook.h>

#include "SummaryPage.h"
#include "AbilityScorePage.h"
#include "RacePage.h"
#include "ClassPage.h"
#include "SkillPage.h"
#include "SpellPage.h"
#include "FeatPage.h"
#include "EquipmentPage.h"

#include <pf_include/Character.h>

#define DEFAULT_WINDOW_SIZE_X 1350
#define DEFAULT_WINDOW_SIZE_Y 800

#define BACKGROUND_COLOR 0xE5E5E5
/* File Menu IDs */
#define FILE_IMPORT_ID 100
#define FILE_EXPORT_ID 101
#define FILE_RESET_ID 102
#define FILE_EXIT_ID 103

/* Help Menu IDs */
#define HELP_VERSION_ID 200
#define HELP_DESCRIBE_PAGE_ID 201

/* Notebook page indices */
#define NOTEBOOK_SUMMARY_PAGE_INDEX 0
#define NOTEBOOK_RACE_PAGE_INDEX 1
#define NOTEBOOK_ABSCR_PAGE_INDEX 2
#define NOTEBOOK_CLASS_PAGE_INDEX 3
#define NOTEBOOK_SKILL_PAGE_INDEX 4
#define NOTEBOOK_SPELL_PAGE_INDEX 5
#define NOTEBOOK_FEAT_PAGE_INDEX 6
#define NOTEBOOK_EQUIPMENT_PAGE_INDEX 7

class cMain : public wxFrame
{
public:
  cMain();
  ~cMain();


public:

  wxMenuBar* menubar_;
  wxMenu* file_;
  wxMenu* help_;

  wxNotebook* notebook_;

  void menuCallback(wxCommandEvent& evt);
  void InitializeNotebook();
  void ResetNotebook();

  void InitializeFeatsPage();
  void InitializeBorderPage();


  //void OnButtonClicked(wxCommandEvent& evt);

  void OnButtonPressed(wxCommandEvent& evt);


private:

  void exportCharacter(void);
  void importCharacter(void);
  void importCharacterTest(void);
  void showPageHelp(void);

  Pathfinder::Character* currChar_ = NULL;
  SummaryPage* summaryPage_ = NULL;
  AbilityScorePage* abilityScorePage_ = NULL;
  RacePage* racePage_ = NULL;
  ClassPage* classPage_ = NULL;
  SkillPage* skillPage_ = NULL;
  SpellPage* spellPage_ = NULL;
  FeatPage* featPage_ = NULL;
  EquipmentPage* equipmentPage_ = NULL;
};

#endif