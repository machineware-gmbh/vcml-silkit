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
namespace silkit {

using namespace SilKit::Services::Orchestration;

participant::participant(const sc_module_name& nm):
    module(nm),
    registry_uri("registry_uri", "silkit://localhost:8500"),
    name("name", "vcml_participant"),
    cfg_path("cfg_path", ""),
    coordinated("coordinated", false) {
    log_info("SilKit Version: %s", SilKit::Version::String());

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

    OperationMode mode = coordinated ? OperationMode::Coordinated
                                     : OperationMode::Autonomous;
    m_lifecycle = m_silkit_part->CreateLifecycleService({ mode });

    m_lifecycle->SetShutdownHandler([this]() {
        log_info("Shutdown requested");
        request_stop();
    });

    m_lifecycle->StartLifecycle();
}

participant::~participant() {
    if (m_lifecycle->State() == ParticipantState::Running)
        m_lifecycle->Stop("User requested");

    if (m_silkit_part)
        delete m_silkit_part;
}

} // namespace silkit
} // namespace vcml
