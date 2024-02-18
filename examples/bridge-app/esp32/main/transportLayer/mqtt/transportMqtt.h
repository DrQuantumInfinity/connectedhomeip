
#pragma once

#include <functional>
#include <stdbool.h>
#include <stdint.h>

#include "transportLayer.h"

#include <app/InteractionModelEngine.h>
#include <app/util/af-types.h>
using namespace::chip;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/

class TransportMqtt : public TransportLayer
{
public:
    TransportMqtt(const char* pTopic);
    virtual ~TransportMqtt(void);

protected:

private:
    char topic[30];
    struct Private;

    void Send(const Device* pDevice, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);
};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/
void TransportMqttHandleTopicRx(const char* pTopic, const char *pPayload);
