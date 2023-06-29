// Copyright (c) Meta Platforms, Inc. and affiliates.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include "fairseq2/native/data/zip_data_source.h"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <system_error>
#include <utility>

#include <fmt/core.h>

#include "fairseq2/native/error.h"
#include "fairseq2/native/data/data_pipeline.h"
#include "fairseq2/native/data/file.h"
#include "fairseq2/native/data/immutable_string.h"
#include "fairseq2/native/data/stream.h"
#include "fairseq2/native/utils/string.h"
#include <zip/src/zip.h>
#include "fairseq2/native/memory.h"

namespace fairseq2::detail {

zip_data_source::zip_data_source(std::string &&pathname)
    : pathname_{std::move(pathname)}
{
    try {
        zip_reader_ = zip_open(pathname_.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
        num_entries_ = (std::size_t)zip_entries_total(zip_reader_);
    } catch (const std::exception &) {
        handle_error();
    }
}

std::optional<data>
zip_data_source::next()
{
    if (num_files_read_ >= num_entries_) return std::nullopt;

    fairseq2::writable_memory_block zip_entry;
    zip_entry_openbyindex(zip_reader_, num_files_read_);
    {
        auto size = zip_entry_size(zip_reader_);
        zip_entry = fairseq2::allocate_memory(size);
        zip_entry_noallocread(zip_reader_, (void *)zip_entry.data(), size);
    }
    zip_entry_close(zip_reader_);

    num_files_read_ += 1;
    return immutable_string{zip_entry};
}

void
zip_data_source::reset()
{
    num_files_read_ = 0;
}

void
zip_data_source::record_position(tape &t) const
{
    t.record(num_files_read_);
}

void
zip_data_source::reload_position(tape &t)
{
    auto num_files_read = t.read<std::size_t>();

    reset();

    // TODO: use random access
    for (std::size_t i = 0; i < num_files_read; i++)
        next();
}

void
zip_data_source::handle_error()
{
    try {
        throw;
    } catch (const stream_error &) {
        throw_read_failure();
    } catch (const std::system_error &) {
        throw_read_failure();
    }
}

inline void
zip_data_source::throw_read_failure()
{
    data_pipeline_error::throw_nested(
        fmt::format("The data pipeline cannot read from '{}'.", pathname_));
}

}  // namespace fairseq2::detail