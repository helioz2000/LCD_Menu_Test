/*
 * LCD_Menu.h
 */

#ifndef LCD_Menu_h
#define LCD_Menu_h

typedef struct {
  char text[16];
  uint8_t datacolumn;
  int datamin;
  int datamax;
} menu_item;

class LCD_Menu {
  public:
    LCD_Menu();
  private:
    uint8_t _menu_item_display_line_1;
    uint8_t _menu_item_selected;
    uint8_t _total_menu_items;
    menuitem* _menuitems;
};

#endif
