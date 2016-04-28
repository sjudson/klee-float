//===-- FindArrayAckermannizationVisitor.h ----------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef KLEE_FIND_ARRAY_ACKERMANNIZATION_VISITOR_H
#define KLEE_FIND_ARRAY_ACKERMANNIZATION_VISITOR_H

#include "klee/util/ExprVisitor.h"
#include <map>

namespace klee {

struct ArrayAckermannizationInfo {
  ref<Expr> toReplace;
  bool isContiguousArrayRead();
  unsigned contiguousMSBitIndex; // only relevant if isContiguousArrayRead() is true
  unsigned contiguousLSBitIndex; // only relevant if isContiguousArrayRead() is true
  const Array *getArray();
  bool isWholeArray();
  ArrayAckermannizationInfo();
};

/// This class looks for opportunities to perform ackermannization (ackermann's
/// reduction) of array reads.  The reduction here is transforming array reads
/// into uses of bitvector variables of bitwidth <= ``maxArrayWidth``. Note
/// this visitor doesn't actually modify the expressions given to it, instead
/// it just looks for opportunities to apply the reduction.
class FindArrayAckermannizationVisitor : ExprVisitor {
public:
  FindArrayAckermannizationVisitor(bool recursive, unsigned maxArrayWidth);
  void clear();
  const unsigned maxArrayWidth; // maxWidth of Arrays in bits to consider as
                                // candidates for ackermannization

  // FIXME: Should we hide this behind an interface?
  /// The recorded Array's that can be ackermannized.
  /// If an Array maps to empty vector then the detected array cannot be
  /// ackermannized.
  /// If an Array maps to a non-empty vector then that vector gives all the
  /// expressions
  /// that can be ackermannized.
  typedef std::map<const Array *, std::vector<ArrayAckermannizationInfo> >
      ArrayToAckermannizationInfoMapTy;
  ArrayToAckermannizationInfoMapTy ackermannizationInfo;
  std::vector<ArrayAckermannizationInfo> *
  getOrInsertAckermannizationInfo(const Array *, bool *wasInsert);

protected:
  Action visitConcat(const ConcatExpr &);
  Action visitRead(const ReadExpr &);
};
}

#endif
