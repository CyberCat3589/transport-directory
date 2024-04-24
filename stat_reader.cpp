#include "stat_reader.h"
#include <iomanip>
#include <unordered_set>
#include <vector>

void ParseAndPrintStat(const TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output)
{
    using namespace std::literals;

    auto space = request.find_first_of(' ');
    std::string_view query_type = request.substr(0, space);
    request = request.substr(space + 1);

    if (query_type == "Bus")
    {
        Bus* find_bus = transport_catalogue.GetBusByName(request);
        if (find_bus == nullptr)
        {
            output << "Bus "s << request << ": not found"s << '\n';
        }
        else
        {
            std::vector<Stop*> bus_stops = find_bus->stops;

            int stops_count = bus_stops.size();  // кол-во остановок в маршруте

            std::unordered_set unique_stops(bus_stops.cbegin(), bus_stops.cend());
            int unique_stops_count = unique_stops.size();  // кол-во уникальных остановок

            // расчёт длины маршрута
            double route_length = 0;
            for (auto it = bus_stops.begin(); it != std::prev(bus_stops.end(), 1); ++it)
            {
                auto next_stop = std::next(it);
                Stop* stop1 = *it;
                Stop* stop2 = *next_stop;

                route_length += ComputeDistance(stop1->coordinates, stop2->coordinates);
            }

            output << "Bus "s << request 
            << ": "s << stops_count << " stops on route, "s 
            << unique_stops_count << " unique stops, "s
             << std::setprecision(6) << route_length << " route length"s << '\n';
        }
    }
    else if (query_type == "Stop")
    {
    }
}