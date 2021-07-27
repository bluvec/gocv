#ifndef MAT_SAFE_H_
#define MAT_SAFE_H_

#ifdef __cplusplus
extern "C" {
#endif

void GoMatAllocateInit();

unsigned long long goMatAllocate(unsigned long long size);
void goMatDeallocate(unsigned long long p);

#ifdef __cplusplus
}
#endif

#endif  // MAT_SAFE_H_
