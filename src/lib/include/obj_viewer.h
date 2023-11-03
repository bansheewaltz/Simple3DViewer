#ifndef OBJ_VIEWER_H_
#define OBJ_VIEWER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float arr[16];
} ObjViewerMatrix4x4;

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

ObjViewerMesh* objviewer_create_cube(float x, float y, float z, float side_len);
unsigned int* objviewer_faces_to_lines(const ObjViewerMesh* m);

typedef struct {
  float x, y, z;
} ObjViewerVec3;

typedef struct {
  float xmin, ymin, zmin;
  float xmax, ymax, zmax;
  float xcen, ycen, zcen;
  float xlen, ylen, zlen;
  float maxlen;
} ObjViewerMeshBounds;

ObjViewerMeshBounds objviewer_find_bounds(const ObjViewerMesh* mesh);

void objviewer_rotate(ObjViewerMesh* mesh, float x, float y, float z);
void objviewer_rotate_x(ObjViewerMesh* mesh, float angle);
void objviewer_rotate_y(ObjViewerMesh* mesh, float agnle);
void objviewer_rotate_z(ObjViewerMesh* mesh, float angle);
void objviewer_rotate_reset(ObjViewerMesh* mesh);
void objviewer_rotate_reset_x(ObjViewerMesh* mesh);
void objviewer_rotate_reset_y(ObjViewerMesh* mesh);
void objviewer_rotate_reset_z(ObjViewerMesh* mesh);

void objviewer_scale(ObjViewerMesh* mesh, float x, float y, float z);
void objviewer_scale_uniformly(ObjViewerMesh* mesh, float factor);
void objviewer_scale_x(ObjViewerMesh* mesh, float factor);
void objviewer_scale_y(ObjViewerMesh* mesh, float factor);
void objviewer_scale_z(ObjViewerMesh* mesh, float factor);
void objviewer_scale_reset(ObjViewerMesh* mesh);
void objviewer_scale_reset_x(ObjViewerMesh* mesh);
void objviewer_scale_reset_y(ObjViewerMesh* mesh);
void objviewer_scale_reset_z(ObjViewerMesh* mesh);

void objviewer_translate(ObjViewerMesh* mesh, float x, float y, float z);
void objviewer_translate_x(ObjViewerMesh* mesh, float x);
void objviewer_translate_y(ObjViewerMesh* mesh, float y);
void objviewer_translate_z(ObjViewerMesh* mesh, float z);
void objviewer_translate_reset(ObjViewerMesh* mesh);
void objviewer_translate_reset_x(ObjViewerMesh* mesh);
void objviewer_translate_reset_y(ObjViewerMesh* mesh);
void objviewer_translate_reset_z(ObjViewerMesh* mesh);

#ifdef __cplusplus
}
#endif

#endif  // OBJ_VIEWER_H_
