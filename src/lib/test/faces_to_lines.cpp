#include "obj_viewer.h"

int main() {
  ObjViewerMesh* m = objviewer_create_cube(0, 0, 0, 1);
  unsigned int* arr = objviewer_faces_to_lines(m);
}
