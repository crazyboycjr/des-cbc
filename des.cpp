#include "common.h"
#include "des.h"

u64 K[NR];

static inline u64 subst(u64 in, const u8 *t, int len) {
	u64 out = 0, j = 64;
	for (int i = 0; i < len; i++) {
		out |= (in << t[i] >> 63 & 1) << --j;
	}
	return out;
}

// This macro implement cycle-shift for 28bit
#define SHIFT(x, w) ((x & (w + 1) << 62) >> 26 | x << w)

static inline void genkey(u64 key) {
	u64 C, D;
	key = subst(key, PC1, 56);
	C = key & 0xfffffffll << 36;
	D = key & 0xfffffffll << 8;
	for (int i = 0; i < NR; i++) {
		C = SHIFT(C, shift[i]);
		D = SHIFT(D, shift[i]);
		K[i] = subst(C | D >> 28, PC2, 48);
	}
}

#undef SHIFT

static inline u64 f(u64 in, u64 K) {
	u64 out = 0, mask = 0xfcll << 56, tmp;
	in = subst(in, E, 48) ^ K;
	for (int i = 0, j = 64; i < 8; i++, mask >>= 6) {
		tmp = (in & mask) >> (j -= 6);
		tmp = (tmp >> 5 << 1 | (tmp & 1)) * 16 + (tmp >> 1 & 0xf);
		out |= S[i][tmp] << (60 - i * 4);
	}
	return subst(out, P, 32);
}

u64 des(u64 in, u64 key, int type) {
	u64 L, R, L0, R0, out, j;
	genkey(key);
	in = subst(in, IP, 64);
	L0 = in >> 32 << 32;
	R0 = in << 32;
	j = type == 1 ? 0 : NR - 1;
	for (int i = 0; i < NR; i++, j += type) {
		L = R0;
		R = L0 ^ f(R0, K[j]);
		L0 = L; R0 = R;
	}
	out = R | L >> 32;
	out = subst(out, FP, 64);
	return out;
}
/*
int main() {
	u64 res = des(1, 1);
	printf("%lx\n", res);
	printf("%lx\n", des(res, 1, -1));
	return 0;
}
*/
