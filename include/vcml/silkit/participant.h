/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#ifndef VCML_SILKIT_PARTICIPANT_H
#define VCML_SILKIT_PARTICIPANT_H

#include "vcml/core/types.h"
#include "vcml/core/systemc.h"
#include "vcml/core/module.h"
#include "vcml/core/model.h"

#include "vcml/properties/property.h"

#include "silkit.h"

namespace vcml {
namespace silkit {

class participant : public module
{
private:
    SilKit::Services::Orchestration::ILifecycleService* m_lifecycle;
    SilKit::IParticipant* m_silkit_part;

public:
    property<string> registry_uri;
    property<string> name;
    property<string> cfg_path;
    property<bool> coordinated;

    virtual SilKit::IParticipant* get_silkit_part() { return m_silkit_part; }

    participant(const sc_module_name& nm);
    virtual ~participant();
    VCML_KIND(silkit::participant);
};

} // namespace silkit
} // namespace vcml

#endif
