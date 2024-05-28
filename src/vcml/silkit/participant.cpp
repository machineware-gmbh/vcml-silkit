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

#include "silkit/SilKit.hpp"
#include "silkit/SilKitVersion.hpp"

namespace vcml {

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

namespace silkit {

using namespace SilKit::Services::Orchestration;

participant::participant(const sc_module_name& nm):
    module(nm),
    m_mtx(),
    m_cond_start(),
    registry_uri("registry_uri", "silkit://localhost:8500"),
    name("name", "vcml_participant"),
    cfg_path("cfg_path", ""),
    mode("mode", SILKIT_MODE_COORDINATED) {
    log_info("SilKit Version: %s", SilKit::Version::String());

    if (mode == SILKIT_MODE_UNKNOWN || mode == SILKIT_MODE_TIME_SYNC)
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
    m_lifecycle->SetShutdownHandler([this]() {
        log_info("Shutdown requested");
        request_stop();
    });

    m_lifecycle->SetStartingHandler([this]() {
        log_info("Start simulation");
        std::unique_lock lock(m_mtx);
        lock.unlock();
        m_cond_start.notify_one();
    });

    m_lifecycle->StartLifecycle();
}

void participant::end_of_elaboration() {
    std::unique_lock lock(m_mtx);
    m_cond_start.wait(lock, [this] {
        return m_lifecycle->State() != ParticipantState::Running;
    });
}

participant::~participant() {
    auto state = m_lifecycle->State();
    if (state == ParticipantState::Running ||
        state == ParticipantState::Paused)
        m_lifecycle->Stop("User requested");
    else if (state != ParticipantState::Shutdown) {
        m_lifecycle->ReportError(
            "end of simulation while not running and not shutdown");
        VCML_ERROR("end of simulation while not running and not shutdown");
    }

    if (m_silkit_part)
        delete m_silkit_part;
}

} // namespace silkit
} // namespace vcml
