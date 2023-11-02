#include "float.h"
#include "obj_viewer.h"

ObjViewerMeshBounds objviewer_find_bounds(const ObjViewerMesh* mesh) {
  float xmax = FLT_MIN, ymax = FLT_MIN, zmax = FLT_MIN;
  float xmin = FLT_MAX, ymin = FLT_MAX, zmin = FLT_MAX;

  float* p = mesh->positions;
  for (int i = 3; i < mesh->position_count * 3; i += 3) {
    if (p[i + 0] > xmax) xmax = p[i + 0];
    if (p[i + 0] < xmin) xmin = p[i + 0];
    if (p[i + 1] > ymax) ymax = p[i + 1];
    if (p[i + 1] < ymin) ymin = p[i + 1];
    if (p[i + 2] > zmax) zmax = p[i + 2];
    if (p[i + 2] < zmin) zmin = p[i + 2];
  }

  float xcen = (xmin + xmax) / 2.0f;
  float ycen = (ymin + ymax) / 2.0f;
  float zcen = (zmin + zmax) / 2.0f;

  float xlen = (xmax - xmin);
  float ylen = (ymax - ymin);
  float zlen = (zmax - zmin);

  float maxlen = xlen;
  if (maxlen < ylen) maxlen = ylen;
  if (maxlen < zlen) maxlen = zlen;

  return (ObjViewerMeshBounds){
      xmin,   ymin, zmin,  //
      xmax,   ymax, zmax,  //
      xcen,   ycen, zcen,  //
      xlen,   ylen, zlen,  //
      maxlen,
  };
}
