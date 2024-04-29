/******************************************************************************
 *                                                                            *
 * Copyright (C) 2024 MachineWare GmbH                                        *
 * All Rights Reserved                                                        *
 *                                                                            *
 * This is work is licensed under the terms described in the LICENSE file     *
 * found in the root directory of this source tree.                           *
 *                                                                            *
 ******************************************************************************/

#include "vcml/silkit/participant.h"
#include "vcml/silkit/version.h"

#include "silkit/SilKit.hpp"
#include "silkit/SilKitVersion.hpp"

namespace vcml {
namespace silkit {

static const sc_time SC_EPSILON_TIME(time_from_value(1));

istream& operator>>(istream& is, silkit_mode& m) {
    std::string str;
    is >> str;
    str = vcml::to_lower(str);

    if (str == "autonomous")
        m = SILKIT_MODE_AUTONOMOUS;
    else if (str == "coordinated")
        m = SILKIT_MODE_COORDINATED;
    else if (str == "time_sync")
        m = SILKIT_MODE_TIME_SYNC;
    else
        m = SILKIT_MODE_UNKNOWN;

    return is;
}

ostream& operator<<(ostream& os, silkit_mode m) {
    switch (m) {
    case SILKIT_MODE_AUTONOMOUS:
        return os << "autonomous";
    case SILKIT_MODE_COORDINATED:
        return os << "coordinated";
    case SILKIT_MODE_TIME_SYNC:
        return os << "time_sync";
    default:
        return os << "unknown";
    }
}

using namespace SilKit::Services::Orchestration;

participant::participant(const sc_module_name& nm):
    module(nm),
    m_lifecycle(),
    m_silkit_part(),
    m_timesync(),
    m_mtx(),
    m_start(false),
    m_cond_start(),
    registry_uri("registry_uri", "silkit://localhost:8500"),
    name("name", "vcml_participant"),
    cfg_path("cfg_path", ""),
    mode("mode", SILKIT_MODE_COORDINATED),
    timestep("timestep", sc_time(1, SC_MS)) {
    log_info("SilKit Version: %s", SilKit::Version::String());

    SC_HAS_PROCESS(participant);
    SC_THREAD(time_sync_thread);
}

const char* participant::version() const {
    return VCML_SILKIT_VERSION_STRING;
}

void participant::shutdown_handler() {
    log_info("Shutdown requested");
    request_stop();
}

void participant::start_handler() {
    log_info("Start simulation");
    std::unique_lock lock(m_mtx);
    m_start = true;
    lock.unlock();
    m_cond_start.notify_one();
}

void participant::step_handler(const sc_time& now, const sc_time& duration) {
    log_debug("next timestep: now: %s duration %s\n", now.to_string().c_str(),
              duration.to_string().c_str());
    if (now != SC_ZERO_TIME && (now - SC_EPSILON_TIME) != sc_time_stamp())
        VCML_ERROR("Silkit time sync out of sync");

    m_mtx.lock();
    m_currtimestep = duration;
    m_mtx.unlock();

    m_start = true;
    m_cond_start.notify_all();
}

void participant::time_sync_thread() {
    if (mode != SILKIT_MODE_TIME_SYNC)
        return;

    sc_time offset(SC_EPSILON_TIME);

    while (true) {
        m_mtx.lock();
        const sc_time duration = m_currtimestep;
        m_mtx.unlock();

        wait(duration - offset);
        offset = SC_ZERO_TIME;

        while (sc_get_curr_simcontext()->pending_activity_at_current_time())
            wait(SC_ZERO_TIME);

        m_timesync->CompleteSimulationStep();

        // Keep simulation time from advancing but still allow delta processes
        // to execute so that the simulation does not lock up
        while (!m_start)
            wait(SC_ZERO_TIME);

        m_start = false;
    }
}

void participant::end_of_elaboration() {
    if (mode == SILKIT_MODE_UNKNOWN)
        VCML_ERROR("silkit mode %s not implementd", to_string(mode).c_str());

    const std::string cfg = R"(
    Description: My participant configuration
    Logging:
        Sinks:
        - Type: Stdout
          Level: Off)";

    std::shared_ptr<SilKit::Config::IParticipantConfiguration> participant_cfg;
    if (cfg_path == "") {
        participant_cfg = SilKit::Config::ParticipantConfigurationFromString(
            cfg);
    } else {
        participant_cfg = SilKit::Config::ParticipantConfigurationFromFile(
            cfg_path);
    }

    auto sp = SilKit::CreateParticipant(participant_cfg, name, registry_uri);
    m_silkit_part = sp.release();

    OperationMode sm;
    if (mode == SILKIT_MODE_AUTONOMOUS)
        sm = OperationMode::Autonomous;
    else
        sm = OperationMode::Coordinated;

    m_lifecycle = m_silkit_part->CreateLifecycleService({ sm });
    m_lifecycle->SetShutdownHandler([this] { shutdown_handler(); });
    m_lifecycle->SetStartingHandler([this]() { start_handler(); });

    if (mode == SILKIT_MODE_TIME_SYNC) {
        log_debug("Starting SilKit time sync with initial step size %s",
                  timestep.get().to_string().c_str());
        m_timesync = m_lifecycle->CreateTimeSyncService();

        using ns = std::chrono::nanoseconds;
        const ns ts(time_to_ns(timestep));

        m_timesync->SetSimulationStepHandlerAsync(
            [this](ns now, ns duration) {
                step_handler(sc_time(now.count(), SC_NS),
                             sc_time(duration.count(), SC_NS));
            },
            ts);
    }

    std::unique_lock lock(m_mtx);

    m_lifecycle->StartLifecycle();

    m_cond_start.wait(lock, [this]() -> bool { return m_start; });
    m_start = false;

    // After returning from StartingHandler simulation is only ReadyToRun
    // We need to wait for transition to Running
    while (m_lifecycle->State() != ParticipantState::Running)
        mwr::cpu_yield();
}

participant::~participant() {
    if (m_lifecycle) {
        auto state = m_lifecycle->State();
        if (state == ParticipantState::Running ||
            state == ParticipantState::Paused)
            m_lifecycle->Stop("User requested");
        else if (state != ParticipantState::Shutdown) {
            const char*
                msg = "end of simulation while not running and not shutdown";
            m_lifecycle->ReportError(msg);
            VCML_ERROR("%s", msg);
        }
    }

    if (m_silkit_part)
        delete m_silkit_part;
}

} // namespace silkit
} // namespace vcml
