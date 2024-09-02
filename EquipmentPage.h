#pragma once

#ifndef EQUIPMENTPAGE_H
#define EQUIPMENTPAGE_H

#include <wx/wx.h>
#include <wx/notebook.h>

#include <pf_include/Character.h>
#include <pf_include/Race.h>
#include <pf_include/Equipment.h>

#define EQUIPMENT_CATEGORY_LABEL_ID 8000
#define EQUIPMENT_CATEGORY_DROPDOWN_ID 8001
#define EQUIPMENT_SEARCH_LABEL_ID 8002
#define EQUIPMENT_SEARCH_BAR_ID 8003
#define EQUIPMENT_AVAILABLE_LABEL_ID 8004
#define EQUIPMENT_OWNED_LABEL_ID 8005
#define EQUIPMENT_OWNED_LOCK_CHECKBOX_ID 8006
#define EQUIPMENT_AVAILABLE_LIST_ID 8007
#define EQUIPMENT_OWNED_LIST_ID 8008
#define EQUIPMENT_PURCHASE_BUTTON_ID 8009
#define EQUIPMENT_MASTERWORK_CHECKBOX_ID 8010
#define EQUIPMENT_SELL_BUTTON_ID 8011
#define EQUIPMENT_WEALTH_TEXT_ID 8012
#define EQUIPMENT_ADD_MONEY_BUTTON_ID 8013
#define EQUIPMENT_WEIGHT_TEXT_ID 8014
#define EQUIPMENT_DESCRIPTION_ID 8015

class EquipmentPage : public wxPanel
{
public:
  EquipmentPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar);
  ~EquipmentPage() {}

  void ResetPage(Pathfinder::Character* currChar);
  void UpdateEquipmentPage();

  void OnItemSelected(wxCommandEvent& evt);
  void PurchaseItemButtonPress(wxCommandEvent& evt);
  void SellItemButtonPress(wxCommandEvent& evt);
  void AddMoneyButtonPress(wxCommandEvent& evt);
  void OnCategorySelected(wxCommandEvent& evt);
  //void OnTextEntered(wxCommandEvent& evt);
  void OnLockBoxChecked(wxCommandEvent& evt);
  //void OnMasterworkBoxChecked(wxCommandEvent& evt);
 
  void UpdateItemDescription(std::shared_ptr<const Pathfinder::Equipment> itemPtr);
  void FilterAvailBox();
  void FilterOwnedBox();
private:
  Pathfinder::Character* charPtr_ = NULL;
  int currentCategory_ = Pathfinder::ALL_EQUIPMENT;
  std::map<int, std::shared_ptr<const Pathfinder::Equipment>> equipMap_ = Pathfinder::PFTable::get_equipment_map();
  std::vector<int> availListIds_;
  std::map<std::string, int> ownedListContents_;
  std::vector<std::shared_ptr<const Pathfinder::Equipment>> ownedItems_;
};

#endif