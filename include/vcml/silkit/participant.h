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

enum silkit_mode {
    SILKIT_MODE_AUTONOMOUS = 0,
    SILKIT_MODE_COORDINATED,
    SILKIT_MODE_TIME_SYNC,
    SILKIT_MODE_UNKNOWN,
};

istream& operator>>(istream& is, silkit_mode& m);
ostream& operator<<(ostream& os, silkit_mode m);

class participant : public module
{
private:
    SilKit::Services::Orchestration::ILifecycleService* m_lifecycle;
    SilKit::IParticipant* m_silkit_part;
    SilKit::Services::Orchestration::ITimeSyncService* m_timesync;

    mutable mutex m_mtx;
    bool m_start;
    sc_time m_currtimestep;
    condition_variable m_cond_start;

public:
    property<string> registry_uri;
    property<string> name;
    property<string> cfg_path;
    property<silkit_mode> mode;
    property<sc_time> timestep;

    virtual SilKit::IParticipant* silkit_part() { return m_silkit_part; }

    void shutdown_handler();
    void start_handler();
    void step_handler(const sc_time& now, const sc_time& duration);
    void end_of_timestep();

    virtual void end_of_elaboration() override;

    participant(const sc_module_name& nm);
    virtual ~participant();
    VCML_KIND(silkit::participant);

    virtual const char* version() const override;
};

} // namespace silkit

VCML_TYPEINFO(silkit::silkit_mode);

} // namespace vcml

#endif
