#include "User.h"
// #include <DataModelTypes.h>
#include "DeviceMngr.h"
#include "DeviceLight.h"

void run(intptr_t context)
{
    static Device gLight1("Light 1", "Office");
    static Device gLight2("Light 2", "Office");
    static Device gLight3("Light 3", "Kitchen");
    static Device gLight4("Light 4", "Den");

    gLight1.SetReachable(true);
    gLight2.SetReachable(true);
    gLight3.SetReachable(true);
    gLight4.SetReachable(true);

    // Whenever bridged device changes its state
    gLight1.SetChangeCallback(&HandleDeviceStatusChanged);
    gLight2.SetChangeCallback(&HandleDeviceStatusChanged);
    gLight3.SetChangeCallback(&HandleDeviceStatusChanged);
    gLight4.SetChangeCallback(&HandleDeviceStatusChanged);


    DataVersion gLight1DataVersions[DEVICE_LIGHT_NUM_CLUSTERS];
    DataVersion gLight2DataVersions[DEVICE_LIGHT_NUM_CLUSTERS];
    DataVersion gLight3DataVersions[DEVICE_LIGHT_NUM_CLUSTERS];
    DataVersion gLight4DataVersions[DEVICE_LIGHT_NUM_CLUSTERS];


        // Add lights 1..3 --> will be mapped to ZCL endpoints 3, 4, 5
    AddDeviceEndpoint(&gLight1, DeviceLightGetEndpoint(), DeviceLightGetTypes(),
                      Span<DataVersion>(gLight1DataVersions), 1);
    AddDeviceEndpoint(&gLight2, DeviceLightGetEndpoint(), DeviceLightGetTypes(),
                      Span<DataVersion>(gLight2DataVersions), 1);
    AddDeviceEndpoint(&gLight3, DeviceLightGetEndpoint(), DeviceLightGetTypes(),
                      Span<DataVersion>(gLight3DataVersions), 1);

    // Remove Light 2 -- Lights 1 & 3 will remain mapped to endpoints 3 & 5
    RemoveDeviceEndpoint(&gLight2);

    // Add Light 4 -- > will be mapped to ZCL endpoint 6
    AddDeviceEndpoint(&gLight4, DeviceLightGetEndpoint(), DeviceLightGetTypes(),
                      Span<DataVersion>(gLight4DataVersions), 1);

    // Re-add Light 2 -- > will be mapped to ZCL endpoint 7
    AddDeviceEndpoint(&gLight2, DeviceLightGetEndpoint(), DeviceLightGetTypes(),
                      Span<DataVersion>(gLight2DataVersions), 1);
}