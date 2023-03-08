#ifndef _BIO_ASSERT_
#define _BIO_ASSERT_

void __bioAssertWithLocation(const char* file, unsigned line, const char* expr);

#define bioassert(C) ({ if (!(C)) { __bioAssertWithLocation(__FILE__, __LINE__, #C); }})

#endif
