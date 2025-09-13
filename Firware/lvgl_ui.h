#ifndef LVGL_UI_H
#define LVGL_UI_H

void ui_init();
void update_user_text(const char *text);
void update_ai_text(const char *text);
void update_status(const char *text);

void add_todo_item(const char *task);
void clear_todo_list();

#endif
