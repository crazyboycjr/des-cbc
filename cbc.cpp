#include "des.h"
#include <string>

using namespace std;

#define MSM(sz) (1ll << 63 >> (sz) * 8 << 1)
void encrypt(string ifile, string ofile, u64 key, u64 iv) {
	FILE *ifp = fopen(ifile.c_str(), "rb");
	FILE *ofp = fopen(ofile.c_str(), "wb");

	u64 b; // cur block

	genkey(key);

	/* Put the encrypted init vector at the first block */
	u64 eiv = __builtin_bswap64(des(iv, key));
	fwrite(&eiv, 8, 1, ofp);

	fseek(ifp, 0, SEEK_END);
	size_t fsz = ftell(ifp), tsz = (fsz - 1) % 8 + 1;

	fseek(ifp, 0, SEEK_SET);

	for (size_t sz, pos = 0; pos < fsz; pos += 8) {
		sz = fread(&b, 8, 1, ifp);
		b = __builtin_bswap64(b);
		if (pos >= (fsz >> 3 << 3)) {
			iv = b = des((b & MSM(tsz)) ^ iv, key);
			b = __builtin_bswap64(b);
			fwrite(&b, 8, 1, ofp);

			break;
		} else {
			iv = b = des(b ^ iv, key);
			b = __builtin_bswap64(b);
			fwrite(&b, 8, 1, ofp);
		}
	}

	/* Append an extra block indicates the bytes of padding */
	b = des((8 - tsz + 8) ^ iv, key);
	b = __builtin_bswap64(b);
	fwrite(&b, 8, 1, ofp);

	fclose(ifp);
	fclose(ofp);
}

void decrypt(string ifile, string ofile, u64 key, u64 iv) {
	FILE *ifp = fopen(ifile.c_str(), "rb");
	FILE *ofp = fopen(ofile.c_str(), "wb");

	u64 b, lb, out; // cur block, last block

	genkey(key);

	/* 
	 * Read the last block of the ciphertext and calcuate the real size 
	 * fsz: real size of decrypted plaintext
	 * tsz: size of the last block
	 */
	fseek(ifp, 0, SEEK_END);
	size_t fsz = ftell(ifp), tsz;
	fseek(ifp, -16, SEEK_END);
	fread(&lb, 8, 1, ifp);
	lb = __builtin_bswap64(lb);
	fread(&tsz, 8, 1, ifp);
	tsz = __builtin_bswap64(tsz);
	tsz = des(tsz, key, -1) ^ lb;
	trace(tsz);
	fsz -= tsz + 8;
	tsz = fsz % 8;
	trace(fsz, tsz);

	fseek(ifp, 0, SEEK_SET);

	/* Read the first block to get iv */
	fread(&iv, 8, 1, ifp);
	iv = des(__builtin_bswap64(iv), key, -1);

	trace(iv);

	for (size_t sz, pos = 0; pos < fsz; pos += 8) {
		sz = fread(&b, 8, 1, ifp);
		b = __builtin_bswap64(b);
		if (pos >= (fsz >> 3 << 3)) {
			b = des(b, key, -1);
			out = __builtin_bswap64((b ^ iv) & MSM(tsz));
			fwrite(&out, tsz, 1, ofp);
			break;
		} else {
			lb = des(b, key, -1);
			out = __builtin_bswap64(lb ^ iv);
			iv = b;
			fwrite(&out, 8, 1, ofp);
		}
	}

	fclose(ifp);
	fclose(ofp);
}


/*
int main() {
	encrypt("/tmp/f", "/tmp/fo", 0x0, 0);
	decrypt("/tmp/fo", "/tmp/foo", 0x0, 0);
	return 0;
}*/
