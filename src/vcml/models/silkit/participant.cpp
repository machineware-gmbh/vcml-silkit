#include "vcml/models/silkit/participant.h"

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
    m_silkit_part = SilKit::CreateParticipant(participant_cfg, name,
                                              registry_uri);

    m_lifecycle = m_silkit_part->CreateLifecycleService(
        { SilKit::Services::Orchestration::OperationMode::Coordinated });
    m_lifecycle->SetShutdownHandler([this]() {
        log_info("Shutdown requested");
        request_stop();
    });

    m_lifecycle->StartLifecycle();
}

participant::~participant() {
    // nothing to do
}
} // namespace silkit

} // namespace vcml
