##################################
# makefile
# cym
# 2024-03-30
##################################
PATH_BIN = bin
PATH_LIB = lib
PATH_OBJ = obj

# 定义路径
PATH_MRPC = mRPC
PATH_COMM = $(PATH_MRPC)/common

PATH_TESTCASES = testcases

CXX := g++
CXXFLAGS += -g -O0 -std=c++11 -Wall -Wno-deprecated -Wno-unused-but-set-variable
CXXFLAGS += -I./ -I$(PATH_MRPC) -I$(PATH_COMM)

LIBS += /usr/local/lib/libprotobuf.a /usr/local/lib/libtinyxml.a

# [1]wildcard筛选所有.cc文件 [2]patsubst.cc替换为.o [3]赋值给COMM_OBJ
COMM_OBJ := $(patsubst $(PATH_COMM)/%.cc, $(PATH_OBJ)/%.o, $(wildcard $(PATH_COMM)/*.cc))

TEST_CASE_OUT := $(PATH_BIN)/test_log

LIB_OUT := $(PATH_LIB)/libmrpc.a

# test
$(PATH_BIN)/test_log : $(LIB_OUT)
	$(CXX) $(CXXFLAGS) $(PATH_TESTCASES)/test_log.cc -o $@ $(LIB_OUT) $(LIBS) -ldl -pthread

$(LIB_OUT) : $(COMM_OBJ)
	cd $(PATH_OBJ) && ar rcv libmrpc.a *.o && mv libmrpc.a ../lib/

# 用各个模块的.cc生成.o,统一放obj目录下
$(PATH_OBJ)/%.o : $(PATH_COMM)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


#######################target
# 生成测试文件
all : 
	ALL_TESTS

ALL_TESTS :
	$(PATH_BIN)/test_log

# 清理中间文件及输出文件
clean :
	rm -f $(COMM_OBJ) $(PATH_OBJ)/*.o $(PATH_LIB)/libmrpc.a 

# makefile经常写错，用这个看下变量值有没有弄错了
print:
	$(info $(PATH_OBJ)) 
	$(info $(PATH_COMM)) 
	$(info $(COMM_OBJ)) 