/**
 * @file online-to-image.cxx
 *
 * Test application that exercises the duneonlinetoimage library.
 * It constructs an OnlineToImage instance for each of the three ICEBERG
 * readout planes (U, V, Z), fills the image with synthetic ADC data, and
 * prints a summary of non-zero pixels to stdout.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "duneonlinetoimage/OnlineToImage.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

int
main(int /*argc*/, char** /*argv*/)
{
  using duneonlinetoimage::OnlineToImage;
  using duneonlinetoimage::View;

  // ICEBERG geometry: 316 U wires, 315 V wires, 240 Z wires (one TPC face)
  const unsigned int nTicksPerWindow = 4096;

  std::vector<OnlineToImage> planes{
    OnlineToImage(316, nTicksPerWindow, View::kU, 0),
    OnlineToImage(315, nTicksPerWindow, View::kV, 1),
    OnlineToImage(240, nTicksPerWindow, View::kZ, 2),
  };

  // Fill planes with synthetic ramp ADC data
  for (auto& plane : planes) {
    for (unsigned int ch = 0; ch < plane.nChannels(); ++ch) {
      for (unsigned int tick = 0; tick < plane.nTicks(); ++tick) {
        auto adc = static_cast<int16_t>((ch + tick) % 256);
        plane.fill(ch, tick, adc);
      }
    }
  }

  // Print a summary: first and last pixel of each plane
  for (const auto& plane : planes) {
    const auto& img = plane.image();
    const std::string vname = OnlineToImage::viewName(plane.view());
    std::cout << "Plane " << vname
              << ": channels=" << plane.nChannels()
              << " ticks=" << plane.nTicks()
              << " image_size=" << img.size()
              << " img[0]=" << img[0]
              << " img[last]=" << img.back()
              << "\n";
  }

  // Demonstrate toPixel for channel 0, tick 0 on each plane
  for (const auto& plane : planes) {
    auto px = plane.toPixel(0, 0, 42);
    std::cout << "toPixel view=" << OnlineToImage::viewName(px.coord.view)
              << " plane=" << px.coord.plane
              << " row=" << px.coord.row
              << " channel=" << px.channel
              << " tick=" << px.tick
              << " adc=" << px.adc
              << "\n";
  }

  return 0;
}
