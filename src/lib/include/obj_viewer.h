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

ObjViewerMesh* obj_viewer_read(const char* path);
void obj_viewer_destroy(ObjViewerMesh* mesh);

void obj_viewer_rotate(ObjViewerMesh* mesh);
void obj_viewer_scale(ObjViewerMesh* mesh);
void obj_viewer_translate(ObjViewerMesh* mesh);

#endif  // OBJ_VIEWER_H_
