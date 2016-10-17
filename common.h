#pragma once

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cassert>

using namespace std;

// Every int type in this program are deal with BigEndian, 
// which means the leftmost is the 0th bit and the rightmost
// is the 63rd bit

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#ifdef DEBUG
#include <iostream>

#define disp(...) __disp(#__VA_ARGS__, __VA_ARGS__)
void __disp(const char *name, u64 x, int b = 8) {
	printf("%s:", name);
	for (int i = 0; i < 64; i++) {
		if (i % b == 0) putchar('\n');
		putchar((x << i >> 63) + '0');
	}
	puts("\n");
}

#ifdef DEBUG
#define trace(...) __f(#__VA_ARGS__, __VA_ARGS__)
const char *nextok(const char *__restrict s) {
	const char *e = s + strlen(s);
	int top = 0;
	for (; s < e; s++) {
		top += *s == '(' || *s == '[' || *s == '{';
		top -= *s == ')' || *s == ']' || *s == '}';
		if (!top && *s == ',') return s;
	}
	return e;
}
template <typename Arg1>
void __f(const char *name, Arg1 &&arg1) {
	cerr << name << " : " << arg1 << endl;
}
template <typename Arg1, typename... Args>
void __f(const char *names, Arg1 &&arg1, Args &&... args) {
	const char *comma = nextok(names + 1);
	cerr.write(names, comma - names) << " : " << arg1 << " | ";
	__f(comma + 1, args...);
}
#else
#define trace(...)
#endif // DEBUG

#endif // DEBUG

