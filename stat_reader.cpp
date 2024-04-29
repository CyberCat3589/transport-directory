#include "stat_reader.h"
#include <iomanip>
#include <vector>

using namespace transport_catalogue;
using namespace std::literals;

void PrintBus(const TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output)
{
    Bus* find_bus = transport_catalogue.GetBusByName(request);
    if (find_bus == nullptr)
    {
        output << "Bus "s << request << ": not found"s << '\n';
    }
    else
    {
        auto [stops_count, unique_stops_count, route_length] = transport_catalogue.GetBusStatistics(find_bus);

        output << "Bus "s << request << ": "s << stops_count << " stops on route, "s << unique_stops_count << " unique stops, "s
               << std::setprecision(6) << route_length << " route length"s << '\n';
    }
}

void PrintStop(const TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output)
{
    Stop* stop = transport_catalogue.GetStopByName(request);
    if (stop == nullptr)
    {
        output << "Stop "s << request << ": not found"s << '\n';
    }
    else
    {
        std::vector<Bus*> buses = transport_catalogue.GetStopBuses(stop);
        if (buses.empty())
        {
            output << "Stop "s << request << ": no buses"s << '\n';
        }
        else
        {
            output << "Stop "s << request << ": buses"s;

            for (auto bus : buses)
            {
                output << " "s << bus->name;
            }
            output << '\n';
        }
    }
}

void transport_catalogue::stat_reader::ParseAndPrintStat(const TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output)
{
    auto space = request.find_first_of(' ');
    std::string_view query_type = request.substr(0, space);
    request = request.substr(space + 1);

    if (query_type == "Bus")
    {
        PrintBus(transport_catalogue, request, output);
    }
    else if (query_type == "Stop")
    {
        PrintStop(transport_catalogue, request, output);
    }
}