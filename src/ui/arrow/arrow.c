#include "arrow.h"

static lv_point_t arrow_up_points[] = { {0, 20}, {20, 0}, {40, 20}, {30, 20}, {30, 40}, {10, 40}, {10, 20}, {0, 20} };
// Flecha abajo mejorada, estilizada y con punta clara
static lv_point_t arrow_down_points[] = {
    {0, 20}, {10, 30}, {20, 10}, {30, 30}, {40, 20}, {20, 40}, {0, 20}
};

static lv_point_t arrow_right_points[] = { {0, 10}, {20, 10}, {20, 0}, {40, 20}, {20, 40}, {20, 30}, {0, 30}, {0, 10} };
static lv_point_t arrow_left_points[] = { {40, 10}, {20, 10}, {20, 0}, {0, 20}, {20, 40}, {20, 30}, {40, 30}, {40, 10} };

lv_obj_t *arrow_up(lv_obj_t *parent) {
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_up_points, sizeof(arrow_up_points)/sizeof(arrow_up_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}

lv_obj_t *arrow_down(lv_obj_t *parent) {
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_down_points, sizeof(arrow_down_points)/sizeof(arrow_down_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}

lv_obj_t *arrow_right(lv_obj_t *parent) {
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_right_points, sizeof(arrow_right_points)/sizeof(arrow_right_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}

lv_obj_t *arrow_left(lv_obj_t *parent) {
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_left_points, sizeof(arrow_left_points)/sizeof(arrow_left_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}
