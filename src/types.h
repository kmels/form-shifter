#ifndef TYPES
#define TYPES

/* A linked list for coordinates */
typedef struct coordinate{
  int x;
  int y;
  struct coordinate *next;
} Coordinates;

/* Color */
typedef struct{
  guint16 red;
  guint16 green;
  guint16 blue;
  guint16 alpha;
} Color;

/* A filled pollygon, containing a list of points and a fill color */
typedef struct{
  Coordinates points;
  int npoints;
  Color color;
} FilledPolygon;

#endif
