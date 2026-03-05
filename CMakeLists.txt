cmake_minimum_required(VERSION 3.12)
project(dune_online_to_image VERSION 1.0.0)

find_package(daq-cmake REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

daq_setup_environment()

##############################################################################
# Main library

daq_add_library(LINK_LIBRARIES)

##############################################################################
# Unit Tests
daq_add_unit_test(DataBlock_test LINK_LIBRARIES dune_online_to_image)

##############################################################################

daq_install()
