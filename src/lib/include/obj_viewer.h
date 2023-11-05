#ifndef OBJ_VIEWER_H_
#define OBJ_VIEWER_H_

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Parsing of .obj file */

typedef struct {
  /* Vertex data: three coordinates for each vertex */
  unsigned int vertex_count;
  float* positions;

  /* Face data: one element for each face */
  unsigned int face_count;
  unsigned int* face_vertex_counts;

  /* Index data: one element for each face vertex */
  unsigned int index_count;
  unsigned int* indices;

} OWV_Mesh;

OWV_Mesh* owv_mesh_read_obj(const char* path);
void owv_mesh_destroy(OWV_Mesh* mesh);
// Mesh example
OWV_Mesh* owv_mesh_create_cube(float x, float y, float z, float side_len);

/* Processing of geometry data */

typedef struct {
  float xmin, ymin, zmin;
  float xmax, ymax, zmax;
  float xcen, ycen, zcen;
  float xlen, ylen, zlen;
  float maxlen;
} OWV_MeshBounds;

OWV_MeshBounds owv_mesh_find_bounds(const OWV_Mesh* mesh);
unsigned int* owv_iarr_to_lines(const OWV_Mesh* m);
unsigned int* owv_iarr_to_unique_lines(OWV_Mesh* m, size_t* newlen);

/* Affine transformations */

typedef struct {
  float arr[16];
} OWV_Mat4x4;

typedef struct {
  float x, y, z;
} OWV_Vec3;

void owv_mat_rotate(OWV_Mat4x4* mat, float angle, OWV_Vec3 axis,
                          bool normalize);
void owv_mat_scale(OWV_Mat4x4* mat, float x, float y, float z);
void owv_mat_translate(OWV_Mat4x4* mat, float x, float y, float z);

#ifdef __cplusplus
}
#endif

#endif  // OBJ_VIEWER_H_
