#include <unordered_map>

#include "EquipmentPage.h"

EquipmentPage::EquipmentPage(wxNotebook* parentNotebook, Pathfinder::Character* currChar) : wxPanel(parentNotebook), charPtr_(currChar)
{
  this->SetBackgroundColour(0xE5E5E5);

  wxBoxSizer* vboxOverall = new wxBoxSizer(wxVERTICAL); /* will contain the various vertical sizers */ 
  wxBoxSizer* hbox0 = new wxBoxSizer(wxHORIZONTAL);/* category dropdown and label, search bar */
  wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);/* Equipment lists */
  wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);/* Description Text */

  wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL); /* Category dropdown and label, search bar and label */
  /* add a spacer */
  hbox0->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND);

  /* Add the category dropdown and label */
  wxStaticText* categoryLabel = new wxStaticText(this, EQUIPMENT_CATEGORY_LABEL_ID, wxT("Category:"));
  hbox0->Add(categoryLabel, 1, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  wxChoice* categoryDropDown = new wxChoice(this, EQUIPMENT_CATEGORY_DROPDOWN_ID, wxDefaultPosition, wxDefaultSize, 0);
  for (int categoryIdx = 0; categoryIdx < Pathfinder::NUM_EQUIPMENT_CATEGORIES; categoryIdx++)
  {
    categoryDropDown->AppendString(Pathfinder::EQUIPMENT_CATEGORY_NAMES[categoryIdx]);
  }
  categoryDropDown->SetSelection(0);
  categoryDropDown->Bind(wxEVT_CHOICE, &EquipmentPage::OnCategorySelected, this);
  hbox0->Add(categoryDropDown, 1, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  /* add a spacer to separate the two */
  hbox0->Add(new wxPanel(this, wxID_ANY), 3, wxEXPAND);
  /* Add the search bar and label */
  wxStaticText* searchLabel = new wxStaticText(this, EQUIPMENT_SEARCH_LABEL_ID, wxT("Search:"));
  hbox0->Add(searchLabel, 0, wxLEFT | wxRIGHT | wxFIXED_MINSIZE, 10);
  wxTextCtrl* searchTxt = new wxTextCtrl(this, EQUIPMENT_SEARCH_BAR_ID);
  //searchText->Bind(wxEVT_TEXT_ENTER, &EquipmentPage::OnTextEntered, this);
  hbox0->Add(searchTxt, 1, wxEXPAND | wxALIGN_LEFT, 10);

  /* add a spacer */
  hbox0->Add(new wxPanel(this, wxID_ANY), 1, wxEXPAND);
  
  vboxOverall->Add(hbox0, 1, wxEXPAND | wxALIGN_LEFT, 10);


  /* Available Equipment List */
  equipMap_ = Pathfinder::PFTable::get_equipment_map();
  wxArrayString dummyStr;
  for (auto equipIter = equipMap_.begin(); equipIter != equipMap_.end(); ++equipIter)
  {
    dummyStr.Add(equipIter->second->getName(Pathfinder::SIZE_MEDIUM));
    availListIds_.push_back(equipIter->first);
  }
  wxBoxSizer* vboxAvail = new wxBoxSizer(wxVERTICAL);
  wxStaticText* availEquipLabel = new wxStaticText(this, EQUIPMENT_AVAILABLE_LABEL_ID, wxT("Available Equipment:"));
  vboxAvail->Add(availEquipLabel, 0, wxBOTTOM, 5);
  wxListBox* availEquipList = new wxListBox(this, EQUIPMENT_AVAILABLE_LIST_ID, wxDefaultPosition, wxDefaultSize, dummyStr, wxLB_NEEDED_SB);
  availEquipList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &EquipmentPage::OnItemSelected, this);
  vboxAvail->Add(availEquipList, 1, wxEXPAND, 0);


  wxBoxSizer* hboxAvailBtns = new wxBoxSizer(wxHORIZONTAL);
  wxButton* buyButton = new wxButton(this, EQUIPMENT_PURCHASE_BUTTON_ID, wxT("Purchase"));
  buyButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &EquipmentPage::PurchaseItemButtonPress, this);
  buyButton->Disable();
  hboxAvailBtns->Add(buyButton);
  wxCheckBox* mwCheck = new wxCheckBox(this, EQUIPMENT_MASTERWORK_CHECKBOX_ID, wxT("Master Work"));
  //mwCheck->Bind(wxEVT_CHECKBOX, &EquipmentPage::OnMasterworkBoxChecked, this);
  hboxAvailBtns->Add(mwCheck);
  vboxAvail->Add(hboxAvailBtns);
  wxStaticText* moneyText = new wxStaticText(this, EQUIPMENT_WEALTH_TEXT_ID, wxT("Wealth on Hand: 0 cp"));
  vboxAvail->Add(moneyText);
  wxButton* addMoneyButton = new wxButton(this, EQUIPMENT_ADD_MONEY_BUTTON_ID, wxT("Add Money"));
  addMoneyButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &EquipmentPage::AddMoneyButtonPress, this);
  vboxAvail->Add(addMoneyButton);
  hbox2->Add(vboxAvail, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 10);

  /* Owned Equipment List */
  wxBoxSizer* vboxOwned = new wxBoxSizer(wxVERTICAL);
  wxStaticText* ownedEquipmentLabel = new wxStaticText(this, EQUIPMENT_OWNED_LABEL_ID, wxT("Owned Equipment:"));
  vboxOwned->Add(ownedEquipmentLabel, 0, wxBOTTOM, 5);
  dummyStr.Clear();
  wxListBox* ownedEquipmentList = new wxListBox(this, EQUIPMENT_OWNED_LIST_ID, wxDefaultPosition, wxDefaultSize, dummyStr, wxLB_NEEDED_SB);
  ownedEquipmentList->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &EquipmentPage::OnItemSelected, this);
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

  hbox2->Add(vboxOwned, 1, wxEXPAND | wxUP | wxDOWN | wxLEFT, 10);

  vboxOverall->Add(hbox2, 5, wxEXPAND);

  /* Description Box */
  wxStaticText* itemDescription = new wxStaticText(this, EQUIPMENT_DESCRIPTION_ID, wxT("Description:"));
  itemDescription->SetBackgroundColour(*wxWHITE);
  vboxOverall->Add(itemDescription, 1, wxEXPAND, 10);

  this->SetSizer(vboxOverall);
}

void EquipmentPage::ResetPage(Pathfinder::Character* currChar)
{
  charPtr_ = currChar;

  currentCategory_ = Pathfinder::ALL_EQUIPMENT;
  ownedListContents_.clear();

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

  static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID))->Clear();
  static_cast<wxChoice*>(wxWindow::FindWindowById(EQUIPMENT_CATEGORY_DROPDOWN_ID))->SetSelection(0);
  static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_SEARCH_BAR_ID))->Clear();
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEALTH_TEXT_ID))->SetLabel("Wealth on Hand : " + wealthStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEIGHT_TEXT_ID))->SetLabel(wxString::Format(wxT("Total Weight Held : %f lbs\nLight Load Capacity : %f lbs\nMedium Load Capacity : %f lbs\nHeavy Load Capacity : %f lbs"), weightHeld, lightCarryCapacity, mediumCarryCapacity, heavyCarryCapacity));
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_DESCRIPTION_ID))->SetLabel("Description: ");
  static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->SetValue(false);
  static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->SetValue(false);
  static_cast<wxButton*>(wxWindow::FindWindowById(EQUIPMENT_PURCHASE_BUTTON_ID))->Enable();
  static_cast<wxButton*>(wxWindow::FindWindowById(EQUIPMENT_SELL_BUTTON_ID))->Enable();

  FilterAvailBox();
  FilterOwnedBox();
}

void EquipmentPage::UpdateEquipmentPage()
{
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

  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEALTH_TEXT_ID))->SetLabel("Wealth on Hand : " + wealthStr);
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_WEIGHT_TEXT_ID))->SetLabel(wxString::Format(wxT("Total Weight Held : %f lbs\nLight Load Capacity : %f lbs\nMedium Load Capacity : %f lbs\nHeavy Load Capacity : %f lbs"), weightHeld, lightCarryCapacity, mediumCarryCapacity, heavyCarryCapacity));
}

void EquipmentPage::OnItemSelected(wxCommandEvent& evt)
{
  wxListBox* listBox = nullptr;
  int equipListIdx = evt.GetSelection();
  std::shared_ptr<const Pathfinder::Equipment> itemPtr = nullptr;
  if (evt.GetId() == EQUIPMENT_AVAILABLE_LIST_ID)
  {
    static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID))->SetSelection(wxNOT_FOUND);
    listBox = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
    itemPtr = equipMap_[availListIds_[equipListIdx]];
  }
  else if (evt.GetId() == EQUIPMENT_OWNED_LIST_ID)
  {
    static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID))->SetSelection(wxNOT_FOUND);
    listBox = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
    itemPtr = ownedItems_[equipListIdx];
  }
  else
  {
    wxMessageBox("Somehow ended up selecting an item that is not in the available list nor the owned list?");
    return;
  }

  UpdateItemDescription(itemPtr);
  
  listBox->GetParent()->Layout();
}

void EquipmentPage::UpdateItemDescription(std::shared_ptr<const Pathfinder::Equipment> itemPtr)
{
  std::string categoryName(Pathfinder::EQUIPMENT_CATEGORY_NAMES[itemPtr->getCategory()]);

  Pathfinder::characterSizeMarker size = (charPtr_ != NULL && charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  if (itemPtr->getCategory() == Pathfinder::WEAPONS)
  {
    std::shared_ptr<const Pathfinder::Weapon> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Weapon>(itemPtr);
    categoryName = std::string(Pathfinder::WEAPON_CATEGORY_NAMES[tmpPtr->getWeaponCategory()]);
  }
  else if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    std::shared_ptr<const Pathfinder::Armor> tmpPtr = std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr);
    categoryName = std::string(Pathfinder::ARMOR_CATEGORY_NAMES[tmpPtr->getArmorCategory()]);
  }

  std::string name = itemPtr->getName(size);
  wxString weightStr = wxString::Format(wxT("%.6g lbs"), itemPtr->getWeight(size));
  wxString descriptionText = "Description:\n" + name + "\n" + categoryName + " | " + weightStr + " | " +
    Pathfinder::currency_to_string(itemPtr->getPrice(size)) + "\n" + itemPtr->getDescription();
  
  static_cast<wxStaticText*>(wxWindow::FindWindowById(EQUIPMENT_DESCRIPTION_ID))->SetLabel(descriptionText);
}

void EquipmentPage::FilterAvailBox()
{
  wxListBox* availList = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID));
  availList->Clear();
  availListIds_.clear();
  
  Pathfinder::characterSizeMarker size = (charPtr_ != NULL && charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  for (auto equipIter = equipMap_.begin(); equipIter != equipMap_.end(); ++equipIter)
  {
    if(currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == equipIter->second->getCategory())
    {
      availList->AppendString(equipIter->second->getName(size));
      availListIds_.push_back(equipIter->first);
    }
  }
  availList->GetParent()->Layout();
}

void EquipmentPage::FilterOwnedBox()
{
  if (charPtr_ == NULL)
  {
    /* If no character, nothing will be here to filter! */
    return;
  }

  wxListBox* ownedList = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
  bool filterOwned = static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->GetValue() == 0;

  ownedList->Clear();
  ownedListContents_.clear();
  ownedItems_.clear();
  std::unordered_map<const Pathfinder::GeneralItem, int, Pathfinder::myItemHash> equipments;
  std::unordered_map<const Pathfinder::Weapon, int, Pathfinder::myWeaponHash> weapons;
  std::unordered_map<const Pathfinder::Armor, int, Pathfinder::myArmorHash> armors;
  charPtr_->getEquipmentList(equipments, weapons, armors);
  Pathfinder::characterSizeMarker size = (charPtr_->race().id() != -1 ? charPtr_->race().charSize() : Pathfinder::SIZE_MEDIUM);
  int count = 0;
  /* Filter the general items */
  for (auto iter = equipments.begin(); iter != equipments.end(); ++iter)
  {
    if (!filterOwned || (currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == iter->first.getCategory()))
    {
      //std::string nameStr(wxString::Format(wxT("%dx %s"), iter->second, iter->first.getName().c_str()).c_str());
      std::string nameStr = iter->first.getName(size);
      if (iter->second > 1)
      {
        nameStr = std::to_string(iter->second) + "x " + nameStr;
      }
      ownedList->AppendString(nameStr);
      ownedListContents_.emplace(std::make_pair(nameStr, count++));
      ownedItems_.push_back(std::make_shared<const Pathfinder::GeneralItem>(iter->first));
    }
  }

  /* Filter the weapons*/
  if (!filterOwned || (currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == Pathfinder::WEAPONS))
  {
    for (auto iter = weapons.begin(); iter != weapons.end(); ++iter)
    {
      //std::string nameStr(wxString::Format(wxT("%dx %s"), iter->second, iter->first.getName(size).c_str()).c_str());
      std::string nameStr = iter->first.getName(size);
      if (iter->second > 1)
      {
        nameStr = std::to_string(iter->second) + "x " + nameStr;
      }
      ownedList->AppendString(nameStr);
      ownedListContents_.emplace(std::make_pair(nameStr, count++));
      ownedItems_.push_back(std::make_shared<const Pathfinder::Weapon>(iter->first));
    }
  }

  /* Filter the armor*/
  if (!filterOwned || (currentCategory_ == Pathfinder::ALL_EQUIPMENT || static_cast<Pathfinder::equipmentCategoryMarker>(currentCategory_) == Pathfinder::ARMOR))
  {
    for (auto iter = armors.begin(); iter != armors.end(); ++iter)
    {
      //std::string nameStr(wxString::Format(wxT("%dx %s"), iter->second, iter->first.getName(size).c_str()));
      std::string nameStr = iter->first.getName(size);
      if (iter->second > 1)
      {
        nameStr = std::to_string(iter->second) + "x " + nameStr;
      }
      ownedList->AppendString(nameStr);
      ownedListContents_.emplace(std::make_pair(nameStr, count++));
      ownedItems_.push_back(std::make_shared<const Pathfinder::Armor>(iter->first));
    }
  }
}

void EquipmentPage::PurchaseItemButtonPress(wxCommandEvent& evt)
{
  /* get the selected item index */
  wxListBox* ownedBox = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
  int availListIdx = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_AVAILABLE_LIST_ID))->GetSelection();
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
      wxMessageBox(wxString::Format(wxT("You do not have enough money to purchase this item [%s] : [%d] vs [%d]"), weapon.getName(size).c_str(), price, charPtr_->wealthCp()));
      return;
    }
    name = weapon.getName(size);
    count = charPtr_->addWeapon(weapon);
  }
  else if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    Pathfinder::Armor armor(*std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr));
    armor.setMasterworkQuality(static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue());
    price = armor.getPrice(size);
    if (price > charPtr_->wealthCp())
    {
      wxMessageBox(wxString::Format(wxT("You do not have enough money to purchase this item [%s] : [%d] vs [%d]"), armor.getName(size).c_str(), price, charPtr_->wealthCp()));
      return;
    }
    name = armor.getName(size);
    count = charPtr_->addArmor(armor);
  }
  else
  {
    Pathfinder::GeneralItem item(*std::reinterpret_pointer_cast<const Pathfinder::GeneralItem>(itemPtr));
    price = item.getPrice(size);
    if (price > charPtr_->wealthCp())
    {
      wxMessageBox(wxString::Format(wxT("You do not have enough money to purchase this item [%s] : [%d] vs [%d]"), item.getName(size).c_str(), price, charPtr_->wealthCp()));
      return;
    }
    name = item.getName(size);
    count = charPtr_->addItem(item);
  }

  charPtr_->subtractWealth(price);

  if (count == 1)
  {
    /* first copy of this item, so append it to the end of the owned box*/
    ownedListContents_.emplace(std::make_pair(name, ownedListContents_.size()));
    ownedBox->AppendString(name);
  }
  else
  {
    ownedBox->Delete(ownedListContents_[name]);
    ownedBox->Insert(wxString::Format(wxT("%dx %s"), count, name.c_str()), ownedListContents_[name]);
  }

  this->UpdateEquipmentPage();
  ownedBox->GetParent()->Layout();
}

void EquipmentPage::SellItemButtonPress(wxCommandEvent& evt)
{
  /* get the selected item index */
  wxListBox* ownedBox = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID));
  int ownedListIdx = static_cast<wxListBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LIST_ID))->GetSelection();
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
    weapon.setMasterworkQuality(static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue());
    price = weapon.getPrice(size);
    name = weapon.getName(size);
    count = charPtr_->removeWeapon(weapon);
  }
  else if (itemPtr->getCategory() == Pathfinder::ARMOR)
  {
    Pathfinder::Armor armor(*std::reinterpret_pointer_cast<const Pathfinder::Armor>(itemPtr));
    armor.setMasterworkQuality(static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_MASTERWORK_CHECKBOX_ID))->GetValue());
    price = armor.getPrice(size);
    name = armor.getName(size);
    count = charPtr_->removeArmor(armor);
  }
  else
  {
    Pathfinder::GeneralItem item(*std::reinterpret_pointer_cast<const Pathfinder::GeneralItem>(itemPtr));
    price = item.getPrice(size);
    name = item.getName(size);
    count = charPtr_->removeItem(item);
  }

  if (count < 0)
  {
    /* Catch the case where charPtr_->remove*() didn't find a matching element in the character's corresponding item map */
    wxMessageBox("Something has gone wrong, you tried to sell an item you didn't have");
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
    ownedBox->Delete(ownedListIdx);
    ownedItems_.erase(ownedItems_.begin() + ownedListIdx);
    for (auto iter = ownedListContents_.begin(); iter != ownedListContents_.end(); )
    {
      if(iter->second == ownedListIdx)
      {
        iter = ownedListContents_.erase(iter);
      }
      else if(iter->second > ownedListIdx)
      {
        iter->second--;
        ++iter;
      }
      else
      {
        ++iter;
      }
    }
  }
  else
  {
    /* Still some items remaining */
    /* Pull out the string that was there and delete it from ownedListContents_*/
    std::string nameStr = ownedBox->GetString(ownedListIdx).ToStdString();
    ownedListContents_.erase(nameStr);

    /* Delete the string from the box*/
    ownedBox->Delete(ownedListIdx);
    nameStr = name;
    if (count > 1)
    {
      nameStr = std::to_string(count) + "x " + nameStr;
    }
    /* Now inject the new string in its place */
    ownedBox->Insert(nameStr, ownedListIdx);
  }

  this->UpdateEquipmentPage();
  ownedBox->GetParent()->Layout();
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
  
  this->FilterAvailBox();
  if(static_cast<wxCheckBox*>(wxWindow::FindWindowById(EQUIPMENT_OWNED_LOCK_CHECKBOX_ID))->GetValue() == 1)
  {
    this->FilterOwnedBox();
  }
}

void EquipmentPage::OnLockBoxChecked(wxCommandEvent& evt)
{
  bool filterActive = (currentCategory_ != Pathfinder::ALL_EQUIPMENT || !static_cast<wxTextCtrl*>(wxWindow::FindWindowById(EQUIPMENT_SEARCH_BAR_ID))->GetValue().IsEmpty());
  if (filterActive)
  {
    /* If filtering is being done, either need to apply it now or un-apply it */
    this->FilterOwnedBox();
  }
}