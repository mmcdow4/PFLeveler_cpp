#include <unordered_map>

#include "EquipmentPage.h"

EquipmentPage::EquipmentPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */ 
  wxBoxSizer* hbox0 = new wxBoxSizer(wxHORIZONTAL);/* category dropdown and label, search bar */
  wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);/* Equipment lists */
  //wxFlexGridSizer* gridSizer = new wxFlexGridSizer(1, 2, 0, 10); // 1 row, 2 columns, no vertical gap, 10px horizontal gap. Equipment lists

  /* add a spacer */
  hbox0->AddStretchSpacer(2);

  /* Add the category dropdown and label */
  wxStaticText* categoryLabel = new wxStaticText(this, EQUIPMENT_CATEGORY_LABEL_ID, wxT("Category:"));
  hbox0->Add(categoryLabel, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  wxChoice* categoryDropDown = new wxChoice(this, EQUIPMENT_CATEGORY_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);
  for (int categoryIdx = 0; categoryIdx < Pathfinder::NUM_EQUIPMENT_CATEGORIES; categoryIdx++)
  {
    categoryDropDown->AppendString(Pathfinder::EQUIPMENT_CATEGORY_NAMES[categoryIdx]);
  }
  categoryDropDown->SetSelection(0);
  categoryDropDown->Bind(wxEVT_CHOICE, &EquipmentPage::OnCategorySelected, this);
  hbox0->Add(categoryDropDown, 1, wxEXPAND | wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  /* add a spacer to separate the two */
  hbox0->AddStretchSpacer(2);//(new wxPanel(this, wxID_ANY), 2, wxEXPAND);
  /* Add the search bar and label */
  wxStaticText* searchLabel = new wxStaticText(this, EQUIPMENT_SEARCH_LABEL_ID, wxT("Search:"));
  hbox0->Add(searchLabel, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  wxTextCtrl* searchTxt = new wxTextCtrl(this, EQUIPMENT_SEARCH_BAR_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  searchTxt->Bind(wxEVT_TEXT_ENTER, &EquipmentPage::OnTextEntered, this);
  hbox0->Add(searchTxt, 1, wxEXPAND | wxALIGN_LEFT, 10);

  /* add a spacer */
  hbox0->AddStretchSpacer(2);//(new wxPanel(this, wxID_ANY), 2, wxEXPAND);
  
  vboxOverall->Add(hbox0, 0, wxALIGN_LEFT | wxBOTTOM, 15);

  /* Available Equipment List */
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availEquipLabel = new wxStaticText(this, EQUIPMENT_AVAILABLE_LABEL_ID, wxT("Available Equipment:"));
  vboxAvail->Add(availEquipLabel, 0, wxBOTTOM, 5);
  wxListCtrl* availEquipList = new wxListCtrl(this, EQUIPMENT_AVAILABLE_LIST_ID, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
  this->SetupListBox(availEquipList);
  availEquipList->Bind(wxEVT_LIST_ITEM_SELECTED, &EquipmentPage::OnItemSelected, this);
  availEquipList->Bind(wxEVT_MOTION, &EquipmentPage::MouseOverEvent, this);
  availEquipList->Bind(wxEVT_LEAVE_WINDOW, &EquipmentPage::MouseOverEvent, this);
  vboxAvail->Add(availEquipList, 1, wxEXPAND, 0);

  wxBoxSizer* hboxAvailBtns = new wxBoxSizer(wxHORIZONTAL);
  wxButton* buyButton = new wxButton(this, EQUIPMENT_PURCHASE_BUTTON_ID, wxT("Purchase"));
  buyButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &EquipmentPage::PurchaseItemButtonPress, this);
  buyButton->Disable();
  hboxAvailBtns->Add(buyButton);
  wxCheckBox* mwCheck = new wxCheckBox(this, EQUIPMENT_MASTERWORK_CHECKBOX_ID, wxT("Master Work"));
  mwCheck->Bind(wxEVT_CHECKBOX, &EquipmentPage::OnMasterworkBoxChecked, this);
  hboxAvailBtns->Add(mwCheck);
  vboxAvail->Add(hboxAvailBtns);
  wxStaticText* moneyText = new wxStaticText(this, EQUIPMENT_WEALTH_TEXT_ID, wxT("Wealth on Hand: 0 cp"));
  vboxAvail->Add(moneyText);
  wxButton* addMoneyButton = new wxButton(this, EQUIPMENT_ADD_MONEY_BUTTON_ID, wxT("Add Money"));
  addMoneyButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &EquipmentPage::AddMoneyButtonPress, this);
  vboxAvail->Add(addMoneyButton);
  //gridSizer->Add(vboxAvail, 1, wxEXPAND | wxALL, 10);
  hbox2->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT | wxFIXED_MINSIZE, 10);

  /* Owned Equipment List */
  wxBoxSizer* vboxOwned = new wxBoxSizer(wxVERTICAL);
  wxStaticText* ownedEquipmentLabel = new wxStaticText(this, EQUIPMENT_OWNED_LABEL_ID, wxT("Owned Equipment:"));
  vboxOwned->Add(ownedEquipmentLabel, 0, wxBOTTOM, 5);
  wxListCtrl* ownedEquipmentList = new wxListCtrl(this, EQUIPMENT_OWNED_LIST_ID, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
  this->SetupListBox(ownedEquipmentList);
  ownedEquipmentList->Bind(wxEVT_LIST_ITEM_SELECTED, &EquipmentPage::OnItemSelected, this);
  ownedEquipmentList->Bind(wxEVT_MOTION, &EquipmentPage::MouseOverEvent, this);
  ownedEquipmentList->Bind(wxEVT_LEAVE_WINDOW, &EquipmentPage::MouseOverEvent, this);
  vboxOwned->Add(ownedEquipmentList, 1, wxEXPAND, 0);
  wxBoxSizer* hboxOwnedBtns = new wxBoxSizer(wxHORIZONTAL);
  wxButton* sellButton = new wxButton(this, EQUIPMENT_SELL_BUTTON_ID, wxT("Sell"));
  sellButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &EquipmentPage::SellItemButtonPress, this);
  sellButton->Disable();
  hboxOwnedBtns->Add(sellButton);
  wxCheckBox* filterCheck = new wxCheckBox(this, EQUIPMENT_OWNED_LOCK_CHECKBOX_ID, wxT("Don't Apply Filters To Owned Equipment"));
  filterCheck->SetValue(true);
  filterCheck->Bind(wxEVT_CHECKBOX, &EquipmentPage::OnLockBoxChecked, this);
  hboxOwnedBtns->Add(filterCheck);

  vboxOwned->Add(hboxOwnedBtns);
  wxStaticText* weightText = new wxStaticText(this, EQUIPMENT_WEIGHT_TEXT_ID, wxT("Total Weight Held: 0 lbs\nLight Load Capacity : 0 lbs\nMedium Load Capacity : 0 lbs\nHeavy Load Capacity : 0 lbs"));
  vboxOwned->Add(weightText);

  //gridSizer->Add(vboxOwned, 1, wxEXPAND | wxALL, 10);
  hbox2->Add(vboxOwned, 1, wxEXPAND | wxUP | wxDOWN | wxLEFT | wxFIXED_MINSIZE, 10);

  //gridSizer->AddGrowableCol(0, 1); // First column grows
  //gridSizer->AddGrowableCol(1, 1); // Second column grows
  
  vboxOverall->Add(hbox2, 5, wxEXPAND);
  //vboxOverall->Add(gridSizer, 5, wxEXPAND);

  /* Description Box */
  wxTextCtrl* itemDescription = new wxTextCtrl(this, EQUIPMENT_DESCRIPTION_ID, wxT("Description:"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  itemDescription->SetBackgroundColour(*wxWHITE);
  itemDescription->Bind(wxEVT_SIZE, &EquipmentPage::ResizeCallback, this);
  vboxOverall->Add(itemDescription, 2, wxEXPAND, 10);
  itemDescWrapper_ = new HardBreakWrapper(itemDescription, wxT("Description:"), GetClientSize().GetWidth() - 20);
  itemDescription->SetLabelText(itemDescWrapper_->GetWrapped());

  /* Populate the available equipment box */
  equipMap_ = Pathfinder::PFTable::get_equipment_map();
  for (auto equipIter = equipMap_.begin(); equipIter != equipMap_.end(); ++equipIter)
  {
    this->InsertListItem(availEquipList, equipIter->second);
    availListIds_.push_back(equipIter->first);
  }

  this->SetSizer(vboxOverall);

}

void EquipmentPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  currentCategory_ = Pathfinder::ALL_EQUIPMENT;
  ownedItems_.clear();

  if (itemDescWrapper_ != NULL)
  {
    delete itemDescWrapper_;
  }

  wxTextCtrl* itemDesc = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_DESCRIPTION_ID));
  itemDescWrapper_ = new HardBreakWrapper(itemDesc, wxT("Description:"), GetClientSize().GetWidth() - 20);
  itemDesc->SetLabelText(itemDescWrapper_->GetWrapped());

  std::string wealthStr = "0 cp";
  double lightCarryCapacity = 0.0;
  double mediumCarryCapacity = 0.0;
  double heavyCarryCapacity = 0.0;
  double weightHeld = 0.0;
  if (charPtr_ != NULL)
  {
    wealthStr = charPtr_->wealthString();
    weightHeld = charPtr_->weightHeld();
    if (charPtr_->race().id() != -1 && charPtr_->abilityScoresSet())
    {
      get_carry_capacities(charPtr_->getAbilityScore(Pathfinder::STRENGTH), charPtr_->race().charSize(), &lightCarryCapacity, &mediumCarryCapacity, &heavyCarryCapacity);
    }
  }

  static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID))->ClearAll();
  static_cast<wxChoice*>(wxWindow::FindWindowById(EQUIPMENT_CATEGORY_DROPDOWN_ID))->SetSelection(0);
  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_SEARCH_BAR_ID))->Clear();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEALTH_TEXT_ID))->SetLabel("Wealth on Hand : " + wealthStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEIGHT_TEXT_ID))->SetLabel(wxString::Format(wxT("Total Weight Held : %f lbs\nLight Load Capacity : %f lbs\nMedium Load Capacity : %f lbs\nHeavy Load Capacity : %f lbs"), weightHeld, lightCarryCapacity, mediumCarryCapacity, heavyCarryCapacity));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_DESCRIPTION_ID))->SetLabel("Description: ");
  static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->SetValue(false);
  static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->SetValue(true);
  static_cast<wxButton*>(wxWindow::FindWindowById(EQUIPMENT_PURCHASE_BUTTON_ID))->Enable();
  static_cast<wxButton*>(wxWindow::FindWindowById(EQUIPMENT_SELL_BUTTON_ID))->Enable();

  PopulateAvailBox();
  PopulateOwnedBox();
}

void EquipmentPage::UpdateEquipmentPage()
{
  int wealthVal = 0;
  std::string wealthStr = "0 cp";
  double lightCarryCapacity = 0.0;
  double mediumCarryCapacity = 0.0;
  double heavyCarryCapacity = 0.0;
  double weightHeld = 0.0;
  if (charPtr_ != NULL)
  {
    wealthVal = charPtr_->wealthCp();
    wealthStr = charPtr_->wealthString();
    weightHeld = charPtr_->weightHeld();
    if (charPtr_->race().id() != -1 && charPtr_->abilityScoresSet())
    {
      get_carry_capacities(charPtr_->getAbilityScore(Pathfinder::STRENGTH), charPtr_->race().charSize(), &lightCarryCapacity, &mediumCarryCapacity, &heavyCarryCapacity);
    }
  }

  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEALTH_TEXT_ID))->SetLabel("Wealth on Hand : " + wealthStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEIGHT_TEXT_ID))->SetLabel(wxString::Format(wxT("Total Weight Held : %f lbs\nLight Load Capacity : %f lbs\nMedium Load Capacity : %f lbs\nHeavy Load Capacity : %f lbs"), weightHeld, lightCarryCapacity, mediumCarryCapacity, heavyCarryCapacity));

  wxListCtrl* listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
  long item = -1;
  while ((item = listBox->GetNextItem(item)) != wxNOT_FOUND) {
     int item_val = Pathfinder::string_to_currency(listBox->GetItemText(item, PRICE_COLUMN).ToStdString());
     if (item_val <= wealthVal)
     {
       listBox->SetItemTextColour(item, *wxBLACK);
     }
     else
     {
       listBox->SetItemTextColour(item, *wxRED);
     }
  }
}

void EquipmentPage::OnItemSelected(wxListEvent& evt)
{
  wxListCtrl* listBox = nullptr;
  int equipListIdx = evt.GetIndex();
  std::shared_ptr<const Pathfinder::Equipment> itemPtr = nullptr;
  bool qualityOverride = false;
  if (evt.GetId() == EQUIPMENT_AVAILABLE_LIST_ID)
  {
    /* Deselect the owned equipment list */
    qualityOverride = static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue();
    listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
    long item = -1;
    while ((item = listBox->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
      listBox->SetItemState(item, 0, wxLIST_STATE_SELECTED);
    }
    listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
    itemPtr = equipMap_[availListIds_[equipListIdx]];
  }
  else if (evt.GetId() == EQUIPMENT_OWNED_LIST_ID)
  {
    /* Deselect the available equipment list */
    listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
    long item = -1;
    while ((item = listBox->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
      listBox->SetItemState(item, 0, wxLIST_STATE_SELECTED);
    }
    listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
    item = -1;
    while ((item = listBox->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
      if (item != equipListIdx)
        listBox->SetItemState(item, 0, wxLIST_STATE_SELECTED);
    }
    itemPtr = ownedItems_[equipListIdx];
  }
  else
  {
    wxMessageBox("Somehow ended up selecting an item that is not in the available list nor the owned list?");
    return;
  }

  UpdateItemDescription(itemPtr, qualityOverride);
  
  this->myLayout();
}

int EquipmentPage::FindOwnedIndex(std::shared_ptr<const Pathfinder::Equipment> itemPtr)
{
  int retValue = wxNOT_FOUND;

  for (size_t index = 0; index < ownedItems_.size(); index++)
  {
    if (itemPtr->getCategory() == ownedItems_[index]->getCategory())
    {
      if (itemPtr->getCategory() == Pathfinder::WEAPONS)
      {
        if (*std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr) == *std::reinterpret_pointer_cast<const Pathfinder::Weapon>(ownedItems_[index]))
        {
          retValue = static_cast<int>(index);
          break;
        }
      }
      else if (itemPtr->getCategory() == Pathfinder::ARMOR)
      {
        if (*std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr) == *std::reinterpret_pointer_cast<const Pathfinder::Armor>(ownedItems_[index]))
        {
          retValue = static_cast<int>(index);
          break;
        }
      }
      else
      {
        if (*std::reinterpret_pointer_cast<const Pathfinder::GeneralItem>(itemPtr) == *std::reinterpret_pointer_cast<const Pathfinder::GeneralItem>(ownedItems_[index]))
        {
          retValue = static_cast<int>(index);
          break;
        }
      }
    }
  }

  return retValue;
}

void EquipmentPage::ResizeCallback(wxSizeEvent& evt)
{
  if (itemDescWrapper_ != NULL)
  {
    int maxWidth = 0;
    wxTextCtrl* itemDescBox = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_DESCRIPTION_ID));
    itemDescBox->GetSize(&maxWidth, NULL);
    itemDescBox->Clear();
    *itemDescBox << itemDescWrapper_->UpdateWidth(maxWidth - 20);

    /*int item = wxNOT_FOUND;
    bool qualityOverride = false;
    std::shared_ptr<const Pathfinder::Equipment> itemPtr = nullptr;
    if ((item = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID))->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
      itemPtr = equipMap_[availListIds_[item]];
      qualityOverride = static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue();
    }
    else if ((item = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID))->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
      itemPtr = ownedItems_[item];
    }

    if (itemPtr != nullptr)
    {
      UpdateItemDescription(itemPtr, qualityOverride);
    }*/
  }
  evt.Skip();
}

void EquipmentPage::UpdateItemDescription(std::shared_ptr<const Pathfinder::Equipment> itemPtr, bool qualityOverride)
{
  std::string categoryName(Pathfinder::EQUIPMENT_CATEGORY_NAMES[itemPtr->getCategory()]);

  Pathfinder::characterSizeMarker size = (charPtr_ != NULL && charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  std::string priceString = Pathfinder::currency_to_string(itemPtr->getPrice(size, qualityOverride));
  std::string extraInfo = "";
  if (itemPtr->getCategory() == Pathfinder::WEAPONS)
  {
    std::shared_ptr<const Pathfinder::Weapon> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr);
    categoryName = std::string(Pathfinder::WEAPON_CATEGORY_NAMES[tmpPtr->getWeaponCategory()]);
    if (static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue())
    {
      Pathfinder::Weapon tmpWeapon = *tmpPtr;
      tmpWeapon.setMasterworkQuality(true);
      priceString = Pathfinder::currency_to_string(tmpWeapon.getPrice(size, qualityOverride));
    }
    extraInfo = " | Damage: " + tmpPtr->getDmg(size) + " | Crit: " + tmpPtr->getCrit() + " | Damage Type: " + std::string(Pathfinder::damageTypeStrings[tmpPtr->getDamageType()]);
    if (!tmpPtr->getRange().empty())
    {
      extraInfo += " | Range: " + tmpPtr->getRange();
    }
    if (!tmpPtr->getSpecial().empty())
    {
      extraInfo += " | Special: " + tmpPtr->getSpecial();
    }
  }
  else if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    std::shared_ptr<const Pathfinder::Armor> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr);
    categoryName = std::string(Pathfinder::ARMOR_CATEGORY_NAMES[tmpPtr->getArmorCategory()]);
    if (static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue())
    {
      Pathfinder::Armor tmpArmor = *tmpPtr;
      tmpArmor.setMasterworkQuality(true);
      priceString = Pathfinder::currency_to_string(tmpArmor.getPrice(size, qualityOverride));
    }
    extraInfo = " | Armor Bonus : " + std::to_string(tmpPtr->getArmorBonus()) + (tmpPtr->getMaxDexBonus() < 20 ? " | Max Dex Bonus: " + std::to_string(tmpPtr->getMaxDexBonus()) : "") +
      " | Armor Check Penalty: " + std::to_string(tmpPtr->getArmorCheckPenalty()) + " | Spell Fail Chance: " + std::to_string(tmpPtr->getSpellFailChance()) + "%";
    if (charPtr_ != NULL && charPtr_->race().speed() == 30 && tmpPtr->getSpeed30Ft() > 0)
    {
      extraInfo += " | Speed: " + std::to_string(tmpPtr->getSpeed30Ft()) + "ft";
    }
    if (charPtr_ != NULL && charPtr_->race().speed() == 20 && tmpPtr->getSpeed20Ft() > 0)
    {
      extraInfo += " | Speed: " + std::to_string(tmpPtr->getSpeed20Ft()) + "ft";
    }
  }

  std::string name = itemPtr->getName(size, qualityOverride);
  wxString weightStr = wxString::Format(wxT("%.6g lbs"), itemPtr->getWeight(size));
  wxString descriptionText = "Description:\n" + name + "\n" + categoryName + " | " + weightStr + " | " +
    priceString + extraInfo + "\n" + itemPtr->getDescription();
  
  wxTextCtrl* itemDescription = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_DESCRIPTION_ID));
  itemDescription->Clear();
  *itemDescription << itemDescWrapper_->UpdateText(descriptionText);
  
}

void EquipmentPage::SetupListBox(wxListCtrl* listBox, bool defaultList)
{
  listBox->ClearAll();
  listBox->InsertColumn(NAME_COLUMN, "Name", wxLIST_FORMAT_LEFT, -1);
  listBox->InsertColumn(PRICE_COLUMN, "Price", wxLIST_FORMAT_LEFT, -1);
  listBox->InsertColumn(WEIGHT_COLUMN, "Weight", wxLIST_FORMAT_LEFT, -1);
  if (!defaultList && currentCategory_ == Pathfinder::WEAPONS)
  {
    listBox->InsertColumn(WEAPON_CATEGORY_COLUMN, "Category", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(WEAPON_PROFICIENCY_COLUMN, "Proficiency", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(WEAPON_DAMAGE_COLUMN, "Damage", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(WEAPON_CRIT_COLUMN, "Crit", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(WEAPON_RANGE_COLUMN, "Range", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(WEAPON_TYPE_COLUMN, "Damage Type", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(WEAPON_SPECIAL_COLUMN, "Special", wxLIST_FORMAT_LEFT, -1);
  }
  else if (!defaultList && currentCategory_ == Pathfinder::ARMOR)
  {
    listBox->InsertColumn(ARMOR_CATEGORY_COLUMN, "Category", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(ARMOR_BONUS_COLUMN, "Armor Bonus", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(ARMOR_MAX_DEX_BONUS_COLUMN, "Max Dex Bonus", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(ARMOR_CHECK_PENALTY_COLUMN, "Armor Check Penalty", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(ARMOR_SPELL_FAIL_CHANCE_COLUMN, "Spell Fail Chance", wxLIST_FORMAT_LEFT, -1);
    listBox->InsertColumn(ARMOR_SPEED_COLUMN, "Speed", wxLIST_FORMAT_LEFT, -1);
  }

  this->myLayout();
}

void EquipmentPage::InsertListItem(wxListCtrl* listBox, std::shared_ptr<const Pathfinder::Equipment> itemPtr, bool colorUnaffordable, std::string namePrefix, long index)
{
  Pathfinder::characterSizeMarker size = (charPtr_ != NULL && charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  bool qualityOverride = colorUnaffordable && static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue();
  bool canAfford = (charPtr_ == NULL || charPtr_->wealthCp() >= itemPtr->getPrice(size, qualityOverride));
  bool proficient = (charPtr_ == NULL || charPtr_->checkProficiency(itemPtr));
  if (index == wxNOT_FOUND)
  {
    index = listBox->GetItemCount();
  }
  // Insert Name
  wxListItem listItem;
  listItem.SetId(index);
  listItem.SetColumn(NAME_COLUMN);
  listItem.SetText(namePrefix + itemPtr->getName(size, qualityOverride));
  listItem.SetTextColour(*wxBLACK);
  if(colorUnaffordable && !canAfford && proficient)
  {
    listItem.SetTextColour(*wxRED);
  }
  else if (colorUnaffordable && !canAfford && !proficient)
  {
    listItem.SetTextColour(*wxBLUE);
  }
  else if (!proficient)
  {
    listItem.SetTextColour(*wxLIGHT_GREY);
  }

  if (index == listBox->GetItemCount())
  {
    listBox->InsertItem(listItem);
  }
  else
  {
    listBox->SetItem(listItem);
  }
  // Insert Price
  listItem.SetColumn(PRICE_COLUMN);
  listItem.SetText(Pathfinder::currency_to_string(itemPtr->getPrice(size, qualityOverride)));
  listBox->SetItem(listItem);
  // Insert Weight
  listItem.SetColumn(WEIGHT_COLUMN);
  listItem.SetText(wxString::Format(wxT("%.6g Lbs."), itemPtr->getWeight(size)));
  listBox->SetItem(listItem);
  if (listBox->GetColumnCount() > WEAPON_CATEGORY_COLUMN && currentCategory_ == Pathfinder::WEAPONS)
  {
    std::shared_ptr<const Pathfinder::Weapon> weaponPtr = std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr);
    // Insert Category
    listItem.SetColumn(WEAPON_CATEGORY_COLUMN);
    listItem.SetText(Pathfinder::WEAPON_CATEGORY_NAMES[weaponPtr->getWeaponCategory()]);
    listBox->SetItem(listItem);
    // Insert Proficiency
    listItem.SetColumn(WEAPON_PROFICIENCY_COLUMN);
    listItem.SetText(Pathfinder::WEAPON_PROFICIENCY_NAMES[weaponPtr->getWeaponProficiency()]);
    listBox->SetItem(listItem);
    //Insert Damage
    listItem.SetColumn(WEAPON_DAMAGE_COLUMN);
    listItem.SetText(weaponPtr->getDmg(size));
    listBox->SetItem(listItem);
    //Insert Crit
    listItem.SetColumn(WEAPON_CRIT_COLUMN);
    listItem.SetText(weaponPtr->getCrit());
    listBox->SetItem(listItem);
    //Insert Range
    listItem.SetColumn(WEAPON_RANGE_COLUMN);
    listItem.SetText(weaponPtr->getRange());
    listBox->SetItem(listItem);
    //Insert Damage Type
    listItem.SetColumn(WEAPON_TYPE_COLUMN);
    listItem.SetText(std::string(Pathfinder::damageTypeStrings[weaponPtr->getDamageType()]));
    listBox->SetItem(listItem);
    //Insert Special
    listItem.SetColumn(WEAPON_SPECIAL_COLUMN);
    listItem.SetText(weaponPtr->getSpecial());
    listBox->SetItem(listItem);
  }
  else if (listBox->GetColumnCount() > ARMOR_CATEGORY_COLUMN && currentCategory_ == Pathfinder::ARMOR)
  {
    std::shared_ptr<const Pathfinder::Armor> armorPtr = std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr);
    // Insert Category
    listItem.SetColumn(ARMOR_CATEGORY_COLUMN);
    listItem.SetText(Pathfinder::ARMOR_CATEGORY_NAMES[armorPtr->getArmorCategory()]);
    listBox->SetItem(listItem);
    //Insert Armor Bonus
    listItem.SetColumn(ARMOR_BONUS_COLUMN);
    listItem.SetText(std::to_string(armorPtr->getArmorBonus()));
    listBox->SetItem(listItem);
    //Insert Max Dex Bonus
    listItem.SetColumn(ARMOR_MAX_DEX_BONUS_COLUMN);
    std::string str = " - ";
    if (armorPtr->getMaxDexBonus() < 20)
    {
      str = std::to_string(armorPtr->getMaxDexBonus());
    }
    listItem.SetText(str);
    listBox->SetItem(listItem);
    //Insert Armor Check Penalty
    listItem.SetColumn(ARMOR_CHECK_PENALTY_COLUMN);
    listItem.SetText(std::to_string(armorPtr->getArmorCheckPenalty()));
    listBox->SetItem(listItem);
    //Insert Spell Fail Chance
    listItem.SetColumn(ARMOR_SPELL_FAIL_CHANCE_COLUMN);
    str = " - ";
    if (armorPtr->getSpellFailChance() > 0)
    {
      str = std::to_string(armorPtr->getSpellFailChance()) + "%";
    }
    listItem.SetText(str);
    listBox->SetItem(listItem);
    //Insert Speed
    listItem.SetColumn(ARMOR_SPEED_COLUMN);
    str = " - ";//std::to_string(armorPtr->getSpeed30Ft()) + "ft";
    if (charPtr_ != NULL && armorPtr->getSpeed20Ft() > 0 && charPtr_->race().speed() == 20)
    {
      str = std::to_string(armorPtr->getSpeed20Ft()) + "ft";
    }
    else if(charPtr_ != NULL && armorPtr->getSpeed30Ft() > 0 && charPtr_->race().speed() == 30)
    {
      str = std::to_string(armorPtr->getSpeed30Ft()) + "ft";
    }
    listItem.SetText(str);
    listBox->SetItem(listItem);
  }
}

void EquipmentPage::PopulateAvailBox()
{
  wxListCtrl* availList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
  availList->ClearAll();
  availListIds_.clear();
  
  Pathfinder::characterSizeMarker size = (charPtr_ != NULL && charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);

  this->SetupListBox(availList);

  for (auto equipIter = equipMap_.begin(); equipIter != equipMap_.end(); ++equipIter)
  {
    if((currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == equipIter->second->getCategory()) &&
      CheckFilterString(equipIter->second))
    {
      this->InsertListItem(availList, equipIter->second);
      availListIds_.push_back(equipIter->first);
    }
  }
  this->myLayout();
}

void EquipmentPage::PopulateOwnedBox()
{
  if (charPtr_ == NULL)
  {
    /* If no character, nothing will be here to filter! */
    return;
  }

  wxListCtrl* ownedList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
  bool skipFilter = static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->GetValue();

  ownedList->ClearAll();
  ownedItems_.clear();

  this->SetupListBox(ownedList, skipFilter);

  std::unordered_map<const Pathfinder::GeneralItem, int, Pathfinder::myItemHash> equipments;
  std::unordered_map<const Pathfinder::Weapon, int, Pathfinder::myWeaponHash> weapons;
  std::unordered_map<const Pathfinder::Armor, int, Pathfinder::myArmorHash> armors;
  charPtr_->getEquipmentList(equipments, weapons, armors);
  Pathfinder::characterSizeMarker size = (charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  int count = 0;
  /* Filter the general items */
  for (auto iter = equipments.begin(); iter != equipments.end(); ++iter)
  {
    std::shared_ptr<const Pathfinder::Equipment> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::GeneralItem>(iter->first));
    if (skipFilter || ((currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == iter->first.getCategory()) &&
        CheckFilterString(tmpPtr)))
    {
      std::string namePrefix = "";
      if (iter->second > 1)
      {
        namePrefix = std::to_string(iter->second) + "x ";
      }
      this->InsertListItem(ownedList, tmpPtr, false, namePrefix);
      ownedItems_.push_back(tmpPtr);
    }
  }

  /* Filter the weapons*/
  if (skipFilter || (currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == Pathfinder::WEAPONS))
  {
    for (auto iter = weapons.begin(); iter != weapons.end(); ++iter)
    {
      std::shared_ptr<const Pathfinder::Equipment> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::Weapon>(iter->first));
      if(skipFilter || CheckFilterString(tmpPtr))
      {
        std::string namePrefix = "";
        if (iter->second > 1)
        {
          namePrefix = std::to_string(iter->second) + "x ";
        }
        this->InsertListItem(ownedList, tmpPtr, false, namePrefix);
        ownedItems_.push_back(tmpPtr);
      }
    }
  }

  /* Filter the armor*/
  if (skipFilter || (currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == Pathfinder::ARMOR))
  {
    for (auto iter = armors.begin(); iter != armors.end(); ++iter)
    {
      std::shared_ptr<const Pathfinder::Equipment> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::Armor>(iter->first));
      if (skipFilter || CheckFilterString(tmpPtr))
      {
        std::string namePrefix = "";
        if (iter->second > 1)
        {
          namePrefix = std::to_string(iter->second) + "x ";
        }
        this->InsertListItem(ownedList, tmpPtr, false, namePrefix);
        ownedItems_.push_back(tmpPtr);
      }
    }
  }
}

void EquipmentPage::PurchaseItemButtonPress(wxCommandEvent& evt)
{
  /* get the selected item index */
  wxListCtrl* ownedBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
  int availListIdx = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID))->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (charPtr_ == NULL)
  {
    wxMessageBox("No character loaded yet");
    return;
  }
  else if (availListIdx == wxNOT_FOUND)
  {
    wxMessageBox("You must select an item in the available items list");
    return;
  }

  int itemIdx = availListIds_[availListIdx];
  std::shared_ptr<const Pathfinder::Equipment> itemPtr = equipMap_[itemIdx];

  std::string name = "";
  int count = 0;
  int price = 0;
  Pathfinder::characterSizeMarker size = charPtr_->race().charSize();
  if (itemPtr->getCategory() == Pathfinder::WEAPONS)
  {
    Pathfinder::Weapon weapon(*std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr));
    weapon.setMasterworkQuality(static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue());
    price = weapon.getPrice(size);
    if (price > charPtr_->wealthCp())
    {
      wxMessageBox(wxString::Format(wxT("You do not have enough money to purchase this item [%s] : [%s] vs [%s]"), weapon.getName(size).c_str(), Pathfinder::currency_to_string(price).c_str(), charPtr_->wealthString().c_str()));
      return;
    }
    name = weapon.getName(size);
    count = charPtr_->addWeapon(weapon);
    itemPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::Weapon>(weapon));
  }
  else if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    Pathfinder::Armor armor(*std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr));
    armor.setMasterworkQuality(static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue());
    price = armor.getPrice(size);
    if (price > charPtr_->wealthCp())
    {
      wxMessageBox(wxString::Format(wxT("You do not have enough money to purchase this item [%s] : [%s] vs [%s]"), armor.getName(size).c_str(), Pathfinder::currency_to_string(price).c_str(), charPtr_->wealthString().c_str()));
      return;
    }
    name = armor.getName(size);
    count = charPtr_->addArmor(armor);
    itemPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::Armor>(armor));
  }
  else
  {
    Pathfinder::GeneralItem item(*std::reinterpret_pointer_cast<const Pathfinder::GeneralItem>(itemPtr));
    price = item.getPrice(size);
    if (price > charPtr_->wealthCp())
    {
      wxMessageBox(wxString::Format(wxT("You do not have enough money to purchase this item [%s] : [%s] vs [%s]"), item.getName(size).c_str(), Pathfinder::currency_to_string(price).c_str(), charPtr_->wealthString().c_str()));
      return;
    }
    name = item.getName(size);
    count = charPtr_->addItem(item);
    itemPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::GeneralItem>(item));
  }

  charPtr_->subtractWealth(price);

  if (count == 1)
  {
    /* first copy of this item, so append it to the end of the owned box*/
    this->InsertListItem(ownedBox, itemPtr, false);
    ownedItems_.push_back(itemPtr);
  }
  else
  {
    int ownedIdx = this->FindOwnedIndex(itemPtr);
    if (ownedIdx == wxNOT_FOUND)
    {
      wxMessageBox("Unable to find this item in your owned list, but expected it to be already owned");
      return;
    }
    std::string namePrefix = std::to_string(count) + "x ";
    this->InsertListItem(ownedBox, itemPtr, false, namePrefix, ownedIdx);
  }

  this->UpdateEquipmentPage();
  this->myLayout();
  if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    /* May need to update the AC values on other pages */
    evt.Skip();
  }
}

void EquipmentPage::SellItemButtonPress(wxCommandEvent& evt)
{
  /* get the selected item index */
  wxListCtrl* ownedBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
  int ownedListIdx = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID))->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (charPtr_ == NULL)
  {
    wxMessageBox("No character loaded yet");
    return;
  }
  else if (ownedListIdx == wxNOT_FOUND)
  {
    wxMessageBox("You must select an item in the owned items list");
    return;
  }

  std::shared_ptr<const Pathfinder::Equipment> itemPtr = ownedItems_[ownedListIdx];

  Pathfinder::characterSizeMarker size = charPtr_->race().charSize();
  std::string name = itemPtr->getName(size);
  int price = itemPtr->getPrice(size);
  int count = 0;
  if (itemPtr->getCategory() == Pathfinder::WEAPONS)
  {
    Pathfinder::Weapon weapon(*std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr));
    count = charPtr_->removeWeapon(weapon);
    itemPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::Weapon>(weapon));
  }
  else if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    Pathfinder::Armor armor(*std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr));
    count = charPtr_->removeArmor(armor);
    itemPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::Armor>(armor));
  }
  else
  {
    Pathfinder::GeneralItem item(*std::reinterpret_pointer_cast<const Pathfinder::GeneralItem>(itemPtr));
    itemPtr = std::reinterpret_pointer_cast<const Pathfinder::Equipment>(std::make_shared<const Pathfinder::GeneralItem>(item));
  }

  if (count < 0)
  {
    /* Catch the case where charPtr_->remove*() didn't find a matching element in the character's corresponding item map */
    wxMessageBox("Something has gone wrong, you tried to sell an item you don't seem to have");
    count = 0; // Do this so the string will be removed from owned list below
  }
  else
  {
    /* sale was successful */
    charPtr_->addWealth(price);
  }

  if (count == 0)
  {
    /* Last copy of this item was removed, so delete it from the owned box and decrement everything else after it*/
    ownedBox->DeleteItem(ownedListIdx);
    ownedItems_.erase(ownedItems_.begin() + ownedListIdx);
  }
  else
  {
    /* Still some items remaining */
    std::string namePrefix = "";
    if (count > 1)
    {
      namePrefix = std::to_string(count) + "x ";
    }
    this->InsertListItem(ownedBox, itemPtr, false, namePrefix, ownedListIdx);
  }

  this->UpdateEquipmentPage();
  this->myLayout();

  if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    /* May need to update the AC values on other pages */
    evt.Skip();
  }
}

void EquipmentPage::AddMoneyButtonPress(wxCommandEvent& evt)
{
  if (charPtr_ == NULL)
  {
    wxMessageBox("No character loaded yet");
    return;
  }
  wxTextEntryDialog *dialog = new wxTextEntryDialog(this,
      "Enter amount of money to earn, of the form \"A pp B gp C sp D cp\" (You don't need a value for each denomination)", "Add Money Window", wxEmptyString, wxTextEntryDialogStyle);
  dialog->ShowModal();
  std::string moneyStr = dialog->GetValue().ToStdString();

  int value = Pathfinder::string_to_currency(moneyStr);
  charPtr_->addWealth(value);

  this->UpdateEquipmentPage();
}

void EquipmentPage::OnCategorySelected(wxCommandEvent& evt)
{
  int selection = static_cast<wxChoice*>(wxWindow::FindWindowById(EQUIPMENT_CATEGORY_DROPDOWN_ID))->GetSelection();
  if (selection == currentCategory_)
  {
    /* Selection didn't actually change, don't do anything */
    return;
  }
  currentCategory_ = selection;
  
  this->PopulateAvailBox();
  if(!static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->GetValue())
  {
    this->PopulateOwnedBox();
  }
}

void EquipmentPage::OnTextEntered(wxCommandEvent& evt)
{
  std::string lower_str = static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_SEARCH_BAR_ID))->GetValue().ToStdString();
  for (size_t idx = 0; idx < lower_str.size(); idx++)
  {
    lower_str[idx] = std::tolower(lower_str[idx]);
  }

  filterString_ = lower_str;

  this->PopulateAvailBox();
  if (static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->GetValue() == 0)
  {
    this->PopulateOwnedBox();
  }
  return;
}

bool EquipmentPage::CheckFilterString(std::shared_ptr<const Pathfinder::Equipment> itemPtr)
{
  if (filterString_.empty())
  {
    return true;
  }

  Pathfinder::characterSizeMarker size = (charPtr_ != NULL && charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  std::string nameStr = itemPtr->getName(size);
  std::string catName = "";
  if (itemPtr->getCategory() == Pathfinder::WEAPONS)
  {
    catName = std::string(Pathfinder::WEAPON_CATEGORY_NAMES[std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr)->getWeaponCategory()]);
  }
  if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    catName = std::string(Pathfinder::ARMOR_CATEGORY_NAMES[std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr)->getArmorCategory()]);
  }

  for (size_t idx = 0; idx < nameStr.size(); idx++)
  {
    nameStr[idx] = std::tolower(nameStr[idx]);
  }
  for (size_t idx = 0; idx < catName.size(); idx++)
  {
    catName[idx] = std::tolower(catName[idx]);
  }

  return((nameStr.find(filterString_) != std::string::npos) || (catName.find(filterString_) != std::string::npos));
}

void EquipmentPage::OnLockBoxChecked(wxCommandEvent& evt)
{
  bool filterActive = (currentCategory_ != Pathfinder::ALL_EQUIPMENT || !filterString_.empty());
  /* If we are turning OFF filtering (i.e. clicking ON the button), and category is currently either weapons or armor, then we need to override the setup and just to default columns */

  if(filterActive)
  {
    /* If filtering is being done, either need to apply it now or un-apply it */
    this->PopulateOwnedBox();
  }
}

void EquipmentPage::OnMasterworkBoxChecked(wxCommandEvent& evt)
{
  wxListCtrl* availList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
  
  for(int index = 0; index < availListIds_.size(); index++)
  {
    /* Just repopulate the whole available equipment list, forcing the recalc of prices and recoloring */
    this->InsertListItem(availList, equipMap_[availListIds_[index]], true, "", index);
  }
}

void EquipmentPage::MouseOverEvent(wxMouseEvent& evt)
{
  wxListCtrl* listBox = static_cast<wxListCtrl*>(wxWindow::FindWindowById(evt.GetId()));
  wxPoint pos = evt.GetPosition();
  int flags = wxLIST_HITTEST_ONITEM;
  int item = listBox->HitTest(pos, flags);
  if (item != wxNOT_FOUND)
  {
    if (evt.Moving())
    {
      wxColour color = listBox->GetItemTextColour(item);
      wxString tooltipText = "";
      if (color == *wxRED)
      {
        listBox->SetToolTip("You cannot afford this item");
      }
      else if (color == *wxLIGHT_GREY)
      {
        listBox->SetToolTip("You are not proficient with this type of item");
      }
      else if (color == *wxBLUE)
      {
        listBox->SetToolTip("You cannot afford this item, and you are not proficient with this type of item");
      }
      else
      {
        listBox->UnsetToolTip();
      } 
    }
    else if (evt.Leaving())
    {
      listBox->UnsetToolTip();
    }
  }
  else
  {
    listBox->UnsetToolTip();
  }
}

void EquipmentPage::myLayout()
{
  int totalWidth = GetClientSize().GetWidth();

  int width = totalWidth/2 - 5;
  wxListCtrl* availList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
  wxListCtrl* ownedList = static_cast<wxListCtrl*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));

  if (availList == NULL || ownedList == NULL)
  {
    return;
  }
  wxPoint ownedPos;
  ownedList->GetPosition(&(ownedPos.x), &(ownedPos.y));
  availList->GetParent()->Layout();
  wxSize availSize = availList->GetSize();
  availSize.SetWidth(width);
  availList->SetSize(availSize);
  wxSize ownedSize = ownedList->GetSize();
  ownedSize.SetWidth(width);
  ownedList->SetPosition(ownedPos);
  ownedList->SetSize(ownedSize);
}