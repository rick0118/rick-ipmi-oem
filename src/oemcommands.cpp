#include "oemcommands.hpp"

#include <boost/container/flat_map.hpp>
#include <ipmid/api.hpp>
#include <ipmid/types.hpp>
#include <ipmid/utils.hpp>
#include <phosphor-logging/log.hpp>

#include <cstdlib>

using namespace phosphor::logging;

// src/oem_command.cpp

const char *temperatureBusName = "mowgli.tmp275.bus";
const char *temperatureObjectPath = "/mowgli/example/temperature";
const char *setThresholdInterfaceName = "mowgli.example.setThreshold";

// 假設 property 和 method 的名稱
constexpr const char *PROPERTY_NAME = "Temperature";
constexpr const char *METHOD_NAME = "setThreshold";
static ipmi::ServiceCache temperatureBus(setThresholdInterfaceName,
                                         temperatureObjectPath);

ipmi_ret_t register_get_temperature_command(ipmi_netfn_t, ipmi_cmd_t, ipmi_request_t,
                                            ipmi_response_t response,
                                            ipmi_data_len_t dataLen, ipmi_context_t)
{

    try
    {
        auto bus = getSdBus();
        std::string service = temperatureBus.getService(*bus);
        auto variant =
            ipmi::getDbusProperty(*bus, temperatureBusName, temperatureObjectPath,
                                  setThresholdInterfaceName, PROPERTY_NAME);

        uint8_t i_temp = std::floor(std::get<double>(variant) + 0.5);
        memcpy(response, &i_temp, sizeof(uint8_t));
        *dataLen = sizeof(uint8_t);
        return IPMI_CC_OK;
    }
    catch (const std::exception &e)
    {
        phosphor::logging::log<phosphor::logging::level::ERR>(
            "Failed to get temp",
            phosphor::logging::entry("EXCEPTION=%s", e.what()));
        return IPMI_CC_UNSPECIFIED_ERROR;
    }
}
ipmi_ret_t register_set_threshold_command(ipmi_netfn_t, ipmi_cmd_t, ipmi_request_t request,
                                          ipmi_response_t,
                                          ipmi_data_len_t, ipmi_context_t)
{
    // std::shared_ptr<sdbusplus::asio::connection> dbus = getSdBus();
    auto bus = sdbusplus::bus::new_default();
    auto setThreshold =
        bus.new_method_call(temperatureBusName, temperatureObjectPath,
                            setThresholdInterfaceName, METHOD_NAME);

    double lc, lnc, unc, uc;
    memcpy(&lc, request, sizeof(double));
    memcpy(&lnc, request + sizeof(double), sizeof(double));
    memcpy(&unc, request + sizeof(double) * 2, sizeof(double));
    static_cast<void>(memcpy(&uc, request + sizeof(double) * 3, sizeof(double)));
    setThreshold.append(static_cast<double>(lc), static_cast<double>(lnc), static_cast<double>(unc), static_cast<double>(uc));

    return IPMI_CC_OK;
}

void registerOEMFunctions()
{
    // prioOemBase 20 Set the priority as the lowest number
    ipmi::registerHandler(ipmi::prioOemBase, ipmi::rick::netFnAPP,
                          ipmi::app::cmdGetTemp275, ipmi::Privilege::User,
                          register_get_temperature_command);

    ipmi::registerHandler(ipmi::prioOemBase, ipmi::rick::netFnAPP,
                          ipmi::app::cmdSetThreshold, ipmi::Privilege::User,
                          register_set_threshold_command);
}
