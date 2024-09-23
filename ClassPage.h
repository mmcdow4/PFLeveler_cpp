#pragma once

#ifndef CLASSPAGE_H
#define CLASSPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>
#include "WrapText.h"
#include <vector>

#define CLASS_DROPDOWN_LABEL_ID 4000
#define CLASS_DROPDOWN_ID 4001
#define CLASS_LEVELUP_BUTTON_ID 4002
#define CLASS_FAVORED_CLASS_BUTTON_ID 4003
#define CLASS_DESCRIPTION_ID 4004
#define CLASS_TODO_FEATURE_LABEL_ID 4005
#define CLASS_TODO_FEATURE_LIST_ID 4006
#define CLASS_SELECTED_FEATURE_LABEL_ID 4007
#define CLASS_SELECTED_FEATURE_LIST_ID 4008
#define CLASS_FEATURE_DESCRIPTION_ID 4009
#define CLASS_FEATURE_BUTTON_ID 4010
#define CLASS_ABILITIES_LABEL_ID 4011
#define CLASS_ABILITIES_LIST_ID 4012
#define CLASS_ABILITIES_DESCRIPTION_ID 4013

class ClassPage : public wxPanel
{

  const int NOT_SWAP_CHOICE = 0;
  const int SWAP_SPELL_CHOICE = 1;
  const int SWAP_FEAT_CHOICE = 2;

public:
  ClassPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~ClassPage() {}

  void ResetPage(Pathfinder::Character* currChar);

  void OnLevelAdded(wxCommandEvent& evt);
  void OnFavoredClassAdded(wxCommandEvent& evt);
  void OnClassSelected(wxCommandEvent& evt);

  void OnAbilitySelected(wxCommandEvent& evt);
  void OnUnselectedFeatureSelected(wxCommandEvent& evt);
  void OnFinishedFeatureSelected(wxCommandEvent& evt);
  void SelectFeatureButtonPress(wxCommandEvent& evt);
  bool MakeFeatureChoice(int classIdx, int classLvl, int numChoices, std::vector<Pathfinder::ClassChoice>& choiceVec);
  bool MakeSpellSwapChoice(int classIdx);
  bool MakeFeatSwapChoice(int classIdx);
  void ResizeCallback(wxSizeEvent& evt);
  void MouseOverEvent(wxMouseEvent& evt);
  bool IsReadyForLevel(int classIdx, std::string& errMsg);

  int ParseNameForLanguage(std::string name, int* languageIdx);
  int ParseNameForSkill(std::string name);
  int ParseNameForSpellSchool(std::string name);

  bool skillsLocked_;
  bool spellsLeft_;
  bool featsLeft_;

  bool grantedFeats_;
  bool grantedSpells_;
  bool skillsChanged_;
  bool languageChange_;
private:
  Pathfinder::Character* charPtr_ = NULL;
  std::vector<Pathfinder::ClassFeature> todoFeatures_;
  std::vector<Pathfinder::ClassAbility> abilities_;

  std::vector<wxString> featureNames_;
  std::vector<wxString> featureDescriptions_;

  std::multiset<int> choicesMade_;

  //wxToolTip* toolTip_;

  HardBreakWrapper* classDescWrapper_ = NULL;
  HardBreakWrapper* featureDescWrapper_ = NULL;
  HardBreakWrapper* abilityDescWrapper_ = NULL;
};


class myDialog : public wxSingleChoiceDialog
{
public:
  myDialog(wxWindow* parent,
    const wxString& message,
    const wxString& caption,
    const wxArrayString& choices,
    void** clientData = NULL,
    long style = wxCHOICEDLG_STYLE,
    const wxPoint& pos = wxDefaultPosition,
    const std::vector<wxString>& descriptions = std::vector<wxString>()) : wxSingleChoiceDialog(parent, message, caption, choices, clientData, style, pos)
  {
    descriptions_ = descriptions;

    m_listbox->Bind(wxEVT_LEAVE_WINDOW, &myDialog::MouseOverEvent, this);
    m_listbox->Bind(wxEVT_MOTION, &myDialog::MouseOverEvent, this);
  }

  int myHitTest(wxPoint pos)
  {
    wxClientDC dc(m_listbox);
    dc.SetFont(m_listbox->GetFont());
    wxCoord itemHeight;
    dc.GetTextExtent("W", nullptr, &itemHeight);  // "W" is a good character for height estimation

    int scrollPos = m_listbox->GetScrollPos(wxVERTICAL);
    
    int itemIndex = std::floor(static_cast<double>(pos.y) / static_cast<double>(itemHeight)) + scrollPos;
    itemIndex = std::max(0, itemIndex);
    return std::min(itemIndex, static_cast<int>(m_listbox->GetCount()-1));
  }

  void MouseOverEvent(wxMouseEvent& evt)
  {
    wxPoint pos =evt.GetPosition();
    
    int item = myHitTest(pos);//m_listbox->HitTest(m_listbox->ClientToScreen(pos));
    if(item != wxNOT_FOUND)
    {
      if (evt.Moving())
      {
        m_listbox->SetToolTip(descriptions_[item]);
      }
      else if (evt.Leaving())
      {
        m_listbox->UnsetToolTip();
      }
    }
    else
    {
      m_listbox->UnsetToolTip();
    }
  }
  private:
    std::vector<wxString> descriptions_;
};
#endif