/**
 * @file OnlineToImage.cxx
 *
 * Implementation of OnlineToImage: converts DUNE online packed channel
 * data to a 2D image representation.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "duneonlinetoimage/OnlineToImage.hpp"

#include <algorithm>
#include <stdexcept>

namespace duneonlinetoimage {

OnlineToImage::OnlineToImage(unsigned int nChannels,
                             unsigned int nTicks,
                             View view,
                             unsigned int planeIndex)
  : m_nChannels(nChannels)
  , m_nTicks(nTicks)
  , m_view(view)
  , m_planeIndex(planeIndex)
  , m_image(nChannels * nTicks, 0)
{
  if (nChannels == 0) {
    throw std::invalid_argument("OnlineToImage: nChannels must be > 0");
  }
  if (nTicks == 0) {
    throw std::invalid_argument("OnlineToImage: nTicks must be > 0");
  }
}

std::string
OnlineToImage::viewName(View v)
{
  switch (v) {
    case View::kU:
      return "U";
    case View::kV:
      return "V";
    case View::kZ:
      return "Z";
    default:
      return "Unknown";
  }
}

Pixel
OnlineToImage::toPixel(unsigned int channel, unsigned int tick, int16_t adc) const
{
  Pixel px{};
  px.channel = channel;
  px.tick = tick;
  px.adc = adc;

  if (channel >= m_nChannels || tick >= m_nTicks) {
    // Signal out-of-range by setting channel to the sentinel value
    px.channel = m_nChannels;
    return px;
  }

  px.coord.plane = m_planeIndex;
  px.coord.row = channel;
  px.coord.view = m_view;
  return px;
}

void
OnlineToImage::fill(unsigned int channel, unsigned int tick, int16_t adc)
{
  if (channel >= m_nChannels || tick >= m_nTicks) {
    return;
  }
  m_image[channel * m_nTicks + tick] = adc;
}

void
OnlineToImage::clear()
{
  std::fill(m_image.begin(), m_image.end(), static_cast<int16_t>(0));
}

} // namespace duneonlinetoimage
