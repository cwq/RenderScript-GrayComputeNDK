#pragma version(1)
#pragma rs java_package_name(com.example.graycomputendk)
#pragma rs_fp_imprecise

const static float3 gMonoMult = {0.299f, 0.587f, 0.114f};

void root(const uchar4 *v_in, uchar4 *v_out)
{
    float4 f4 = rsUnpackColor8888(*v_in);
    float3 mono = dot(f4.rgb, gMonoMult);
    //rsDebug("test", f4);
    *v_out = rsPackColorTo8888(mono);
}
