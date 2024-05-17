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

participant::participant(const sc_module_name& nm):
    module(nm),
    registry_uri("registry_uri", "silkit://localhost:8500"),
    name("name", "vcml_participant"),
    cfg_path("cfg_path", "") {
    log_info("SilKit Version: %s", SilKit::Version::String());

    auto participant_cfg = SilKit::Config::ParticipantConfigurationFromFile(
        cfg_path);
    auto sp = SilKit::CreateParticipant(participant_cfg, name, registry_uri);
    m_silkit_part = sp.release();

    m_lifecycle = m_silkit_part->CreateLifecycleService(
        { SilKit::Services::Orchestration::OperationMode::Coordinated });
    m_lifecycle->SetShutdownHandler([this]() {
        log_info("Shutdown requested");
        request_stop();
    });

    m_lifecycle->StartLifecycle();
}

participant::~participant() {
    if (m_silkit_part)
        delete m_silkit_part;
}

} // namespace silkit
} // namespace vcml
