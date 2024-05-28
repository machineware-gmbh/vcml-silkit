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

typedef enum silkit_mode {
    SILKIT_MODE_AUTONOMOUS = 0,
    SILKIT_MODE_COORDINATED,
    SILKIT_MODE_TIME_SYNC,
    SILKIT_MODE_UNKNOWN,
} silkit_mode;

VCML_TYPEINFO(silkit_mode);

istream& operator>>(istream& is, silkit_mode& m);
ostream& operator<<(ostream& os, silkit_mode m);

namespace silkit {

class participant : public module
{
private:
    SilKit::Services::Orchestration::ILifecycleService* m_lifecycle;
    SilKit::IParticipant* m_silkit_part;

    mutex m_mtx;
    bool m_start = true;
    condition_variable m_cond_start;

public:
    property<string> registry_uri;
    property<string> name;
    property<string> cfg_path;
    property<silkit_mode> mode;

    virtual SilKit::IParticipant* get_silkit_part() { return m_silkit_part; }

    virtual void end_of_elaboration() override;

    participant(const sc_module_name& nm);
    virtual ~participant();
    VCML_KIND(silkit::participant);
};

} // namespace silkit
} // namespace vcml

#endif
