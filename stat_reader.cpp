#include "stat_reader.h"
#include <unordered_set>
#include <numeric>

void ParseAndPrintStat(const TransportCatalogue& transport_catalogue, std::string_view request,
                       std::ostream& output)
{
    using namespace std::literals;

    auto entry = 4;
    request = request.substr(entry);
    Bus* find_bus = transport_catalogue.GetBusByName(request);
    if(find_bus == nullptr)
    {
        output << "Bus "s << request << ": not found"s;
    }
    else
    {
        int stops_count = find_bus->stops.size(); //кол-во остановок в маршруте

        std::unordered_set unique_stops(find_bus->stops.cbegin(), find_bus->stops.cend());
        int unique_stops_count = unique_stops.size(); //кол-во уникальных остановок

        //расчёт длины маршрута без учета его типа
        int route_length = std::accumulate(find_bus->stops.cbegin(), find_bus->stops.cend(), 0,
        [](Stop* stop1, Stop* stop2)
        {
            return ComputeDistance(stop1->coordinates, stop2->coordinates);
        }); //длина маршрута


        output << "Bus "s << request << ": "s 
        << stops_count << " stops on route, "s 
        << unique_stops_count << " unique stops, "s
        << route_length << " route length"s;
    }
}