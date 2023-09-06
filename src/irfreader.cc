#include "irfreader.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
// IR 函数 reader
IRFReader::IRFReader(Function* f) {
    this->f = f;
}

set<Instruction*> IRFReader::get_asserts() const {
    set<Instruction*> ret;
    // function -> basicBlock
    for(BasicBlock& b : *(this->f)) {
        for(Instruction& i : b) {    // basicBlock -> Instruction
            if(!isa<CallInst>(&i)) {    // Instruction不是CallInst 跳过
                continue;
            }

            CallInst* ci = dyn_cast<CallInst>(&i);    // 强制类型转换 安全地向下转型
            Function* callee = ci->getCalledFunction();
            if(callee == nullptr) {
                continue;
            }

            string name = string(callee->getName());
            if(name != "abort" && name != "__assert_fail") {
                continue;
            }
            // abort 或者 __assert_fail 函数
            ret.insert(&i);     
        }
    }

    return ret;
}
