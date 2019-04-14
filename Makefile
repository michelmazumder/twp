CPPFLAGS=-std=c++17 -Isrc
LPPFLAGS=${CPPFLAGS} -lpthread -lpanel -lncurses
DEST_DIR=/Users/michel/programmazione/twp
#DEST_DIR=/Users/michel/Documents/Progetti\ Software/twp
TARGET_EXE_NAME=$(DEST_DIR)/twp

all:
	make twp

clean:
	rm $(DEST_DIR)/obj/*


twp: src/main.cpp $(DEST_DIR)/obj/TwpApplication.o \
		$(DEST_DIR)/obj/m3d_Thread.o $(DEST_DIR)/obj/m3d_Semaforo.o $(DEST_DIR)/obj/m3d_Lock.o \
		$(DEST_DIR)/obj/twp_Document.o $(DEST_DIR)/obj/twp_Attributes.o \
		$(DEST_DIR)/obj/util_MethodLogger.o $(DEST_DIR)/obj/util_StringExtensions.o $(DEST_DIR)/obj/util_VersionDataObject.o \
		$(DEST_DIR)/obj/win_Screen.o $(DEST_DIR)/obj/win_Window.o $(DEST_DIR)/obj/win_Application.o $(DEST_DIR)/obj/win_BorderedWindow.o \
		$(DEST_DIR)/obj/tdm_Chunk.o $(DEST_DIR)/obj/tdm_Rope.o $(DEST_DIR)/obj/tdm_AttributedRope.o \
		$(DEST_DIR)/obj/tdm_TreeAttributedRope.o
	c++ $(DEST_DIR)/obj/* src/main.cpp $(LPPFLAGS) -o $(TARGET_EXE_NAME)

$(DEST_DIR)/obj/TwpApplication.o: src/TwpApplication.cpp src/TwpApplication.h
	c++ $(CPPFLAGS) -c src/TwpApplication.cpp -o $(DEST_DIR)/obj/TwpApplication.o

$(DEST_DIR)/obj/util_MethodLogger.o: src/util/MethodLogger.h src/util/MethodLogger.cpp
	c++ $(CPPFLAGS) -c src/util/MethodLogger.cpp -o $(DEST_DIR)/obj/util_MethodLogger.o

$(DEST_DIR)/obj/util_StringExtensions.o: src/util/StringExtensions.h src/util/StringExtensions.cpp
	c++ $(CPPFLAGS) -c src/util/StringExtensions.cpp -o $(DEST_DIR)/obj/util_StringExtensions.o

$(DEST_DIR)/obj/util_VersionDataObject.o: src/util/VersionDataObject.h src/util/VersionDataObject.cpp
	c++ $(CPPFLAGS) -c src/util/VersionDataObject.cpp -o $(DEST_DIR)/obj/util_VersionDataObject.o

$(DEST_DIR)/obj/twp_Attributes.o: src/twp/Attributes.h src/twp/Attributes.cpp
	c++ $(CPPFLAGS) -c src/twp/Attributes.cpp -o $(DEST_DIR)/obj/twp_Attributes.o

$(DEST_DIR)/obj/twp_Document.o: src/twp/Document.h src/twp/Document.cpp
	c++ $(CPPFLAGS) -c src/twp/Document.cpp -o $(DEST_DIR)/obj/twp_Document.o

$(DEST_DIR)/obj/win_Screen.o: src/win/Screen.h src/win/Screen.cpp
	c++ $(CPPFLAGS) -c src/win/Screen.cpp -o $(DEST_DIR)/obj/win_Screen.o

$(DEST_DIR)/obj/win_Window.o: src/win/Window.h src/win/Window.cpp
	c++ $(CPPFLAGS) -c src/win/Window.cpp -o $(DEST_DIR)/obj/win_Window.o
	
$(DEST_DIR)/obj/win_BorderedWindow.o: src/win/BorderedWindow.h src/win/BorderedWindow.cpp
	c++ $(CPPFLAGS) -c src/win/BorderedWindow.cpp -o $(DEST_DIR)/obj/win_BorderedWindow.o
	
$(DEST_DIR)/obj/win_Application.o: src/win/Application.h src/win/Application.cpp
	c++ $(CPPFLAGS) -c src/win/Application.cpp -o $(DEST_DIR)/obj/win_Application.o

$(DEST_DIR)/obj/m3d_Thread.o: src/m3d/Thread.cpp src/m3d/Thread.h
	c++ $(CPPFLAGS) -c src/m3d/Thread.cpp -o $(DEST_DIR)/obj/m3d_Thread.o

$(DEST_DIR)/obj/m3d_Semaforo.o: src/m3d/Semaforo.cpp src/m3d/Semaforo.h
	c++ $(CPPFLAGS) -c src/m3d/Semaforo.cpp -o $(DEST_DIR)/obj/m3d_Semaforo.o

$(DEST_DIR)/obj/m3d_Lock.o: src/m3d/Lock.cpp src/m3d/Lock.h
	c++ $(CPPFLAGS) -c src/m3d/Lock.cpp -o $(DEST_DIR)/obj/m3d_Lock.o

$(DEST_DIR)/obj/tdm_Chunk.o: src/tdm/CharType.h src/tdm/Chunk.h src/tdm/Chunk.cpp
	c++ $(CPPFLAGS) -c src/tdm/Chunk.cpp -o $(DEST_DIR)/obj/tdm_Chunk.o

$(DEST_DIR)/obj/tdm_Rope.o: src/tdm/CharType.h src/tdm/Rope.h src/tdm/Rope.cpp
	c++ $(CPPFLAGS) -c src/tdm/Rope.cpp -o $(DEST_DIR)/obj/tdm_Rope.o

$(DEST_DIR)/obj/tdm_AttributedRope.o: src/tdm/CharType.h src/tdm/AttributedRope.h src/tdm/AttributedRope.cpp
	c++ $(CPPFLAGS) -c src/tdm/AttributedRope.cpp -o $(DEST_DIR)/obj/tdm_AttributedRope.o

$(DEST_DIR)/obj/tdm_TreeAttributedRope.o: src/tdm/CharType.h src/tdm/TreeAttributedRope.h src/tdm/TreeAttributedRope.cpp
	c++ $(CPPFLAGS) -c src/tdm/TreeAttributedRope.cpp -o $(DEST_DIR)/obj/tdm_TreeAttributedRope.o
