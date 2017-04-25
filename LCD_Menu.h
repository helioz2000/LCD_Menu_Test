/*
 * LCD_Menu.h
 */

#ifndef LCD_Menu_h
#define LCD_Menu_h

typedef struct {
  char text[20];
  uint8_t datacolumn;
  int datamin;
  int datamax;
} menu_item;

class LCD_Menu {
  public:
    LCD_Menu(uint8_t rows, uint8_t columns);
    void setMenuItems(menu_item* menuitems, uint8_t number_of_items);
    void updateLCD();
    void moveMenu(bool down);
  private:
    uint8_t _lcd_rows;
    uint8_t _lcd_columns;
    uint8_t _menu_item_first_visible;
    uint8_t _menu_item_selected;
    uint8_t _menu_items_count;
    menu_item *_menuitems;
};

#endif
