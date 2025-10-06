#include "Gtk.h"
#include "../frontend/parserApi.h"
#include "../Generacion_arm64/generador_arm64.h"
#include <string.h>

void clear_treeview(GtkListStore* store) {
    gtk_list_store_clear(store);
}

// Función para llenar la tabla de errores desde texto formateado
void fill_error_treeview(GtkListStore* store, const char* error_text) {
    clear_treeview(store);
    
    if (!error_text || !strlen(error_text))
        return;
    
    // Buscar la línea que comienza con "=== REPORTE"
    const char* start = strstr(error_text, "=== REPORTE");
    if (!start) return;
    
    // Saltar la cabecera
    start = strchr(start, '\n');
    if (!start) return;
    start++;
    
    // Saltar la línea de formato
    start = strchr(start, '\n');
    if (!start) return;
    start++;
    
    // Saltar la línea de separadores
    start = strchr(start, '\n');
    if (!start) return;
    start++;
    
    // Procesar hasta encontrar la línea de ===
    const char* end = strstr(start, "===================================");
    if (!end) end = start + strlen(start);
    
    // Extraer líneas con errores
    char line[1024];
    const char* line_start = start;
    const char* line_end;
    
    while (line_start < end) {
        line_end = strchr(line_start, '\n');
        if (!line_end) line_end = end;
        
        size_t line_len = line_end - line_start;
        if (line_len > 0 && line_len < sizeof(line) - 1) {
            strncpy(line, line_start, line_len);
            line[line_len] = '\0';
            
            // Parsear la línea con formato "No. | Descripción | Ámbito | Linea | Col"
            char id[10] = "";
            char descripcion[100] = "";
            char ambito[50] = "";
            char linea[10] = "";
            char columna[10] = "";

            // Extraer valores usando sscanf con formato fijo
            sscanf(line, "%9[^|]|%99[^|]|%49[^|]|%9[^|]|%9[^\n]",
                id, descripcion, ambito, linea, columna);

            // Eliminar espacios extras
            g_strstrip(id);
            g_strstrip(descripcion);
            g_strstrip(ambito);
            g_strstrip(linea);
            g_strstrip(columna);

            // Agregar a la tabla
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                            0, id,
                            1, descripcion,
                            2, ambito,
                            3, linea,
                            4, columna,
                            -1);
        }
        
        line_start = line_end + 1;
    }
}

void fill_symbol_treeview(GtkListStore* store, const char* symbol_text) {
    clear_treeview(store);
    
    if (!symbol_text || !strlen(symbol_text))
        return;
    
    // Buscar la línea que comienza con "=== Tabla"
    const char* start = strstr(symbol_text, "=== Tabla de Símbolos");
    if (!start) return;
    
    // Saltar la cabecera
    start = strchr(start, '\n');
    if (!start) return;
    start++;
    
    // Saltar la línea de formato (ID Nombre Clase...)
    start = strchr(start, '\n');
    if (!start) return;
    start++;
    
    // Saltar la línea de separadores (---...)
    start = strchr(start, '\n');
    if (!start) return;
    start++;
    
    // Procesar hasta encontrar la línea de ===
    const char* end = strstr(start, "=============================================");
    if (!end) end = start + strlen(start);
    
    // Extraer líneas con símbolos
    char line[1024];
    const char* line_start = start;
    const char* line_end;
    
    while (line_start < end) {
        line_end = strchr(line_start, '\n');
        if (!line_end) line_end = end;
        
        size_t line_len = line_end - line_start;
        if (line_len > 0 && line_len < sizeof(line) - 1) {
            strncpy(line, line_start, line_len);
            line[line_len] = '\0';
            
            // Saltar líneas vacías
            if (strlen(g_strstrip(g_strdup(line))) == 0) {
                line_start = line_end + 1;
                continue;
            }
            
            // Inicializar variables
            char id[10] = "";
            char nombre[50] = "";
            char clase[30] = "";
            char tipo[20] = "";
            char valor[200] = "";
            char ambito[30] = "";
            char linea[10] = "";
            char columna[10] = "";
            
            // POSICIONES CORREGIDAS basadas en el printf real:
            // printf("%-4s %-23s %-28s %-10s %-70s %-28s %-6s %-6s\n"
            // Posiciones acumulativas: 0-3, 5-27, 29-56, 58-67, 69-138, 140-167, 169-174, 176-181
            
            // ID (posición 0-3, ancho 4)
            if (strlen(line) > 0) {
                int end_pos = (strlen(line) >= 4) ? 4 : strlen(line);
                strncpy(id, line, end_pos);
                id[end_pos] = '\0';
                g_strstrip(id);
            }
            
            // Nombre (posición 5-27, ancho 23) 
            if (strlen(line) > 5) {
                int start_pos = 5;
                int end_pos = (strlen(line) >= 28) ? 28 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0) {
                    strncpy(nombre, line + start_pos, copy_len);
                    nombre[copy_len] = '\0';
                    g_strstrip(nombre);
                }
            }
            
            // Clase (posición 29-56, ancho 28)
            if (strlen(line) > 29) {
                int start_pos = 29;
                int end_pos = (strlen(line) >= 57) ? 57 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0 ) {
                    strncpy(clase, line + start_pos, copy_len);
                    clase[copy_len] = '\0';
                    g_strstrip(clase);
                }
            }

            // Tipo (posición 58-67, ancho 10)
            if (strlen(line) > 58) {
                int start_pos = 58;
                int end_pos = (strlen(line) >= 68) ? 68 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0 ) {
                    strncpy(tipo, line + start_pos, copy_len);
                    tipo[copy_len] = '\0';
                    g_strstrip(tipo);
                }
            }

            // Valor (posición 69-138, ancho 70)
            if (strlen(line) > 69) {
                int start_pos = 69;
                int end_pos = (strlen(line) >= 139) ? 139 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0 ) {
                    strncpy(valor, line + start_pos, copy_len);
                    valor[copy_len] = '\0';
                    g_strstrip(valor);
                }
            }

            // Ámbito (posición 140-167, ancho 28)
            if (strlen(line) > 140) {
                int start_pos = 140;
                int end_pos = (strlen(line) >= 168) ? 168 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0) {
                    strncpy(ambito, line + start_pos, copy_len);
                    ambito[copy_len] = '\0';
                    g_strstrip(ambito);
                }
            }

            // Línea (posición 169-174, ancho 6)
            if (strlen(line) > 169) {
                int start_pos = 169;
                int end_pos = (strlen(line) >= 175) ? 175 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0) {
                    strncpy(linea, line + start_pos, copy_len);
                    linea[copy_len] = '\0';
                    g_strstrip(linea);
                }
            }

            // Columna (posición 176 en adelante, ancho 6)
            if (strlen(line) > 176) {
                int start_pos = 176;
                int end_pos = (strlen(line) >= 182) ? 182 : strlen(line);
                int copy_len = end_pos - start_pos;
                if (copy_len > 0 ) {
                    strncpy(columna, line + start_pos, copy_len);
                    columna[copy_len] = '\0';
                    g_strstrip(columna);
                }
            }
            
            // Agregar a la tabla si tenemos al menos el ID
            if (strlen(id) > 0) {
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 
                                   0, id,
                                   1, nombre,
                                   2, clase,
                                   3, tipo,
                                   4, valor,
                                   5, ambito,
                                   6, linea,
                                   7, columna,
                                   -1);
            }
        }
        
        line_start = line_end + 1;
    }
}
static char* get_code(GtkTextBuffer* b){
    GtkTextIter s,e;
    gtk_text_buffer_get_start_iter(b,&s);
    gtk_text_buffer_get_end_iter(b,&e);
    return gtk_text_buffer_get_text(b,&s,&e,FALSE);
}

void ui_set_text(GtkTextBuffer* b, const char* txt){
    gtk_text_buffer_set_text(b, txt?txt:"", -1);
}

void on_parse(GtkButton* btn, gpointer data){
    (void)btn;
    AppUI* ui = (AppUI*)data;
    char* code = get_code(ui->buf_code);
    ParseResult r = parse_source(code);
    g_free(code);

    // Llenar siempre la salida
    ui_set_text(ui->buf_output, r.salida);
    
    // Llenar tablas de símbolos y errores
    fill_symbol_treeview(ui->store_simbolos, r.simbolos);
    fill_error_treeview(ui->store_errores, r.errores);

    // Siempre mostrar el AST, incluso si hay errores
    const char* codigoARM64 = obtenerCodigoGenerado();
    ui_set_text(ui->buf_ast, codigoARM64);
    ui_set_text(ui->buf_ast, r.astDot);
    
    // Mostrar imagen si existe
    if(g_file_test("ast.png", G_FILE_TEST_EXISTS)){
        gtk_image_set_from_file(ui->img_ast, "ast.png");
    } else {
        gtk_image_clear(ui->img_ast);
    }
    
    free_parse_result(&r);
}

void on_clear(GtkButton* btn, gpointer data){
    (void)btn;
    AppUI* ui = (AppUI*)data;
    ui_set_text(ui->buf_code,"");
    ui_set_text(ui->buf_ast,"");
    ui_set_text(ui->buf_output,"");
    
    // Limpiar tablas
    clear_treeview(ui->store_simbolos);
    clear_treeview(ui->store_errores);
    
    gtk_image_clear(ui->img_ast);
}

void on_load_file(GtkButton* btn, gpointer data){
    (void)btn;
    AppUI* ui = (AppUI*)data;
    
    // Crear diálogo de selección de archivo
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Cargar archivo USL",
        GTK_WINDOW(ui->window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancelar", GTK_RESPONSE_CANCEL,
        "_Abrir", GTK_RESPONSE_ACCEPT,
        NULL
    );

    // Filtro para archivos .usl
    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Archivos USL (*.usl)");
    gtk_file_filter_add_pattern(filter, "*.usl");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Filtro para todos los archivos
    GtkFileFilter* filter_all = gtk_file_filter_new();
    gtk_file_filter_set_name(filter_all, "Todos los archivos");
    gtk_file_filter_add_pattern(filter_all, "*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_all);

    // Mostrar diálogo
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (result == GTK_RESPONSE_ACCEPT) {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        
        // Verificar extensión .usl
        if (filename) {
            size_t len = strlen(filename);
            if (len >= 4 && strcmp(filename + len - 4, ".usl") == 0) {
                // Leer contenido del archivo
                FILE* file = fopen(filename, "r");
                if (file) {
                    // Obtener tamaño del archivo
                    fseek(file, 0, SEEK_END);
                    long file_size = ftell(file);
                    fseek(file, 0, SEEK_SET);
                    
                    // Leer contenido
                    char* content = malloc(file_size + 1);
                    if (content) {
                        size_t bytes_read = fread(content, 1, file_size, file);
                        content[bytes_read] = '\0';
                        
                        // Cargar contenido en el editor
                        ui_set_text(ui->buf_code, content);
                        
                        free(content);
                    }
                    fclose(file);
                } else {
                    // Mostrar error si no se puede abrir el archivo
                    GtkWidget* error_dialog = gtk_message_dialog_new(
                        GTK_WINDOW(ui->window),
                        GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_ERROR,
                        GTK_BUTTONS_OK,
                        "Error al abrir el archivo:\n%s", filename
                    );
                    gtk_dialog_run(GTK_DIALOG(error_dialog));
                    gtk_widget_destroy(error_dialog);
                }
            } else {
                // Mostrar error si no es archivo .usl
                GtkWidget* error_dialog = gtk_message_dialog_new(
                    GTK_WINDOW(ui->window),
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Por favor seleccione un archivo con extensión .usl"
                );
                gtk_dialog_run(GTK_DIALOG(error_dialog));
                gtk_widget_destroy(error_dialog);
            }
            g_free(filename);
        }
    }
    
    gtk_widget_destroy(dialog);
}

void ui_build(AppUI* ui){
    ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(ui->window),"Javalang IDE");
    gtk_window_set_default_size(GTK_WINDOW(ui->window),1200,700);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
    gtk_container_add(GTK_CONTAINER(ui->window), vbox);

    GtkWidget* hbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    GtkWidget* bParse = gtk_button_new_with_label("Parsear");
    GtkWidget* bClear = gtk_button_new_with_label("Limpiar");
    GtkWidget* bLoad = gtk_button_new_with_label("Cargar");
    gtk_box_pack_start(GTK_BOX(hbar), bParse, FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbar), bClear, FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(hbar), bLoad, FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(vbox), hbar, FALSE,FALSE,0);

    GtkWidget* paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), paned, TRUE,TRUE,0);

    // Panel código
    GtkWidget* frameCode = gtk_frame_new("Código");
    GtkWidget* scCode = gtk_scrolled_window_new(NULL,NULL);
    GtkWidget* tvCode = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scCode), tvCode);
    gtk_container_add(GTK_CONTAINER(frameCode), scCode);
    gtk_paned_add1(GTK_PANED(paned), frameCode);

    // Panel derecho (notebook)
    GtkWidget* nb = gtk_notebook_new();
    gtk_paned_add2(GTK_PANED(paned), nb);

    GtkWidget* scOutput = gtk_scrolled_window_new(NULL,NULL);
    GtkWidget* tvOutput = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(tvOutput), FALSE);
    gtk_container_add(GTK_CONTAINER(scOutput), tvOutput);
    gtk_notebook_append_page(GTK_NOTEBOOK(nb), scOutput, gtk_label_new("Salida"));

    // AST DOT
    GtkWidget* scAst = gtk_scrolled_window_new(NULL,NULL);
    GtkWidget* tvAst = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(tvAst), FALSE);
    gtk_container_add(GTK_CONTAINER(scAst), tvAst);
    gtk_notebook_append_page(GTK_NOTEBOOK(nb), scAst, gtk_label_new("AST DOT"));

    // TABLA DE SÍMBOLOS
    GtkWidget* scSym = gtk_scrolled_window_new(NULL, NULL);
    
    // Crear modelo para la tabla de símbolos
    ui->store_simbolos = gtk_list_store_new(8, 
                                           G_TYPE_STRING,  // ID
                                           G_TYPE_STRING,  // Nombre
                                           G_TYPE_STRING,  // Clase
                                           G_TYPE_STRING,  // Tipo
                                           G_TYPE_STRING,  // Valor
                                           G_TYPE_STRING,  // Ámbito
                                           G_TYPE_STRING,  // Línea
                                           G_TYPE_STRING); // Columna
    
    // Crear vista de tabla para símbolos
    ui->tree_simbolos = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ui->store_simbolos));
    
    // Agregar columnas
    GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn* column;
    
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    column = gtk_tree_view_column_new_with_attributes("Nombre", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    column = gtk_tree_view_column_new_with_attributes("Tipo de símbolo", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    column = gtk_tree_view_column_new_with_attributes("Tipo", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);

    column = gtk_tree_view_column_new_with_attributes("Valor", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    column = gtk_tree_view_column_new_with_attributes("Ámbito", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    column = gtk_tree_view_column_new_with_attributes("Línea", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    column = gtk_tree_view_column_new_with_attributes("Columna", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_simbolos), column);
    
    gtk_container_add(GTK_CONTAINER(scSym), ui->tree_simbolos);
    gtk_notebook_append_page(GTK_NOTEBOOK(nb), scSym, gtk_label_new("Símbolos"));

    // TABLA DE ERRORES
    GtkWidget* scErr = gtk_scrolled_window_new(NULL, NULL);
    
    // Crear modelo para la tabla de errores
    ui->store_errores = gtk_list_store_new(5, 
                                            G_TYPE_STRING,  // ID
                                            G_TYPE_STRING,  // Descripción
                                            G_TYPE_STRING,  // Ámbito
                                            G_TYPE_STRING,  // Línea
                                            G_TYPE_STRING); // Columna
    
    // Crear vista de tabla para errores
    ui->tree_errores = gtk_tree_view_new_with_model(GTK_TREE_MODEL(ui->store_errores));
    
    // Agregar columnas
    renderer = gtk_cell_renderer_text_new();
    
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_errores), column);

    column = gtk_tree_view_column_new_with_attributes("Descripción", renderer, "text", 1, NULL);
    gtk_tree_view_column_set_min_width(column, 250);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_errores), column);

    column = gtk_tree_view_column_new_with_attributes("Ámbito", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_errores), column);

    column = gtk_tree_view_column_new_with_attributes("Línea", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_errores), column);

    column = gtk_tree_view_column_new_with_attributes("Col", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(ui->tree_errores), column);
    gtk_tree_view_column_set_min_width(column, 200);
    
    gtk_container_add(GTK_CONTAINER(scErr), ui->tree_errores);
    gtk_notebook_append_page(GTK_NOTEBOOK(nb), scErr, gtk_label_new("Errores"));

    // Imagen
    GtkWidget* scImg = gtk_scrolled_window_new(NULL,NULL);
    ui->img_ast = GTK_IMAGE(gtk_image_new());
    gtk_container_add(GTK_CONTAINER(scImg), GTK_WIDGET(ui->img_ast));
    gtk_notebook_append_page(GTK_NOTEBOOK(nb), scImg, gtk_label_new("AST Img"));

    ui->buf_code     = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tvCode));
    ui->buf_output   = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tvOutput));
    ui->buf_ast      = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tvAst));

    g_signal_connect(ui->window,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(bLoad,"clicked",G_CALLBACK(on_load_file),ui);
    g_signal_connect(bParse,"clicked",G_CALLBACK(on_parse),ui);
    g_signal_connect(bClear,"clicked",G_CALLBACK(on_clear),ui);
}