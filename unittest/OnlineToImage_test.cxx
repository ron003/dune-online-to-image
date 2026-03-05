/**
 * @file OnlineToImage_test.cxx
 *
 * Unit tests for the duneonlinetoimage::OnlineToImage class.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#define BOOST_TEST_MODULE OnlineToImage_test // NOLINT

#include "duneonlinetoimage/OnlineToImage.hpp"

#include "boost/test/unit_test.hpp"

using duneonlinetoimage::OnlineToImage;
using duneonlinetoimage::View;

BOOST_AUTO_TEST_SUITE(OnlineToImage_test)

BOOST_AUTO_TEST_CASE(ConstructionAndAccessors)
{
  OnlineToImage oti(100, 512, View::kU, 0);
  BOOST_REQUIRE_EQUAL(oti.nChannels(), 100u);
  BOOST_REQUIRE_EQUAL(oti.nTicks(), 512u);
  BOOST_REQUIRE(oti.view() == View::kU);
  BOOST_REQUIRE_EQUAL(oti.image().size(), 100u * 512u);
}

BOOST_AUTO_TEST_CASE(ViewNames)
{
  BOOST_REQUIRE_EQUAL(OnlineToImage::viewName(View::kU), "U");
  BOOST_REQUIRE_EQUAL(OnlineToImage::viewName(View::kV), "V");
  BOOST_REQUIRE_EQUAL(OnlineToImage::viewName(View::kZ), "Z");
  BOOST_REQUIRE_EQUAL(OnlineToImage::viewName(View::kUnknown), "Unknown");
}

BOOST_AUTO_TEST_CASE(FillAndReadBack)
{
  OnlineToImage oti(10, 20, View::kV, 1);

  // All pixels start at zero
  for (auto v : oti.image()) {
    BOOST_REQUIRE_EQUAL(v, 0);
  }

  oti.fill(3, 7, 42);
  BOOST_REQUIRE_EQUAL(oti.image()[3 * 20 + 7], 42);

  // Other pixels remain zero
  BOOST_REQUIRE_EQUAL(oti.image()[3 * 20 + 6], 0);
  BOOST_REQUIRE_EQUAL(oti.image()[3 * 20 + 8], 0);
}

BOOST_AUTO_TEST_CASE(FillOutOfRange)
{
  OnlineToImage oti(10, 20, View::kZ, 2);
  // Should not crash or modify anything
  oti.fill(10, 0, 99);  // channel out of range
  oti.fill(0, 20, 99);  // tick out of range
  for (auto v : oti.image()) {
    BOOST_REQUIRE_EQUAL(v, 0);
  }
}

BOOST_AUTO_TEST_CASE(ClearResetsBuffer)
{
  OnlineToImage oti(5, 8, View::kU, 0);
  oti.fill(2, 3, 100);
  oti.clear();
  for (auto v : oti.image()) {
    BOOST_REQUIRE_EQUAL(v, 0);
  }
}

BOOST_AUTO_TEST_CASE(ToPixelInRange)
{
  OnlineToImage oti(16, 32, View::kZ, 2);
  auto px = oti.toPixel(5, 10, 77);
  BOOST_REQUIRE_EQUAL(px.channel, 5u);
  BOOST_REQUIRE_EQUAL(px.tick, 10u);
  BOOST_REQUIRE_EQUAL(px.adc, 77);
  BOOST_REQUIRE_EQUAL(px.coord.plane, 2u);
  BOOST_REQUIRE_EQUAL(px.coord.row, 5u);
  BOOST_REQUIRE(px.coord.view == View::kZ);
}

BOOST_AUTO_TEST_CASE(ToPixelOutOfRange)
{
  OnlineToImage oti(16, 32, View::kU, 0);
  // channel out of range — sentinel channel value returned
  auto px = oti.toPixel(16, 0, 1);
  BOOST_REQUIRE_EQUAL(px.channel, oti.nChannels());

  // tick out of range
  auto px2 = oti.toPixel(0, 32, 1);
  BOOST_REQUIRE_EQUAL(px2.channel, oti.nChannels());
}

BOOST_AUTO_TEST_CASE(InvalidConstructionThrows)
{
  BOOST_REQUIRE_THROW(OnlineToImage(0, 10, View::kU, 0), std::invalid_argument);
  BOOST_REQUIRE_THROW(OnlineToImage(10, 0, View::kU, 0), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
