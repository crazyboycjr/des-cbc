#include "des.h"
#include <string>

using namespace std;

#define MSM(sz) (1ll << 63 >> (sz) * 8 << 1)
int encrypt(string ifile, string ofile, u64 key, u64 iv = 0) {
	FILE *ifp = fopen(ifile.c_str(), "rb");
	FILE *ofp = fopen(ofile.c_str(), "wb");

	u64 b; // cur block

	fseek(ifp, 0, SEEK_END);
	size_t fsz = ftell(ifp), tsz = (fsz - 1) % 8 + 1;

	fseek(ifp, 0, SEEK_SET);

	for (size_t sz, pos = 0; pos < fsz; pos += 8) {
		sz = fread(&b, 8, 1, ifp);
		b = __builtin_bswap64(b);
		if (pos >= (fsz >> 3 << 3)) {
			iv = b = des((b & MSM(tsz)) ^ iv, key);
			printf("%lx\n", b);
			b = __builtin_bswap64(b);
			fwrite(&b, 8, 1, ofp);

			break;
		} else {
			iv = b = des(b ^ iv, key);
			printf("%lx\n", b);
			b = __builtin_bswap64(b);
			fwrite(&b, 8, 1, ofp);
		}
	}
	b = des((8 - tsz + 8) ^ iv, key);
	printf("%lx\n", b);
	b = __builtin_bswap64(b);
	fwrite(&b, 8, 1, ofp);

	fclose(ifp);
	fclose(ofp);
}

int decrypt(string ifile, string ofile, u64 key, u64 iv = 0) {
	FILE *ifp = fopen(ifile.c_str(), "rb");
	FILE *ofp = fopen(ofile.c_str(), "wb");

	u64 b, lb, out; // cur block, last block

	fseek(ifp, 0, SEEK_END);
	size_t fsz = ftell(ifp), tsz;
	fseek(ifp, -16, SEEK_END);
	fread(&lb, 8, 1, ifp);
	lb = __builtin_bswap64(lb);
	fread(&tsz, 8, 1, ifp);
	tsz = __builtin_bswap64(tsz);
	tsz = des(tsz, key, -1) ^ lb;
	trace(tsz);
	fsz -= tsz;
	tsz = fsz % 8;
	trace(fsz, tsz);

	fseek(ifp, 0, SEEK_SET);

	for (size_t sz, pos = 0; pos < fsz; pos += 8) {
		sz = fread(&b, 8, 1, ifp);
		b = __builtin_bswap64(b);
		if (pos >= (fsz >> 3 << 3)) {
			b = des(b, key, -1);
			out = __builtin_bswap64((b ^ iv) & MSM(tsz));
			printf("%lx\n", (b ^ iv) & MSM(tsz));
			fwrite(&out, tsz, 1, ofp);
			break;
		} else {
			lb = des(b, key, -1);
			printf("%lx\n", lb ^ iv);
			out = __builtin_bswap64(lb ^ iv);
			iv = b;
			fwrite(&out, 8, 1, ofp);
		}
	}

	fclose(ifp);
	fclose(ofp);
}



int main() {
	encrypt("/tmp/f", "/tmp/fo", 0x0);
	decrypt("/tmp/fo", "/tmp/foo", 0x0);
	return 0;
}
