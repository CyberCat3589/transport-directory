#include "stat_reader.h"
#include <iomanip>
#include <vector>

using namespace transport_catalogue;
using namespace std::literals;

std::ostream& operator<<(std::ostream& out, BusStatistics bus_stat)
{
    out << "Bus "s << bus_stat.name << ": "s << bus_stat.stops_count << " stops on route, "s 
    << bus_stat.unique_stops_count << " unique stops, "s 
    << std::setprecision(6) << bus_stat.route_length << " route length"s << '\n';
    return out;
}

std::ostream& operator<<(std::ostream& out, std::vector<Bus*> buses)
{
    for (auto bus : buses)
    {
        out << " "s << bus->name;
    }
    out << '\n';
    return out;
}

void PrintBus(const TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output)
{
    Bus* find_bus = transport_catalogue.GetBusByName(request);
    if (find_bus == nullptr)
    {
        output << "Bus "s << request << ": not found"s << '\n';
    }
    else
    {
        BusStatistics bus_stat = transport_catalogue.GetBusStatistics(find_bus->name);

        output << bus_stat;
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
        std::vector<Bus*> buses = transport_catalogue.GetStopBuses(stop->name);
        if (buses.empty())
        {
            output << "Stop "s << request << ": no buses"s << '\n';
        }
        else
        {
            output << "Stop "s << request << ": buses"s << buses;
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