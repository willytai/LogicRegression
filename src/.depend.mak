Parser.o: Parser.cpp Mgr.h Variable.h patternBank.h RandPatGen.h
GeneratePLA.o: GeneratePLA.cpp Mgr.h Variable.h patternBank.h \
 RandPatGen.h
Mgr.o: Mgr.cpp Mgr.h Variable.h patternBank.h RandPatGen.h myUsage.h
ABC.o: ABC.cpp Mgr.h Variable.h patternBank.h RandPatGen.h
myUsage.o: myUsage.cpp myUsage.h
patternBank.o: patternBank.cpp patternBank.h RandPatGen.h
main.o: main.cpp Mgr.h Variable.h patternBank.h RandPatGen.h myUsage.h
