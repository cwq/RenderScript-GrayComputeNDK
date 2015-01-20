#include "com_example_graycomputendk_JNIHelper.h"
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <RenderScript.h>

#include "ScriptC_mono.h"
#include "LogHelper.h"

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} argb;

/* return current time in milliseconds */
static double now_ms()
{
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

//#include <arm_neon.h>
//void neon_convert(uint8_t *__restrict dest, uint8_t *__restrict src, int n) {
//    int i;
//    uint8x8_t rfac = vdup_n_u8(77);
//    uint8x8_t gfac = vdup_n_u8(151);
//    uint8x8_t bfac = vdup_n_u8(28);
//    n /= 8;
//    for (i = 0; i < n; i++) {
//        uint16x8_t tmp;
//        uint8x8x4_t rgb;
//        uint8x8_t result;
//
//        rgb = vld4_u8(src);
//        tmp = vmull_u8(rgb.val[0], rfac);
//        tmp = vmlal_u8(tmp, rgb.val[1], gfac);
//        tmp = vmlal_u8(tmp, rgb.val[2], bfac);
//
//        result = vshrn_n_u16(tmp, 8);
//        vst1_u8(dest, result);
//        src += 8 * 4;
//        dest += 8;
//    }
//}
//
//void neon_asm_convert(uint8_t *__restrict dest, uint8_t *__restrict src,
//        int n) {
////    __asm__ __volatile__("lsr %2, %2, #3 \n"
////            "mov r4, #28 \n"
////            "mov r5, #151 \n"
////            "mov r6, #77 \n"
////            "vdup.8 d4, r4 \n"
////            "vdup.8 d5, r5 \n"
////            "vdup.8 d6, r6 \n"
////            ".loop: \n"
////            "vld4.8 {d0-d3}, [%1]! \n"
////            "vmull.u8 q7, d0, d4 \n"
////            "vmlal.u8 q7, d1, d5 \n"
////            "vmlal.u8 q7, d2, d6 \n"
////            "vshrn.u16 d7, q7, #8 \n"
////            "vst1.8 {d7}, [%0]! \n"
////            "subs %2, %2, #1 \n"
////            "bne .loop \n"
////            :
////            : "r"(dest), "r"(src), "r"(n)
////            : "r4", "r5", "r6"
////    );
//    asm volatile (
//            "vmov.u8    d24, #15                       \n"  // B * 0.11400 coefficient
//            "vmov.u8    d25, #75                       \n"  // G * 0.58700 coefficient
//            "vmov.u8    d26, #38                       \n"  // R * 0.29900 coefficient
//            ".p2align   2                              \n"
//            "1:                                          \n"
//            "vld4.8     {d0, d1, d2, d3}, [%0]!        \n"  // load 8 ARGB pixels.
//            "subs       %2, %2, #8                     \n"  // 8 processed per loop.
//            "vmull.u8   q2, d0, d24                    \n"  // B
//            "vmlal.u8   q2, d1, d25                    \n"  // G
//            "vmlal.u8   q2, d2, d26                    \n"  // R
//            "vqrshrun.s16 d0, q2, #7                   \n"  // 15 bit to 8 bit B
//            "vst1.8     {d0}, [%1]!                    \n"  // store 8 ARGB pixels.
//            "bgt        1b                             \n"
//            : "+r"(src),  // %0
//            "+r"(dest),  // %1
//            "+r"(n)      // %2
//            :
//        : "cc", "memory", "q0", "q1", "q2", "q12", "q13"
//            );
//}


JNIEXPORT void JNICALL Java_com_example_graycomputendk_JNIHelper_nativeMono
  (JNIEnv *env, jclass jthis, jint w, jint h, jobject in, jobject out, jstring name)
{
    LOGE("1");
    void* inputPtr = NULL;
    void* outputPtr = NULL;

    const char* cname = (env)->GetStringUTFChars(name, NULL);
    std::string cacheName(cname);
    (env)->ReleaseStringUTFChars(name, cname);

    LOGE("2");
    AndroidBitmapInfo inInfo;
    AndroidBitmapInfo outInfo;
    AndroidBitmap_getInfo(env, in, &inInfo);
    AndroidBitmap_getInfo(env, out, &outInfo);

    AndroidBitmap_lockPixels(env, in, &inputPtr);
    AndroidBitmap_lockPixels(env, out, &outputPtr);

    LOGE("3");
//    double start = now_ms();
//    for(int y = 0; y < inInfo.height; ++y)
//    {
//        uint8_t* line = (uint8_t*) inputPtr;
//        uint8_t* grayline = (uint8_t*) outputPtr;
//        int size = inInfo.width;
//        neon_asm_convert(grayline, line, size);
////        for(int x = 0; x < inInfo.width; ++x)
////        {
////            grayline[x] = 0.3*line[x].red + 0.59*line[x].green + 0.11*line[x].blue;
////        }
//        inputPtr += inInfo.stride;
//        outputPtr += outInfo.stride;
//    }
//    double time = now_ms() - start;
//    LOGE("time: %f", time);

    sp<RS> rs = new RS();
    LOGE("31");
    rs->init(cacheName);

    LOGE("4");
    sp<const Element> e = Element::RGBA_8888(rs);
    sp<const Type> t = Type::create(rs, e, w, h, 0);
    sp<Allocation> inputAlloc = Allocation::createTyped(rs, t, RS_ALLOCATION_MIPMAP_NONE,
            RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT, inputPtr);
    sp<Allocation> outputAlloc = Allocation::createTyped(rs, t, RS_ALLOCATION_MIPMAP_NONE,
                RS_ALLOCATION_USAGE_SHARED | RS_ALLOCATION_USAGE_SCRIPT, outputPtr);

    LOGE("5");
    double start = now_ms();
    inputAlloc->copy2DRangeFrom(0, 0, w, h, inputPtr);
    ScriptC_mono* sc = new ScriptC_mono(rs);
    sc->forEach_root(inputAlloc, outputAlloc);
    LOGE("6");
    outputAlloc->copy2DRangeTo(0, 0, w, h, outputPtr);
    double time = now_ms() - start;
    LOGE("time: %f", time);

    AndroidBitmap_unlockPixels(env, in);
    AndroidBitmap_unlockPixels(env, out);
}
