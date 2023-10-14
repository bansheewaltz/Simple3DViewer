#include <assert.h>

#include "external.h"
#include "obj_viewer.h"

// temporary solution
ObjViewerMesh* objviewer_read(const char* path) {
  fastObjMesh* mesh_orig = fast_obj_read(path);
  ObjViewerMesh* mesh = calloc(1, sizeof(ObjViewerMesh));
  assert(mesh);
  mesh->face_count = mesh_orig->face_count;
  mesh->face_vertices = mesh_orig->face_vertices;
  mesh->index_count = mesh_orig->index_count;
  mesh->indices = (ObjViewerIndex*)mesh_orig->indices;
  mesh->position_count = mesh_orig->position_count;
  mesh->positions = mesh_orig->positions;
  return mesh;
}

void objviewer_destroy(ObjViewerMesh* mesh) {
  ;
  ;
}
