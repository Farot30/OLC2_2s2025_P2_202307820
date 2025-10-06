#ifndef GTK_APP_H
#define GTK_APP_H
#include <gtk/gtk.h>

typedef struct {
    GtkWidget* window;
    GtkTextBuffer* buf_code;
    GtkTextBuffer* buf_ast;
    
    // Reemplazar TextBuffers por TreeViews para tablas
    GtkWidget* tree_simbolos;
    GtkListStore* store_simbolos;
    
    GtkWidget* tree_errores;
    GtkListStore* store_errores;
    
    GtkTextBuffer* buf_output;  
    GtkImage* img_ast;
} AppUI;

void ui_build(AppUI* ui);
void ui_set_text(GtkTextBuffer* b, const char* txt);
void on_parse(GtkButton* btn, gpointer data);
void on_clear(GtkButton* btn, gpointer data);
void on_load_file(GtkButton* btn, gpointer data);

// Nuevas funciones para manejar tablas
void clear_treeview(GtkListStore* store);
void fill_error_treeview(GtkListStore* store, const char* error_text);
void fill_symbol_treeview(GtkListStore* store, const char* symbol_text);

#endif