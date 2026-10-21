// crilayla.dll - unpack one CRILAYLA-compressed file (CRI archives store many files this way).
// The public algorithm (CriPakTools): the data is read backwards as a bit stream; the first 0x100 bytes of the
// file are stored uncompressed at the end. Returns the unpacked size, or -1.
#include <string.h>
#include <stdint.h>
static const unsigned char* g_in; static long g_pos; static unsigned g_pool; static int g_left;
static unsigned bits(int n) {
    unsigned out = 0; int got = 0;
    while (got < n) {
        if (g_left == 0) { g_pool = g_pos >= 0 ? g_in[g_pos] : 0; g_left = 8; g_pos--; }
        int take = g_left < n - got ? g_left : n - got;
        out = (out << take) | ((g_pool >> (g_left - take)) & ((1u << take) - 1));
        g_left -= take; got += take;
    }
    return out;
}
__declspec(dllexport) int crilayla(const unsigned char* in, int n, unsigned char* out, int cap) {
    if (n < 0x110 || memcmp(in, "CRILAYLA", 8)) return -1;
    uint32_t usize, hoff; memcpy(&usize, in + 8, 4); memcpy(&hoff, in + 12, 4);
    if ((long long)usize + 0x100 > cap || (long long)hoff + 0x110 > n) return -1;
    memcpy(out, in + hoff + 0x10, 0x100);
    static const int vle[4] = {2, 3, 5, 8};
    g_in = in; g_pos = (long)n - 0x100 - 1; g_pool = 0; g_left = 0;
    long end = 0x100 + (long)usize - 1; uint32_t done = 0;
    while (done < usize) {
        if (bits(1)) {
            long ref = end - done + bits(13) + 3; uint32_t len = 3; int lv;
            for (lv = 0; lv < 4; lv++) { unsigned t = bits(vle[lv]); len += t; if (t != (1u << vle[lv]) - 1) break; }
            if (lv == 4) { unsigned t; do { t = bits(8); len += t; } while (t == 255); }
            for (uint32_t i = 0; i < len && done < usize; i++) {
                if (ref < 0 || ref > end) return -1;
                out[end - done] = out[ref--]; done++;
            }
        } else { out[end - done] = (unsigned char)bits(8); done++; }
    }
    return (int)usize + 0x100;
}
