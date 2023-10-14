#ifndef OBJ_VIEWER_H_
#define OBJ_VIEWER_H_

typedef unsigned int ObjViewerUInt;

typedef struct {
  ObjViewerUInt p;
  ObjViewerUInt t;
  ObjViewerUInt n;
} ObjViewerIndex;

typedef struct {
  /* Vertex data */
  ObjViewerUInt position_count;
  float* positions;

  /* Face data: one element for each face */
  ObjViewerUInt face_count;
  ObjViewerUInt* face_vertices;

  /* Index data: one element for each face vertex */
  ObjViewerUInt index_count;
  ObjViewerIndex* indices;

} ObjViewerMesh;

ObjViewerMesh* objviewer_read(const char* path);
void objviewer_destroy(ObjViewerMesh* mesh);
void objviewer_rotate(ObjViewerMesh* mesh, float x, float y, float z);
void objviewer_rotate_x(ObjViewerMesh* mesh, float angle);
void objviewer_rotate_y(ObjViewerMesh* mesh, float agnle);
void objviewer_rotate_z(ObjViewerMesh* mesh, float angle);
void objviewer_rotate_set_initial(ObjViewerMesh* mesh);
void objviewer_rotate_set_initial_x(ObjViewerMesh* mesh);
void objviewer_rotate_set_initial_y(ObjViewerMesh* mesh);
void objviewer_rotate_set_initial_z(ObjViewerMesh* mesh);

void objviewer_scale(ObjViewerMesh* mesh, float x, float y, float z);
void objviewer_scale_uniformly(ObjViewerMesh* mesh, float factor);
void objviewer_scale_x(ObjViewerMesh* mesh, float factor);
void objviewer_scale_y(ObjViewerMesh* mesh, float factor);
void objviewer_scale_z(ObjViewerMesh* mesh, float factor);
void objviewer_scale_set_initial(ObjViewerMesh* mesh);
void objviewer_scale_set_initial_x(ObjViewerMesh* mesh);
void objviewer_scale_set_initial_y(ObjViewerMesh* mesh);
void objviewer_scale_set_initial_z(ObjViewerMesh* mesh);

void objviewer_translate(ObjViewerMesh* mesh, float x, float y, float z);
void objviewer_translate_x(ObjViewerMesh* mesh, float x);
void objviewer_translate_y(ObjViewerMesh* mesh, float y);
void objviewer_translate_z(ObjViewerMesh* mesh, float z);
void objviewer_translate_set_initial(ObjViewerMesh* mesh);
void objviewer_translate_set_initial_x(ObjViewerMesh* mesh);
void objviewer_translate_set_initial_y(ObjViewerMesh* mesh);
void objviewer_translate_set_initial_z(ObjViewerMesh* mesh);

#endif  // OBJ_VIEWER_H_
