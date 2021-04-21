CC = gcc

TARGET = modbus_scanner

#TARGET = testscan


CFLAGS = -W -Wall

POWOPT = -lm

INCHDR = -I/home/hys/modbusscanner/Header
CMDLIB = libcmd2
LOGLIB = libNwLogNew
LIBPATH = /home/hys/libs

SOURCE = main.c modbus_tcp_connect.c modbus_scan.c etcfunction.c cmd_connect.c send_modbus_msg.c recv_modbus_msg.c print_file.c
OBJS = main.o modbus_tcp_connect.o modbus_scan.o etcfunction.o cmd_connect.o send_modbus_msg.o recv_modbus_msg.o print_file.o

OBJSDIR = objs


all : $(TARGET)



$(OBJS) : $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE) -I$(LIBPATH)/$(CMDLIB)/include -I$(LIBPATH)/$(LOGLIB) $(INCHDR) -L$(LIBPATH)/$(CMDLIB)/lib -L$(LIBPATH)/$(LOGLIB) -lcmd2x86 -lNwLogNew $(POWOPT) -lpthread

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -I$(LIBPATH)/$(CMDLIB)/include -I$(LIBPATH)/$(LOGLIB) $(INCHDR) -L$(LIBPATH)/$(CMDLIB)/lib -L$(LIBPATH)/$(LOGLIB) -lcmd2x86 -lNwLogNew $(POWOPT) -lpthread
	mv $(OBJS) $(OBJSDIR)

dir : 
	mkdir $(OBJSDIR)

clean :
	rm -rf $(TARGET) $(OBJSDIR)/*.o
