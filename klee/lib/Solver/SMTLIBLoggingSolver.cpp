//===-- SMTLIBLoggingSolver.cpp -------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "klee/util/ExprSMTLIBPrinter.h"
#include "QueryLoggingSolver.h"

using namespace klee;

/// This QueryLoggingSolver will log queries to a file in the SMTLIBv2 format
/// and pass the query down to the underlying solver.
class SMTLIBLoggingSolver : public QueryLoggingSolver {
private:
    ExprSMTLIBPrinter printer;

    virtual void printQuery(const Query &query, const Query *falseQuery = 0, const ArrayVec &objects = ArrayVec()) {
        if (falseQuery == nullptr) {
            printer.setQuery(query);
        } else {
            printer.setQuery(*falseQuery);
        }

        if (!objects.empty()) {
            printer.setArrayValuesToGet(objects);
        }

        printer.generateOutput();
    }

public:
    SMTLIBLoggingSolver(Solver *_solver, std::string path, int queryTimeToLog)
        : QueryLoggingSolver(_solver, path, ";", queryTimeToLog) {
        // Setup the printer
        printer.setOutput(logBuffer);
    }
};

Solver *klee::createSMTLIBLoggingSolver(Solver *_solver, std::string path, int minQueryTimeToLog) {
    return new Solver(new SMTLIBLoggingSolver(_solver, path, minQueryTimeToLog));
}
