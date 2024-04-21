#include "stat_reader.h"

void ParseAndPrintStat(const TransportCatalogue& transport_catalogue, std::string_view request,
                       std::ostream &output)
{
    auto entry = 4;
    request = request.substr(entry);
    
}