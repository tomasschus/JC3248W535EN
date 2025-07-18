#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"

// --- PASO 1: DECLARAR LA IMAGEN DEL LOGO ---
// Este es un prototipo para la imagen que generarás.
// El nombre 're_logo_img' debe coincidir con el que uses en el convertidor de LVGL.
// LV_IMG_DECLARE(re_logo_img);

// Definimos los colores icónicos de Royal Enfield para fácil reutilización
#define RE_GOLD lv_color_hex(0xFFC72C)      // Un dorado/amarillo vibrante
#define RE_RED lv_color_hex(0xCE1126)       // Un rojo clásico
#define RE_OFF_WHITE lv_color_hex(0xE0E0E0) // Un blanco roto para texto secundario

/**
 * @brief Muestra una splash screen con el estilo de Royal Enfield.
 * * Esta función crea una pantalla de carga visualmente atractiva con el logo,
 * colores y tipografía que evocan la marca Royal Enfield.
 */
void splash_screen_royal_enfield(void)
{
    bsp_display_lock(0);

    // 1. Contenedor principal (pantalla completa en negro)
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);

    // 2. Logo de Royal Enfield (Imagen)
    // lv_obj_t *img_logo = lv_img_create(scr);
    // lv_img_set_src(img_logo, &re_logo_img);
    // lv_obj_align(img_logo, LV_ALIGN_CENTER, 0, -60); // Centrado, ligeramente hacia arriba

    // 3. Línea decorativa superior (Pinstripe)
    lv_obj_t *line_top = lv_obj_create(scr);
    lv_obj_set_size(line_top, 150, 2);
    lv_obj_set_style_bg_color(line_top, RE_GOLD, 0);
    lv_obj_set_style_border_width(line_top, 0, 0);
    lv_obj_align(line_top, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t *label_brand = lv_label_create(scr);
    lv_label_set_text(label_brand, "ROYAL ENFIELD");
    lv_obj_set_style_text_color(label_brand, RE_GOLD, 0);
    lv_obj_set_style_text_font(label_brand, &lv_font_montserrat_48, 0);
    lv_obj_align_to(label_brand, line_top, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    // 5. Línea decorativa inferior (Pinstripe)
    lv_obj_t *line_bottom = lv_obj_create(scr);
    lv_obj_set_size(line_bottom, 150, 2);
    lv_obj_set_style_bg_color(line_bottom, RE_GOLD, 0);
    lv_obj_set_style_border_width(line_bottom, 0, 0);
    lv_obj_align_to(line_bottom, label_brand, LV_ALIGN_OUT_BOTTOM_MID, 0, 20); // Alineado debajo del texto

    // 6. Texto de "Cargando..."
    // lv_obj_t *label_loading = lv_label_create(scr);
    // lv_label_set_text(label_loading, "Cargando...");
    // lv_obj_set_style_text_color(label_loading, RE_OFF_WHITE, 0); // Color más sutil
    // lv_obj_set_style_text_font(label_loading, &lv_font_montserrat_18, 0);
    // lv_obj_align(label_loading, LV_ALIGN_BOTTOM_MID, 0, -20); // En la parte inferior de la pantalla

    bsp_display_unlock();
}