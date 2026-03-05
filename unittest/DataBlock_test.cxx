/**
 * @file DataBlock_test.cxx DataBlock class Unit Tests
 *
 * This is part of the DUNE online-to-image package.
 */

#include "dune_online_to_image/DataBlock.hpp"

#define BOOST_TEST_MODULE DataBlock_test

#include "boost/test/unit_test.hpp"

#include <cstdint>
#include <stdexcept>

using namespace dunedaq::dune_online_to_image;

BOOST_AUTO_TEST_SUITE(DataBlock_test)

BOOST_AUTO_TEST_CASE(CompileTimeConstants)
{
  BOOST_REQUIRE_EQUAL(DataBlock::s_num_rows, 64u);
  BOOST_REQUIRE_EQUAL(DataBlock::s_num_cols, 64u);
  BOOST_REQUIRE_EQUAL(DataBlock::s_bits_per_value, 14u);
  BOOST_REQUIRE_EQUAL(DataBlock::s_row_size_bytes, 112u);
  BOOST_REQUIRE_EQUAL(DataBlock::s_max_value, 0x3FFFu);

  // Total size of the raw packed data: 64 rows * 112 bytes
  BOOST_REQUIRE_EQUAL(sizeof(DataBlock::data), 64u * 112u);
}

BOOST_AUTO_TEST_CASE(ZeroInitialised)
{
  DataBlock block{};
  for (size_t row = 0; row < DataBlock::s_num_rows; ++row) {
    for (size_t col = 0; col < DataBlock::s_num_cols; ++col) {
      BOOST_REQUIRE_EQUAL(block.get_value(row, col), 0u);
    }
  }
}

BOOST_AUTO_TEST_CASE(SetAndGetRoundTrip)
{
  DataBlock block{};

  // Test corner values: 0, max, and a mid-range value
  const uint16_t test_values[] = { 0, 1, 0x1234 & DataBlock::s_max_value,
                                   DataBlock::s_max_value };

  for (uint16_t v : test_values) {
    for (size_t row = 0; row < DataBlock::s_num_rows; ++row) {
      for (size_t col = 0; col < DataBlock::s_num_cols; ++col) {
        block.set_value(row, col, v);
        BOOST_REQUIRE_EQUAL(block.get_value(row, col), v);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(ValuesDoNotBleedIntoNeighbours)
{
  DataBlock block{};

  // Set every value to max, then clear col 0 and verify neighbour cols are intact
  for (size_t row = 0; row < DataBlock::s_num_rows; ++row) {
    for (size_t col = 0; col < DataBlock::s_num_cols; ++col) {
      block.set_value(row, col, DataBlock::s_max_value);
    }
  }

  for (size_t row = 0; row < DataBlock::s_num_rows; ++row) {
    block.set_value(row, 0, 0u);
    BOOST_REQUIRE_EQUAL(block.get_value(row, 0), 0u);
    BOOST_REQUIRE_EQUAL(block.get_value(row, 1), DataBlock::s_max_value);
  }
}

BOOST_AUTO_TEST_CASE(AllColumnsIndependent)
{
  DataBlock block{};

  // Write a distinct value into each column of row 0
  for (size_t col = 0; col < DataBlock::s_num_cols; ++col) {
    block.set_value(0, col, static_cast<uint16_t>(col));
  }
  for (size_t col = 0; col < DataBlock::s_num_cols; ++col) {
    BOOST_REQUIRE_EQUAL(block.get_value(0, col), static_cast<uint16_t>(col));
  }
}

BOOST_AUTO_TEST_CASE(OutOfRangeThrows)
{
  DataBlock block{};
  BOOST_REQUIRE_THROW(block.get_value(64, 0), std::out_of_range);
  BOOST_REQUIRE_THROW(block.get_value(0, 64), std::out_of_range);
  BOOST_REQUIRE_THROW(block.set_value(64, 0, 0), std::out_of_range);
  BOOST_REQUIRE_THROW(block.set_value(0, 64, 0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(ValueMaskedTo14Bits)
{
  DataBlock block{};
  // Pass a 16-bit value that exceeds 14 bits; only the lower 14 bits should be stored
  block.set_value(0, 0, 0xFFFF);
  BOOST_REQUIRE_EQUAL(block.get_value(0, 0), DataBlock::s_max_value);
}

BOOST_AUTO_TEST_SUITE_END()
