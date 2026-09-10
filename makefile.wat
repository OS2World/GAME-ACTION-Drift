# Drift for OS/2 - Open Watcom wmake build file
# Usage: wmake -f makefile.wat [all | clean]

CC     = wcc386
RC     = wrc
LD     = wlink

!ifndef OS2TK
OS2TK  = c:\os2tk45
!endif

CFLAGS = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0 &
         -i=$(OS2TK)\h &
         -i=src

RCFLAGS = -bt=os2 -zm &
          -i=$(OS2TK)\h &
          -i=src

OBJS   = bin\drift.obj bin\objekts.obj bin\llist.obj bin\common.obj
TARGET = bin\driftos2.exe

all : $(TARGET) .SYMBOLIC

$(TARGET) : $(OBJS) bin\drift.res
    $(LD) system os2v2 pm &
          name $(TARGET) &
          file { $(OBJS) } &
          option stack=65536 &
          option map=bin\driftos2.map
    $(RC) bin\drift.res $(TARGET)

bin\drift.res : src\drift.rc src\drift.h src\resource.h src\llist.h
    $(RC) $(RCFLAGS) -r -fo=bin\drift.res src\drift.rc

bin\drift.obj   : src\drift.c src\drift.h src\resource.h src\llist.h
    $(CC) $(CFLAGS) -fo=bin\drift.obj src\drift.c

bin\objekts.obj : src\objekts.c src\drift.h src\llist.h
    $(CC) $(CFLAGS) -fo=bin\objekts.obj src\objekts.c

bin\llist.obj   : src\llist.c src\drift.h src\llist.h src\resource.h
    $(CC) $(CFLAGS) -fo=bin\llist.obj src\llist.c

bin\common.obj  : src\common.c src\common.h
    $(CC) $(CFLAGS) -fo=bin\common.obj src\common.c

clean : .SYMBOLIC
    -del bin\drift.obj bin\objekts.obj bin\llist.obj bin\common.obj 2>nul
    -del bin\drift.res 2>nul
    -del bin\driftos2.exe bin\driftos2.map 2>nul
