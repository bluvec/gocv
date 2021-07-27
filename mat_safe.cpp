#include "mat_safe.h"
#include <iostream>
#include "opencv2/core/core_c.h"
#include "opencv2/core/mat.hpp"

namespace {

using namespace cv;

class GoMatAllocator : public MatAllocator {
 public:
  GoMatAllocator() = default;

  UMatData *allocate(int dims, const int *sizes, int type, void *data0,
                     size_t *step, AccessFlag /*flags*/,
                     UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE {
    size_t total = CV_ELEM_SIZE(type);
    for (int i = dims - 1; i >= 0; i--) {
      if (step) {
        if (data0 && step[i] != CV_AUTOSTEP) {
          CV_Assert(total <= step[i]);
          total = step[i];
        } else {
          step[i] = total;
        }
      }
      total *= sizes[i];
    }

    uchar *data = data0 ? (uchar *)data0 : (uchar *)goMatAllocate(total);
    UMatData *u = new UMatData(this);
    u->data = u->origdata = data;
    u->size = total;

    if (data0) {
      u->flags |= UMatData::USER_ALLOCATED;
    }

    return u;
  }

  bool allocate(UMatData *u, AccessFlag /*accessFlags*/,
                UMatUsageFlags /*usageFlags*/) const CV_OVERRIDE {
    return u != nullptr;
  }

  void deallocate(UMatData *u) const CV_OVERRIDE {
    if (u) {
      CV_Assert(u->urefcount == 0);
      CV_Assert(u->refcount == 0);

      if (!(u->flags & UMatData::USER_ALLOCATED)) {
        goMatDeallocate((unsigned long long)u->origdata);
        u->origdata = nullptr;
      }

      delete u;
    }
  }
};

GoMatAllocator g_go_mat_allocator;

}  // namespace

void GoMatAllocateInit() {
  cv::Mat::setDefaultAllocator(&g_go_mat_allocator);
}
