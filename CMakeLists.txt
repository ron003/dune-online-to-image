# This is a skeleton CMakeLists.txt file, auto-generated on
# Thu Mar  5 01:24:33 UTC 2026.  The developer(s) of this package should delete
# this comment as well as adding dependent targets, packages,
# etc. specific to the package. For details on how to write a package,
# please see
# https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/

cmake_minimum_required(VERSION 3.12)
project(dune-online-to-image VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

##############################################################################
# Main library

# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_library

daq_add_library(OnlineToImage.cxx LINK_LIBRARIES)

##############################################################################
# Applications

# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_application

daq_add_application(online-to-image online-to-image.cxx LINK_LIBRARIES duneonlinetoimage)

##############################################################################
# Unit Tests

# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_unit_test

daq_add_unit_test(OnlineToImage_test LINK_LIBRARIES duneonlinetoimage)

##############################################################################

daq_install()
