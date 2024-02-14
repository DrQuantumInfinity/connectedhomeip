
#pragma once

#include <functional>
#include <stdbool.h>
#include <stdint.h>

#include <app/InteractionModelEngine.h>
#include <app/util/af-types.h>
using namespace ::chip;
/**************************************************************************
 *                                  Constants
 **************************************************************************/
/**************************************************************************
 *                                  Macros
 **************************************************************************/
/**************************************************************************
 *                                  Types
 **************************************************************************/

class TransportLayer
{
public:
    TransportLayer(void);
    virtual ~TransportLayer(void);
    virtual void Send(const void* pDevice, ClusterId clusterId, const EmberAfAttributeMetadata* attributeMetadata, uint8_t* buffer);

protected:

private:

};
/**************************************************************************
 *                                  Prototypes
 **************************************************************************/