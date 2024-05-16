/******************************************************************************
*                                                                            *
* Copyright 2024 MachineWare GmbH                                            *
* All Rights Reserved                                                        *
*                                                                            *
* This is unpublished proprietary work and may not be used or disclosed to   *
* third parties, copied or duplicated in any form, in whole or in part,      *
* without prior written permission of the authors.                           *
*                                                                            *
******************************************************************************/

#ifndef VCML_SILKIT_BRIDGE_H
#define VCML_SILKIT_BRIDGE_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"
#include "vcml/core/module.h"
#include "vcml/core/model.h"

#include "vcml/properties/property.h"

#include "silkit/SilKit.hpp"

namespace vcml {
namespace silkit {

class participant : public module
{
private:
    SilKit::Services::Orchestration::ILifecycleService* m_lifecycle;

public:
    property<string> registry_uri;
    property<string> name;
    property<string> cfg_path;

    unique_ptr<SilKit::IParticipant> m_silkit_part;

    participant(const sc_module_name& nm);
    virtual ~participant();
    VCML_KIND(silkit::participant);
};

} // namespace silkit
} // namespace vcml

#endif
