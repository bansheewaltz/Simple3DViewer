#include <assert.h>

#include <chrono>
#include <iostream>

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj/fast_obj.h"
#include "obj_viewer.h"

using namespace std::chrono;
struct Timer {
  time_point<steady_clock> start, stop;

  Timer() { start = high_resolution_clock::now(); }

  ~Timer() {
    stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "took " << duration.count() << "ms" << std::endl;
  }
};

#define model "/Users/mark/Downloads/3Dmodels/Romanesco Top.obj"
int main() {
  fastObjMesh *m = nullptr;
  {
    Timer timer;
    m = fast_obj_read(model);
    assert(m);
  }
  fast_obj_destroy(m);
  ObjViewerMesh *m2 = nullptr;
  {
    Timer timer;
    m2 = objviewer_read(model);
    assert(m2);
  }
  {
    Timer timer;
    ObjViewerMeshBounds mb = objviewer_find_bounds(m2);
    (void)mb;
  }
  objviewer_destroy(m2);
}
