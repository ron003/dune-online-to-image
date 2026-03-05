/**
 * @file DataBlock.hpp Defines a block of 64x64 14-bit values packed into 64x112 bytes
 *
 * This is part of the DUNE online-to-image package.
 *
 * Each DataBlock contains 64 rows of 64 14-bit pixel values.
 * Each row occupies exactly 112 bytes (64 * 14 bits = 896 bits = 112 bytes),
 * with values packed sequentially with no padding between them.
 */

#ifndef DUNE_ONLINE_TO_IMAGE_INCLUDE_DUNE_ONLINE_TO_IMAGE_DATABLOCK_HPP_
#define DUNE_ONLINE_TO_IMAGE_INCLUDE_DUNE_ONLINE_TO_IMAGE_DATABLOCK_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace dunedaq::dune_online_to_image {

/**
 * @brief DataBlock holds 64x64 14-bit values packed into 64x112 bytes.
 *
 * Values are packed sequentially: each 14-bit value occupies bits
 * [col*14 + 13 : col*14] within the 896-bit (112-byte) row buffer,
 * stored in little-endian bit order within each byte.
 *
 * The maximum representable value for a 14-bit sample is 16383 (0x3FFF).
 */
struct DataBlock
{
  /// Number of rows in one block
  static constexpr size_t s_num_rows = 64;
  /// Number of columns (samples) per row
  static constexpr size_t s_num_cols = 64;
  /// Bits per sample
  static constexpr size_t s_bits_per_value = 14;
  /// Bytes per row: 64 * 14 bits = 896 bits = 112 bytes
  static constexpr size_t s_row_size_bytes = (s_num_cols * s_bits_per_value) / 8; // 112
  /// Maximum value representable in 14 bits
  static constexpr uint16_t s_max_value = (1u << s_bits_per_value) - 1; // 16383

  static_assert(s_row_size_bytes == 112, "Row size must be 112 bytes");
  // The last 14-bit field in a row ends at bit (s_num_cols-1)*s_bits_per_value + s_bits_per_value - 1
  // = 63*14 + 13 = 895, which is in byte 895/8 = 111 -- exactly the last byte of s_row_size_bytes.
  static_assert((s_num_cols * s_bits_per_value) == (s_row_size_bytes * 8),
                "Row bytes must exactly hold all column bits with no spare bits");

  /// Raw packed data: 64 rows x 112 bytes each
  uint8_t data[s_num_rows][s_row_size_bytes]{};

  /**
   * @brief Read a single 14-bit value from the block.
   * @param row  Row index [0, 63]
   * @param col  Column index [0, 63]
   * @return     The 14-bit value at (row, col)
   */
  uint16_t get_value(size_t row, size_t col) const
  {
    if (row >= s_num_rows || col >= s_num_cols) {
      throw std::out_of_range("DataBlock::get_value: index out of range");
    }

    const size_t bit_offset = col * s_bits_per_value;
    const size_t byte_idx   = bit_offset / 8;
    const size_t bit_shift  = bit_offset % 8;

    // Read two bytes and extract the 14-bit field.
    // The field always fits within at most 3 bytes (max shift is 6, field is 14 bits).
    uint32_t raw = static_cast<uint32_t>(data[row][byte_idx])
                 | (static_cast<uint32_t>(data[row][byte_idx + 1]) << 8);
    if (bit_shift + s_bits_per_value > 16) {
      raw |= static_cast<uint32_t>(data[row][byte_idx + 2]) << 16;
    }
    return static_cast<uint16_t>((raw >> bit_shift) & s_max_value);
  }

  /**
   * @brief Write a single 14-bit value into the block.
   * @param row    Row index [0, 63]
   * @param col    Column index [0, 63]
   * @param value  14-bit value to store (bits above 13 are ignored)
   */
  void set_value(size_t row, size_t col, uint16_t value)
  {
    if (row >= s_num_rows || col >= s_num_cols) {
      throw std::out_of_range("DataBlock::set_value: index out of range");
    }

    const uint16_t masked_value = value & s_max_value;
    const size_t bit_offset     = col * s_bits_per_value;
    const size_t byte_idx       = bit_offset / 8;
    const size_t bit_shift      = bit_offset % 8;

    // Clear the 14-bit field, then OR in the new value.
    const uint32_t field_mask = static_cast<uint32_t>(s_max_value) << bit_shift;

    uint32_t raw = static_cast<uint32_t>(data[row][byte_idx])
                 | (static_cast<uint32_t>(data[row][byte_idx + 1]) << 8);
    if (bit_shift + s_bits_per_value > 16) {
      raw |= static_cast<uint32_t>(data[row][byte_idx + 2]) << 16;
    }

    raw = (raw & ~field_mask) | (static_cast<uint32_t>(masked_value) << bit_shift);

    data[row][byte_idx]     = static_cast<uint8_t>(raw & 0xFF);
    data[row][byte_idx + 1] = static_cast<uint8_t>((raw >> 8) & 0xFF);
    if (bit_shift + s_bits_per_value > 16) {
      data[row][byte_idx + 2] = static_cast<uint8_t>((raw >> 16) & 0xFF);
    }
  }
};

} // namespace dunedaq::dune_online_to_image

#endif // DUNE_ONLINE_TO_IMAGE_INCLUDE_DUNE_ONLINE_TO_IMAGE_DATABLOCK_HPP_
