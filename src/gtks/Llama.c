#include "Llama.h"
#include "Gtk.h"
#include <gtk/gtk.h>

int run_gui(int argc, char** argv){
    gtk_init(&argc,&argv);
    AppUI ui={0};
    ui_build(&ui);
    gtk_widget_show_all(ui.window);
    gtk_main();
    return 0;
}
