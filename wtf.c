extern void exit(int code);
extern int printf(const char* fmt, ...);
typedef struct Complex32 { int r, i; } c32;
#define pc32(c) c.r, c.i
c32 point(int r, int i) { c32 _; _.r = r; _.i = i; return _; };
unsigned long num(c32 v) { return (v.r << 8) + v.i; }
c32 complex(unsigned long v) { c32 _; _.r = v >> 8; _.i = v & 0x00FF; return _; };
int main(void) {
c32 a = point(5, 1);
c32 b = point(6, 4);
c32 c = complex((num(a) + num(b)));
printf("%d+%di + %d+%di = %d+%di", pc32(a), pc32(b), pc32(c));
}
