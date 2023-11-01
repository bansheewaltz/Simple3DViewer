#define model "/Users/mark/Downloads/3Dmodels/cube_not_centered.obj"

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj/fast_obj.h"
#include "obj_viewer.h"

int main() {
  //  fastObjMesh *m = fast_obj_read(model);
  ObjViewerMesh *m = objviewer_read(model);
  ObjViewerVec3 vec3 = objviewer_find_geometry_centre(m);
  objviewer_destroy(m);
}
