#include "arrow.h"

static lv_point_t arrow_up_points[] = {
    {0, 60}, {60, 0}, {120, 60}, {90, 60}, {90, 120}, {30, 120}, {30, 60}, {0, 60}};
static lv_point_t arrow_down_points[] = {
    {0, 60}, {30, 90}, {60, 30}, {90, 90}, {120, 60}, {60, 120}, {0, 60}};
static lv_point_t arrow_right_points[] = {
    {0, 30}, {60, 30}, {60, 0}, {120, 60}, {60, 120}, {60, 90}, {0, 90}, {0, 30}};
static lv_point_t arrow_left_points[] = {
    {120, 30}, {60, 30}, {60, 0}, {0, 60}, {60, 120}, {60, 90}, {120, 90}, {120, 30}};

lv_obj_t *arrow_up(lv_obj_t *parent)
{
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_up_points, sizeof(arrow_up_points) / sizeof(arrow_up_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}

lv_obj_t *arrow_down(lv_obj_t *parent)
{
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_down_points, sizeof(arrow_down_points) / sizeof(arrow_down_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}

lv_obj_t *arrow_right(lv_obj_t *parent)
{
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_right_points, sizeof(arrow_right_points) / sizeof(arrow_right_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}

lv_obj_t *arrow_left(lv_obj_t *parent)
{
    lv_obj_t *arrow = lv_line_create(parent);
    lv_line_set_points(arrow, arrow_left_points, sizeof(arrow_left_points) / sizeof(arrow_left_points[0]));
    lv_obj_set_style_line_width(arrow, 6, 0);
    lv_obj_set_style_line_color(arrow, lv_color_white(), 0);
    return arrow;
}
