/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_TRACE_PROCESSOR_DB_COLUMN_NULL_OVERLAY_H_
#define SRC_TRACE_PROCESSOR_DB_COLUMN_NULL_OVERLAY_H_

#include <memory>
#include <variant>

#include "src/trace_processor/containers/bit_vector.h"
#include "src/trace_processor/db/column/column.h"
#include "src/trace_processor/db/column/types.h"

namespace perfetto {
namespace trace_processor {
namespace column {

// Overlay which introduces the layer of nullability. Specifically, spreads out
// the storage with nulls using a BitVector.
class NullOverlay : public Column {
 public:
  NullOverlay(std::unique_ptr<Column> storage, const BitVector* non_null);

  SearchValidationResult ValidateSearchConstraints(SqlValue,
                                                   FilterOp) const override;

  RangeOrBitVector Search(FilterOp op,
                          SqlValue value,
                          RowMap::Range range) const override;

  RangeOrBitVector IndexSearch(FilterOp op,
                               SqlValue value,
                               uint32_t* indices,
                               uint32_t indices_count,
                               bool sorted) const override;

  void StableSort(uint32_t* rows, uint32_t rows_size) const override;

  void Sort(uint32_t* rows, uint32_t rows_size) const override;

  void Serialize(StorageProto*) const override;

  uint32_t size() const override { return non_null_->size(); }

 private:
  std::unique_ptr<Column> storage_ = nullptr;
  const BitVector* non_null_ = nullptr;
};

}  // namespace column
}  // namespace trace_processor
}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_DB_COLUMN_NULL_OVERLAY_H_
