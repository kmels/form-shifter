#ifndef TYPES
#define TYPES

#include <gtk/gtk.h>

/* A linked list for coordinates */
typedef struct coordinate{
  int x;
  int y;
  struct coordinate *next;
} Coordinate;

/* Color */
typedef struct{
  guint16 red;
  guint16 green;
  guint16 blue;
  guint16 alpha;
} Color;

/* A filled pollygon, containing a list of points and a fill color */
typedef struct{
  Coordinate *points;
  int npoints;
  Color color;
} FilledPolygon;

/* Composed polygons (list of polygons) */
typedef struct polygon_list{
  FilledPolygon *polygon;
  struct polygon_list *next;
} FilledPolygonList;

/* Tool types */

typedef enum{
  FORM_SHIFTER_MOUSE_TOOL,
  FORM_SHIFTER_POLYGON_TOOL
} FormShifterToolItemType;

typedef struct{
  char *path_to_image;
  char *name;
  FormShifterToolItemType type;  
} FormShifterToolItem;

/* Mouse events */
enum {
  MOUSE_CLICK,
  MOUSE_MOTION
} MouseEventType;

#endif
