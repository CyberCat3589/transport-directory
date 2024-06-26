#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace transport_catalogue::stat_reader
{
void ParseAndPrintStat(const TransportCatalogue &tansport_catalogue, std::string_view request,
                       std::ostream &output);
}
