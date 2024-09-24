#include "oemcommands.hpp"

#include <boost/container/flat_map.hpp>
#include <ipmid/api.hpp>
#include <ipmid/types.hpp>
#include <ipmid/utils.hpp>
#include <phosphor-logging/log.hpp>

#include <cstdlib>

using namespace phosphor::logging;

// src/oem_command.cpp

const char* temperatureBusName = "mowgli.tmp275.bus";
const char* temperatureObjectPath = "/mowgli/example/temperature";
const char* setThresholdInterfaceName = "mowgli.example.setThreshold";

// 假設 property 和 method 的名稱
constexpr const char* PROPERTY_NAME = "Temperature";
constexpr const char* METHOD_NAME = "setThreshold";
static ipmi::ServiceCache
    temperatureBusName(setThresholdInterfaceName, temperatureObjectPath);

ipmi::RspType<> register_get_temperature_command()
{
    auto bus = getSdBus();
    std::string service = temperatureBusName.getService(*bus);
    auto variant =
        ipmi::getDbusProperty(*bus, temperatureBusName, temperatureObjectPath,
                              setThresholdInterfaceName, PROPERTY_NAME);
}

ipmi::RspType<> register_set_threshold_command()
{
    std::shared_ptr<sdbusplus::asio::connection> dbus = getSdBus();
    auto setThreshold =
        dbus.new_method_call(temperatureBusName, temperatureObjectPath,
                             setThresholdInterfaceName, METHOD_NAME);
    setThreshold.append(0);
    setThreshold.append(10);
    setThreshold.append(40);
    setThreshold.append(80);
    try
    {
        dbus->call(setThreshold);
    }
    catch (const sdbusplus::exception_t&)
    {
        phosphor::logging::log<phosphor::logging::level::ERR>(
            "Failed to set Threshold");
    }
}
int main()
{
    static ipmi::ServiceCache LEDService(setThresholdInterfaceName,
                                         temperatureObjectPath);
    void registerOEMFunctions()
    {
        // prioOemBase 20 Set the priority as the lowest number
        ipmi::registerHandler(ipmi::prioOemBase, ipmi::rick::netFnAPP,
                              ipmi::general::cmdGetTemp275,
                              ipmi::Privilege::User,
                              register_get_temperature_command);

        ipmi::registerHandler(ipmi::prioOemBase, ipmi::rick::netFnAPP,
                              ipmi::general::cmdSetThreshold,
                              ipmi::Privilege::User,
                              register_set_threshold_command);
    }
    return 0;
}
