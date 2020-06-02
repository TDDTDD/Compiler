cmake_minimum_required(VERSION 3.14)
project(compile)

set(CMAKE_CXX_STANDARD 14)

add_executable(compile main.cpp lexicalAnalysis.cpp lexicalAnalysis.h shared_base.h grammarAnalysis.cpp grammarAnalysis.h shared_base.cpp createAnalysisTable.cpp createAnalysisTable.h printOut.cpp printOut.h)