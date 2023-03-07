#include "ctx.h"

BBox GwaCtx::cam_extents() const {
    return BBox::from(cam, view_sz);
}