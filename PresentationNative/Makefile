C_SRCS=wrappers.c classification.c loline.c loservice.c fspage.c

all: PresentationNative_cor3.dll
.PHONY: all

%.o: $(SRCDIR)/%.c
	$(MINGW)-gcc -o $@ $(CFLAGS) -D_WIN32_WINNT=0x0602 --std=c99 -c -Wall -Wno-format -Werror -Wno-pragma-pack -I$(SRCDIR)/.. $<

classification.o: $(SRCDIR)/class_table.h
fspage.o: $(SRCDIR)/fspage.h
loline.o: $(SRCDIR)/loservice.h $(SRCDIR)/../wine/debug.h
loservice.o: $(SRCDIR)/loservice.h

debug.o: $(SRCDIR)/../wine/debug.c $(SRCDIR)/../wine/debug.h $(SRCDIR)/../wine/heap.h
	$(MINGW)-gcc -o $@ $(CFLAGS) -D_WIN32_WINNT=0x0602 -D__WINESRC__ --std=c99 -c -Wall -Wno-format -Werror -I$(SRCDIR)/.. $<

PresentationNative_cor3.dll: $(C_SRCS:%.c=%.o) debug.o
	$(MINGW)-gcc -o $@ $(LDFLAGS) -shared -Wl,--kill-at $(C_SRCS:%.c=%.o) debug.o -luuid -lgdi32
