#ifndef OBJ_VIEWER_H_
#define OBJ_VIEWER_H_

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Parsing of .obj file */

typedef struct {
  /* Vertex data */
  unsigned int position_count;
  float* positions;

  /* Face data: one element for each face */
  unsigned int face_count;
  unsigned int* face_vertex_counts;

  /* Index data: one element for each face vertex */
  unsigned int index_count;
  unsigned int* indices;

} ObjViewerMesh;

ObjViewerMesh* objviewer_read(const char* path);
void objviewer_destroy(ObjViewerMesh* mesh);
// Mesh example
ObjViewerMesh* objviewer_create_cube(float x, float y, float z, float side_len);

/* Processing of geometry data */

typedef struct {
  float xmin, ymin, zmin;
  float xmax, ymax, zmax;
  float xcen, ycen, zcen;
  float xlen, ylen, zlen;
  float maxlen;
} ObjViewerMeshBounds;

ObjViewerMeshBounds objviewer_find_bounds(const ObjViewerMesh* mesh);
unsigned int* objviewer_to_lines_index_arr(const ObjViewerMesh* m);
unsigned int* objviewer_to_unique_lines(ObjViewerMesh* m, size_t* newlen);

/* Affine transformations */

typedef struct {
  float arr[16];
} ObjViewerMatrix4x4;

typedef struct {
  float x, y, z;
} ObjViewerVec3;

void objviewer_rotate(ObjViewerMatrix4x4* mat, float angle, ObjViewerVec3 axis,
                      bool normalize);
void objviewer_scale(ObjViewerMatrix4x4* mat, float x, float y, float z);
void objviewer_translate(ObjViewerMatrix4x4* mat, float x, float y, float z);

#ifdef __cplusplus
}
#endif

#endif  // OBJ_VIEWER_H_
