/**
 * @file OnlineToImage.hpp
 *
 * OnlineToImage provides utilities for converting DUNE online packed
 * channel data to a 2D image representation suitable for further
 * processing or visualization.
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef DUNEONLINETOIMAGE_INCLUDE_DUNEONLINETOIMAGE_ONLINETOIMAGE_HPP_
#define DUNEONLINETOIMAGE_INCLUDE_DUNEONLINETOIMAGE_ONLINETOIMAGE_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace duneonlinetoimage {

/// Plane/view labels used in the detector readout
enum class View : uint8_t
{
  kU = 0, ///< U induction plane
  kV = 1, ///< V induction plane
  kZ = 2, ///< Z collection plane
  kUnknown = 3
};

/// Coordinates of a single channel in image space
struct ImageCoord
{
  unsigned int plane; ///< Plane index (0 = U, 1 = V, 2 = Z)
  unsigned int row;   ///< Row in the image (wire index within the plane)
  View view;          ///< Plane view label
};

/// One pixel entry in an image frame: channel, tick, and ADC value
struct Pixel
{
  unsigned int channel; ///< Online channel number
  unsigned int tick;    ///< Time sample (tick) index
  int16_t adc;          ///< ADC value
  ImageCoord coord;     ///< Corresponding image coordinates
};

/**
 * @class OnlineToImage
 * @brief Maps online DUNE channel IDs and ADC samples to a 2D image.
 *
 * The image axes are:
 *   - x (column) : time tick
 *   - y (row)    : wire / channel index within a view plane
 *
 * A single instance covers one detector plane (U, V, or Z).
 * Call fill() repeatedly for each channel/tick/ADC triple, then
 * retrieve the flattened pixel list via pixels().
 */
class OnlineToImage
{
public:
  /**
   * @brief Construct for a given plane.
   *
   * @param nChannels  Number of online channels belonging to this plane.
   * @param nTicks     Number of time ticks per readout window.
   * @param view       The plane view (U, V, or Z).
   * @param planeIndex Plane index used in ImageCoord (0 = U, 1 = V, 2 = Z).
   */
  OnlineToImage(unsigned int nChannels,
                unsigned int nTicks,
                View view,
                unsigned int planeIndex);

  /// Return the number of channels covered by this plane
  unsigned int nChannels() const { return m_nChannels; }

  /// Return the number of time ticks per readout window
  unsigned int nTicks() const { return m_nTicks; }

  /// Return the view associated with this plane
  View view() const { return m_view; }

  /// Return a human-readable name for the view ("U", "V", "Z", or "Unknown")
  static std::string viewName(View v);

  /**
   * @brief Map an online channel + tick to image coordinates.
   *
   * @param channel  Online channel index (0-based, within this plane).
   * @param tick     Time-sample index.
   * @param adc      ADC value for this sample.
   * @return         Populated Pixel, or a default-constructed Pixel with
   *                 channel == nChannels() if the inputs are out of range.
   */
  Pixel toPixel(unsigned int channel, unsigned int tick, int16_t adc) const;

  /**
   * @brief Fill the internal image buffer with one ADC sample.
   *
   * Out-of-range (channel, tick) pairs are silently ignored.
   *
   * @param channel  Online channel index (0-based, within this plane).
   * @param tick     Time-sample index.
   * @param adc      ADC value for this sample.
   */
  void fill(unsigned int channel, unsigned int tick, int16_t adc);

  /**
   * @brief Access the flat ADC image buffer.
   *
   * Layout: image[channel * nTicks() + tick]
   *
   * @return Const reference to the internal buffer.
   */
  const std::vector<int16_t>& image() const { return m_image; }

  /// Reset all pixels in the image buffer to zero
  void clear();

private:
  unsigned int m_nChannels;
  unsigned int m_nTicks;
  View m_view;
  unsigned int m_planeIndex;
  std::vector<int16_t> m_image; ///< Flat [channel][tick] buffer
};

} // namespace duneonlinetoimage

#endif // DUNEONLINETOIMAGE_INCLUDE_DUNEONLINETOIMAGE_ONLINETOIMAGE_HPP_
