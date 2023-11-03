#include <stdlib.h>

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

ObjViewerMesh* objviewer_create_cube(float x, float y, float z,
                                     float side_len) {
  const float hside = side_len / 2;
  static unsigned int position_count = 8;
  float positions[] = {
      x + hside, y - hside, z + hside,  // right bottom front
      x + hside, y - hside, z - hside,  // right bottom back
      x - hside, y - hside, z + hside,  // left bottom front
      x - hside, y - hside, z - hside,  // left bottom back

      x + hside, y + hside, z + hside,  // right top front
      x + hside, y + hside, z - hside,  // right top back
      x - hside, y + hside, z + hside,  // left top front
      x - hside, y + hside, z - hside,  // left top back
  };
  static unsigned int face_count = 6;
  static unsigned int face_vertex_counts[] = {4, 4, 4, 4, 4, 4};
  static unsigned int index_count = 8;
  static unsigned int indices[] = {3, 2, 0, 1,   // bottom
                                   4, 5, 7, 6,   // top
                                   2, 3, 7, 6,   // left
                                   0, 1, 5, 4,   // right
                                   0, 4, 6, 2,   // near
                                   1, 3, 7, 5};  // far

  ObjViewerMesh* m = malloc(sizeof(ObjViewerMesh));
  *m = (ObjViewerMesh){position_count,     positions,   face_count,
                       face_vertex_counts, index_count, indices};
  return m;
}

unsigned int* objviewer_faces_to_lines(const ObjViewerMesh* m) {
  unsigned int* farr = m->indices;
  unsigned int* larr = malloc(2 * sizeof(unsigned int) * m->index_count);

  unsigned int* face_indices = farr;
  size_t i = 0;
  for (int j = 0; j < m->face_count; j++) {
    size_t face_vertex_count = m->face_vertex_counts[j];
    for (int k = 0; k < face_vertex_count; k++) {
      larr[i++] = face_indices[k];
      if (k == 0) continue;
      if (k == face_vertex_count - 1) {
        larr[i++] = face_indices[k];
        larr[i++] = face_indices[0];
      } else {
        larr[i++] = face_indices[k];
      }
    }
    face_indices += face_vertex_count;
  }

  return larr;
}
